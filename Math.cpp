#include "Math.h"
#include <cmath>

/**********************************
XMFLOAT2‰‰ŽZŽq
**********************************/

XMFLOAT2 operator+(const XMFLOAT2& a, const XMFLOAT2& b) 
{
	return XMFLOAT2(a.x + b.x, a.y + b.y); 
}

XMFLOAT2 operator-(const XMFLOAT2& a, const XMFLOAT2& b) 
{
	return XMFLOAT2(a.x - b.x, a.y - b.y); 
}

XMFLOAT2 operator*(const XMFLOAT2& a, const float b) 
{
	return XMFLOAT2(a.x * b, a.y * b); 
}

XMFLOAT2 operator*(const float a, const XMFLOAT2& b) 
{	
	return XMFLOAT2(a * b.x, a * b.y);
}

void operator+=(XMFLOAT2& a, const XMFLOAT2& b) 
{
	a.x += b.x;
	a.y += b.y;
}

void operator-=(XMFLOAT2& a, const XMFLOAT2& b)
{
	a.x -= b.x;
	a.y -= b.y;
}

void operator*=(XMFLOAT2& a, float b)
{
	a.x *= b;
	a.y *= b;
}

/**********************************
XMFLOAT3‰‰ŽZŽq
**********************************/

XMFLOAT3 operator+(const XMFLOAT3& a, const XMFLOAT3& b)
{
	return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}

XMFLOAT3 operator-(const XMFLOAT3& a, const XMFLOAT3& b)
{
	return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
}

XMFLOAT3 operator*(const XMFLOAT3& a, const float b)
{
	return XMFLOAT3(a.x * b, a.y * b, a.z * b);
}

XMFLOAT3 operator*(const float a, const XMFLOAT3& b)
{
	return XMFLOAT3(b.x * a, b.y * a, b.z * a);
}

XMFLOAT3 operator*(const XMFLOAT4& vec, const XMMATRIX& mat)
{
	float v[4] = { vec.x,vec.y,vec.z,vec.w };
	float output[3]{};

	for (int i = 0;i < 3;i++)
	{
		output[i] = 0.0f;
		for (int j = 0;j < 4;j++)
		{
			output[i] += v[j] * mat.r[j].m128_f32[i];
		}
	}

	return XMFLOAT3(output[0], output[1], output[2]);
}

void operator+=(XMFLOAT3& a, const XMFLOAT3& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
}

void operator-=(XMFLOAT3& a, const XMFLOAT3& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
}

void operator*=(XMFLOAT3& a, float b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
}