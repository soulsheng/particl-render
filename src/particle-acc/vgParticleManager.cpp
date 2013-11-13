


#include <ParticleStableHeaders.h>
#include "vgParticle/vgParticleManager.h"


#include <vgLibrary/vgkSystemUtility.h>
#include <vgLibrary/vgkTrace.h>

#include <vgImage/vgiTextureManager.h>
#include <vgImage/vgiImageManager.h>
#include <vgImage/vgiTextureIoRequestDispatcher.h>

#include <vgRenderer/vgkRendererManager.h>
#include <vgParticle/vgFountain.h>

#include <vgConfig/vgcoProjectConfig.h>

#define VGPARTICLE_SECTION					"ParticleModule"
#define VGPARTICLE_TEXTURE_FILENAME			"TextureFileName"

namespace  vgParticle {


	//----------------------------------------------------------------
	void ParticleManager::addParticleNode( vgParticle::E_PARTICLE_TYPE type, 
		vgKernel::Vec3 pos/* = vgKernel::Vec3(0.0f)*/ )
	{
		ostringstream particleName;
		
		particleName << "Particle" << _particleNodeVec.size();

		switch(type)
		{
		case PARTICLE_TYPE_FOUNTAIN:
			particleName << "*Fountain";
			break;

		case PARTICLE_TYPE_FIRE:
			break;

		default :
			break;
		}

		addParticleNode( particleName.str(), type, pos);
	}
	
	//----------------------------------------------------------------
	void ParticleManager::addParticleNode( vgParticle::ParticleBase *node )
	{
		//ParticleNodePtr aa( node );

		_particleNodeVec.push_back( ParticleNodePtr(node) );

		vgKernel::RendererManager::getSingleton().addRenderer( node );

		_strTypeVec[node->getSubType()] = node->getSubTypeString();
	}	
	//----------------------------------------------------------------
	vgParticle::ParticleBase* ParticleManager::addParticleNode( String name, vgParticle::E_PARTICLE_TYPE type, 
		vgKernel::Vec3 pos/* = vgKernel::Vec3(0.0f)*/)
	{
		vgParticle::ParticleBase *node = getParticleNodeByName( name );

		if ( node != NULL )
		{// 同名节点已存在，不允许重复添加
			return node;
		}

		switch(type)
		{
		case PARTICLE_TYPE_FOUNTAIN:
			node = new vgFountain( pos, name, _textureFilePath);
			break;

		case PARTICLE_TYPE_FIRE:
			break;

		default :
			break;
		}

		if(node)
		{
			node->Initialize();
			addParticleNode( node );

			int number = _particleNodeVec.size();
			ostringstream os;
			os << number << " number of Particle! \r\n";
			VGK_SHOW( os.str() );
		}

		return node;
	}
	//-----------------------------------------------------------------
	void ParticleManager::renderParticleNode()
	{
#if 1
		vgKernel::RendererQueue* pQueue = vgKernel::RendererManager::getSingleton().
			getCulledRenderQueueByType( RENDERER_TYPE_PARTICLE );

		if( !pQueue || pQueue->empty() )
		{
			return;
		}

		vgKernel::RendererQueue::iterator itr = pQueue->begin();
		vgKernel::RendererQueue::iterator itrend = pQueue->end();
		for (; itr != itrend; ++itr)
		{
			(*itr)->render();
		}
#endif
	}

	//////////////////////////////////////////////////////////////////////////chunyongma2008.9.8
	bool ParticleManager::onDeleteRenderer( vgKernel::Renderer *render )
	{
		if (
				(render->getType() != vgParticle::RENDERER_TYPE_PARTICLE)  
			)
		{
			return false;
		}

		switch (render->getType())
		{


		case vgParticle::RENDERER_TYPE_PARTICLE:
			{
				ParticleNodePtrVec::iterator iter = _particleNodeVec.begin();
				ParticleNodePtrVec::iterator iterEnd = _particleNodeVec.end();
				for( ; iter != iterEnd; iter++ )
				{
					if ((*iter).getPointer() == render)
					{
						_particleNodeVec.erase( iter );
						break;
					}
				}
			}
			break;


		default:
			break;
		}//switch

		return true;
	}

	
	vgParticle::ParticleBase* ParticleManager::getParticleNodeByName( const String& strRendererName )
	{
		ParticleNodePtrVec::iterator itr = _particleNodeVec.begin();
		ParticleNodePtrVec::iterator itrend = _particleNodeVec.end();
		for (; itr != itrend; ++itr)
		{
			if ((*itr)->getName() == strRendererName)
			{
				return	itr->getPointer();
			}
		}

		return NULL;		
	}

	
	//----------------------------------------------------------------
	bool ParticleManager::setTextureFilePath( String filePath )
	{
		//filePath = filePath.substr(0, filePath.find_last_of('.')+1);
		//filePath += "vgi";

		//vgGlobal::GlobalFacade::getSingleton().importImageFileOnly(filePath);
		//vgImage::TextureManager::getSingleton().;
		_textureFilePath = filePath;
		
		_textureFilePath += "\\data\\texture\\Particle\\";

		//vgConf::ProjectConfig::getSingleton().setProfilePath( getProgramConfigPath() );
		String textureFileName = VG_PROJCONF_GET_STRING( VGPARTICLE_SECTION, VGPARTICLE_TEXTURE_FILENAME);

		_textureFilePath += textureFileName;

		return true;
	}

	bool ParticleManager::copyTexture( String attachDataPath, String filePath )
	{
		String fileName;

		filePath = filePath.substr(0, filePath.find_last_of('.')+1);
		filePath += "vgi";
		
		// 源目录
		fileName = vgKernel::StringUtility::getFilenameFromAbsolutePath(filePath);

		// 目的目录
		attachDataPath += fileName;


		//bool save_ret = vgKernel::SystemUtility::copyFile( 
		//	filePath.c_str() , attachDataPath.c_str() );

		//assert( save_ret == true );

		return true;
	}

	
	void ParticleManager::reset()
	{
		_particleNodeVec.clear();
	}

	void ParticleManager::setDefault()
	{
		_inputState = PARTICLE_TYPE_UNKNOWN;

		_strTypeVec.assign( PARTICLE_TYPE_COUNT , String("") );
		_strTypeVec[PARTICLE_TYPE_FOUNTAIN] = "Fountain";
	}

	void ParticleManager::OnLButtonDown(UINT nFlags, int positionX, int positionY)
	{
		if( _inputState == PARTICLE_TYPE_UNKNOWN)
		{
			return;
		}

		vgKernel::Vec3 _newPos = vgKernel::Math::trans2DPointTo3DVec( positionX, positionY );
		
		addParticleNode( _inputState, _newPos );
		
	}

	E_PARTICLE_TYPE ParticleManager::getTypeByString(const String& str)
	{
		int type = -1;
		int index = 0;
		vgKernel::StringVector::iterator itr = _strTypeVec.begin();
		for (; itr != _strTypeVec.end(); itr ++ , index ++)
		{
			if ( *itr == str )
			{
				type = index;
				break;
			}
		}

		return (E_PARTICLE_TYPE)type;
	}
}// end of namespace  
