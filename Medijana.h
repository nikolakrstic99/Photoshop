#pragma once
#include "Operation.h"

class Medijana :public Operation {
public:
	Medijana() {}
	void operation(std::vector<Pixel>& pixels,int h,int w)override {
		std::vector<Pixel> tmpPixels = pixels;
		
		int n = pixels.size();
		pixels[0].setBlue(medijana3(pixels[0].getBlue(), pixels[1].getBlue(), pixels[w].getBlue()));
		pixels[0].setGreen(medijana3(pixels[0].getGreen(), pixels[1].getGreen(), pixels[w].getGreen()));
		pixels[0].setRed(medijana3(pixels[0].getRed(), pixels[1].getRed(), pixels[w].getRed()));
		for (int i = 1; i < n - 1; i++) {
			if (i % w == 0 || i % w == w - 1 || i / w == 0 || i / w == h - 1) {
				if (i == w - 1) {
					pixels[i].setBlue(medijana3(tmpPixels[i].getBlue(), tmpPixels[i - 1].getBlue(), tmpPixels[i + w].getBlue()));
					pixels[i].setGreen(medijana3(tmpPixels[i].getGreen(), tmpPixels[i - 1].getGreen(), tmpPixels[i + w].getGreen()));
					pixels[i].setRed(medijana3(tmpPixels[i].getRed(), tmpPixels[i - 1].getRed(), tmpPixels[i + w].getRed()));
				}
				else if (i == w * (h - 1)) {
					pixels[i].setBlue(medijana3(tmpPixels[i].getBlue(), tmpPixels[i + 1].getBlue(), tmpPixels[i - w].getBlue()));
					pixels[i].setGreen(medijana3(tmpPixels[i].getGreen(), tmpPixels[i + 1].getGreen(), tmpPixels[i - w].getGreen()));
					pixels[i].setRed(medijana3(tmpPixels[i].getRed(), tmpPixels[i + 1].getRed(), tmpPixels[i - w].getRed()));
				}
				else if (i < w) {
					pixels[i].setBlue(medijana4(tmpPixels[i].getBlue(),tmpPixels[i - 1].getBlue(),tmpPixels[i + 1].getBlue(),tmpPixels[i + w].getBlue()));
					pixels[i].setGreen(medijana4(tmpPixels[i].getGreen(), tmpPixels[i - 1].getGreen(), tmpPixels[i + 1].getGreen(), tmpPixels[i + w].getGreen()));
					pixels[i].setRed(medijana4(tmpPixels[i].getRed(), tmpPixels[i - 1].getRed(), tmpPixels[i + 1].getRed(), tmpPixels[i + w].getRed()));
				}
				else if ((i % w)==0) {
					pixels[i].setBlue(medijana4(tmpPixels[i].getBlue(), tmpPixels[i - w].getBlue(), tmpPixels[i + w].getBlue(), tmpPixels[i + 1].getBlue()));
					pixels[i].setGreen(medijana4(tmpPixels[i].getGreen(), tmpPixels[i - w].getGreen(), tmpPixels[i + w].getGreen(), tmpPixels[i + 1].getGreen()));
					pixels[i].setRed(medijana4(tmpPixels[i].getRed(), tmpPixels[i - w].getRed(), tmpPixels[i + w].getRed(), tmpPixels[i + 1].getRed()));
				}
				else if ((i % w)==(w - 1)) {
					pixels[i].setBlue(medijana4(tmpPixels[i].getBlue(), tmpPixels[i - w].getBlue(), tmpPixels[i - 1].getBlue(), tmpPixels[i + w].getBlue()));
					pixels[i].setGreen(medijana4(tmpPixels[i].getGreen(), tmpPixels[i - w].getGreen(), tmpPixels[i - 1].getGreen(), tmpPixels[i + w].getGreen()));
					pixels[i].setRed(medijana4(tmpPixels[i].getRed(), tmpPixels[i - w].getRed(), tmpPixels[i - 1].getRed(), tmpPixels[i + w].getRed()));
				}
				else if (i > w * (h - 1)) {
					pixels[i].setBlue(medijana4(tmpPixels[i].getBlue(), tmpPixels[i - w].getBlue(), tmpPixels[i - 1].getBlue(),tmpPixels[i + 1].getBlue()));
					pixels[i].setGreen(medijana4(tmpPixels[i].getGreen(), tmpPixels[i - w].getGreen(), tmpPixels[i - 1].getGreen(), tmpPixels[i + 1].getGreen()));
					pixels[i].setRed(medijana4(tmpPixels[i].getRed(), tmpPixels[i - w].getRed(), tmpPixels[i - 1].getRed(), tmpPixels[i + 1].getRed()));
				}

			}
			else {
				pixels[i].setBlue(medijana5(tmpPixels[i].getBlue(), tmpPixels[i - 1].getBlue(), tmpPixels[i+1].getBlue(),tmpPixels[i-w].getBlue(),tmpPixels[i+w].getBlue()));
				pixels[i].setGreen(medijana5(tmpPixels[i].getGreen(), tmpPixels[i - 1].getGreen(), tmpPixels[i + 1].getGreen(), tmpPixels[i - w].getGreen(), tmpPixels[i + w].getGreen()));
				pixels[i].setRed(medijana5(tmpPixels[i].getRed(), tmpPixels[i - 1].getRed(), tmpPixels[i + 1].getRed(), tmpPixels[i - w].getRed(), tmpPixels[i + w].getRed()));
			}


		}
		pixels[n - 1].setBlue(medijana3(tmpPixels[n-2].getBlue(),tmpPixels[n-1-w].getBlue(),tmpPixels[n-1].getBlue()));
		pixels[n - 1].setGreen(medijana3(tmpPixels[n - 2].getGreen(), tmpPixels[n - 1 - w].getGreen(), tmpPixels[n - 1].getGreen()));
		pixels[n - 1].setRed(medijana3(tmpPixels[n - 2].getRed(), tmpPixels[n - 1 - w].getRed(), tmpPixels[n - 1].getRed()));
	}

	void getXML(std::ofstream& file)override {
		file << "<Op>" << "Medijana" << "</Op>" << std::endl;
		file << "<Const>" << 0 << "</Const>" << std::endl;
	}

	std::string getName()override {
		return "Medijana";
	}
protected:
	void op(Pixel&) override{

	}

	int medijana3(int a, int b, int c) {
		if (a > b)
		{
			if (b > c)
				return b;
			else if (a > c)
				return c;
			else
				return a;
		}
		else
		{
			if (a > c)
				return a;
			else if (b > c)
				return c;
			else
				return b;
		}
	}

	int medijana4(int a, int b, int c,int d) {
		int max = a, min = a;
		if (b > max)b = max;
		if (b < min)b = min;
		if (c > max)c = max;
		if (c < min)c = min;
		if (d > max)d = max;
		if (d < min)d = min;
		return(a + b + c + d - max - min) / 2;
	}

	int medijana5(int a, int b, int c, int d,int e) {
		int arr[] = { a,b,c,d,e };
		int n = sizeof(arr) / sizeof(arr[0]);
		std::sort(arr, arr + n);
		return arr[2];
	}
};