#pragma once
#include "Operation.h"

class Min :public Operation {
	int n;
public:
	Min(int q) :n(q) {}
	void operation(Pixel& p)override {
		op(p);
		scale(p);
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "Min" << "</Op>" << std::endl;
		file << "<Const>" << n << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "Min";
	}
protected:
	void op(Pixel& p)override {
		if (p.getBlue() > n)p.setBlue(n);
		if (p.getRed() > n)p.setRed(n);
		if (p.getGreen() > n)p.setGreen(n);
	}
};