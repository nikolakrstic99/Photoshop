#pragma once
#include "Operation.h"

class Div :public Operation {
	int n;
public:
	Div(int q) :n(q) {}
	void operation(Pixel& p)override {
		op(p);
		scale(p);
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "Div" << "</Op>" << std::endl;
		file << "<Const>" << n << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "Div";
	}
protected:
	void op(Pixel& p) {
		p.setBlue(n / p.getBlue());
		p.setRed(n / p.getRed());
		p.setGreen(n / p.getGreen());
	}
};