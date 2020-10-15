#pragma once
#include "Operation.h"

class Mul :public Operation {
	int n;
public:
	Mul(int q) :n(q) {}
	void operation(Pixel& p)override {
		op(p);
		scale(p);
	}
	void getXML(std::ofstream &file)override {
		file << "<Op>" << "Mul" << "</Op>" << std::endl;
		file << "<Const>" << n << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "Mul";
	}
protected:
	void op(Pixel& p) {
		p.setBlue(p.getBlue() * n);
		p.setRed(p.getRed() * n);
		p.setGreen(p.getGreen() * n);
	}
};