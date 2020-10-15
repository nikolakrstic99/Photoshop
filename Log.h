#pragma once
#include "Operation.h"
#include <cmath>

class Log :public Operation {
public:
	Log() {}
	void operation(Pixel& p)override {
		op(p);
		scale(p);
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "Log" << "</Op>" << std::endl;
		file << "<Const>" << 0 << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "Log";
	}
protected:
	void op(Pixel& p)override {
		p.setBlue(log(p.getBlue()));
		p.setGreen(log(p.getGreen()));
		p.setRed(log(p.getRed()));
	}
};