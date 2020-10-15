#pragma once
#include "Operation.h"

class InvDiv :public Operation {
	int n;
public:
	InvDiv(int q) :n(q) {}
	void operation(Pixel& p)override {
		op(p);
		scale(p);
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "InvDiv" << "</Op>" << std::endl;
		file << "<Const>" << n << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "InvDiv";
	}
protected:
	void op(Pixel& p) {
		p.setBlue(p.getBlue() / n);
		p.setRed(p.getRed() / n);
		p.setGreen(p.getGreen() / n);
	}
};