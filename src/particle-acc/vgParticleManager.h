


#ifndef __VGMOVINGMANAGER_H__
#define __VGMOVINGMANAGER_H__



//#include <vgCore/vgPrerequisites.h>
//#include <vgCore/vgSingleton.h>
//#include "ParticleDefinition.h"
#include "ParticleDefinition.h"
#include <vgLibrary/vgkSingleton.h>
#include <vgLibrary/vgkSharePtr.h>
#include <vgLibrary/vgkStreamReader.h>
#include <vgLibrary/vgkStreamWriter.h>

//#include <vgMs3d/ms3d.h>
#include "vgParticle/vgParticle.h"

#include<vgRenderer/vgkRendererHolder.h>


namespace  vgParticle {
	

	class PARTICLEMODULE_API  ParticleManager  
		: public vgKernel::Singleton<ParticleManager>
		, public vgKernel::RendererHolder//chunyongma2008.9.8
	{
		friend class vgKernel::Singleton<ParticleManager>;
	private:
		ParticleManager() 
			: vgKernel::Singleton<ParticleManager>( 
			VGK_SINGLETON_LEFE_ParticleManager ) 
		{
			VGK_TRACE(("ParticleManager created."));
			setDefault();
		}
		~ParticleManager()
		{
			VGK_TRACE(("ParticleManager destroyed."));
		}


	protected:

		virtual bool initialise()
		{
			return true;
		}

		virtual bool shutdown()
		{
			return true;
		}


		void reset();
		void setDefault();

	public://file series
		
		//String getTextureFilePath();
		bool setTextureFilePath(String filePath);
		bool copyTexture( String attachDataPath, String filePath);


		void addParticleNode( vgParticle::ParticleBase *node);
		void addParticleNode( vgParticle::E_PARTICLE_TYPE type, 
			vgKernel::Vec3 pos = vgKernel::Vec3(0.0f) );
		vgParticle::ParticleBase* addParticleNode( String name, vgParticle::E_PARTICLE_TYPE type, 
			vgKernel::Vec3 pos = vgKernel::Vec3(0.0f) );

		void renderParticleNode();
		
		virtual bool onDeleteRenderer( vgKernel::Renderer *render );//chunyongma2008.9.8

		vgParticle::ParticleBase*	getParticleNodeByName( const String& strRendererName);
	
	public:
		ParticleNodePtrVec* getParticleNodeVector()
		{
			return &_particleNodeVec;
		}

		void setInputState( E_PARTICLE_TYPE inputState = PARTICLE_TYPE_UNKNOWN ) { _inputState = inputState; }
		E_PARTICLE_TYPE getInputState() { return _inputState; }


		// mouse function
		void OnLButtonDown(UINT nFlags, int positionX, int positionY);

		E_PARTICLE_TYPE getTypeByString(const String& str);

	protected:
		ParticleNodePtrVec _particleNodeVec;
		E_PARTICLE_TYPE _inputState;

		String	_textureFilePath;

		vgKernel::StringVector	_strTypeVec;
	};




}// end of namespace 

#endif // end of __VGSOUNDMANAGER_H__

