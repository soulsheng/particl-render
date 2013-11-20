
// includes
#include "stdafx.h"
#include "vgFountain.h"
#include "utils.h"

// class vgFountain

	vgFountain::vgFountain( float4 pos, String particleName ) 
		:ParticleBase(pos, particleName, "")
	{
		setDefault();
	}

	vgFountain::vgFountain() 
	{
		setDefault();
	}

	void vgFountain::setDefault()
	{
		m_pdrop	= NULL ;

		m_height = 25.0f;
		m_width  = 10.0f;
		m_speed	 = 5.0f;
#if 0
		String exepath = vgKernel::SystemUtility::getCurrentProcessPath();

		String texturePath =  VG_EXECONF_GET_STRING( 
			VGDATA_PATHCONF,  
			VGDATA_PATHCONF_TEXTURE_PATH);

		String textureName =  VG_EXECONF_GET_STRING( 
			VGDATA_FILECONF,  
			VGDATA_FILECONF_TEXTURE_NAME);

		texturePath = exepath + texturePath;
		m_texturename = texturePath + textureName;
		//m_texturename = "WaterDrop.jpg";
		//#else
		m_texturename =  VG_EXECONF_GET_STRING( 
			VGDATA_FILECONF,  
			VGDATA_FILECONF_TEXTURE_NAME);
#endif
		m_PartLenth		=	0.1f;
		m_PartWidth		=	0.1f;

		m_eSubType = PARTICLE_TYPE_FOUNTAIN;
		m_strSubType = "Fountain";

		//String texturePathBase = "L:\\SvnProject\\trunk_doc\\vrgis_DocAndCase\\test cases\\数据素材\\vgdb\\新建筑";
		//m_texturename = texturePathBase + "\\data\\Texture\\Particle24.tga";

		t = 0;
		r1 = (rand() % 9000) + 1000;
		r2 = (rand() % 900000) + 100000;
		r3 = (rand() % 1000000000) + 1000000000;
		m_noiseGenerator.setParameter(r1, r2, r3);
	}

	vgFountain::~vgFountain()
	{
		delete[] m_pdrop;

		m_pdrop = NULL;
	} 
	
	void vgFountain::resetPosition( tagDROP* pParticles ,int index )
	{
		pParticles [index].position =  m_Position ;
		//m_pdrop [index].position.s[1] =  m_Position.s[1] ;
		//m_pdrop [index].position.s[2] =  m_Position.s[2] ;

		pParticles [index].vgen.s[0] =  m_height/100.0f;  //5.0f/100
		pParticles [index].vgen.s[1] = float (index%(int)m_width + 2 ) * (float)PI / 180.0f ;
		pParticles [index].vgen.s[2] = float (index%360)  * (float)PI / 180.0f ;

		pParticles [index].vlen.s[0]  = pParticles [index].vgen.s[0] * (float)sin( pParticles [index].vgen.s[1] )  *
			(float)cos( pParticles [index].vgen.s[2] );
		pParticles [index].vlen.s[1]  = pParticles [index].vgen.s[0] * (float)cos( pParticles [index].vgen.s[1] ) ;
		pParticles [index].vlen.s[2]  = pParticles [index].vgen.s[0] * (float)sin( pParticles [index].vgen.s[1] )  *
			(float)sin( pParticles [index].vgen.s[2] ) ;

	}

	void vgFountain::updatePosition( tagDROP* pParticles, int index )
	{
		pParticles [index].position.s[0]	+=  pParticles [index].vlen.s[0] ;
		pParticles [index].position.s[1]	+=  pParticles [index].vlen.s[1] ;
		pParticles [index].position.s[2]	+=  pParticles [index].vlen.s[2] ;

		pParticles [index].vlen.s[0]		+=  pParticles [index].acc.s[0] ;
		pParticles [index].vlen.s[1]		+=  pParticles [index].acc.s[1] ;
		pParticles [index].vlen.s[2]		+=  pParticles [index].acc.s[2] ;
	}

	void vgFountain::resetPosition( int index, tagDROP* pParticle )
	{
			pParticle->position =  m_Position ;
#if ENABLE_ONLY_Y
			pParticle->vgen.s[0] =  m_height/100.0f;  //5.0f/100
			pParticle->vgen.s[1] = 0.0f;//float (rand()%(int)m_width + 2 ) * (float)PI / 180.0f ;
			pParticle->vgen.s[2] = float (rand()%360)  * (float)PI / 180.0f ;
#else
			pParticle->vgen.s[0] =  m_height/100.0f;  //5.0f/100
			pParticle->vgen.s[1] = float (index%(int)m_width + 2 ) * (float)PI / 180.0f ;
			pParticle->vgen.s[2] = float (index%360)  * (float)PI / 180.0f ;
#endif

			pParticle->vlen.s[0]  = pParticle->vgen.s[0] * (float)sin( pParticle->vgen.s[1] )  *
				(float)cos( pParticle->vgen.s[2] );
			pParticle->vlen.s[1]  = pParticle->vgen.s[0] * (float)cos( pParticle->vgen.s[1] ) ;
			pParticle->vlen.s[2]  = pParticle->vgen.s[0] * (float)sin( pParticle->vgen.s[1] )  *
				(float)sin( pParticle->vgen.s[2] ) ;

	}
	
	void vgFountain::updatePosition( tagDROP* pParticle )
	{
		pParticle->position.s[0] +=  pParticle->vlen.s[0] ;
		pParticle->position.s[1] +=  pParticle->vlen.s[1] ;
		pParticle->position.s[2] +=  pParticle->vlen.s[2] ;
#if 0
		pParticle->position.x	+=  pParticle->vlen.x ;
		pParticle->position.y	+=  pParticle->vlen.y ;
		pParticle->position.z	+=  pParticle->vlen.z ;
#endif
		pParticle->vlen.s[0]		+=  pParticle->acc.s[0];
		pParticle->vlen.s[1]		+=  pParticle->acc.s[1] ;
		pParticle->vlen.s[2]		+=  pParticle->acc.s[2] ;

		float   dx = (pParticle->position.s[0]-m_Position.s[0])*100;
		float	factorPerlinNoise = m_noiseGenerator.perlinNoise( pParticle->position.s[0], 0, 100 );
		pParticle->acc.s[0]		=  factorPerlinNoise/100 ;
		//cout << factorPerlinNoise << "\t";
	}

	void vgFountain::Initialize(tagPropOCL* propOCL)
	{

		if ( m_pdrop)
		{
			delete[] m_pdrop;
		}
		m_pdrop = new tagDROP[ m_nParticleCount ];

		//srand( GetTickCount() );


		GLuint	delayFrame ;
		delayFrame	= int ( m_height/m_speed * 20.0f + 0.5f);
		GLuint loop1;
		for (loop1=0;loop1< m_nParticleCount;loop1++)
		{

			m_pdrop[loop1].vgen.s[3]		=	0.0f;  // 激活状态
						
			resetPosition( loop1, m_pdrop + loop1 );

			m_pdrop[loop1].position.s[3]	=	rand()%delayFrame ; // 生命期
			
			m_pdrop[loop1].acc.s[0]   = 0.0f;
			m_pdrop[loop1].acc.s[1]   =-m_speed/1000.0f/*-0.005f*/;
			m_pdrop[loop1].acc.s[2]   = 0.0f;				// Set Pull On Z Axis To Zero
		}

#if RENDERMODE_VBO
		initializeVBO();
#endif

#if ENABLE_OPENCL
		m_pPropOCL = propOCL;
		SetupKernel();
#endif

	} // void vgFountain::Init()
	
	void vgFountain::render()
	{
#if RENDERMODE_MOVING

#if ENABLE_OPENCL
		ExecuteKernel();
#else
		UpdateEachFrame();
#endif//ENABLE_OPENCL

#endif//RENDERMODE_MOVING

#if ENABLE_DRAW
		glColor4f(1.0f  ,1.0f  ,1.0f ,1.0f);

#if RENDERMODE_POINT

#if RENDERMODE_VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_nIDVBO);
		glVertexPointer( 3, GL_FLOAT, sizeof(tagDROP) , BUFFER_OFFSET( 0 ) );
#else
		glVertexPointer(3, GL_FLOAT, sizeof(tagDROP), &m_pdrop[0].position.s[0]);
#endif
		glEnableClientState( GL_VERTEX_ARRAY );

		glDrawArrays(GL_POINTS, 0, m_nParticleCount );

		glDisableClientState( GL_VERTEX_ARRAY );
#if RENDERMODE_VBO
		glBindBuffer( GL_ARRAY_BUFFER, NULL );
#endif

#else

		for (int loop1=0;loop1< m_nParticleCount;loop1++)
		{
			float &x = m_pdrop[loop1].position.x;
			float &y = m_pdrop[loop1].position.y;
			float &z = m_pdrop[loop1].position.z;
			glBegin( GL_QUADS );		
			glTexCoord2d(1,1); glVertex3f(x+0.2f, y+0.4f, z+0.0f); // Top Right
			glTexCoord2d(0,1); glVertex3f(x-0.2f, y+0.4f, z+0.0f); // Top Left
			glTexCoord2d(0,0); glVertex3f(x-0.2f, y-0.4f, z+0.0f); // Bottom Right
			glTexCoord2d(1,0); glVertex3f(x+0.2f, y-0.4f, z+0.0f); // Bottom Left
			glEnd();			
		}// for (int loop)

#endif
#endif
		
	} // void vgFountain::Render()

	void vgFountain::UpdateEachFrame()
	{

		//m_mapFountainParticle.clear();
#if	RENDERMODE_VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_nIDVBO);
		tagDROP  *pParticleSet = (tagDROP  *)glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );
