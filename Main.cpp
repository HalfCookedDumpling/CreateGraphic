#include <windows.h>
#include <windowsx.h>
#include <cmath>
#include <vector>
#include <fstream>
#include "Main.h"
#include "Keyboard.h"
#include "Shape.h"
#include "Mouse.h"
#include "IconResource.h"

#define IDLE_MODE 0
#define RECTANGLE_MODE 1
#define CIRCLE_MODE 2
#define TRIANGLE_MODE 3
#define BRUSH_MODE 4
#define ERASE_MODE 5
#define SELECTION_MODE 6

/* Variables to hold the shapes drawn to the screen including the currently selected object */

static std::vector<Shape *> *shapes;

/* Variables to hold the pages of shapes */

static std::vector<std::vector<Shape *> *> pages;

class Shape *selected = NULL;

/* Variables for keeping track of undos and redos. Does not yet support undoing mass delete using d key */

static int nShapesDrawn = 0;

/* Global variable for showing which mode is being used to alter the screen */

static int mode;

/* Global variables for the current color of drawing objects */

static int red = 0;

static int green = 0;

static int blue = 200;

/* Global Variables for the current color of the drawing canvas */

static int backRed = 0;

static int backGreen = 0;

static int backBlue = 0;

/* Global Variables for changing the size of the pen and eraser */

static int brushSize = 5;

static int eraseSize = 20;

