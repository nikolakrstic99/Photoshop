#pragma once
#include <string>
#include <iostream>
#include "Rectangle.h"
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>
#include <array>



class Selection {
	std::string name;
	bool active=false;
	std::set<Rectangle> rectangles;
public:
	Selection(){}
	Selection(std::string s,std::vector<Rectangle> r,int a) :name(s),active(a) {	
	
		for (int i = 0; i < r.size(); i++) {
			rectangles.insert(r[i]);
		}
	}
	void setActive(bool a) { active = a; }
	void setActive() { active = true; }
	void setNoActive() { active = false; }
	bool isActive() { return active; }
	std::string getString() { return name; }
	std::set<Rectangle> getRectangles() { return rectangles; }
};
