#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include "Pixel.h"
#include "Formater.h"
#include "Layer.h"
#include "BMP.h"
#include "Selection.h"
#include "Rectangle.h"
#include "toBlackAndWhite.h"
#include "Add.h"
#include "toGrey.h"
#include "Inversion.h"
#include "Medijana.h"
#include "CompositeOperation.h"
#include <exception>


class SelectionOutOfRange :public std::exception {
	const char* what()const override {
		return "Lose dimenzije selekcije";
	}
};

class OutputFormat :public std::exception {
	const char* what()const override {
		return "Los izlazni format";
	}
};

class noOperation :public std::exception {
	const char* what()const override {
		return "Ne posotji operacija";
	}
};

class noSelection :public std::exception {
	const char* what()const override {
		return "Ne posotji selekcija";
	}
};

class Image {
	Layer* one;
	int width, height;
	std::vector<Layer> layers;
	
	std::map<std::string, int> hashMap;
	std::map<std::string, Operation*> operations;
	std::string extension;
public:
	std::map<std::string, Selection> selections;
	Image(){ predefined(); }
	Image(std::string s);
	Image(std::string, int, bool);
	Image(int w, int h);
	//~Image();
	Layer* getOne() { return one; }
	void setOne(Layer l) { one = new Layer; *one = l; }
	std::map<std::string, Operation*>& getOperations() { return operations; }
	std::vector<Layer> getLayers() { return layers; }
	std::map<std::string, Selection>& getSelections() { return selections; }
	void removeLayer(int);
	void addLayer(std::string s, bool _active = true, int _transparency = 0);
	std::vector<Layer> extendAll();
	void write(std::string s);
	void updateOne(Layer&);
	void addSelection(Selection selection);
	bool isActiveSelections();
	bool isActivePixel(int, int);
	Operation* readComposite(std::ifstream& file, std::string name, int tmp);
	void fillHashMap();
	Operation* returnOp(std::string s, int n);
	void paintSelection(std::string, Pixel);
	void addOperation(Operation* o);
	void save(std::string str);
	void read(std::string str);
	void doOperation(std::string s);
	void predefined();
	std::string loadComposite(std::string);
	void saveComposite(std::string, std::string);
	void setExtension(std::string);
};