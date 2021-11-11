#include "Shape.h"
#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>
#include <sstream>

/* Type /TBD to search for unimplemented */

int Shape::getX () const {

	return x;

}

int Shape::getY () const {

	return y;

}

int Shape::getRed () const {

	return red;

}

int Shape::getGreen () const {

	return green;

}

int Shape::getBlue () const {
		
	return blue;

}

int Shape::getType () const {

	return type;

}

void Shape::setX (int a) {

	x = a;

}

void Shape::setY (int a) {

	y = a;

}

void Shape::setXY (int a, int b) {

	x = a;

	y = b;

}

void Shape::setRed (int a) {

	red = a;

}

void Shape::setGreen (int a) {

	green = a;

}

void Shape::setBlue (int a) {

	blue = a;

}

void Shape::setColor (int r, int g, int b) {

	red = r;

	green = g;

	blue = b;

}

void Shape::setType (int a) {

	type = a;

}


Rectangle::Rectangle () : width(0), height(0) { 

	setXY(0, 0);

	setColor(0, 0, 0);

	setType(SHAPE_RECTANGLE);

}

Rectangle::Rectangle (int x, int y) : width(0), height(0) {

	setXY(x, y);

	setColor(0, 0, 0);

	setType(SHAPE_RECTANGLE);

}

Rectangle::Rectangle (RECT *r) {

	setXY(r->left, r->top);

	width = r->right - r->left;

	height = r->bottom - r->top;

	setColor(0, 0, 0);

	setType(SHAPE_RECTANGLE);

}

Rectangle::Rectangle (int x, int y, int width, int height) : width(width), height(height) {

	setXY(x, y);

	setColor(0, 0, 0);

	setType(SHAPE_RECTANGLE);

}

Rectangle::Rectangle (int x, int y, int width, int height, int r, int g, int b) : width(width), height(height) {

	setXY(x, y);

	setColor(r, g, b);

	setType(SHAPE_RECTANGLE);

}

Rectangle::Rectangle (RECT *rect, int r, int g, int b) {

	setXY(rect->left, rect->top);

	width = rect->right - rect->left;

	height = rect->bottom - rect->top;

	setColor(r, g, b);

	setType(SHAPE_RECTANGLE);

}

Rectangle::Rectangle (char *line) {

	int i, j;

	int currentI = 0;

	int spaceI;

	int values[8];

	int length;

	for (i = 0; i < 8; i++) {

		for (spaceI = currentI; line[spaceI] != ' ' && line[spaceI] != '\r'; spaceI++);

		char numTemp[spaceI - currentI + 1];

		numTemp[spaceI - currentI] = '\0';

		for (j = 0; j < spaceI - currentI; j++) {

			numTemp[j] = line[currentI + j];

		}

		values[i] = atoi(numTemp);

		currentI = spaceI + 1;

	}

	setType(SHAPE_RECTANGLE);

	setColor(values[1], values[2], values[3]);

	setXY(values[4], values[5]);

	setWidth(values[6]);

	setHeight(values[7]);


}

int Rectangle::getWidth () const {

	return width;

}

int Rectangle::getHeight () const {

	return height;

}

void Rectangle::setWidth (int a) {

	width = a;

}

void Rectangle::setHeight (int a) {

	height = a;

}

void Rectangle::paint (HDC *hdc) {

	HPEN pen = (HPEN) CreatePen(PS_SOLID, 1, RGB(getRed(), getGreen(), getBlue()));

	HBRUSH brush = (HBRUSH) CreateSolidBrush(RGB(getRed(), getGreen(), getBlue()));

	HPEN prevPen = (HPEN) SelectObject(*hdc, pen);

	HBRUSH prevBrush = (HBRUSH) SelectObject(*hdc, brush);

	POINT points[4] = { {getX(), getY()}, {getX() + width, getY()}, {getX() + width, getY() + height}, {getX(), getY() + height}};

	Polygon(*hdc, points, 4);

	SelectObject(*hdc, prevPen);

	SelectObject(*hdc, prevBrush);

	DeleteObject(pen);

	DeleteObject(brush);

}

bool Rectangle::touching (int x, int y) {

	if (x < getX() || x > getX() + width) {

		return false;

	}

	if (y < getY() || y > getY() + height) {

		return false;

	}

	return true;

}

