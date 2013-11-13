


#include <ParticleStableHeaders.h>
#include <vgParticle/vgParticle.h>


//#include <vgConf/vgcoExeConfig.h>


//#define VGDATA_FILECONF						"DataFileConfig"
//#define VGDATA_FILECONF_TEXTURE_NAME		"ParticleTexName"
#define	XML_ELEMENT_NAME		"name"
#define	XML_ELEMENT_POSITION	"position"
#define	XML_ELEMENT_TYPE		"type"

namespace vgParticle 
{
	
	//----------------------------------------------------------------
	void ParticleBase::translate(const float& x, const float& y, const float& z)
	{
		m_Position = m_Position + vgKernel::Vec3(x, y, z);
		
		m_prePosition = m_Position;
		
		Initialize();

		updateBoundingBox();

		UpdatePos();
	}
	

	//----------------------------------------------------------------
	void ParticleBase::updateBoundingBox()
	{
		// 由参数m_height、m_width、m_speed计算特效空间范围
		float	gravity =-m_speed / 1000.0f;
		float	maxVelocity= m_height/ 100.0f;
		float	maxTheta   = (m_width + 2.0f)*vgKernel::Math::PI/ 180.0f;
		
		float	maxHeight = (maxVelocity*maxVelocity) 
			/ (-2.0f*gravity); // Smax = v^2 / 2*a;
		float   maxWidth  = (maxVelocity*maxVelocity*sin(maxTheta)) 
			/ (-gravity);
		
		vgKernel::Box &m_BoundryBox = box;

		m_BoundryBox._maxVertex.x = m_Position.x + maxWidth;
		m_BoundryBox._maxVertex.y = m_Position.y + maxHeight/2;
		m_BoundryBox._maxVertex.z = m_Position.z + maxWidth;
		
		m_BoundryBox._minVertex.x = m_Position.x - maxWidth;
		m_BoundryBox._minVertex.y = m_Position.y - maxHeight/2;
		m_BoundryBox._minVertex.z = m_Position.z - maxWidth;
		
		//return m_BoundryBox;	
	}	//	updateBoundingBox() 
	

	//----------------------------------------------------------------
	ParticleBase::ParticleBase(vgKernel::Vec3 pos, String particleName, String textureFileName)
		:_uniqueID(vgKernel::UniqueIDFactory::getUniqueID() )
		,_meshName(particleName)
	{
		setDefault();

		m_Position = pos;
		m_prePosition = pos;
		m_texturename = textureFileName;
	}

	//----------------------------------------------------------------
	ParticleBase::ParticleBase()
		:_uniqueID(vgKernel::UniqueIDFactory::getUniqueID() )
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

		configTextureName();

		_xmlElements.push_back( XML_ELEMENT_NAME );
		_xmlElements.push_back( XML_ELEMENT_POSITION );
		_xmlElements.push_back( XML_ELEMENT_TYPE );

	}

	void ParticleBase::configTextureName()
	{
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
//#else

		m_texturename =  VG_EXECONF_GET_STRING( 
			VGDATA_FILECONF,  
			VGDATA_FILECONF_TEXTURE_NAME);

#endif
	}
	
	
	//----------------------------------------------------------------
	ParticleBase::~ParticleBase()	
	{
		
	}






	
	
	//--继承Renderer而来的接口-------------------------------------------
	float	ParticleBase::setDistanceToViewer( const vgKernel::Vec3& viewer_pos )
	{
		// 1.首先通过包围盒计算与视点的距离.
		float delta[3];

		vgKernel::Box &m_BoundryBox = box;
		for ( int i = 0 ; i < 3 ; ++ i )
		{
			float _cur = viewer_pos[i];
			float _max = m_BoundryBox._maxVertex.v[i];
			float _min = m_BoundryBox._minVertex.v[i];

			if ( _cur > _max )
			{
				delta[i] = _cur - _max;
			}
			else if ( _cur < _min )
			{
				delta[i] = _min - _cur;
			}
			else
			{
				delta[i] = 0.0f;
			}
		}

		_squaredDistanceToViewer = 
			delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];

		// 2.更新_distanceLod

		return _squaredDistanceToViewer;
	}



	
	
	//----------------------------------------------------------------
	std::pair<bool, float> ParticleBase::testIntersectWithTriangles( 
		const vgKernel::Ray& ray )

	{
		std::pair<bool, float> result;

		bool test_result = false;
		if (m_isVisible == false )
		{
			result.first = false;
			return result;
		}

		// 遍历

		result = vgKernel::Math::intersects( ray , getBoundingBox() );

		if( result.first == true )
		{
			test_result = true;
			goto TEST_INTERSECTION_END;
		}

TEST_INTERSECTION_END:
		return result;
	}

	//----------------------------------------------------------------
	void ParticleBase::UpdatePos()
	{
		vgKernel::Box &m_BoundryBox = box;

		m_Position.x = (m_BoundryBox._maxVertex.x + m_BoundryBox._minVertex.x) / 2;

		m_Position.y = m_BoundryBox._minVertex.y;

		m_Position.z = (m_BoundryBox._maxVertex.z + m_BoundryBox._minVertex.z) / 2;

		m_prePosition = m_Position;

		/*vgCore::QuadtreeSceneManager &mgr = 
		vgCore::QuadtreeSceneManager::getSingleton();

		if (mgr.GetNodeAbstract(m_id))
		{
		mgr.GetNodeAbstract(m_id)->GetBoundryBox() = m_BoundryBox;

		mgr.GetNodeAbstract(m_id)->m_Position = m_Position;
		}*/

	}

	//----------------------------------------------------------------
	int ParticleBase::saveToString( vgKernel::StringPairVector& fields )
	{
		int length = 0;

		// 1. 格式化属性数据
		for ( vgKernel::StringVector::iterator itr = _xmlElements.begin();
			itr != _xmlElements.end(); itr ++ )
		{
			vgKernel::StringPair field;

			// 1.1 字段名称
			field.first =  itr->c_str();

			String	str;

			// 1.2.1 字段值，名字
			if( XML_ELEMENT_NAME == *itr )
			{
				str = this->getName();				
			}

			// 1.2.2 字段值，位置
			else if( XML_ELEMENT_POSITION == *itr )
			{
				str = this->getPosition().getDetails();
			}

			// 1.2.3 字段值，类型
			else if( XML_ELEMENT_TYPE == *itr )
			{
				str = this->getSubTypeString();
			}

			field.second = str;

			fields.push_back( field );
		}// for

		return length;
	}

	//----------------------------------------------------------------
	int ParticleBase::readFromString( const vgKernel::StringPairVector& fields )
	{
		int length = 0;

		// 1. 设置各项属性值
		for ( vgKernel::StringPairVector::const_iterator itr = fields.begin();
			itr != fields.end(); itr ++ )
		{
			vgKernel::Vec3 vec;
			vec.setValueByString( itr->second );	
			//vec = - vec;

			// 1.2.1 字段值，名字
			if( XML_ELEMENT_NAME == itr->first )
			{
				this->setName( itr->second );
			}

			// 1.2.2 字段值，位置
			else if( XML_ELEMENT_POSITION == itr->first )
			{		
				vgKernel::Vec3 offset = vec - this->getPosition();
				this->setPosition( vec );

				translate( offset.x, offset.y, offset.z );
			}

			// 1.2.3 字段值，类型
			else if( XML_ELEMENT_TYPE == itr->first )
			{		
				if( itr->second == "Fountain" )
				{
					m_eSubType = PARTICLE_TYPE_FOUNTAIN ;
				}
			}
		}// for

		return length;
	}
}	//	namespace vgCore