#else
		tagDROP  *pParticleSet = m_pdrop;
#endif

		GLuint	delayFrame ;
		delayFrame	= int ( m_height/m_speed * 20.0f + 0.5f);

		GLuint loop1;

		for (loop1=0;loop1< m_nParticleCount ;loop1++)					// Loop Through All The Particles
		{
			if ( 0 == pParticleSet [loop1].position.s[3] )
			{
				pParticleSet [loop1].vgen.s[3]	=	1.0f;
			}
			else
			{
				pParticleSet [loop1].position.s[3] -- ;
			}

			if ( pParticleSet [loop1].vgen.s[3] == 1.0f )							// If The Particle Is Active
			{	

				// Update & Prepare for the next flame
#if ENABLE_OPTIMIZE
				updatePosition( pParticleSet+loop1 );
#else
				updatePosition( pParticleSet, loop1 );
#endif
			
				if ( pParticleSet [loop1].position.s[1] < m_Position.s[1] )
				{
#if ENABLE_OPTIMIZE
					resetPosition( loop1, pParticleSet+loop1 );
#else
					resetPosition( pParticleSet, loop1  );
#endif
				}// if ( pParticle [loop1].position.y< m_Position.y ) 粒子落后地面，重置属性

			}// if ( pParticle [loop1].active ) 粒子等待时刻结束，被激活

		} // for(loop1) 遍历所有粒子


		
