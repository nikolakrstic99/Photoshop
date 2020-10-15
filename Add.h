#pragma once
#include "Operation.h"

class Add:public Operation {
	int n;
public:
	Add(int q):n(q) {}
	void operation(Pixel& pixel)override {
		op(pixel);
		scale(pixel);
	}
	
	void getXML(std::ofstream& file)override {
		file << "<Op>" << "Add" << "</Op>" << std::endl;
		file << "<Const>" << n << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "Add";
	}
protected:
	void op(Pixel& p) {
		p.setBlue(p.getBlue() + n);
		p.setRed(p.getRed() + n);
		p.setGreen(p.getGreen() + n);
	}
};