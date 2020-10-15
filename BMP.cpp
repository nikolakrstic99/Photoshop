#include "BMP.h"

BMP::BMP(int32_t width, int32_t height,bool has_alpha) {
	if (width <= 0 || height <= 0) {
		throw std::runtime_error("The image width and height must be positive numbers.");
	}

	bmp_info_header.width = width;
	bmp_info_header.height = height;
	if (has_alpha) {
		bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
		file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);

		bmp_info_header.bit_count = 32;
		bmp_info_header.compression = 3;
		row_stride = width * 4;
		data.resize(row_stride * height);
		file_header.file_size = file_header.offset_data + data.size();
	}
	else {
		bmp_info_header.size = sizeof(BMPInfoHeader);
		file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

		bmp_info_header.bit_count = 24;
		bmp_info_header.compression = 0;
		row_stride = width * 3;
		data.resize(row_stride * height);

		uint32_t new_stride = make_stride_aligned(4);
		file_header.file_size = file_header.offset_data + static_cast<uint32_t>(data.size()) + bmp_info_header.height * (new_stride - row_stride);
	}
}

uint32_t BMP::make_stride_aligned(uint32_t align_stride) {
	uint32_t new_stride = row_stride;
	while (new_stride % align_stride != 0) {
		new_stride++;
	}
	return new_stride;
}

BMP::BMP(const char* fname) {
	read(fname);
}

void BMP::read(const char* fname) {
	std::ifstream inp{ fname, std::ios_base::binary };
	if (inp) {
		inp.read((char*)&file_header, sizeof(file_header));
		if (file_header.file_type != 0x4D42) {
			throw std::runtime_error("Error! Unrecognized file format.");
		}
		inp.read((char*)&bmp_info_header, sizeof(bmp_info_header));

		// The BMPColorHeader is used only for transparent images
		if (bmp_info_header.bit_count == 32) {
			// Check if the file has bit mask color information
			if (bmp_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
				inp.read((char*)&bmp_color_header, sizeof(bmp_color_header));
				// Check if the pixel data is stored as BGRA and if the color space type is sRGB
				check_color_header(bmp_color_header);
			}
			else {
				std::cerr << "Error! The file \"" << fname << "\" does not seem to contain bit mask information\n";
				throw std::runtime_error("Error! Unrecognized file format.");
			}
		}

		// Jump to the pixel data location
		inp.seekg(file_header.offset_data, inp.beg);

		// Adjust the header fields for output.
		// Some editors will put extra info in the image file, we only save the headers and the data.
		if (bmp_info_header.bit_count == 32) {
			bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
			file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
		}
		else {
			bmp_info_header.size = sizeof(BMPInfoHeader);
			file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
		}
		file_header.file_size = file_header.offset_data;

		if (bmp_info_header.height < 0) {
			throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
		}

		data.resize(bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8);

		// Here we check if we need to take into account row padding
		if (bmp_info_header.width % 4 == 0) {
			inp.read((char*)data.data(), data.size());
			file_header.file_size += static_cast<uint32_t>(data.size());
		}
		else {
			row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
			uint32_t new_stride = make_stride_aligned(4);
			std::vector<uint8_t> padding_row(new_stride - row_stride);

			for (int y = 0; y < bmp_info_header.height; ++y) {
				inp.read((char*)(data.data() + row_stride * y), row_stride);
				inp.read((char*)padding_row.data(), padding_row.size());
			}
			file_header.file_size += static_cast<uint32_t>(data.size()) + bmp_info_header.height * static_cast<uint32_t>(padding_row.size());
		}
	}
	else {
		throw std::runtime_error("Unable to open the input image file.");
	}
}

void BMP::write(std::string fname) {
	std::ofstream of{ fname, std::ios_base::binary };
	if (of) {
		if (bmp_info_header.bit_count == 32) {
			write_headers_and_data(of);
		}
		else if (bmp_info_header.bit_count == 24) {
			if (bmp_info_header.width % 4 == 0) {
				write_headers_and_data(of);
			}
			else {
				uint32_t new_stride = make_stride_aligned(4);
				std::vector<uint8_t> padding_row(new_stride - row_stride);

				write_headers(of);

				for (int y = 0; y < bmp_info_header.height; ++y) {
					of.write((const char*)(data.data() + row_stride * y), row_stride);
					of.write((const char*)padding_row.data(), padding_row.size());
				}
			}
		}
		else {
			throw std::runtime_error("The program can treat only 24 or 32 bits per pixel BMP files");
		}
	}
	else {
		throw std::runtime_error("Unable to open the output image file.");
	}
}

