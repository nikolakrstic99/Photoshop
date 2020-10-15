#pragma once
#include "Operation.h"

class Sub :public Operation {
	int n;
public:
	Sub(int q) :n(q) {}
	void operation(Pixel& p)override {
		op(p);
		scale(p);
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "Sub" << "</Op>" << std::endl;
		file << "<Const>" << n << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "Sub";
	}
protected:
	void op(Pixel& p) {
		p.setBlue(n - p.getBlue());
		p.setRed(n - p.getRed());
		p.setGreen(n - p.getGreen());
	}
};