void Rectangle::resize (double factor) {

	int centX = getX() + (width / 2);

	int centY = getY() + (height / 2);

	width = (int) (width * factor);

	height = (int) (height * factor);

	setXY(centX - (width / 2), centY - (height / 2 ));

}

void Rectangle::rotate (double angle) {

/* TBD */	

}

void Rectangle::translate (double x, double y) {

	setXY(getX() + (int) x, getY() + (int) y);

}

void Rectangle::saveShape (HANDLE *fout) {

	std::ostringstream print;

	print << getType() << " " << getRed() << " " << getGreen() << " " << getBlue() << " " << getX() << " " << getY() << " " << getWidth() << " " << getHeight() << "\r\n";

	std::string temp = print.str();

	char buffer[temp.length() + 1];

	int i;

	for (i = 0; i < temp.length(); i++) {

		buffer[i] = temp[i];

	}

	int count = 0;

	WriteFile(*fout, (LPCVOID) buffer, temp.length(), (LPDWORD) &count, NULL); 

}


Circle::Circle () : radius(0) {

	setXY(0, 0);

	setColor(0, 0, 0);

	setType(SHAPE_CIRCLE);

}

Circle::Circle (int x, int y) : radius(0) {

	setXY(x, y);

	setColor(0, 0, 0);

	setType(SHAPE_CIRCLE);

}

Circle::Circle (int x, int y, int radius) : radius(radius) {

	setXY(x, y);

	setColor(0, 0, 0);

	setType(SHAPE_CIRCLE);

}

Circle::Circle (int x, int y, int radius, int r, int g, int b) : radius(radius) {

	setXY(x, y);

	setColor(r, g, b);

	setType(SHAPE_CIRCLE);

}

Circle::Circle (char *line) {

	int i, j;

	int currentI = 0;

	int spaceI;

	int values[7];

	int length;

	for (i = 0; i < 7; i++) {

		for (spaceI = currentI; line[spaceI] != ' ' && line[spaceI] != '\r'; spaceI++);

		char numTemp[spaceI - currentI + 1];

		numTemp[spaceI - currentI] = '\0';

		for (j = 0; j < spaceI - currentI; j++) {

			numTemp[j] = line[currentI + j];

		}

		values[i] = atoi(numTemp);

		currentI = spaceI + 1;

	}

	setType(SHAPE_CIRCLE);

	setColor(values[1], values[2], values[3]);

	setXY(values[4], values[5]);

	setRadius(values[6]);

}

int Circle::getRadius () const {

	return radius;

}

void Circle::setRadius (int a) {

	radius = a;

}

void Circle::paint (HDC *hdc) {

	HPEN pen = (HPEN) CreatePen(PS_SOLID, 1, RGB(getRed(), getGreen(), getBlue()));

	HBRUSH brush = (HBRUSH) CreateSolidBrush(RGB(getRed(), getGreen(), getBlue()));

	HPEN prevPen = (HPEN) SelectObject(*hdc, pen);

	HBRUSH prevBrush = (HBRUSH) SelectObject(*hdc, brush);

	Ellipse(*hdc, getX() - radius, getY() - radius, getX() + radius, getY() + radius);

	SelectObject(*hdc, prevPen);

	SelectObject(*hdc, prevBrush);

	DeleteObject(pen);

	DeleteObject(brush);

}

bool Circle::touching (int x, int y) {

	if (((getX() - x) * (getX() - x)) + ((getY() - y) * (getY() - y)) <= (radius * radius)) {

		return true;

	}

	return false;

}

void Circle::resize (double factor) {

	radius = (int) (radius * factor);

}

void Circle::rotate (double angle) {


}

void Circle::translate (double x, double y) {

	setXY(getX() + (int) x, getY() + (int) y);

}

void Circle::saveShape (HANDLE *fout) {

	std::ostringstream print;

	print << getType() << " " << getRed() << " " << getGreen() << " " << getBlue() << " " << getX() << " " << getY() << " " << getRadius() << "\r\n";

	std::string temp = print.str();

	char buffer[temp.length() + 1];

	int i;

	for (i = 0; i < temp.length(); i++) {

		buffer[i] = temp[i];

	}

	int count = 0;

	WriteFile(*fout, (LPCVOID) buffer, temp.length(), (LPDWORD) &count, NULL); 


}

