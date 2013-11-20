// Copyright (c) 2009-2011 Intel Corporation
// All rights reserved.
// 
// WARRANTY DISCLAIMER
// 
// THESE MATERIALS ARE PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THESE
// MATERIALS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// Intel Corporation is the author of the Materials, and requests that all
// problem reports or change requests be submitted to it directly

//#include "ParticleDefinition.h"

#define SIZE_PER_BONE		1 //每个顶点关联骨骼的数目
#define PI	3.1415926

__kernel void
SimpleKernel( const __global float *input, __global float *output)
{
	size_t index = get_global_id(0);
    output[index] = rsqrt( input[index] );
}

__kernel /*__attribute__((vec_type_hint(float4))) */ void
SimpleKernel4( const __global float4 *input, __global float4 *output)
{
	size_t index = get_global_id(0);
    output[index] = rsqrt( input[index] );
}

__kernel void
updateVectorByMatrix( const __global float *pInput, const __global int *pIndex,const __global float *pMatrix,__global float *pOutput)
{
	size_t index = get_global_id(0) + get_global_id(1) *get_global_size(0);
	
	int offset =  pIndex[index]*3*4;

	pOutput[4*index+0] = pInput[4*index] * pMatrix[offset+0] + pInput[4*index+1] * pMatrix[offset+1] + pInput[4*index+2] * pMatrix[offset+2]  + pMatrix[offset+3];
	pOutput[4*index+1] = pInput[4*index] * pMatrix[offset+4] + pInput[4*index+1] * pMatrix[offset+5] + pInput[4*index+2] * pMatrix[offset+6]  + pMatrix[offset+7];
	pOutput[4*index+2] = pInput[4*index] * pMatrix[offset+8] + pInput[4*index+1] * pMatrix[offset+9] + pInput[4*index+2] * pMatrix[offset+10]  + pMatrix[offset+11];
}


__kernel void
updateVectorByMatrix4( const __global float4 *pInput, const __global int *pIndex,__constant float4 *pMatrix,__global float4 *pOutput)
{
	size_t index = get_global_id(0) + get_global_id(1) *get_global_size(0);
	
	int offset = pIndex[index]*4;

	float4 vIn = pInput[index]; 
	
	pOutput[index] = (float4)( 
		vIn.x * pMatrix[offset+0].x + vIn.y * pMatrix[offset+0].y + vIn.z * pMatrix[offset+0].z  + pMatrix[offset+0].w ,
		vIn.x * pMatrix[offset+1].x + vIn.y * pMatrix[offset+1].y + vIn.z * pMatrix[offset+1].z  + pMatrix[offset+1].w ,
		vIn.x * pMatrix[offset+2].x + vIn.y * pMatrix[offset+2].y + vIn.z * pMatrix[offset+2].z  + pMatrix[offset+2].w ,
		1.0f);

}

__kernel void
transformVectorByMatrix4( const __global float4 *pInput, const __global int *pIndex,__constant float4 *pMatrix,__global float4 *pOutput,  int sizeMax,  const __global float *pWeight)
{
	size_t index = get_global_id(0) + get_global_id(1) *get_global_size(0);
	
	if( index >= sizeMax )
		return;

	int offset = pIndex[index*SIZE_PER_BONE+0]*4 ;
	float weight = pWeight[index*SIZE_PER_BONE+0] ;
	float4 weight4 = (float4)( weight ) ;

	float4 m0 = pMatrix[offset+0] * weight4 ;
	float4 m1 = pMatrix[offset+1] * weight4 ;
	float4 m2 = pMatrix[offset+2] * weight4 ;
	float4 m3 = pMatrix[offset+3] * weight4 ;

	for(int i=1;i<SIZE_PER_BONE; i++)
	{
		offset = pIndex[index*SIZE_PER_BONE+i]*4 ;
		weight = pWeight[index*SIZE_PER_BONE+i] ;
		weight4 = (float4)( weight, weight, weight, weight ) ;

		m0 += pMatrix[offset+0] * weight4 ;
		m1 += pMatrix[offset+1] * weight4 ;
		m2 += pMatrix[offset+2] * weight4 ;
		m3 += pMatrix[offset+3] * weight4 ;
	}

	float4 pIn = pInput[index];
	float4 px = (float4)pIn.x ;
	float4 py = (float4)pIn.y ;
	float4 pz = (float4)pIn.z ;

	pOutput[index] = px * m0 + py * m1 + pz * m2 + m3;

}


