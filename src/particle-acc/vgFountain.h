 

#ifndef  _CFOUNTIN_H_INCLUDED_
#define  _CFOUNTIN_H_INCLUDED_
/************************************************************************/
/*    vgFountain�������
���ܣ�����һ�����λ�ÿɵ�����Ȫ����
�÷���  1����opengl������ʵ����һ����Ȫ���� vgFountain pfountin;
		2����opengl��ʼ��ʱ���ò���         pfountin.SetFountin();
		3����opengl��ʼ��ʱִ�г�ʼ��       pfountin.InitFountin();
		4����opengl��Ⱦʱִ��				pfountin.DrawFountin();
		5, ���κ�ʱ�򻹿ɿ���ѩ��			pfountin.SetFountin()��pfountin.StopFountin() ;
ע�⣺ ���λ����ʱֻ�������Ա���� SetFountin()���ã� ��֧��������ʵʱ�༭     */
/************************************************************************/


#include "vgParticle.h"
#include "PerlinNoise.h"
	


	// ��Ȫ�� class    vgFountain
	class  vgFountain : public ParticleBase
	{
		// ���졢����
	public:
		vgFountain( vector4 pos, String particleName );
		vgFountain();
		virtual	~vgFountain();
		
		void setDefault();

		// �Ӹ���̳к���
	public:
		virtual void	Initialize(tagPropOCL* propOCL);	//��ʼ��
		virtual void	render();  //��Ⱦ

		void UpdateEachFrame();			 //ÿ֡����
		void resetPosition( tagDROP* pParticles, int index );
		void updatePosition( tagDROP* pParticles, int index );
		void resetPosition( int index, tagDROP* pParticle );
		void updatePosition( tagDROP* pParticle );
		void initializeVBO();

		void SetupKernel();
		void ExecuteKernel();

	public:

		tagDROP  *m_pdrop;			//  ����ָ��ָ�� ��̬����ı�����Ȫ����ϵͳ���Ե� \

	
		float	maxWidth	;

		float	maxHeight	;
		//////////////////////////////////////////////////////////////////////////
		//std::multimap <float ,tagDROP *> m_mapFountainParticle;

		std::multimap <float, tagDROP *>::iterator m_mapFountainParticleItor;

		typedef std::pair <float, tagDROP *>	m_pairFountainParticle;

		//CameraNode *pCurrentCamera;

		//vgCore::QuadtreeSceneManager	*pSceneManager;

		vgKernel::Vec3	rayFromEye ;

		float	distanceFromEye;

		tagDROP  *pCurrentParticle;	
		 
		GLuint	m_nIDVBO;

	protected:
		struct  OCLKernelArguments
		{
			cl_mem m_pOclBuffer ;

			size_t globalWorkSize[2];
			size_t localWorkSize[2];
		};
		OCLKernelArguments m_oclKernelArg;

		PerlinNoise	m_noiseGenerator;// Perlin��������������
		int r1,r2,r3, t; // Perlin������������
	}; // ��Ȫ�� class    vgFountain
		
#endif // _CFOUNTIN_H_INCLUDED_