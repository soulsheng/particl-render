


#ifndef __VGMOVINGMANAGER_H__
#define __VGMOVINGMANAGER_H__

#include "ParticleDefinition.h"
#include "vgParticle.h"

	class   ParticleManager
	{
		
	public:
		ParticleManager() 
		{
			setDefault();
		}
		~ParticleManager()
		{
		}

		void setPropOCL( tagPropOCL* propOCL );

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
		

		void addParticleNode( ParticleBase *node);
		void addParticleNode( E_PARTICLE_TYPE type );
		ParticleBase* addParticleNode( String name, E_PARTICLE_TYPE type, tagPropOCL* propOCL );

		void renderParticleNode();

		ParticleBase*	getParticleNodeByName( const String& strRendererName);
	
	public:
		ParticleNodePtrVec* getParticleNodeVector()
		{
			return &_particleNodeVec;
		}

		void setInputState( E_PARTICLE_TYPE inputState = PARTICLE_TYPE_UNKNOWN ) { _inputState = inputState; }
		E_PARTICLE_TYPE getInputState() { return _inputState; }



	protected:
		ParticleNodePtrVec _particleNodeVec;
		E_PARTICLE_TYPE _inputState;

	};



#endif // end of __VGSOUNDMANAGER_H__

