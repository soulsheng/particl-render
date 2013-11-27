


#include <stdafx.h>
#include "vgParticleManager.h"

#include "vgFountain.h"
	
	//----------------------------------------------------------------
	void ParticleManager::addParticleNode( ParticleBase *node )
	{
		_particleNodeVec.push_back( node );

	}	
	//----------------------------------------------------------------
	ParticleBase* ParticleManager::addParticleNode( String name, E_PARTICLE_TYPE type, tagPropOCL* propOCL, int id, std::string platform_type )
	{
		ParticleBase *node = getParticleNodeByName( name );

		if ( node != NULL )
		{// 同名节点已存在，不允许重复添加
			return node;
		}

		vector4 offset(70.0f, 50.0f, 520.0f);
		if ( platform_type =="CPU" )
		{
			offset.x -= id*2;
			offset.z += id*2;
		}
		else
		{
			offset.x += id*2;
			offset.z -= id*2;
		}

		switch(type)
		{
		case PARTICLE_TYPE_FOUNTAIN:
			node = new vgFountain( offset, name, platform_type );
			break;

		case PARTICLE_TYPE_FIRE:
			break;

		default :
			break;
		}

		if(node)
		{
			node->Initialize( propOCL );
			addParticleNode( node );

			int number = _particleNodeVec.size();
			ostringstream os;
			os << number << " number of Particle! \r\n";
		}

		return node;
	}
	//-----------------------------------------------------------------
	void ParticleManager::renderParticleNode()
	{
#if 1
		ParticleNodePtrVecItr itr = _particleNodeVec.begin();
		ParticleNodePtrVecItr itrend = _particleNodeVec.end();
		for (; itr != itrend; ++itr)
		{
			(*itr)->render();
		}
#endif
	}

	
	ParticleBase* ParticleManager::getParticleNodeByName( const String& strRendererName )
	{
		ParticleNodePtrVec::iterator itr = _particleNodeVec.begin();
		ParticleNodePtrVec::iterator itrend = _particleNodeVec.end();
		for (; itr != itrend; ++itr)
		{
			if ((*itr)->getName() == strRendererName)
			{
				return	*itr;
			}
		}

		return NULL;		
	}

	
	void ParticleManager::reset()
	{
		_particleNodeVec.clear();
	}

	void ParticleManager::setDefault()
	{
		_inputState = PARTICLE_TYPE_UNKNOWN;

	}

	void ParticleManager::setPropOCL( tagPropOCL* propOCL )
	{
		ParticleNodePtrVec::iterator itr = _particleNodeVec.begin();
		ParticleNodePtrVec::iterator itrend = _particleNodeVec.end();
		for (; itr != itrend; ++itr)
		{
			(*itr)->setPropOCL( propOCL );
		}
	}
