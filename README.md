# FBXConverter

## 目次
- [概要と使用手順](#概要と使用手順)
- [使用技術](#使用技術)
- [システム構成](#システム構成)
- [プログラム　ピックアップ](#プログラム--ピックアップ)

## 概要と使用手順
FBXファイルをFBFIXに変換するコンバータ<br>
FBFIXはゲーム開発向けに最適化された独自の3Dデータファイルのこと<br><br>
システムの詳細や工夫点は以下からもご確認いただけます。また使用手順は以下の「使用手順」トピックでご確認いただけます。<br>
[🔗 FBXConverter 詳細（Notionページ）](https://picturesque-kayak-ac4.notion.site/FBX-196281634a1680ba9dadc5c617618675?pvs=4)

## 使用技術
- FBX SDK
- C++

## システム構成
``` mermaid
classDiagram
		direction L

		%%エントリーポイント
		class Main.cpp {
			+int Main()
		}

    %%FBXファイルの保存・読み込みを管理
    class FileManager {
        +static const char* RetrieveTextureFilePath(FbxMesh*, FbxNode*)
        +static void ParseFileName(const char*, std::string&)
        +static bool SaveMeshData(std::vector<MeshData>&, std::string&, \n float, float, float, float)
        +static bool SaveAnimMeshData(std::vector<AnimMeshData>&, std::string&, \n float, float, float, float)
    }

    %%FBXからメッシュ情報を抽出するクラス
    class MeshExtractor {
        +static void RetrieveMeshList(FbxNode*, std::unordered_map<std::string, FbxNode*>&)
        +static bool ExtractMeshData(std::unordered_map<std::string, FbxNode*>&, std::vector<MeshData>&)
        +static bool ExtractAnimMeshData(std::unordered_map<std::string, FbxNode*>&, std::vector<AnimMeshData>&, \n FbxAnimEvaluator*, const FbxTime&, const FbxTime&, float)
    }

    %%メッシュ情報を保持するデータ構造
    class MeshData {
        +std::vector<VertexNormal> vertices
        +std::vector<unsigned short> indices
        +std::wstring textureFileName
    }

    %%アニメーション付きメッシュデータ
    class AnimMeshData {
        +std::vector<VertexAnim> vertices
        +std::vector<unsigned short> indices
        +AnimData animData
        +std::vector<XMMATRIX> inverseGPoseMatrices
        +std::vector<std::vector<XMMATRIX>> poseMatrices
        +std::wstring textureFileName
    }

    %%文字列変換・行列コピーなどの汎用関数
    class Utilities {
        +static void SetMatrixToContainer(const XMMATRIX&, std::vector<float>&)
        +static void CopyMatrix(const FbxAMatrix&, XMMATRIX&)
        +static void ConvertString(const std::string&, std::wstring&)
        +static bool IsEqualVector2D(const XMFLOAT2&, const XMFLOAT2&)
        +static bool IsEqualVector3D(const XMFLOAT3&, const XMFLOAT3&)
    }

    %%座標変換や行列演算
    class Math {
        XMFLOAT2,XMFLOAT3の演算子オーバーロードを提供
    }

    %% 🔗 クラス間の依存関係
    Main.cpp --> MeshExtractor : メッシュ抽出
    Main.cpp --> FileManager : メッシュ保存
    FileManager --> MeshData : 保存
    FileManager --> AnimMeshData : 保存
    MeshExtractor --> MeshData : 抽出
    MeshExtractor --> AnimMeshData : 抽出
    Utilities ..> Math : 計算支援
```

## プログラム--ピックアップ
- **`SoundSystem.cs`**<br>
  - **エントリーポイントとして全機能を統括**
  - `AudioMixer`のパラメータを取得,設定する機能を提供
