
#ifndef    _SPECIALEFFECT_H_INCLUDED_
#define    _SPECIALEFFECT_H_INCLUDED_


#include "ParticleDefinition.h"



	enum	E_PARTICLE_TYPE
	{
		PARTICLE_TYPE_FOUNTAIN,
		PARTICLE_TYPE_FIRE,
		PARTICLE_TYPE_SMOKE,
		PARTICLE_TYPE_FLAG,
		PARTICLE_TYPE_UNKNOWN,

		PARTICLE_TYPE_COUNT
	};

	class   ParticleBase 
	{
	public:
		//	Part 0 :新增属性，相对class Node
		String _meshName;
		float	_squaredDistanceToViewer;

		cl_float4 m_Position;//起始点
		bool m_isVisible;
		
		vector4 m_prePosition;//上一帧起始点

	public:
		//	新增属性，相对class Node
		int		  m_nParticleCount;		// 粒子数
		BOOL      m_bStopSE;			// 隐藏标记
		BOOL      m_bSwitchSE;			// 隐藏/显示模式切换
		
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

		}
		void SetWidth (float width )	
		{	
			m_width	=	width;

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
		//	自定义的虚函数
		virtual	void Initialize(tagPropOCL* propOCL){}	//初始化

	protected:
		void	setDefault();

	public:

		// 构造 虚构
		ParticleBase( vector4 pos, String particleName, String textureFileName);
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




		virtual long getNumOfTriangles()	{ return 0;}

		/**
		注意,返回的是Distance的平方
		*/

		virtual void	render(){}
#endif		
		void setPropOCL( tagPropOCL* propOCL )	{ m_pPropOCL = propOCL; }
	protected:
		tagPropOCL* m_pPropOCL;// OpenCL 环境参数
	};// class  ParticleBase


	typedef vector<ParticleBase*> ParticleNodePtrVec;
	typedef vector<ParticleBase*>::iterator ParticleNodePtrVecItr;

#endif  //   _SPECIALEFFECT_H_INCLUDED_