LRESULT CALLBACK WinProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

		case WM_CREATE:

		{

			RECT r;

			GetClientRect(hwnd, &r);
	
			shapes = new std::vector<Shape *>();

			shapes->push_back(new class Rectangle(&r, backRed, backGreen, backBlue));

			pages.push_back(shapes);

			break;

		}

		case WM_CLOSE:
		
			DestroyWindow(hwnd);

			int i;

			for (i = 0; i < shapes->size(); i++) {

				delete (*shapes)[i];

			}

			break;

		case WM_DESTROY:

			PostQuitMessage(0);

			break;

		case WM_SIZE: 
		
		{

			int width = LOWORD(lParam);

			int height = HIWORD(lParam);

			class Rectangle *back = (class Rectangle *) (*shapes)[0];

			back->setWidth(width);

			back->setHeight(height);

			break;

		}

		case WM_PAINT: 

		{

			PAINTSTRUCT ps;

			HDC hdc = BeginPaint(hwnd, &ps);

			HDC buffer = CreateCompatibleDC(hdc);

			class Rectangle *screen = (class Rectangle *) ((*shapes)[0]);

			HBITMAP bufBM = CreateCompatibleBitmap(hdc, screen->getWidth(), screen->getHeight());

			SelectObject(buffer, bufBM);

			int i;

			for (i = 0; i < shapes->size(); i++) {

				(*shapes)[i]->paint(&buffer);

			}

			BitBlt(hdc, screen->getX(), screen->getY(), screen->getWidth(), screen->getHeight(), buffer, screen->getX(), screen->getY(), SRCCOPY);

			DeleteObject(bufBM);

			DeleteDC(buffer);

			EndPaint(hwnd, &ps);

			break;

		}

		case WM_ERASEBKGND: 

		{	

			return TRUE;

		}

		case WM_KEYDOWN:

		{

			Keyboard::getKeyboard()->press(wParam);

			if (wParam == VK_ESCAPE) {

				mode = IDLE_MODE;

			}
		
			if (wParam == 'R') {
				
				mode = RECTANGLE_MODE;

			}

			if (wParam == 'C') {

				mode = CIRCLE_MODE;

			}

			if (wParam == 'T') {

				/*

				mode = TRIANGLE_MODE;

				*/

			}

			if (wParam == 'B') {

				mode = BRUSH_MODE;

			}

			if (wParam == 'E') {

				mode = ERASE_MODE;

			}

			if (wParam == 'U') {

				if (nShapesDrawn > 0) {
				
					nShapesDrawn--;

				}

			}

			if (wParam == 'D') {

				mode = IDLE_MODE;

				int i;

				for (i = 0; i < shapes->size(); i++) {

					delete (*shapes)[i];

				}

				shapes->clear();

				RECT r;

				GetClientRect(hwnd, &r);

				shapes->push_back(new class Rectangle(&r, backRed, backGreen, backBlue));

			}

			if (wParam == 'S' && Keyboard::getKeyboard()->isDown(VK_CONTROL)) {

				saveShapes(hwnd);

				Keyboard::getKeyboard()->release(VK_CONTROL);


			}

			if (wParam == 'O' && Keyboard::getKeyboard()->isDown(VK_CONTROL)) {

				loadShapes(hwnd);

				Keyboard::getKeyboard()->release(VK_CONTROL);

			}

			if (wParam == VK_OEM_4) {

				if (mode == SHAPE_ERASE && eraseSize > 1) {

					eraseSize--;


				} else if (mode == SHAPE_CURVE && brushSize > 1) {
					
					brushSize--;

				}

			}

			if (wParam == VK_OEM_6) {

				if (mode == SHAPE_ERASE && eraseSize < 100) {

					eraseSize++;

				} else if (mode == SHAPE_CURVE && brushSize < 100) {

					brushSize++;

				}

			}

			setColor();

			InvalidateRect(hwnd, NULL, FALSE);

			break;

		}

		case WM_KEYUP:

		{

			Keyboard::getKeyboard()->release(wParam);

			break;

		}

		case WM_LBUTTONDOWN:

		{

			Mouse::getMouse()->pressLButton();

			/* Deals with creating new rectangles */

			if (mode == RECTANGLE_MODE) {

				shapes->push_back(new class Rectangle(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));

				(*shapes)[shapes->size() - 1]->setColor(red, green, blue);

			/* Deals with creating new circles */

			} else if (mode == CIRCLE_MODE) {

				shapes->push_back(new Circle(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));

				(*shapes)[shapes->size() - 1]->setColor(red, green, blue);

			} else if (mode == TRIANGLE_MODE) {

				/* Insert code handling triangles */

			} else if (mode == BRUSH_MODE) {

				Curve *brush = new Curve(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), red, green, blue);

				brush->setWidth(brushSize);

				shapes->push_back(brush);

			} else if (mode == ERASE_MODE) {

				Curve *eraser = new Curve(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), backRed, backGreen, backBlue);
			
				eraser->setWidth(eraseSize);

				eraser->setType(SHAPE_ERASE);

				shapes->push_back(eraser);

			}

			if (mode != IDLE_MODE) {

				selected = (*shapes)[shapes->size() - 1];

				InvalidateRect(hwnd, NULL, FALSE);

			/* Selects an existing object if its in idle mode and sets it to selection mode */

			} else {

				int x = GET_X_LPARAM(lParam);

				int y = GET_Y_LPARAM(lParam);

				int i;

				for (i = shapes->size() - 1; i > 0; i--) {

					if ((*shapes)[i]->touching(x, y)) {

						selected = (*shapes)[i];

						mode = SELECTION_MODE;

						break;

					}
		
				}

				Mouse::getMouse()->setXYTemp(x, y);

			}

			break;

		}

		case WM_LBUTTONUP:

		{

			Mouse::getMouse()->releaseLButton();

			/* Deselect an object */

			if (selected != NULL) {

				selected = NULL;

				InvalidateRect(hwnd, NULL, FALSE);

			}

			/* Set back to IDLE_MODE after selection */

			if (mode == SELECTION_MODE) {

				mode = IDLE_MODE;

			}

			break;

		}

		case WM_MOUSEMOVE:

		{

			Mouse *m = Mouse::getMouse();

			int x = GET_X_LPARAM(lParam);

			int y = GET_Y_LPARAM(lParam);

			m->set(x, y);

			/* Changes the size of the rectangle as you click and drag it */

			if (m->lButtonPressed() && selected != NULL && mode == RECTANGLE_MODE) {

				class Rectangle *temp = (class Rectangle *) selected;

				temp->setWidth(x - selected->getX());

				temp->setHeight(y - selected->getY());

			/* Changes the size of the circle as you click and drag it */

			} else if (m->lButtonPressed() && selected != NULL && mode == CIRCLE_MODE) {

				Circle *temp = (Circle *) selected;

				temp->setRadius(sqrt(((temp->getX() - x) * (temp->getX() - x)) + ((temp->getY() - y) * (temp->getY() - y))));

			/* Translates the shape as you click and move your mouse */

			} else if (m->lButtonPressed() && mode == SELECTION_MODE && selected != NULL) {
			
				selected->translate(x - m->getXTemp(), y - m->getYTemp());

				Mouse::getMouse()->setXYTemp(x, y);

			} else if (m->lButtonPressed() && (mode == BRUSH_MODE || ERASE_MODE) && selected != NULL) {

				Curve *temp = (Curve *) selected;

				POINT p;

				p.x = x;

				p.y = y;

				temp->addPoint(p);
				
			} 

			InvalidateRect(hwnd, NULL, FALSE);

			break;

		}
		
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);

}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	LPCWSTR className = L"Create Graphic";

	WNDCLASSEX wnd;

	wnd.cbSize = sizeof(WNDCLASSEX);
	
	wnd.cbClsExtra = 0;

	wnd.cbWndExtra = 0;

	wnd.style = 0;

	wnd.hbrBackground = NULL;

	wnd.hCursor = (HCURSOR) LoadCursor(NULL, IDC_ARROW);

	wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(POTATO_ICON));

	wnd.hIconSm = NULL;

	wnd.hInstance = hInstance;

	wnd.lpszMenuName = NULL;

	wnd.lpszClassName = className;

	wnd.lpfnWndProc = &WinProc;

	if (!RegisterClassEx(&wnd)) {

		MessageBox(NULL, L"Failed to register class", L"Failed to register class", MB_OK);

		return 0;

	}

	HWND screen;

	screen = CreateWindowEx(WS_EX_WINDOWEDGE, className, L" Create Graphic", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 720, 480, NULL, NULL, hInstance, NULL);

	ShowWindow(screen, nCmdShow);

	UpdateWindow(screen);

	MSG message;

	while (GetMessage(&message, NULL, 0, 0) > 0) {

		TranslateMessage(&message);

		DispatchMessage(&message);

	}

	return 0;

}