#if	RENDERMODE_VBO
		glUnmapBuffer( GL_ARRAY_BUFFER );
		glBindBuffer( GL_ARRAY_BUFFER, NULL );
#endif

	}// void vgFountain::UpdateEachFrame()

	void vgFountain::initializeVBO()
	{
		glGenBuffers( 1, &m_nIDVBO);

		glBindBuffer(GL_ARRAY_BUFFER, m_nIDVBO);

		glBufferData(GL_ARRAY_BUFFER, m_nParticleCount * sizeof(tagDROP), m_pdrop, GL_STATIC_DRAW);
	}

	void vgFountain::SetupKernel()
	{
		if ( NULL == m_pPropOCL )
		{
			return;
		}

#if ENABLE_CL_GL_INTER
		cl_int errcode_ret;
		m_oclKernelArg.m_pOclBuffer = clCreateFromGLBuffer( m_pPropOCL->g_context, CL_MEM_WRITE_ONLY, m_nIDVBO, &errcode_ret);

		if ( CL_SUCCESS != errcode_ret )
		{
			return;
		}
#else
		const cl_mem_flags INFlags  = CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY; 
		const cl_mem_flags OUTFlags = CL_MEM_COPY_HOST_PTR | CL_MEM_READ_WRITE;

		m_oclKernelArg.m_pOclBuffer = clCreateBuffer( m_pPropOCL->g_context, INFlags,  m_nParticleCount * sizeof(tagDROP), m_pdrop,	NULL); 
#endif

		// thread size
		m_oclKernelArg.localWorkSize[0] = LocalWorkX;
		m_oclKernelArg.localWorkSize[1] = LocalWorkX;

		size_t  workGroupSizeMaximum;
		clGetKernelWorkGroupInfo( m_pPropOCL->g_kernel, m_pPropOCL->g_device_ID, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), (void *)&workGroupSizeMaximum, NULL);

		int nElementSizePadding = roundToPowerOf2( m_nParticleCount );
		if ( nElementSizePadding > workGroupSizeMaximum )
		{
			m_oclKernelArg.globalWorkSize[0] = workGroupSizeMaximum;
			m_oclKernelArg.globalWorkSize[1] = nElementSizePadding / workGroupSizeMaximum;
		}
	}

	void vgFountain::ExecuteKernel()
	{
		clEnqueueAcquireGLObjects( m_pPropOCL->g_cmd_queue, 1, &m_oclKernelArg.m_pOclBuffer, 0, 0, NULL);


#if TIME_CL_MEMERY_CALCULATE
		clSetKernelArg( m_pPropOCL->g_kernel, 0, sizeof(cl_mem), (void *) &m_oclKernelArg.m_pOclBuffer);

		clSetKernelArg( m_pPropOCL->g_kernel, 1, sizeof(int), &m_nParticleCount);
	
		clSetKernelArg( m_pPropOCL->g_kernel, 2, sizeof(float), &m_Position.s[0]);
		clSetKernelArg( m_pPropOCL->g_kernel, 3, sizeof(float), &m_Position.s[1]);
		clSetKernelArg( m_pPropOCL->g_kernel, 4, sizeof(float), &m_Position.s[2]);
		clSetKernelArg( m_pPropOCL->g_kernel, 5, sizeof(float), &m_height);
		clSetKernelArg( m_pPropOCL->g_kernel, 6, sizeof(float), &m_width);

		clSetKernelArg( m_pPropOCL->g_kernel, 7, sizeof(int), &r1);
		clSetKernelArg( m_pPropOCL->g_kernel, 8, sizeof(int), &r2);
		clSetKernelArg( m_pPropOCL->g_kernel, 9, sizeof(int), &r3);
		clSetKernelArg( m_pPropOCL->g_kernel, 10, sizeof(int), &t);

		cl_event g_perf_event = NULL;
		cl_int err = CL_SUCCESS;
		// execute kernel, pls notice g_bAutoGroupSize
#if LocalWorkSizeDef
		err= clEnqueueNDRangeKernel( m_pPropOCL->g_cmd_queue, m_pPropOCL->g_kernel, 2, NULL, m_oclKernelArg.globalWorkSize, NULL, 0, NULL, &g_perf_event);
#else
		err= clEnqueueNDRangeKernel( m_pPropOCL->g_cmd_queue, m_pPropOCL->g_kernel, 2, NULL, m_oclKernelArg.globalWorkSize, m_oclKernelArg.localWorkSize, 0, NULL, &g_perf_event);
#endif
		if (err != CL_SUCCESS)
		{
			printf("ERROR: Failed to execute kernel...\n");
			return ;
		}
		err = clWaitForEvents(1, &g_perf_event);
		if (err != CL_SUCCESS)
		{
			printf("ERROR: Failed to clWaitForEvents...\n");
			return ;
		}
#endif//TIME_CL_MEMERY_CALCULATE

		clEnqueueReleaseGLObjects( m_pPropOCL->g_cmd_queue, 1, &m_oclKernelArg.m_pOclBuffer, 0, 0, 0);
		clFinish( m_pPropOCL->g_cmd_queue );

#if 0
		float* pVertexArrayDynamic = &m_pdrop[0].position.s[0];
		err = clEnqueueReadBuffer( m_pPropOCL->g_cmd_queue, m_oclKernelArg.m_pOclBuffer, CL_TRUE, 0, m_nParticleCount * sizeof(tagDROP) , pVertexArrayDynamic, 0, NULL, NULL);
		if (err != CL_SUCCESS)
		{
			printf("ERROR: Failed to clEnqueueReadBuffer...\n");
			return ;
		}
#endif
	}

	