Triangle::Triangle () {

	setXY(0, 0);

	vertices[0].x = 0;

	vertices[0].y = 0;

	vertices[1].x = 0;

	vertices[1].y = 0;

	vertices[2].x = 0;

	vertices[2].y = 0;

	setColor(0, 0, 0);

	setType(SHAPE_TRIANGLE);

} 

Triangle::Triangle (POINT points[3]) {

	setXY(points[0].x, points[0].y);

	vertices[0] = points[0];

	vertices[1] = points[1];

	vertices[2] = points[2];

	setColor(0, 0, 0);

	setType(SHAPE_TRIANGLE);

}

Triangle::Triangle (int x1, int y1, int x2, int y2, int x3, int y3) {

	setXY(x1, y1);

	vertices[0].x = x1;

	vertices[0].y = y1;

	vertices[1].x = x2;

	vertices[1].y = y2;

	vertices[2].x = x3;

	vertices[2].y = y3;

	setColor(0, 0, 0);
	
	setType(SHAPE_TRIANGLE);

}

Triangle::Triangle (POINT points[3], int r, int g, int b) {

	setXY(points[0].x, points[0].y);

	vertices[0] = points[0];

	vertices[1] = points[1];

	vertices[2] = points[2];

	setColor(r, g, b);

	setType(SHAPE_TRIANGLE);

}

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b) {

	setXY(x1, y1);

	vertices[0].x = x1;

	vertices[0].y = y1;

	vertices[1].x = x2;

	vertices[1].y = y2;

	vertices[2].x = x3;

	vertices[2].y = y3;

	setColor(r, g, b);
	
	setType(SHAPE_TRIANGLE);

}

POINT Triangle::getVertex (int i) const {

	return vertices[i % 3];

}

void Triangle::setVertex (int i, int x, int y) {

	vertices[i % 3].x = x;
	
	vertices[i % 3].y = y;

}

void Triangle::setVertex (int i, POINT p) {

	vertices[i % 3].x = p.x;

	vertices[i % 3].y = p.y;

}

void Triangle::paint (HDC* hdc) {

	HPEN pen = (HPEN) CreatePen(PS_SOLID, 1, RGB(getRed(), getGreen(), getBlue()));

	HBRUSH brush = (HBRUSH) CreateSolidBrush(RGB(getRed(), getGreen(), getBlue()));

	HPEN prevPen = (HPEN) SelectObject(*hdc, pen);

	HBRUSH prevBrush = (HBRUSH) SelectObject(*hdc, brush);

	Polygon(*hdc, vertices, 3);

	SelectObject(*hdc, prevPen);

	SelectObject(*hdc, prevBrush);

	DeleteObject(pen);

	DeleteObject(brush);

}

bool Triangle::touching (int x, int y) {

/* TBD */

}

void Triangle::resize (double factor) {

/* TBD */

}

void Triangle::rotate (double angle) {

/* TBD */

}

void Triangle::translate (double x, double y) {

	int i;

	for (i = 0; i < 3; i++) {

		vertices[i].x += (int) x;

		vertices[i].y += (int) y;

	}

	setXY(vertices[0].x, vertices[0].y);

}

void Triangle::saveShape (HANDLE *fout) {



}


Curve::Curve () : width(5) {

	points = new std::vector<POINT>;

	setXY(0, 0);

	setColor(0, 0, 0);

	setType(SHAPE_CURVE);

}

Curve::Curve (int x, int y) : width(5) {

	points = new std::vector<POINT>;

	setXY(x, y);

	setColor(0, 0, 0);

	setType(SHAPE_CURVE);

}

Curve::Curve (int x, int y, int red, int green, int blue) : width(5) {

	points = new std::vector<POINT>;

	setXY(x, y);

	setColor(red, green, blue);

	setType(SHAPE_CURVE);

}

