#include "Layer.h"
#include <regex>

int Layer::idNext = 0;

Layer::Layer(std::string s,bool _active, int _transparency):transparency(_transparency),active(_active),id(++idNext) {
	const char* m = s.c_str();
	height = width = 0;
	Formater* p;
	if (s.substr(s.length() - 3) == "pam") {
			p = new PAM(m);						
			pixels = p->formatToLayer();
			height = p->getHeight();
			width = p->getWidth();
	}else if (s.substr(s.length() - 3) == "bmp") {
		p = new BMP(m);
		pixels = p->formatToLayer();
		height = p->getHeight();
		width = p->getWidth();
	}
	else {
		throw InputFormat();
	}
}

Layer::Layer(bool _active, int _transparency) :transparency(_transparency), active(_active),width(0),height(0),pixels(0),id(++idNext) {

};

void Layer::extend(int ImageWidth, int ImageHeight, int layerWidth, int layerHeight) {
	std::vector<Pixel> tmp;
	tmp.resize(ImageWidth * ImageHeight);
	for (int i = 0; i < layerHeight; i++) {
		for (int j = 0; j < layerWidth; j++) {
			tmp[j + i * ImageWidth] = pixels[j + i * layerWidth];
		}
		for (int j = layerWidth; j < ImageWidth; j++)
			tmp[j + i * ImageWidth] = Pixel(0, 0, 0, 0);
	}
	for (int i = layerHeight; i < ImageHeight; i++)
		for (int j = 0; j < ImageWidth; j++)
			tmp[j + i * ImageWidth] = Pixel(0, 0, 0, 0);

	pixels.clear();
	pixels = tmp;
	width = ImageWidth;
	height = ImageHeight;
}