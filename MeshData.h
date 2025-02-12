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

//�@���Ή��̒��_�\����
struct VertexNormal
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
};

//���_�ɉe�����y�ڂ��{�[���̍ő吔
constexpr auto ANIM_BONE_MAX_NUM = (4);
struct VertexAnim
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	unsigned short boneID[ANIM_BONE_MAX_NUM]; //�e������{�[���̃C���f�b�N�X
	float weight[ANIM_BONE_MAX_NUM]; //�{�[���̉e���x
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
	std::vector<XMMATRIX> inverseGPoseMatrices; //�v�f���̓{�[���̌�
	std::vector<std::vector<XMMATRIX>> poseMatrices;
	std::wstring textureFileName;
};