Curve::Curve (char *line) {

	int isCurve = 3;

	if (line[0] == SHAPE_ERASE + 48) {
	
		isCurve = 0;

	}

	int i, j;

	int currentI = 0;

	int spaceI;

	int values[4 + isCurve];

	int length;

	for (i = 0; i < 4 + isCurve; i++) {

		for (spaceI = currentI; line[spaceI] != ' ' && line[spaceI] != '\r'; spaceI++);

		char numTemp[spaceI - currentI + 1];

		numTemp[spaceI - currentI] = '\0';

		for (j = 0; j < spaceI - currentI; j++) {

			numTemp[j] = line[currentI + j];

		}

		values[i] = atoi(numTemp);

		currentI = spaceI + 1;

	}

	if (isCurve) {

		setType(SHAPE_CURVE);

		setColor(values[1], values[2], values[3]);

		setWidth(values[4]);
	
		setXY(values[5], values[6]);

	} else {

		setType(SHAPE_ERASE);

		setWidth(values[1]);

		setXY(values[2], values[3]);

	}

	int spaces = 0;

	while (line[currentI] != '\r') {

		if (line[currentI] == ' ') {

			spaces++;

		}

		currentI++;

	}

	points = new std::vector<POINT>();

	currentI = spaceI + 1;

	for (i = 0; i < (spaces + 1) / 2; i++) {

		POINT p;

		for (spaceI = currentI; line[spaceI] != ' ' && line[spaceI] != '\r'; spaceI++);

		char xTemp[spaceI - currentI + 1];

		xTemp[spaceI - currentI] = '\0';

		for (j = 0; j < spaceI - currentI; j++) {

			xTemp[j] = line[currentI + j];

		}

		p.x = atoi(xTemp);

		currentI = spaceI + 1;


		for (spaceI = currentI; line[spaceI] != ' ' && line[spaceI] != '\r'; spaceI++);

		char yTemp[spaceI - currentI + 1];

		yTemp[spaceI - currentI] = '\0';

		for (j = 0; j < spaceI - currentI; j++) {

			yTemp[j] = line[currentI + j];

		}

		p.y = atoi(yTemp);

		currentI = spaceI + 1;

		points->push_back(p);

	}


}

Curve::~Curve () {

	points->clear();

	delete points;

	points = NULL;

}

std::vector<POINT> * Curve::getPoints () const {

	return points;

}

int Curve::getWidth () const {

	return width;

}

void Curve::setWidth (int x) {
	
	this->width = x;

}

void Curve::addPoint (POINT p) {

	points->push_back(p);

}

void Curve::addPoint (int x, int y) {

	POINT p;

	p.x = x;

	p.y = y;

	points->push_back(p);

}

void Curve::paint (HDC *hdc) {

	HPEN pen = (HPEN) CreatePen(PS_SOLID, width, RGB(getRed(), getGreen(), getBlue()));

	HPEN prev = (HPEN) SelectObject(*hdc, pen);

	if (points->size() == 0) {

		Ellipse(*hdc, getX() - width / 2, getY() - width / 2, getX() + width / 2, getY() + width / 2);
	
	} else {

		POINT arr[points->size()];

		int i;

		for (i = 0; i < points->size(); i++) {

			arr[i] = (*points)[i];

		}
	
		Polyline(*hdc, arr, points->size());

	}
	
	SelectObject(*hdc, prev);

	DeleteObject(pen);

}

bool Curve::touching (int x, int y) {

	return false;

}

void Curve::resize (double factor) {

	return;

}

void Curve::rotate (double angle) {

	return;

}

void Curve::translate (double x, double y) {

	return;

}

void Curve::saveShape (HANDLE *fout) {

	std::ostringstream print;

	print << getType() << " ";

	if (getType() == SHAPE_CURVE) {

		print << getRed() << " " << getGreen() << " " << getBlue() << " ";

	}

	print << getWidth() << " " << getX() << " " << getY() << " ";

	int i;

	for (i = 0; i < points->size(); i++) {

		print << (*points)[i].x << " " << (*points)[i].y << " ";

	}

	print << "\r\n";

	std::string temp = print.str();

	char buffer[temp.length() + 1];

	for (i = 0; i < temp.length(); i++) {

		buffer[i] = temp[i];

	}

	int count = 0;

	WriteFile(*fout, (LPCVOID) buffer, temp.length(), (LPDWORD) &count, NULL); 

}