void saveShapes (HWND hwnd) {

	OPENFILENAME ofn;

	wchar_t szFileName[MAX_PATH];

	memset(szFileName, 0, sizeof(szFileName));

	const char *generic = "NewGraphic.txt";

	int i;

	for (i = 0; i < 14; i++) {

		szFileName[i] = generic[i];

	}

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	
	ofn.hwndOwner = hwnd;
	
	ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	
	ofn.lpstrFile = (LPTSTR) szFileName;
	
	ofn.nMaxFile = MAX_PATH;
	
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	
	ofn.lpstrDefExt = L"txt";

	if (GetSaveFileName(&ofn)) {

		HANDLE fout;

		fout = CreateFile((LPCWSTR) szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		int count;

		int i;

		for (i = 1; i < shapes->size(); i++) {

			(*shapes)[i]->saveShape(&fout);

		}
		
		CloseHandle(fout);

		/*

		int i;

		for (i = 0; i < shapes.size(); i++) {

			fout << shapes[i]->getType() << " ";

			if (shapes[i]->getType() == SHAPE_RECTANGLE) {

				class Rectangle *temp = (class Rectangle *) shapes[i];


			} else if (shapes[i]->getType() == SHAPE_CIRCLE) {

				Circle *temp = (Circle *) shapes[i];


			} else if (shapes[i]->getType() == SHAPE_TRIANGLE) {

				Triangle *temp = (Triangle *) shapes[i];


			} else if (shapes[i]->getType() == SHAPE_CURVE) {

				Curve *temp = (Curve *) shapes[i];


			} else if (shapes[i]->getType() == SHAPE_ERASE) {

				Curve *temp = (Curve *) shapes[i];

			}


		}

		*/
			
	}
}


void loadShapes (HWND hwnd) {

	OPENFILENAME ofn;

	wchar_t szFileName[MAX_PATH];

	memset(szFileName, 0, sizeof(szFileName));

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	
	ofn.hwndOwner = hwnd;
	
	ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	
	ofn.lpstrFile = (LPTSTR) szFileName;
	
	ofn.nMaxFile = MAX_PATH;
	
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	
	ofn.lpstrDefExt = L"txt";

	if (GetOpenFileName(&ofn)) {

		HANDLE fin;

		fin = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

		int bytes;

		bytes = GetFileSize(fin, NULL);

		if (bytes == 0xFFFFFFFF) {

			CloseHandle(fin);

			return;

		}

		int count = 0;

		char buffer[bytes + 1];

		ReadFile((HANDLE) fin, buffer, bytes, (LPDWORD) &count, NULL);

		int currentI = 0;

		int lineI = 0;

		int i;

		while (currentI < bytes) {

			for (lineI = currentI; buffer[lineI] != '\n'; lineI++);

			char lineBuffer[lineI - currentI + 2];

			lineBuffer[lineI - currentI + 1] = '\0';

			for (i = 0; i < lineI - currentI + 1; i++) {

				lineBuffer[i] = buffer[currentI + i];

			}

			if (lineBuffer[0] == SHAPE_RECTANGLE + 48) {

				shapes->push_back(new class Rectangle(lineBuffer));

			} else if (lineBuffer[0] == SHAPE_CIRCLE + 48) {

				shapes->push_back(new Circle(lineBuffer));

			} else if (lineBuffer[0] == SHAPE_CURVE + 48) {

				shapes->push_back(new Curve(lineBuffer));

			} else if (lineBuffer[0] == SHAPE_ERASE +48) {

				shapes->push_back(new Curve(lineBuffer));

				(*shapes)[shapes->size() - 1]->setColor(backRed, backGreen, backBlue);

			}

			currentI = lineI + 1;

		}

		CloseHandle(fin);

	}

}

void setColor () {

	Keyboard *k = Keyboard::getKeyboard();

	if (k->isDown(VK_CONTROL) && k->isDown('1')) {

		backRed = 0;

		backGreen = 0;

		backBlue = 0;

		(*shapes)[0]->setColor(0, 0, 0);

		int i;

		for (i = 1; i < shapes->size(); i++) {

			if ((*shapes)[i]->getType() == SHAPE_ERASE) {

				Curve *temp = (Curve *) ((*shapes)[i]);

				temp->setColor(backRed, backGreen, backBlue);

			}

		}

	} else if (k->isDown('1')) {

		red = 0;

		green = 0;

		blue = 0;

	}

	if (k->isDown(VK_CONTROL) && k->isDown('2')) {

		backRed = 255;

		backGreen = 255;

		backBlue = 255;

		(*shapes)[0]->setColor(255, 255, 255);

		int i;

		for (i = 1; i < shapes->size(); i++) {

			if ((*shapes)[i]->getType() == SHAPE_ERASE) {

				Curve *temp = (Curve *) ((*shapes)[i]);

				temp->setColor(backRed, backGreen, backBlue);

			}

		}


	} else if (k->isDown('2')) {

		red = 255;

		green = 255;

		blue = 255;

	}

	if (k->isDown(VK_CONTROL) && k->isDown('3')) {

		backRed = 255;

		backGreen = 0;

		backBlue = 0;

		(*shapes)[0]->setColor(255, 0, 0);

		int i;

		for (i = 1; i < shapes->size(); i++) {

			if ((*shapes)[i]->getType() == SHAPE_ERASE) {

				Curve *temp = (Curve *) ((*shapes)[i]);

				temp->setColor(backRed, backGreen, backBlue);

			}

		}

	} else if (k->isDown('3')) {

		red = 255;

		green = 0;

		blue = 0;

	}

	if (k->isDown(VK_CONTROL) && k->isDown('4')) {

		backRed = 0;

		backGreen = 255;

		backBlue = 0;

		(*shapes)[0]->setColor(0, 255, 0);

		int i;

		for (i = 1; i < shapes->size(); i++) {

			if ((*shapes)[i]->getType() == SHAPE_ERASE) {

				Curve *temp = (Curve *) ((*shapes)[i]);

				temp->setColor(backRed, backGreen, backBlue);

			}

		}

	} else if (k->isDown('4')) {

		red = 0;

		green = 255;

		blue = 0;

	}

	if (k->isDown(VK_CONTROL) && k->isDown('5')) {

		backRed = 0;

		backGreen = 0;

		backBlue = 255;

		(*shapes)[0]->setColor(0, 0, 255);

		int i;

		for (i = 1; i < shapes->size(); i++) {

			if ((*shapes)[i]->getType() == SHAPE_ERASE) {

				Curve *temp = (Curve *) ((*shapes)[i]);

				temp->setColor(backRed, backGreen, backBlue);

			}

		}

	} else if (k->isDown('5')) {

		red = 0;

		green = 0;

		blue = 255;

	}

	if (k->isDown(VK_CONTROL) && k->isDown('6')) {

		backRed = 255;

		backGreen = 255;

		backBlue = 0;

		(*shapes)[0]->setColor(255, 255, 0);

		int i;

		for (i = 1; i < shapes->size(); i++) {

			if ((*shapes)[i]->getType() == SHAPE_ERASE) {

				Curve *temp = (Curve *) ((*shapes)[i]);

				temp->setColor(backRed, backGreen, backBlue);

			}

		}

	} else if (k->isDown('6')) {

		red = 255;

		green = 255;

		blue = 0;

	}


}

void deleteUndoBuffer () {

	if (nShapesDrawn != shapes->size()) {



	}

}
