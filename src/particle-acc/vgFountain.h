 

#ifndef  _CFOUNTIN_H_INCLUDED_
#define  _CFOUNTIN_H_INCLUDED_
/************************************************************************/
/*    vgFountain类的描述
功能：生成一个喷口位置可调的喷泉景象
用法：  1，在opengl环境下实例化一个喷泉对象 vgFountain pfountin;
		2，在opengl初始化时设置参数         pfountin.SetFountin();
		3，在opengl初始化时执行初始化       pfountin.InitFountin();
		4，在opengl渲染时执行				pfountin.DrawFountin();
		5, 在任何时候还可控制雪景			pfountin.SetFountin()＆pfountin.StopFountin() ;
注意： 喷口位置暂时只能由类成员函数 SetFountin()设置， 不支持鼠标键盘实时编辑     */
/************************************************************************/


#include "vgParticle.h"

	// 喷泉类 class    vgFountain
	class  vgFountain : public ParticleBase
	{
		// 构造、析构
	public:
		vgFountain(vgKernel::Vec3 pos, String particleName);
		vgFountain();
		virtual	~vgFountain();
		
		void setDefault();

		// 从父类继承函数
	public:
		virtual void	Initialize();	//初始化
		virtual void	render();  //渲染

		void UpdateEachFrame();			 //每帧更新
		void resetPosition(int index);
		void updatePosition(int index);
	public:
		// 水珠结构体 struct tagDROP
		typedef struct	tagDROP					
		{
			vgKernel::Vec3	vgen;			//	球形坐标系(r , a, b)，柱状发射模式
			vgKernel::Vec3	position;		//  世界坐标系(x , y, z)
			vgKernel::Vec3	vlen;			//  速度
			vgKernel::Vec3	acc;			//  加速度
			int		lifeFrame;				//	生命期
			bool	active;					//	激活状态

		} tagDROP, * LPDROP;  // 水珠结构体 struct tagDROP
		
		tagDROP  *m_pdrop;			//  定义指针指向 动态申请的保存喷泉粒子系统属性的 \
		
	
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

	}; // 喷泉类 class    vgFountain
		
#endif // _CFOUNTIN_H_INCLUDED_