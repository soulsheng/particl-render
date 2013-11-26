
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
		//	Part 0 :�������ԣ����class Node
		String _meshName;
		float	_squaredDistanceToViewer;

		cl_float4 m_Position;//��ʼ��
		bool m_isVisible;
		
		vector4 m_prePosition;//��һ֡��ʼ��

	public:
		//	�������ԣ����class Node
		int		  m_nParticleCount;		// ������
		BOOL      m_bStopSE;			// ���ر��
		BOOL      m_bSwitchSE;			// ����/��ʾģʽ�л�
		
		string m_texturename;			// ��������

		float	m_height;
		float	m_width;
		float	m_speed;
		float	m_PartLenth;
		float   m_PartWidth;

		E_PARTICLE_TYPE	m_eSubType;
		String	m_strSubType;
	public:
		//	�����Զ��������
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
		//	�Զ�����麯��
		virtual	void Initialize(tagPropOCL* propOCL){}	//��ʼ��

	protected:
		void	setDefault();

	public:

		// ���� �鹹
		ParticleBase( vector4 pos, String particleName, String textureFileName);
		ParticleBase();
		virtual	~ParticleBase();
			
#if 1
	public:
		//	�̳�Renderer�����Ľӿ�
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
		ע��,���ص���Distance��ƽ��
		*/

		virtual void	render(){}
#endif		
		void setPropOCL( tagPropOCL* propOCL )	{ m_pPropOCL = propOCL; }
	protected:
		tagPropOCL* m_pPropOCL;// OpenCL ��������
	};// class  ParticleBase


	typedef vector<ParticleBase*> ParticleNodePtrVec;
	typedef vector<ParticleBase*>::iterator ParticleNodePtrVecItr;

#endif  //   _SPECIALEFFECT_H_INCLUDED_