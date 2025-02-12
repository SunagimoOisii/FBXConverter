#include <iostream>

#include <fbxsdk.h>
using namespace fbxsdk;
#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib,"libxml2-md.lib")
#pragma comment(lib,"zlib-md.lib")

#include "FileManager.h"
#include "Utilities.h"
#include "Math.h"
#include "MeshData.h"
#include "MeshExtractor.h"

int main()
{
	std::cout << "FBXMeshConverter" << std::endl;

	//読み込みFBXファイル名を取得
	std::string srcFileName;
	std::cout << "Input 3DModel FileName(with extension) = ";
	std::cin >> srcFileName;
	srcFileName = SRC_FILEPATH + srcFileName;

	//出力FBXファイル名を取得
	std::string dstFileName;
	std::cout << "Output fbfix fileName = ";
	std::cin >> dstFileName;

	//FBXライブラリの準備
	FbxManager*	   manager   = FbxManager::Create();
	FbxIOSettings* ioSetting = FbxIOSettings::Create(manager, IOSROOT);
	FbxImporter*   importer  = FbxImporter::Create(manager, "");
	if (!importer->Initialize(srcFileName.c_str(), -1, manager->GetIOSettings()))
	{
		std::cout << "Fail to read the file: " << srcFileName.c_str() << std::endl;
		importer->Destroy();
		ioSetting->Destroy();
		manager->Destroy();
		return 1;
	}

	//シーン取得
	FbxScene* scene = FbxScene::Create(manager, "scene");
	importer->Import(scene);

	//トポロジー整形(三角形化)
	FbxGeometryConverter converter(manager);
	converter.Triangulate(scene, true);

	//ノードデータ解析,メッシュリスト取得
	std::unordered_map<std::string, FbxNode*> meshList;
	MeshExtractor::RetrieveMeshList(scene->GetRootNode(), meshList);

	//メッシュの選択(スタティックかアニメーションか)
	bool animMode = false;
	char m;
	std::cout << "AnimationMesh(y/n): ";
	std::cin >> m;
	animMode = (m == 'y') ? true : false;

	//スケールと回転処理
	float scale, rotX, rotY, rotZ;
	std::cout << "Scale: ";
	std::cin >> scale;
	std::cout << "RotX: ";
	std::cin >> rotX;
	std::cout << "RotY: ";
	std::cin >> rotY;
	std::cout << "RotZ: ";
	std::cin >> rotZ;

	if (animMode) //アニメーションメッシュ時の処理
	{
		dstFileName += ".fbanim";

		float fps;
		std::cout << "Animation_fps = ";
		std::cin >> fps;
		std::vector<AnimMeshData> meshData;
		FbxAnimEvaluator* animEval = scene->GetAnimationEvaluator();
		FbxTakeInfo* info = importer->GetTakeInfo(0);
		FbxTime startTime = info->mLocalTimeSpan.GetStart();
		FbxTime stopTime  = info->mLocalTimeSpan.GetStop();
		if (!MeshExtractor::ExtractAnimMeshData(meshList, meshData, animEval,
			startTime, stopTime, fps))
		{
			std::cout << "Error: Failed to retrieve MeshData" << std::endl;
			scene->Destroy();
			importer->Destroy();
			ioSetting->Destroy();
			manager->Destroy();
			return 1;
		}

		if (!FileManager::SaveAnimMeshData(meshData, dstFileName, scale, rotX, rotY, rotZ))
		{
			std::cout << "Error: Failed to Save " << dstFileName << std::endl;
			scene->Destroy();
			importer->Destroy();
			ioSetting->Destroy();
			manager->Destroy();
			return 1;
		}
	}
	else //スタティックメッシュ時の処理
	{
		dstFileName += ".fbfix";

		//fbfix用データの取得(頂点,インデックス情報、テクスチャファイル名)
		std::vector<MeshData> meshData;
		if (!MeshExtractor::ExtractMeshData(meshList, meshData))
		{
			std::cout << "Error: Failed to retrieve MeshData" << std::endl;
			scene->Destroy();
			importer->Destroy();
			ioSetting->Destroy();
			manager->Destroy();
			return 1;
		}

		//fbfixファイルの保存
		if (!FileManager::SaveMeshData(meshData, dstFileName, scale, rotX, rotY, rotZ))
		{
			std::cout << "Error: Failed to Save " << dstFileName << std::endl;
			scene->Destroy();
			importer->Destroy();
			ioSetting->Destroy();
			manager->Destroy();
			return 1;
		}
	}

	importer->Destroy();
	ioSetting->Destroy();
	manager->Destroy();

	return 0;
}