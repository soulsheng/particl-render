// LightDefinition.h : 灯光处理模块共同需要的h文件，
//

#pragma once

#include <string>
#include <map>
#include <sstream>
using namespace std;

typedef std::string String;

namespace vgKernel{

struct Vec3 
{
	float x,y,z;
	Vec3(float xVal, float yVal, float zVal)
		:x(xVal), y(yVal), z(zVal) {}
	Vec3(float val=0.0f)
		:x(val), y(val), z(val) {}
};


}

#define PI		3.1415926

#define PARTICLECOUNT	100000