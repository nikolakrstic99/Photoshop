#pragma once
#include <stdio.h>

class Pixel {
	int red;
	int green;
	int blue;
	int transparency;
public:
	Pixel(int r=255, int g=255, int b=255,int t=255);
	int getRed() { return red; }
	int getGreen() { return green; }
	int getBlue() { return blue; }
	int getTransparency() { return transparency; }
	void setBlue(int b) { 
		blue = b; 
	}
	void setGreen(int g) {
		green = g; 
	}
	void setRed(int r) { red = r; }
	void setTransparency(int t) { transparency = t; }
};
