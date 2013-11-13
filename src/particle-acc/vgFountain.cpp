
// includes
#include <stdafx.h>
#include "vgFountain.h"

// class vgFountain

	vgFountain::vgFountain(vgKernel::Vec3 pos, String particleName ) 
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
	}

	vgFountain::~vgFountain()
	{
		delete[] m_pdrop;

		m_pdrop = NULL;
	} 
	
	
	void vgFountain::Initialize()
	{

		if ( m_pdrop)
		{
			delete[] m_pdrop;
		}
		m_pdrop = new tagDROP[ m_nParticleCount ];

		srand( GetTickCount() );


		GLuint	delayFrame ;
		delayFrame	= int ( m_height/m_speed * 20.0f + 0.5f);
		GLuint loop1;
		for (loop1=0;loop1< m_nParticleCount;loop1++)
		{

			m_pdrop[loop1].lifeFrame	=	rand()%delayFrame ;

			m_pdrop[loop1].active		=	false;
						
			m_pdrop[loop1].position =  m_Position ;
				
			m_pdrop[loop1].vgen.x =  m_height/100.0f;  //5.0f/100
			m_pdrop[loop1].vgen.y = float (rand()%(int)m_width + 2 ) * PI / 180.0f ;
			m_pdrop[loop1].vgen.z = float (rand()%360)  * PI / 180.0f ;
			
			m_pdrop[loop1].vlen.x  = m_pdrop[loop1].vgen.x * (float)sin( m_pdrop[loop1].vgen.y )  *
				(float)cos( m_pdrop[loop1].vgen.z );
			m_pdrop[loop1].vlen.y  = m_pdrop[loop1].vgen.x * (float)cos( m_pdrop[loop1].vgen.y ) ;
			m_pdrop[loop1].vlen.z  = m_pdrop[loop1].vgen.x * (float)sin( m_pdrop[loop1].vgen.y )  *
				(float)sin( m_pdrop[loop1].vgen.z ) ;
			
			m_pdrop[loop1].acc.x   = 0.0f;
			m_pdrop[loop1].acc.y   =-m_speed/1000.0f/*-0.005f*/;
			m_pdrop[loop1].acc.z   = 0.0f;				// Set Pull On Z Axis To Zero
		}
		
	} // void vgFountain::Init()
	
	void vgFountain::render()
	{
		UpdateEachFrame();

		glColor4f(1.0f  ,1.0f  ,1.0f ,1.0f);

#if RENDERMODE_POINT

		glBegin( GL_POINTS );
		for (int loop1=0;loop1< m_nParticleCount;loop1++)
		{
			glVertex3f( m_pdrop[loop1].position.x, m_pdrop[loop1].position.y, m_pdrop[loop1].position.z ); 
		}// for (int loop)
		glEnd();

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
		
		
	} // void vgFountain::Render()

	void vgFountain::UpdateEachFrame()
	{

		//m_mapFountainParticle.clear();

		GLuint	delayFrame ;
		delayFrame	= int ( m_height/m_speed * 20.0f + 0.5f);

		GLuint loop1;

		for (loop1=0;loop1< m_nParticleCount ;loop1++)					// Loop Through All The Particles
		{
			if ( 0 == m_pdrop [loop1].lifeFrame )
			{
				m_pdrop [loop1].active	=	true;
			}
			else
			{
				m_pdrop [loop1].lifeFrame -- ;
			}

			if ( m_pdrop [loop1].active )							// If The Particle Is Active
			{	
				// 粒子 按离视点距离的远近顺序 放入渲染队列

				distanceFromEye	=  0.0f;//rayFromEye.length() ;

				//m_mapFountainParticle.insert( m_pairFountainParticle(distanceFromEye,  m_pdrop + loop1 ) );

				// Update & Prepare for the next flame
				m_pdrop [loop1].position.x	+=  m_pdrop [loop1].vlen.x ;
				m_pdrop [loop1].position.y	+=  m_pdrop [loop1].vlen.y ;
				m_pdrop [loop1].position.z	+=  m_pdrop [loop1].vlen.z ;

				m_pdrop [loop1].vlen.x		+=  m_pdrop  [loop1].acc.x ;
				m_pdrop [loop1].vlen.y		+=  m_pdrop  [loop1].acc.y ;
				m_pdrop [loop1].vlen.z		+=  m_pdrop  [loop1].acc.z ;
			
				if ( m_pdrop [loop1].position.y < m_prePosition.y )
				{
					//srand( GetTickCount() );

					m_pdrop[loop1].position =  m_Position ;

					m_pdrop[loop1].vgen.x =  m_height/100.0f;  //5.0f/100
					m_pdrop[loop1].vgen.y = float (rand()%(int)m_width + 2 ) * (float)PI / 180.0f ;
					m_pdrop[loop1].vgen.z = float (rand()%360)  * (float)PI / 180.0f ;

					m_pdrop[loop1].vlen.x  = m_pdrop[loop1].vgen.x * (float)sin( m_pdrop[loop1].vgen.y )  *
						(float)cos( m_pdrop[loop1].vgen.z );
					m_pdrop[loop1].vlen.y  = m_pdrop[loop1].vgen.x * (float)cos( m_pdrop[loop1].vgen.y ) ;
					m_pdrop[loop1].vlen.z  = m_pdrop[loop1].vgen.x * (float)sin( m_pdrop[loop1].vgen.y )  *
						(float)sin( m_pdrop[loop1].vgen.z ) ;

				}// if ( m_pdrop [loop1].position.y< m_Position.y ) 粒子落后地面，重置属性

			}// if ( m_pdrop [loop1].active ) 粒子等待时刻结束，被激活

		} // for(loop1) 遍历所有粒子

	}// void vgFountain::UpdateEachFrame()

	