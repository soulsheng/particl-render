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

tagPropOCL* COclManager::Setup_OpenCL( const char *program_source , const char *kernel_name)
{
	
	cl_device_id devices[16];
	size_t cb;
	cl_uint size_ret = 0;
	cl_int err;
	int num_cores;
	char device_name[128] = {0};

	static const char buildOpts[] = "-cl-fast-relaxed-math";

	cl_platform_id intel_platform_id = GetIntelOCLPlatform();
	if( intel_platform_id == NULL )
	{
		printf("ERROR: Failed to find Intel OpenCL platform.\n");
		return NULL;
	}

#if !ENABLE_CL_GL_INTER
	cl_context_properties context_properties[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties)intel_platform_id, NULL };

	// create the OpenCL context on a CPU/PG 
	m_propOCL.g_context = clCreateContextFromType(context_properties, CL_DEVICE_TYPE_CPU, NULL, NULL, NULL);
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
			&m_propOCL.g_device_ID, 
			NULL);

		//利用刚刚获取的设备ID创建上下文
		m_propOCL.g_context = clCreateContext(properties, 1, &m_propOCL.g_device_ID, NULL, NULL, &err);
#endif

		if (m_propOCL.g_context == (cl_context)0 )
			return NULL;

		// get the list of CPU devices associated with context
		err = clGetContextInfo(m_propOCL.g_context, CL_CONTEXT_DEVICES, 0, NULL, &cb);


#if !ENABLE_CL_GL_INTER
		clGetContextInfo(m_propOCL.g_context, CL_CONTEXT_DEVICES, cb, devices, NULL);
		m_propOCL.g_device_ID = devices[0];
#endif

		m_propOCL.g_cmd_queue = clCreateCommandQueue(m_propOCL.g_context, m_propOCL.g_device_ID, 0, NULL);
		if (m_propOCL.g_cmd_queue == (cl_command_queue)0)
		{
			Cleanup();
			return NULL;
		}

	char *sources = ReadSources(program_source);	//read program .cl source file
	m_propOCL.g_program = clCreateProgramWithSource(m_propOCL.g_context, 1, (const char**)&sources, NULL, NULL);
	if (m_propOCL.g_program == (cl_program)0)
	{
		printf("ERROR: Failed to create Program with source...\n");
		Cleanup();
		free(sources);
		return NULL;
	}

	err = clBuildProgram(m_propOCL.g_program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS)
	{
		printf("ERROR: Failed to build program...\n");
		Cleanup();
		free(sources);
		return NULL;
	}

	m_propOCL.g_kernel = clCreateKernel(m_propOCL.g_program, kernel_name, NULL);

	if (m_propOCL.g_kernel == (cl_kernel)0)
	{
		printf("ERROR: Failed to create kernel...\n");
		Cleanup();
		free(sources);
		return NULL;
	}
	
	free(sources);

	// use first device ID
	//m_propOCL.g_device_ID = devices[0];
	err = clGetDeviceInfo(m_propOCL.g_device_ID, CL_DEVICE_NAME, 128, device_name, NULL);
	if (err!=CL_SUCCESS)
	{
		printf("ERROR: Failed to get device information (device name)...\n");
		Cleanup();
		return NULL;
	}
	printf("Using device %s...\n", device_name);

	err = clGetDeviceInfo(m_propOCL.g_device_ID, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &num_cores, NULL);
	if (err!=CL_SUCCESS)
	{
		printf("ERROR: Failed to get device information (max compute units)...\n");
		Cleanup();
		return NULL;
	}
	printf("Using %d compute units...\n", num_cores);


	return &m_propOCL; // success...
}

void COclManager::Cleanup()
{
	if( m_propOCL.g_kernel ) {clReleaseKernel( m_propOCL.g_kernel );  m_propOCL.g_kernel = NULL;}
	if( m_propOCL.g_program ) {clReleaseProgram( m_propOCL.g_program );  m_propOCL.g_program = NULL;}
	if( m_propOCL.g_cmd_queue ) {clReleaseCommandQueue( m_propOCL.g_cmd_queue );  m_propOCL.g_cmd_queue = NULL;}
	if( m_propOCL.g_context ) {clReleaseContext( m_propOCL.g_context );  m_propOCL.g_context = NULL;}

}

void COclManager::initialize()
{
	m_propOCL.g_context = NULL;
	m_propOCL.g_cmd_queue = NULL;
	m_propOCL.g_program = NULL;
	m_propOCL.g_kernel = NULL;

	m_propOCL.g_min_align = 0;
	m_propOCL.g_device_ID =0;
}
