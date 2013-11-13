 

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

	// ��Ȫ�� class    vgFountain
	class  vgFountain : public ParticleBase
	{
		// ���졢����
	public:
		vgFountain(vgKernel::Vec3 pos, String particleName);
		vgFountain();
		virtual	~vgFountain();
		
		void setDefault();

		// �Ӹ���̳к���
	public:
		virtual void	Initialize();	//��ʼ��
		virtual void	render();  //��Ⱦ

		void UpdateEachFrame();			 //ÿ֡����
		void resetPosition(int index);
		void updatePosition(int index);
	public:
		// ˮ��ṹ�� struct tagDROP
		typedef struct	tagDROP					
		{
			vgKernel::Vec3	vgen;			//	��������ϵ(r , a, b)����״����ģʽ
			vgKernel::Vec3	position;		//  ��������ϵ(x , y, z)
			vgKernel::Vec3	vlen;			//  �ٶ�
			vgKernel::Vec3	acc;			//  ���ٶ�
			int		lifeFrame;				//	������
			bool	active;					//	����״̬

		} tagDROP, * LPDROP;  // ˮ��ṹ�� struct tagDROP
		
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

	}; // ��Ȫ�� class    vgFountain
		
#endif // _CFOUNTIN_H_INCLUDED_