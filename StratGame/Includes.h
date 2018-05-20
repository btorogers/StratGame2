#ifndef _INCLUDES_H_
#define _INCLUDES_H_
#pragma once

#include <windows.h>
//#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <bitset>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#define GRID_X 25
#define GRID_Y 25
#define TICK_INTERVAL 20

#define COLOR(r,g,b) D3DXCOLOR(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f)
#define RANDCOL D3DXCOLOR(rand() / 32767.0f, rand() / 32767.0f, rand() / 32767.0f, 1.0f)
#define RANDINT(x) (rand() / 32767.0f * x)
#define RANDPOSNEG(x) (RANDINT(x*2)-x)

#endif

/*
std::stringstream stream;
stream << p.x << ", " << p.y << std::endl;
OutputDebugString(stream.str().c_str());
*/

// TRANSPOSE YOUR GODDAMN MATRICES