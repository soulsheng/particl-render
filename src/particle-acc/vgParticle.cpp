


#include <stdafx.h>
#include "vgParticle.h"

	
	//----------------------------------------------------------------
	ParticleBase::ParticleBase(vgKernel::Vec3 pos, String particleName, String textureFileName)
		:_meshName(particleName)
	{
		setDefault();

		m_Position = pos;
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
		m_Position.x = 0.0f;
		m_Position.y = 0.0f;
		m_Position.z = 0.0f;
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

