 

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


#include <vgParticle/vgParticle.h>

#include <vgTexture/vgtBillboard.h>
#include <vgTexture/oldTex/vgtEffectTexture.h>

namespace vgParticle
{	

	//class QuadtreeSceneManager;
	//const CategoryType NODE_TYPE_FOUNTAIN	= 31;


	// ��Ȫ�� class    vgFountain
	class  vgFountain : public ParticleBase
	{
		// ���졢����
	public:
		vgFountain(vgKernel::Vec3 pos, String particleName, String textureFileName);
		vgFountain();
		virtual	~vgFountain();
		
		void setDefault();

		// �Ӹ���̳к���
	public:
		virtual void	Initialize();	//��ʼ��
		virtual void	render();  //��Ⱦ

		void UpdateEachFrame();			 //ÿ֡����
		void updateBoundingBox();
	public:
		// ˮ��ṹ�� struct tagDROP
		typedef struct	tagDROP					
		{
			int		lifeFrame;					// ��ʱ֡��
			bool	active;						// ������
			vgKernel::Vec3	vgen;			//	��������ϵ(r , a, b)
			vgKernel::Vec3	position;		//  ��������ϵ(x , y, z)
			vgKernel::Vec3	vlen;			//  �ٶ�
			vgKernel::Vec3	acc;			//  ���ٶ�

			vgTexture::Billboard	bb;
		} tagDROP, * LPDROP;  // ˮ��ṹ�� struct tagDROP
		
		tagDROP  *m_pdrop;			//  ����ָ��ָ�� ��̬����ı�����Ȫ����ϵͳ���Ե� \
		
		vgTexture::EffectTexturePtr  m_pTexture;
		
		float	maxWidth	;

		float	maxHeight	;
		//////////////////////////////////////////////////////////////////////////
		std::multimap <float ,tagDROP *> m_mapFountainParticle;

		std::multimap <float, tagDROP *>::iterator m_mapFountainParticleItor;

		typedef std::pair <float, tagDROP *>	m_pairFountainParticle;

		//CameraNode *pCurrentCamera;

		//vgCore::QuadtreeSceneManager	*pSceneManager;

		vgKernel::Vec3	rayFromEye ;

		float	distanceFromEye;

		tagDROP  *pCurrentParticle;	

	}; // ��Ȫ�� class    vgFountain
		
} // namespace vgCore
#endif // _CFOUNTIN_H_INCLUDED_