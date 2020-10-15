#pragma once
#include "Operation.h"

class InvSub :public Operation {
	int n;
public:
	InvSub(int q) :n(q) {}
	void operation(Pixel& p)override {
		op(p);
		scale(p);
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "InvSub" << "</Op>" << std::endl;
		file << "<Const>" << n << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "InvSub";
	}
protected:
	void op(Pixel& p) {
		p.setBlue(p.getBlue() - n);
		p.setRed(p.getRed() - n);
		p.setGreen(p.getGreen() - n);
	}
};