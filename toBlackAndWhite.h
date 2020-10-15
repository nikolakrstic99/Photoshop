#pragma once
#include "Operation.h"

class toBlackAndWhite :public Operation {
public:
	toBlackAndWhite():Operation() {};
	void operation(Pixel& p)override{
		op(p);
		scale(p);
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "toBlackAndWhite" << "</Op>" << std::endl;
		file << "<Const>" << 0 << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "toBlackAndWhite";
	}
protected:
	void op(Pixel &p)override {
		if (((p.getBlue() + p.getGreen() + p.getRed()) / 3) > 127)
			set(p, 255);
		else set(p, 0);
	}
	void set(Pixel& p, int n) {
		p.setBlue(n);
		p.setGreen(n);
		p.setRed(n);
	}
};