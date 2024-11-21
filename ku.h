#pragma once


// std
#include <cstdio>
#include <iostream>
#include <string>

#define _USE_MATH_DEFINES
#include <cmath>

// glew
#define GLEW_STATIC                     //  π”√ glew æ≤Ã¨ø‚
#include <glew/glew.h>

// glfw
#include <glfw/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

// SOIL2
#include <SOIL2/SOIL2.h>

// imgui
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl2.h>

#if defined(__BORLANDC__ ) || defined(_MSC_VER)

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#if defined(_M_AMD64) || defined(_M_X64)
#pragma comment(lib, "x64/glew32s.lib")
#pragma comment(lib, "x64/glfw3.lib")
#pragma comment(lib, "x64/SOIL2.lib")
#pragma comment(lib, "x64/imgui.lib")
#else
#pragma comment(lib, "x86/glew32s.lib")
#pragma comment(lib, "x86/glfw3.lib")
#pragma comment(lib, "x86/SOIL2.lib")
#pragma comment(lib, "x86/imgui.lib")
#endif
#endif

