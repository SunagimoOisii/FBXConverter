#include <cmath>
#include <vector>

#include "Utilities.h"

namespace Utilities
{
    void SetMatrixToContainer(const XMMATRIX& mat, std::vector<float>& v)
    {
        for (int i = 0;i < 4;i++)
        {
            for (int j = 0;j < 4;j++)
            {
                v.push_back(mat.r[i].m128_f32[j]);
            }
        }
    }

    void CopyMatrix(const FbxAMatrix& src, XMMATRIX& dst)
    {
        for (int i = 0;i < 4;i++)
        {
            for (int j = 0;j < 4;j++)
            {
                dst.r[i].m128_f32[j] = (float)src.Get(i, j);
            }
        }
    }

    void ConvertString(const std::string& src, std::wstring& dst)
    {
        wchar_t* buff = new wchar_t[src.size() + 1];
        size_t cvt;
        mbstowcs_s(&cvt, buff, src.size() + 1, src.c_str(), _TRUNCATE);
        dst = buff;
        delete[] buff;
    }

    bool IsEqualVector2D(const XMFLOAT2& v1, const XMFLOAT2& v2)
    {
        return std::fabs(v1.x - v2.x) < FLT_EPSILON &&
               std::fabs(v1.y - v2.y) < FLT_EPSILON;
    }

    bool IsEqualVector3D(const XMFLOAT3& v1, const XMFLOAT3& v2)
    {
        return std::fabs(v1.x - v2.x) < FLT_EPSILON &&
               std::fabs(v1.y - v2.y) < FLT_EPSILON &&
               std::fabs(v1.z - v2.z) < FLT_EPSILON;
    }
}