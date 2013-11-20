
#pragma once

class PerlinNoise
{
public:
	float perlinNoise(float x, float y, int t);
	void  setParameter(int r1, int r2, int r3);
protected:
	float scale(float from, float to, int scale, int t);
	float smooth(float x, float y);
	float noise(int x, int y);
	float interpolate(float x, float y, float a);

	int r1, r2, r3;
};