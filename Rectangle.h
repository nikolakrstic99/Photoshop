#pragma once
#include <stdio.h>

class Rectangle {
	int row, column, width, height;
public:
	Rectangle(int r, int c, int w, int h) :row(r), column(c), width(w), height(h) {};
	int getRow() const{ return row; }
	int getColumn() const{ return column; }
	int getHeight() const{ return height; }
	int getWidth() const{ return width; }
	bool operator<(const Rectangle& _Right) const {
		if (this->row < _Right.row)return 1;
		else if (this->row > _Right.row)return 0;
		else if (this->column < _Right.row)return 1;
		else if (this->column > _Right.row) return 0;
		if (this->height == _Right.height && this->width == _Right.width)return 0; ////?????????????
		else return 1;
	}
};
