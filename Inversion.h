#pragma once
#include "Operation.h"

class Inversion :public Operation {
public:
	Inversion() {}
	void operation(Pixel& p)override {
		op(p);
		scale(p);
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "Inversion" << "</Op>" << std::endl;
		file << "<Const>" << 0 << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "Inversion";
	}

protected:
	void op(Pixel& p) override{
		p.setBlue(255 - p.getBlue());
		p.setGreen(255 - p.getGreen());
		p.setRed(255 - p.getRed());
	}
};