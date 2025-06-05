#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")


#include <DirectXMath.h>
using namespace DirectX;

#include "DirectXTex.h"

#if _DEBUG
#pragma comment(lib,"DirectXTex_Debug.lib")
#else
#pragma comment(lib,"DirectXTex_Release.lib")
#endif

#pragma comment (lib, "winmm.lib")


#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)

HWND GetWindow();
float GetMouseWheelDelta();

//float dot(const XMFLOAT3 vec1, const XMFLOAT3 vec2) 
//{
//	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
//}
//
//float Length(XMFLOAT3 vec1, XMFLOAT3 vec2)
//{
//	float vx = vec1.x - vec2.x;
//	float vy = vec1.y - vec2.y;
//	float vz = vec1.z - vec2.z;
//
//	float length = sqrt(vx * vx + vy * vy + vz * vz);
//
//
//	return length;
//}



//void Invoke(std::function<void()> Function, int Time);