__kernel void
transformVectorByMatrix4One( const __global float4 *pInput, const __global int *pIndex,__constant float4 *pMatrix,__global float4 *pOutput,  int sizeMax,  const __global float *pWeight)
{
	size_t index = get_global_id(0) + get_global_id(1) *get_global_size(0);
	
	if( index >= sizeMax )
		return;

	float4 pIn = pInput[index];
	float4 px = (float4)pIn.x  ;
	float4 py = (float4)pIn.y  ;
	float4 pz = (float4)pIn.z  ;

	int offset = pIndex[index]*4 ;

	float4 m0 = pMatrix[offset+0] ;
	float4 m1 = pMatrix[offset+1] ;
	float4 m2 = pMatrix[offset+2] ;
	float4 m3 = pMatrix[offset+3] ;

	pOutput[index] = px * m0 + py * m1 + pz * m2 + m3 ;

}


float interpolate(float x, float y, float a){
	float val = (1 - cos(a * M_PI)) * 0.5;
	return x * (1 - val) + y * val;
}

float noise(int x, int y, int r1, int r2, int r3){
	int n = x + y * 57;
	n = (n << 13) ^ n;

	return (1.0 - ((n * (n * n * r1 + r2) + r3) & 0x7fffffff) / 1073741824.0);
}

float smooth(float x, float y, int r1, int r2, int r3 ){
	float n1 = noise((int)x, (int)y , r1, r2, r3);
	float n2 = noise((int)x + 1, (int)y  , r1, r2, r3);
	float n3 = noise((int)x, (int)y + 1  , r1, r2, r3);
	float n4 = noise((int)x + 1, (int)y + 1  , r1, r2, r3);

	float i1 = interpolate(n1, n2, x - (int)x);
	float i2 = interpolate(n3, n4, x - (int)x);

	return interpolate(i1, i2, y - (int)y);
}

float scale(float from, float to, int scale, int t){
	float direction = t % (scale * 2);

	if(direction < scale){
		return from + (to - from) * (float)(t % scale) / (float)scale;
	} else {
		return to - (to - from) * (float)(t % scale) / (float)scale;
	}
}

float perlinNoise(float x, float y, int r1, int r2, int r3, int t ){
	float total = 0.0;

	float frequency = scale(0.010, 0.025, 5000, t);
	float persistence = scale(0.20, 0.65, 5000, t);
	float octaves = scale(2, 33, 5000, t);
	float amplitude = 0.5;

	x = x + t / 10;

	for(int lcv = 0; lcv < octaves; ++lcv){
		total += smooth(x * frequency, y * frequency, r1, r2, r3 ) * amplitude;
		frequency *= 2;
		amplitude *= persistence;
	}

	const float cloudCoverage = 0;
	const float cloudDensity = 0.75;

	total = (total + cloudCoverage) * cloudDensity;

	return total;
}

