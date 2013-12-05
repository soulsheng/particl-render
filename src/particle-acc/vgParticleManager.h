


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
		

		void addParticleNode( ParticleBase *node, std::string platform_type);

		ParticleBase* addParticleNode( String name, E_PARTICLE_TYPE type, tagPropOCL* propOCL, int id, std::string platform_type );

		void renderParticleNode();

		ParticleBase*	getParticleNodeByName( const String& strRendererName);
	
	public:
		

		void setInputState( E_PARTICLE_TYPE inputState = PARTICLE_TYPE_UNKNOWN ) { _inputState = inputState; }
		E_PARTICLE_TYPE getInputState() { return _inputState; }



	protected:
		ParticleNodePtrVec _particleNodeVecCPU;
		ParticleNodePtrVec _particleNodeVecGPU;

		E_PARTICLE_TYPE _inputState;

	};



#endif // end of __VGSOUNDMANAGER_H__

