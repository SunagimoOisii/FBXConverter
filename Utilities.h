#pragma once
#include <string>

#include <fbxsdk.h>
using namespace fbxsdk;
#pragma comment(lib,"libfbxsdk-md.lib")

#include <DirectXMath.h>
using namespace DirectX;

namespace Utilities
{
    const XMFLOAT2 ZeroVec2(0.0f, 0.0f);
    const XMFLOAT3 ZeroVec3(0.0f, 0.0f, 0.0f);

    void SetMatrixToContainer(const XMMATRIX& mat, std::vector<float>& v);

    /// <summary>
    /// FbxAMatrixの内容をXMMATRIXにコピー
    /// </summary>
    void CopyMatrix(const FbxAMatrix& src, XMMATRIX& dst);

    void ConvertString(const std::string& src, std::wstring& dst);

    bool IsEqualVector2D(const XMFLOAT2& v1, const XMFLOAT2& v2);

    bool IsEqualVector3D(const XMFLOAT3& v1, const XMFLOAT3& v2);
}