#include "stdafx.h"
#include "COclManager.h"

#include "common\utils.h"

#define FILENAME_MS3D "data/tortoise.ms3d"


COclManager::COclManager()
{
	m_model = new MilkshapeModel[COUNT_MODEL];

}

COclManager::~COclManager()
{

	if ( m_model )
	{
		delete[] m_model;
		m_model = NULL;
	}
}

bool COclManager::Setup_OpenCL( const char *program_source , const char *kernel_name, const char *platform_name, tagPropOCL* _propOCL )
{
	
	cl_device_id devices[16];
	size_t cb;
	cl_uint size_ret = 0;
	cl_int err;
	int num_cores;
	char device_name[128] = {0};

	static const char buildOpts[] = "-cl-fast-relaxed-math";

	cl_platform_id intel_platform_id = GetIntelOCLPlatform( platform_name );
	if( intel_platform_id == NULL )
	{
		printf("ERROR: Failed to find Intel OpenCL platform.\n");
		return NULL;
	}

#if !ENABLE_CL_GL_INTER
	cl_context_properties context_properties[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties)intel_platform_id, NULL };

	// create the OpenCL context on a CPU/PG 
	_propOCL->g_context = clCreateContextFromType(context_properties, CL_DEVICE_TYPE_CPU, NULL, NULL, NULL);
#else

	cl_context_properties properties[] = {
		CL_GL_CONTEXT_KHR, (cl_context_properties) wglGetCurrentContext(), //获得OpenGL上下文
		CL_WGL_HDC_KHR, (cl_context_properties) wglGetCurrentDC(), //获得OpenGl设备信息
		CL_CONTEXT_PLATFORM, (cl_context_properties) intel_platform_id,  //获得平台信息
		0};

		if (!clGetGLContextInfoKHR) 
		{
			clGetGLContextInfoKHR = (clGetGLContextInfoKHR_fn) clGetExtensionFunctionAddressForPlatform(intel_platform_id, "clGetGLContextInfoKHR");
			if (!clGetGLContextInfoKHR) 
			{
				std::cout << "Failed to query proc address for clGetGLContextInfoKHR";
				return false;
			}
		}
#if 0
		size_t deviceSize = 0;
		cl_int status = clGetGLContextInfoKHR( properties, 
			CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR,
			0, 
			NULL, 
			&deviceSize);

		if (CL_SUCCESS != status || 0 == deviceSize )
			deviceSize=0;
#endif
		// 获取支持GL-CL互操作的OpenCL设备的ID
		cl_int status = clGetGLContextInfoKHR( properties, 
			CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR,
			sizeof(cl_device_id), 
			&_propOCL->g_device_ID, 
			NULL);

		//利用刚刚获取的设备ID创建上下文
		_propOCL->g_context = clCreateContext(properties, 1, &_propOCL->g_device_ID, NULL, NULL, &err);
#endif

		if (_propOCL->g_context == (cl_context)0 )
			return NULL;

		// get the list of CPU devices associated with context
		err = clGetContextInfo(_propOCL->g_context, CL_CONTEXT_DEVICES, 0, NULL, &cb);


#if !ENABLE_CL_GL_INTER
		clGetContextInfo(_propOCL->g_context, CL_CONTEXT_DEVICES, cb, devices, NULL);
		_propOCL->g_device_ID = devices[0];
#endif

		_propOCL->g_cmd_queue = clCreateCommandQueue(_propOCL->g_context, _propOCL->g_device_ID, 0, NULL);
		if (_propOCL->g_cmd_queue == (cl_command_queue)0)
		{
			Cleanup();
			return NULL;
		}

	char *sources = ReadSources(program_source);	//read program .cl source file
	_propOCL->g_program = clCreateProgramWithSource(_propOCL->g_context, 1, (const char**)&sources, NULL, NULL);
	if (_propOCL->g_program == (cl_program)0)
	{
		printf("ERROR: Failed to create Program with source...\n");
		Cleanup();
		free(sources);
		return NULL;
	}

	err = clBuildProgram(_propOCL->g_program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS)
	{
		printf("ERROR: Failed to build program...\n");
		Cleanup();
		free(sources);
		return NULL;
	}

	_propOCL->g_kernel = clCreateKernel(_propOCL->g_program, kernel_name, NULL);

	if (_propOCL->g_kernel == (cl_kernel)0)
	{
		printf("ERROR: Failed to create kernel...\n");
		Cleanup();
		free(sources);
		return NULL;
	}
	
	free(sources);

	// use first device ID
	//_propOCL->g_device_ID = devices[0];
	err = clGetDeviceInfo(_propOCL->g_device_ID, CL_DEVICE_NAME, 128, device_name, NULL);
	if (err!=CL_SUCCESS)
	{
		printf("ERROR: Failed to get device information (device name)...\n");
		Cleanup();
		return NULL;
	}
	printf("Using device %s...\n", device_name);

	err = clGetDeviceInfo(_propOCL->g_device_ID, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &num_cores, NULL);
	if (err!=CL_SUCCESS)
	{
		printf("ERROR: Failed to get device information (max compute units)...\n");
		Cleanup();
		return NULL;
	}
	printf("Using %d compute units...\n", num_cores);


	return true; // success...
}

void COclManager::Cleanup()
{
	
}

void COclManager::initialize()
{
	
}
