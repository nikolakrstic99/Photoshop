#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Pixel.h"

class Formater {
	int i;
public:
	
	Formater() { i = 1; };
	
	virtual int getWidth()=0;
	virtual int getHeight()=0;
	virtual int getDepth()=0;
	/*
	virtual std::vector<Pixel> formatToLayer()=0;
	virtual void layerToFormat(std::vector<Pixel>)=0;
	virtual void write(const char*)=0;
	*/
	virtual void read(const char*) = 0;

	virtual void write(std::string) = 0;

	virtual void setVector(std::vector<Pixel>) = 0;

	virtual std::vector<Pixel> formatToLayer()=0;
};