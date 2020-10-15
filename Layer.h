#pragma once
#include "Pixel.h"
#include "BMP.h"
#include "PAM.h"
#include "Formater.h"
#include <vector>
#include <list>
#include <string>
#include <exception>

class InputFormat :public std::exception {
	const char* what()const override {
		return "Los ulazni format";
	}
};

class Image;

class Layer {
	int id;
	static int idNext;
	//Formater* p;
	int transparency,width,height;
	bool active;
	
public:
	std::vector<Pixel> pixels;
	Layer(int _width,int _height,bool _active, int _transparency) :pixels(0),transparency(_transparency),
		active(_active),width(_width),height(_height),id(++idNext) {};

	Layer(std::string,bool _active=true, int _transparency = 0);
	
	Layer(bool _active=true, int _transparency = 0);

	~Layer() { 
		pixels.clear();
	}
	
	int getId() { return id; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getTransparency() { return transparency; }
	void setId(int i) { id = i; }
	void setWidth(int i) { width = i; }
	void setHeight(int i) { height = i; }
	void setTrasnparency(int i) { transparency = i; }
	void setActive(bool i) { active = i; }
	
	void setTransparency(int _transparency) { transparency = _transparency; };
	//Formater* getFormat() { return p; };
	std::vector<Pixel> getPixels() { return pixels; };
	std::vector<Pixel>& getRefPixels() { return pixels; };
	void setPixels(std::vector<Pixel>& p) { pixels = p; }
	void setActive() { active = true; }
	void setNoActive() { active = false; }
	bool getActive() { return active; }
	void extend(int ImageWidth, int ImageHeight, int layerWidth, int layerHeight);
};


