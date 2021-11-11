#ifndef __SHAPE__
#define __SHAPE__
#include <windows.h>
#include <wingdi.h>
#include <vector>

#define SHAPE_RECTANGLE 1
#define SHAPE_CIRCLE 2
#define SHAPE_TRIANGLE 3
#define SHAPE_CURVE 4
#define SHAPE_ERASE 5

class Shape {

	private:

	int x;

	int y;

	int red;

	int green;

	int blue;

	int type;

	public:

	int getX () const;

	int getY () const;

	int getRed () const;
	
	int getGreen () const;

	int getBlue () const;

	int getType () const;

	void setX (int a);

	void setY (int a);

	void setXY (int a, int b);

	void setRed (int a);

	void setGreen (int a);

	void setBlue (int a);

	void setColor (int r, int g, int b);

	virtual void paint (HDC *hdc) = 0;

	virtual bool touching (int x, int y) = 0;

	virtual void resize (double factor) = 0;

	virtual void rotate (double angle) = 0;

	virtual void translate (double x, double y) = 0;

	virtual void saveShape (HANDLE *fout) = 0;

	void setType (int a);

};

class Rectangle : public Shape {

	private:

	int width;

	int height;

	public:

	Rectangle ();

	Rectangle (int x, int y);

	Rectangle (RECT *r);

	Rectangle (int x, int y, int width, int height);

	Rectangle (int x, int y, int width, int height, int r, int g, int b);

	Rectangle (RECT *rect, int r, int g, int b);

	Rectangle (char *line);

	int getWidth () const;

	int getHeight () const;

	void setWidth (int a);

	void setHeight (int a);

	void paint (HDC *hdc);

	bool touching (int x, int y);

	void resize (double factor);

	void rotate (double angle);

	void translate (double x, double y);

	void saveShape (HANDLE *fout);	

};

class Circle : public Shape {

	private:

	int radius;

	public:

	Circle ();

	Circle (int x, int y);
	
	Circle (int x, int y, int radius);

	Circle (int x, int y, int radius, int r, int g, int b);

	Circle (char *line);

	int getRadius () const;

	void setRadius (int a);

	void paint (HDC *hdc);

	bool touching (int x, int y);

	void resize (double factor);

	void rotate (double angle);

	void translate (double x, double y);

	void saveShape (HANDLE *fout);

};

class Triangle : public Shape {

	private:

	POINT vertices[3];

	public:

	Triangle ();

	Triangle (POINT points[3]);

	Triangle (int x1, int y1, int x2, int y2, int x3, int y3);

	Triangle (POINT points[3], int r, int g, int b);

	Triangle (int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b);

	Triangle (char *line);

	POINT getVertex (int i) const;

	void setVertex (int i, int x, int y);

	void setVertex (int i, POINT p);

	void paint (HDC* hdc);

	bool touching (int x, int y);

	void resize (double factor);

	void rotate (double angle);

	void translate (double x, double y);

	void saveShape (HANDLE *fout);

};

class Curve : public Shape {

	private:

	std::vector<POINT> *points;

	int width;

	public:

	Curve ();

	Curve (int x, int y);

	Curve (int x, int y, int red, int green, int blue);

	Curve (char *line);

	~Curve ();

	std::vector<POINT> * getPoints () const;

	int getWidth () const;

	void setWidth (int x);

	void addPoint (POINT p);

	void addPoint (int x, int y);

	void paint (HDC *hdc);

	bool touching (int x, int y);

	void resize (double factor);

	void rotate (double angle);

	void translate (double x, double y);

	void saveShape (HANDLE *fout);
	
};

#endif
