#ifndef __Create_Graphic__
#define __Create_Graphic__

LRESULT CALLBACK WinProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

void saveShapes (HWND hwnd);

void loadShapes (HWND hwnd);

void setColor ();

void setBackground (int r, int g, int b);

#endif