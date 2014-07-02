#ifndef __SYS_WINDOW_H__
#define __SYS_WINDOW_H__

#include "sys_main_header.h"


#define CLASS_NAME	"QoomGame"

struct MainGameWindow {
	HWND		hWnd;
	HINSTANCE	hInstance;

	INT width;
	INT height;

	BOOL done;

};

#endif