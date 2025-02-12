#pragma once

#include <vector>
#include <string>

#include<DirectXMath.h>
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

//法線対応の頂点構造体
struct VertexNormal
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
};

//頂点に影響を及ぼすボーンの最大数
constexpr auto ANIM_BONE_MAX_NUM = (4);
struct VertexAnim
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	unsigned short boneID[ANIM_BONE_MAX_NUM]; //影響するボーンのインデックス
	float weight[ANIM_BONE_MAX_NUM]; //ボーンの影響度
};

struct MeshData
{
	std::vector<VertexNormal> vertices;
	std::vector<unsigned short> indices;
	std::wstring textureFileName;
};

struct AnimData
{
	unsigned short boneNum;
	unsigned short frameNum;
	float interval;
};

struct AnimMeshData
{
	std::vector<VertexAnim> vertices;
	std::vector<unsigned short> indices;
	AnimData animData;
	std::vector<XMMATRIX> inverseGPoseMatrices; //要素数はボーンの個数
	std::vector<std::vector<XMMATRIX>> poseMatrices;
	std::wstring textureFileName;
};