#pragma once
#include "Operation.h"

class Abs:public Operation {
public:
	Abs(){}
	void operation(Pixel& p)override {
		op(p);
		scale(p);
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "Abs" << "</Op>" << std::endl;
		file << "<Const>" << 0 << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "Abs";
	}
protected:
	void op(Pixel& p)override {
		p.setBlue(abs(p.getBlue()));
		p.setRed(abs(p.getRed()));
		p.setGreen(abs(p.getGreen()));
	}
};