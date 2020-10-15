#pragma once
#include "Operation.h"

class toGrey :public Operation {
public:
	toGrey() {}
	void operation(Pixel& p)override {
		op(p);
		scale(p);
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "toGrey" << "</Op>" << std::endl;
		file << "<Const>" << 0 << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "toGrey";
	}
	std::string string() { return "toGrey"; }
protected:
	void op(Pixel & p)override {
		set(p, ((p.getBlue() + p.getGreen() + p.getRed()) / 3));
	}
	void set(Pixel& p, int n) {
		p.setBlue(n);
		p.setGreen(n);
		p.setRed(n);
	}
	
};