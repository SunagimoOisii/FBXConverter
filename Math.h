#pragma once

#include<DirectXMath.h>
using namespace DirectX;

XMFLOAT2 operator+(const XMFLOAT2& a, const XMFLOAT2& b);
XMFLOAT2 operator-(const XMFLOAT2& a, const XMFLOAT2& b);
XMFLOAT2 operator*(const XMFLOAT2& a, const float b);
XMFLOAT2 operator*(const float a, const XMFLOAT2& b);
void operator+=(XMFLOAT2& a, const XMFLOAT2& b);
void operator-=(XMFLOAT2& a, const XMFLOAT2& b);
void operator*=(XMFLOAT2& a, float b);

XMFLOAT3 operator+(const XMFLOAT3& a, const XMFLOAT3& b);
XMFLOAT3 operator-(const XMFLOAT3& a, const XMFLOAT3& b);
XMFLOAT3 operator*(const XMFLOAT3& a, const float b);
XMFLOAT3 operator*(const float a, const XMFLOAT3& b);
XMFLOAT3 operator*(const XMFLOAT4& vec, const XMMATRIX& mat);
void operator+=(XMFLOAT3& a, const XMFLOAT3& b);
void operator-=(XMFLOAT3& a, const XMFLOAT3& b);
void operator*=(XMFLOAT3& a, float b);