__kernel void
updateParticlSystem( const __global float4 *pParticleSet, int sizeMax, 
					float m_PositionX, float m_PositionY, float m_PositionZ, float m_height , float m_width
					,int r1, int r2, int r3, int t)
{
	size_t index = get_global_id(0) + get_global_id(1) *get_global_size(0);
	
	if( index >= sizeMax )
		return;
	
	int lifeTime = (int)pParticleSet[index*4+0].w;
	bool bActive = false;
	if ( lifeTime == 0 )
	{
		bActive = true;
	}
	else
	{
		lifeTime -- ;
		pParticleSet[index*4+0].w = lifeTime;
	}

	if ( bActive )							// If The Particle Is Active
	{	
		//updatePosition( pParticleSet+index );
		pParticleSet [index*4+0].x	+=  pParticleSet [index*4+1].x ;
		pParticleSet [index*4+0].y	+=  pParticleSet [index*4+1].y ;
		pParticleSet [index*4+0].z	+=  pParticleSet [index*4+1].z ;

		pParticleSet [index*4+1].x		+=  pParticleSet [index*4+2].x ;
		pParticleSet [index*4+1].y		+=  pParticleSet [index*4+2].y ;
		pParticleSet [index*4+1].z		+=  pParticleSet [index*4+2].z ;
		
		//float   dx = (pParticleSet [index*4+0].x-m_PositionX)*100;
		float	factorPerlinNoise = perlinNoise( pParticleSet [index*4+0].x, 0.0f, r1, r2, r3, 100 );
		pParticleSet [index*4+2].x		=  factorPerlinNoise/100 ;


		if ( pParticleSet [index*4+0].y < 50.0f )
		{
			//resetPosition( pParticleSet+index );
			pParticleSet [index*4+0].x =  m_PositionX ;
			pParticleSet [index*4+0].y =  m_PositionY ;
			pParticleSet [index*4+0].z =  m_PositionZ ;
				
			pParticleSet [index*4+3].x =  m_height/100.0f;  //5.0f/100
			pParticleSet [index*4+3].y = (index%(int)m_width + 2) * (float)PI/180.0f;//float (index%(int)m_width + 2 ) ;//* (float)PI / 180.0f ;
			pParticleSet [index*4+3].z = (index%360)  * (float)PI / 180.0f ;
			
			pParticleSet [index*4+1].x  = pParticleSet [index*4+3].x * sin( pParticleSet [index*4+3].y )  *	cos( pParticleSet [index*4+3].z );
			pParticleSet [index*4+1].y  = pParticleSet [index*4+3].x * cos( pParticleSet [index*4+3].y ) ;
			pParticleSet [index*4+1].z  = pParticleSet [index*4+3].x * sin( pParticleSet [index*4+3].y )  *	sin( pParticleSet [index*4+3].z ) ;

		}// if ( pParticle [loop1].position.y< m_Position.y ) 粒子落后地面，重置属性
		
	}// if ( pParticle [loop1].active ) 粒子等待时刻结束，被激活
}
/*
__kernel void
updateParticlSystem( const __global tagDROP *pParticleSet, int sizeMax )
{
	size_t index = get_global_id(0) + get_global_id(1) *get_global_size(0);
	
	if( index >= sizeMax )
		return;
	
	if ( 0 == pParticleSet [index].lifeFrame )
	{
		pParticleSet [index].active	=	true;
	}
	else
	{
		pParticleSet [index].lifeFrame -- ;
	}

	if ( pParticleSet [index].active )							// If The Particle Is Active
	{	
		//updatePosition( pParticleSet+index );
		pParticleSet [index].position.x	+=  pParticleSet [index].vlen.x ;
		pParticleSet [index].position.y	+=  pParticleSet [index].vlen.y ;
		pParticleSet [index].position.z	+=  pParticleSet [index].vlen.z ;

		pParticleSet [index].vlen.x		+=  pParticleSet [index].acc.x ;
		pParticleSet [index].vlen.y		+=  pParticleSet [index].acc.y ;
		pParticleSet [index].vlen.z		+=  pParticleSet [index].acc.z ;

		if ( pParticleSet [index].position.y < m_prePosition.y )
		{
			//resetPosition( pParticleSet+index );
			pParticleSet [index].position =  m_Position ;
				
			pParticleSet [index].vgen.x =  m_height/100.0f;  //5.0f/100
			pParticleSet [index].vgen.y = float (rand()%(int)m_width + 2 ) * (float)PI / 180.0f ;
			pParticleSet [index].vgen.z = float (rand()%360)  * (float)PI / 180.0f ;
			
			pParticleSet [index].vlen.x  = pParticleSet [index].vgen.x * (float)sin( pParticleSet [index].vgen.y )  *
				(float)cos( pParticleSet [index].vgen.z );
			pParticleSet [index].vlen.y  = pParticleSet [index].vgen.x * (float)cos( pParticleSet [index].vgen.y ) ;
			pParticleSet [index].vlen.z  = pParticleSet [index].vgen.x * (float)sin( pParticleSet [index].vgen.y )  *
				(float)sin( pParticleSet [index].vgen.z ) ;

		}// if ( pParticle [loop1].position.y< m_Position.y ) 粒子落后地面，重置属性

	}// if ( pParticle [loop1].active ) 粒子等待时刻结束，被激活
	
}*/