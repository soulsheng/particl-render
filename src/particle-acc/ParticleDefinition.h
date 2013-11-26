// LightDefinition.h : 灯光处理模块共同需要的h文件，
//

#pragma once

#include <string>
#include <map>
#include <sstream>
using namespace std;

typedef std::string String;

#include <CL/cl.h>
#include <CL/cl_gl.h>


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

struct vector4
{
	float x,y,z,w;
	vector4(float xVal, float yVal, float zVal, float wVal=1.0f)
		: x(xVal), y(yVal), z(zVal), w(wVal) {}
	vector4(float xVal = 0.0f)
		: x(xVal), y(xVal), z(xVal), w(xVal) {}

	vector4& operator+=(vector4& refVal)
	{
		this->x += refVal.x ;
		this->y += refVal.y ;
		this->z += refVal.z ;
		this->w += refVal.w ;

		return *this;
	}
};
// 水珠结构体 struct tagDROP
typedef struct	tagDROP					
{
	cl_float4	position;		//  世界坐标系(x , y, z)
	cl_float4	vlen;			//  速度
	cl_float4	acc;			//  加速度
	cl_float4	vgen;			//	球形坐标系(r , a, b)，柱状发射模式
	//int		lifeFrame;				//	生命期 position.w
	//bool	active;					//	激活状态 vgen.w

} tagDROP, * LPDROP;  // 水珠结构体 struct tagDROP

struct tagPropOCL
{
	cl_context	g_context;
	cl_command_queue g_cmd_queue;
	cl_program	g_program;
	cl_kernel	g_kernel;

	cl_uint     g_min_align;
	cl_device_id g_device_ID;

	tagPropOCL()
	{
		g_context = NULL;
		g_cmd_queue = NULL;
		g_program = NULL;
		g_kernel = NULL;

		g_min_align = 0;
		g_device_ID =0;
	}
	~tagPropOCL()
	{
		if( g_kernel ) {clReleaseKernel( g_kernel );  g_kernel = NULL;}
		if( g_program ) {clReleaseProgram( g_program );  g_program = NULL;}
		if( g_cmd_queue ) {clReleaseCommandQueue( g_cmd_queue );  g_cmd_queue = NULL;}
		if( g_context ) {clReleaseContext( g_context );  g_context = NULL;}
	}
};

#define PI		3.1415926

#define PARTICLECOUNT	100000

