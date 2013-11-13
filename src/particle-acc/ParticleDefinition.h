// LightDefinition.h : �ƹ⴦��ģ�鹲ͬ��Ҫ��h�ļ���
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

// ˮ��ṹ�� struct tagDROP
typedef struct	tagDROP					
{
	vgKernel::Vec3	vgen;			//	��������ϵ(r , a, b)����״����ģʽ
	vgKernel::Vec3	position;		//  ��������ϵ(x , y, z)
	vgKernel::Vec3	vlen;			//  �ٶ�
	vgKernel::Vec3	acc;			//  ���ٶ�
	int		lifeFrame;				//	������
	bool	active;					//	����״̬

} tagDROP, * LPDROP;  // ˮ��ṹ�� struct tagDROP

struct tagPropOCL
{
	cl_context	g_context;
	cl_command_queue g_cmd_queue;
	cl_program	g_program;
	cl_kernel	g_kernel;

	cl_uint     g_min_align;
	cl_device_id g_device_ID;
};

#define PI		3.1415926

#define PARTICLECOUNT	100000

