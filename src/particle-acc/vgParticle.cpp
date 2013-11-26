


#include <stdafx.h>
#include "vgParticle.h"

	
	//----------------------------------------------------------------
	ParticleBase::ParticleBase( vector4 pos, String particleName, String textureFileName)
		:_meshName(particleName)
	{
		setDefault();

		m_Position.s[0]  = pos.x ;
		m_Position.s[1]  = pos.y ;
		m_Position.s[2]  = pos.z ;
		m_Position.s[3]  = pos.w ;
		m_prePosition = pos;
		m_texturename = textureFileName;
	}

	//----------------------------------------------------------------
	ParticleBase::ParticleBase()
	{
		setDefault();
	}

	//----------------------------------------------------------------
	void ParticleBase::setDefault()
	{
		m_Position.s[0]  = 0.0f ;
		m_Position.s[1]  = 0.0f ;
		m_Position.s[2]  = 0.0f ;
		m_Position.s[3]  = 0.0f ;
		//pMats		 = NULL;

		m_nParticleCount = PARTICLECOUNT;
		m_bStopSE	= TRUE;
		m_bSwitchSE = FALSE;
		m_height	= 0.0f;
		m_width		= 0.0f;
		m_speed		= 0.0f;


		//m_numOfFaces = 499;

		//TYPE = VG_PARTICLE;
		m_pPropOCL = NULL;
	}


	//----------------------------------------------------------------
	ParticleBase::~ParticleBase()	
	{
		
	}

