// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#pragma once

#ifndef PCH_H
#define PCH_H

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <Windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <unordered_map>
#include <wincodec.h>
#include <functional>
#include <vector>
#include <string>

#include <d3d11.h>
#include <DirectXMath.h>
#include <D3Dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3Dcompiler.lib")

// Direct2D Stuff
#include <d2d1_2.h>
#include <d2d1_1.h>
#include <d2d1.h>
#include <dwrite.h>

#pragma comment (lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace DirectX;

#include "lodepng.h"

#pragma warning(push)
#pragma warning(disable: 4996)
#include "cgltf_write.h"
#include "cgltf.h"
#pragma warning(pop)

#endif //PCH_H
