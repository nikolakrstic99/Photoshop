#include "PAM.h"

void PAM::readBody(std::ifstream& is) {
	data.resize(header.depth * header.height * header.width);
	is.read((char*)data.data(), data.size());
};

void PAM::readHeader(std::ifstream& is) {
	std::string line, lines;
	std::smatch result;

	for (int i = 0; i < 7; i++) {
		getline(is, line);
		lines += line;
	}

	std::regex rx("(P7)WIDTH ([0-9]*)HEIGHT ([0-9]*)DEPTH ([0-9]*)MAXVAL ([0-9]*)TUPLTYPE ([A-_]*)(ENDHDR)");
	if (std::regex_match(lines, result, rx)) {
		header.id = result.str(1);
		header.width = stoi(result.str(2));
		header.height = stoi(result.str(3));
		header.depth = stoi(result.str(4));
		header.maxval = stoi(result.str(5));
		header.type = result.str(6);
		header.end = result.str(7);
	}
}

void PAM::read(const char* fname) {
	std::ifstream file{ fname, std::ios_base::binary };
	readHeader(file);
	readBody(file);
}

std::vector<Pixel> PAM::formatToLayer() {
	std::vector <Pixel> tmp;

	Pixel* p;
	switch (this->header.depth) {
	case 4:
		for (int i = 0; i < header.height * header.width; i++) {
			p = new Pixel((int)data[i * 4], (int)data[i * 4 + 1], (int)data[i * 4 + 2], (int)data[i * 4 + 3]);
			tmp.push_back(*p);
		}
	case 3:
		for (int i = 0; i < header.height * header.width; i++) {
			p = new Pixel((int)data[i * 3], (int)data[i * 3 + 1], (int)data[i * 3 + 2]);
			tmp.push_back(*p);
		}
		break;
	case 2:
		for (int i = 0; i < header.height * header.width; i++) {
			p = new Pixel((int)data[i * 2], (int)data[i * 2 + 1]);
			tmp.push_back(*p);
		}
		break;
	case 1:for (int i = 0; i < header.height * header.width; i++) {
		p = new Pixel((int)data[i]);
		tmp.push_back(*p);
	}
		  break;
	}
	return tmp;
};

void PAM::setVector(std::vector<Pixel> p) {
	std::vector<unsigned char> tmp;
	unsigned char c;

	switch (header.depth) {
	case 4:
		for (int i = 0; i < p.size(); i++) {
			c = p[i].getRed();
			tmp.push_back(c);
			c = p[i].getGreen();
			tmp.push_back(c);
			c = p[i].getBlue();
			tmp.push_back(c);	
			c = p[i].getTransparency();
			tmp.push_back(c);		
		}
		break;
	case 3:
		for (int i = 0; i < p.size(); i++) {
			c = p[i].getRed();
			tmp.push_back(c);
			c = p[i].getGreen();
			tmp.push_back(c);
			c = p[i].getBlue();
			tmp.push_back(c);			
		}
		break;
	case 2:
		for (int i = 0; i < p.size(); i++) {
			c = p[i].getRed();
			tmp.push_back(c);
			c = p[i].getTransparency();
			tmp.push_back(c);

		}
		break;
	case 1:
		for (int i = 0; i < p.size(); i++) {
			c = p[i].getRed();
			tmp.push_back(c);

		}
		break;
	}
	data = tmp;
}

void PAM::write(std::string fname) {
	std::ofstream of{ fname, std::ios_base::binary };
	
	if (of) {
		of << header.id << std::endl << "WIDTH " << header.width << std::endl << "HEIGHT " << header.height << std::endl << "DEPTH " << header.depth << std::endl <<
			"MAXVAL " << header.maxval << std::endl << "TUPLTYPE " << header.type << std::endl << "ENDHDR" << std::endl;;
		of.write((const char*)data.data(), data.size());
	}
	else {
		throw std::runtime_error("Unable to open the output image file.");
	}
}

PAM::PAM(int32_t width, int32_t height) {
	header.id = "P7";
	header.width = width;
	header.height = height;
	header.depth = 4;
	header.maxval = 255;
	header.type = "RGB_ALPHA";
	header.end = "ENDHDR";
};
