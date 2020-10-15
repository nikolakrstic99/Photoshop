#pragma once
#include "Pixel.h"
//#include "Image.h"
#include <algorithm>
#include <vector>
#include <string>

class Operation {
public:
	virtual void operation(Pixel&){}
	virtual void operation(std::vector<Pixel>&,int,int) {}
	void scale(Pixel& p) {
		if (p.getBlue() > 255)p.setBlue(255);
		if (p.getBlue() < 0)p.setBlue(0);
		if (p.getGreen() > 255)p.setGreen(255);
		if (p.getGreen() < 0)p.setGreen(0);
		if (p.getRed() > 255)p.setRed(255);
		if (p.getRed() < 0)p.setRed(0);
	}

	virtual int getSize() { return 1; }
	virtual std::string getName() { return "x"; }
	virtual void getXML(std::ofstream &file) = 0;
	friend class CompositeOperation;
	virtual void addOperation(Operation* o){}
protected:
	virtual void op(Pixel&) {};
};