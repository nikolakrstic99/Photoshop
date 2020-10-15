#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include "Pixel.h"
#include "Formater.h"

struct PAMheader {
	std::string id;
	int width;
	int height;
	int depth;
	int maxval;
	std::string type;
	std::string end;
};

class PAM:public Formater{
	PAMheader header;
	std::vector<uint8_t>data;
	
	//void read(const char*);
	void readBody(std::ifstream&);
	void readHeader(std::ifstream&);
	
public:
	PAM(int32_t width, int32_t height);
	
	PAM(const char* fname) {
		read(fname);
	};

	~PAM(){}

	void read(const char*)override;

	void write(std::string)override;

	void setVector(std::vector<Pixel>) override;
	
	std::vector<Pixel> formatToLayer();

	//friend class Layer;
	/*
	void write(const char*);
	void layerToFormat(std::vector<Pixel>);
	*/
	int getWidth() { return header.width; }
	int getHeight() { return header.height; }
	int getDepth() { return header.depth; }
	

};

