#pragma once
#include "Operation.h"
#include <cmath>

class Power :public Operation {
	int n;
public:
	Power(int q) :n(q) {}
	void operation(Pixel& p)override {
		op(p);
		scale(p);
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "Power" << "</Op>" << std::endl;
		file << "<Const>" << n << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "Power";
	}
protected:
	void op(Pixel& p) {
		p.setBlue(pow(p.getBlue(),n));
		p.setRed(pow(p.getRed(),n));
		p.setGreen(pow(p.getGreen(),n));
	}
};