void BMP::check_color_header(BMPColorHeader& bmp_color_header) {
	BMPColorHeader expected_color_header;
	if (expected_color_header.red_mask != bmp_color_header.red_mask ||
		expected_color_header.blue_mask != bmp_color_header.blue_mask ||
		expected_color_header.green_mask != bmp_color_header.green_mask ||
		expected_color_header.alpha_mask != bmp_color_header.alpha_mask) {
		throw std::runtime_error("Unexpected color mask format! The program expects the pixel data to be in the BGRA format");
	}
	if (expected_color_header.color_space_type != bmp_color_header.color_space_type) {
		throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
	}
}

void BMP::write_headers(std::ofstream& of) {
	of.write((const char*)&file_header, sizeof(file_header));
	of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
	if (bmp_info_header.bit_count == 32) {
		of.write((const char*)&bmp_color_header, sizeof(bmp_color_header));
	}
}

void BMP::write_headers_and_data(std::ofstream& of) {
	write_headers(of);
	of.write((const char*)data.data(), data.size());
}

void BMP::setVector(std::vector<Pixel> p) {
	std::vector<unsigned char> tmp;
	unsigned char c;
	int height = bmp_info_header.height;
	int width = bmp_info_header.width;
	int depth = bmp_info_header.bit_count / 8;

	for(int i=width*(height-1);i>=0;i-=width)
		for (int j = 0; j < width; j++) {
			c = p[i + j].getBlue();
			tmp.push_back(c);

			c = p[i + j].getGreen();
			tmp.push_back(c);

			c = p[i + j].getRed();
			tmp.push_back(c);

			if (depth == 4) {
				c = p[i + j].getTransparency();
				tmp.push_back(c);
			}
		}
	data = tmp;
}

std::vector<Pixel> BMP::formatToLayer() {
	std::vector<Pixel> tmp;
	int w = bmp_info_header.width;
	int h = bmp_info_header.height;
	int d = bmp_info_header.bit_count == 32 ? 4 : 3;
	int R, G, B, A;
	for (int i = h - 1; i >= 0; i--) {
		for (int j = 0; j < w * d; j += d) {
			B = data[i * w * d + j];
			G = data[i * w * d + j + 1];
			R = data[i * w * d + j + 2];
			
			if (d == 4) {
				A = data[i * w * d + j + 3];
				tmp.push_back(Pixel(R, G, B, A));
			}else
			tmp.push_back(Pixel(R,G,B,255));
		}
	}
	return tmp;
}

/*


void BMP::fill_region(uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, uint8_t B, uint8_t G, uint8_t R, uint8_t A) {
	if (x0 + w > (uint32_t)bmp_info_header.width || y0 + h > (uint32_t)bmp_info_header.height) {
		throw std::runtime_error("The region does not fit in the image!");
	}

	uint32_t channels = bmp_info_header.bit_count / 8;
	for (uint32_t y = y0; y < y0 + h; ++y) {
		for (uint32_t x = x0; x < x0 + w; ++x) {
			data[channels * (y * bmp_info_header.width + x) + 0] = B;
			data[channels * (y * bmp_info_header.width + x) + 1] = G;
			data[channels * (y * bmp_info_header.width + x) + 2] = R;
			if (channels == 4) {
				data[channels * (y * bmp_info_header.width + x) + 3] = A;
			}
		}
	}
}

void BMP::set_pixel(uint32_t x0, uint32_t y0, uint8_t B, uint8_t G, uint8_t R, uint8_t A) {
	if (x0 > (uint32_t)bmp_info_header.width || y0 > (uint32_t)bmp_info_header.height) {
		throw std::runtime_error("The point is outside the image boundaries!");
	}

	uint32_t channels = bmp_info_header.bit_count / 8;
	data[channels * (y0 * bmp_info_header.width + x0) + 0] = B;
	data[channels * (y0 * bmp_info_header.width + x0) + 1] = G;
	data[channels * (y0 * bmp_info_header.width + x0) + 2] = R;
	if (channels == 4) {
		data[channels * (y0 * bmp_info_header.width + x0) + 3] = A;
	}
}

void BMP::draw_rectangle(uint32_t x0, uint32_t y0, uint32_t w, uint32_t h,
	uint8_t B, uint8_t G, uint8_t R, uint8_t A, uint8_t line_w) {
	if (x0 + w > (uint32_t)bmp_info_header.width || y0 + h > (uint32_t)bmp_info_header.height) {
		throw std::runtime_error("The rectangle does not fit in the image!");
	}

	fill_region(x0, y0, w, line_w, B, G, R, A);                                             // top line
	fill_region(x0, (y0 + h - line_w), w, line_w, B, G, R, A);                              // bottom line
	fill_region((x0 + w - line_w), (y0 + line_w), line_w, (h - (2 * line_w)), B, G, R, A);  // right line
	fill_region(x0, (y0 + line_w), line_w, (h - (2 * line_w)), B, G, R, A);                 // left line
}

*/