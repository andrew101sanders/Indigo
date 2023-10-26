#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "resource.h"
#include <chrono>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <unordered_map>
#include <wincodec.h>

#include "lodepng.h"

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

#include "Win32Application.h"
#include "DirectXManager.h"
#include "Camera.h"
#include "GraphicsDevice.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "RenderState.h"
#include "ShaderProgram.h"
