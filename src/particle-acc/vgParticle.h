
#ifndef    _SPECIALEFFECT_H_INCLUDED_
#define    _SPECIALEFFECT_H_INCLUDED_


#include "ParticleDefinition.h"
#include <vgRenderer/vgkRenderer.h>
#include <vgRenderer/vgkRendererObserver.h>
#include <vgLibrary/vgkBox.h>
#include <vgLibrary/vgkSharePtr.h>

#include <vgLibrary/vgkStringPairVector.h>
// defines
//#define		PI				 3.14159265


//////////////////////////////////////////////////////////////////////////
// classes
namespace vgParticle
{
	/*
	5010 -- 5030之间为QuadTreeSceneMananger的管辖范围之内
	*/
	const vgKernel::RendererType RENDERER_TYPE_PARTICLE = 5011;
	
	//const CategoryType NODE_TYPE_PARTICLE	= 30;

	enum	E_PARTICLE_TYPE
	{
		PARTICLE_TYPE_FOUNTAIN,
		PARTICLE_TYPE_FIRE,
		PARTICLE_TYPE_SMOKE,
		PARTICLE_TYPE_FLAG,
		PARTICLE_TYPE_UNKNOWN,

		PARTICLE_TYPE_COUNT
	};

	class  PARTICLEMODULE_API ParticleBase : public vgKernel::Renderer
	{
	public:
		//	Part 0 :新增属性，相对class Node
		String _meshName;
		vgKernel::UniqueID _uniqueID;
		float	_squaredDistanceToViewer;

		vgKernel::Box box;
		vgKernel::Vec3 m_Position;//起始点
		bool m_isVisible;
		
		vgKernel::Vec3 m_prePosition;//上一帧起始点

	public:
		//	新增属性，相对class Node
		int		  m_nParticleCount;		// 粒子数
		BOOL      m_bStopSE;			// 隐藏标记
		BOOL      m_bSwitchSE;			// 隐藏/显示模式切换
		enum   CONST2
		{
			PARTICLECOUNT = 2000,		// 缺省粒子数
		};
		string m_texturename;			// 材质名字

		float	m_height;
		float	m_width;
		float	m_speed;
		float	m_PartLenth;
		float   m_PartWidth;

		E_PARTICLE_TYPE	m_eSubType;
		String	m_strSubType;
	public:
		//	访问自定义的属性
		void SetHeight(float height)	
		{	
			m_height =	height;

			translate( 0,0,0 );
		}
		void SetWidth (float width )	
		{	
			m_width	=	width;
			translate( 0,0,0 );

		}
		void SetSpeed (float speed )	{	m_speed	=	speed;	}
		void SetPartLength(float length){	m_PartLenth	=	length;	}
		void SetPartWidth (float width)	{	m_PartWidth	=	width;	}
		
		void SetCount(GLuint count = PARTICLECOUNT)	{	m_nParticleCount  = count ; }
		void StopSpecialEffect(BOOL m_bStopSE)		{	m_bStopSE = m_bStopSE;}

		E_PARTICLE_TYPE	getSubType()				{	return m_eSubType;}
		void	setSubType(E_PARTICLE_TYPE ptype)	{	m_eSubType = ptype;}
		
		String	getSubTypeString()				{	return m_strSubType;}
		void	setSubType(String ptype)	{	m_strSubType = ptype;}


	public:
		//	实现父类的虚函数
		virtual	void	updateBoundingBox();

	public:
		//	自定义的虚函数
		virtual	void Initialize(){}	//初始化
		virtual	void UpdatePos();

	protected:
		void	configTextureName();
		void	setDefault();

	public:
		//	自定义的其它功能函数
		int saveToString( vgKernel::StringPairVector& fields );
		int readFromString( const vgKernel::StringPairVector& fields );

		// 构造 虚构
		ParticleBase(vgKernel::Vec3 pos, String particleName, String textureFileName);
		ParticleBase();
		virtual	~ParticleBase();
			
#if 1
	public:
		//	继承Renderer而来的接口
		virtual String getName()
		{
			return _meshName;
		}

		virtual String* getNamePtr()
		{
			return &_meshName;
		}

		virtual void setName( const String& newname )
		{
			_meshName = newname;
		}

		virtual vgKernel::UniqueID getUniqueID()
		{
			return _uniqueID;
		}

		virtual void translate(const float& x, const float& y, const float& z);
		

		virtual void rotateSelf(const float& angle, const float& x, const float& y, const float& z)
		{

		}

		virtual void scale(const float& cx, const float& cy, const float& cz, const float& x, const float& y, const float& z)
		{

		}

		virtual void rotate(const float& angle, const float& cx, const float& cy, const float& cz, 
			const float& x, const float& y, const float& z) 
		{

		}

		virtual std::pair<bool, float> testIntersectWithTriangles( 
			const vgKernel::Ray& ray );

		virtual vgKernel::Box getBoundingBox()
		{
			return box;
		}

		virtual long getNumOfTriangles()	{ return 0;}

		/**
		注意,返回的是Distance的平方
		*/
		virtual float setDistanceToViewer( const vgKernel::Vec3& viewer_pos  );

		virtual void	render(){}

		virtual vgKernel::RendererType getType() //lss
		{
			return RENDERER_TYPE_PARTICLE;
		}
#endif		

	protected:
		vgKernel::StringVector	_xmlElements;

	};// class  ParticleBase
	typedef vgKernel::SharePtr<vgParticle::ParticleBase> ParticleNodePtr;
	typedef vector<ParticleNodePtr> ParticleNodePtrVec;

}// namespace vgParticle

#endif  //   _SPECIALEFFECT_H_INCLUDED_