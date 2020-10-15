#pragma once
#include "Operation.h"

class Max :public Operation {
	int n;
public:
	Max(int q) :n(q) {}
	void operation(Pixel& p)override {
		op(p);
		scale(p);
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "Max" << "</Op>" << std::endl;
		file << "<Const>" << 0 << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "Max";
	}
protected:
	void op(Pixel& p)override {
		if (p.getBlue() < n)p.setBlue(n);
		if (p.getRed() < n)p.setRed(n);
		if (p.getGreen() < n)p.setGreen(n);
	}
};