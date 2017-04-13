/************************************************************************
* @project 		:  sloth
* @class   		:  ModelDataNM
* @version 		:  v1.0.0
* @description	:  记录包含法线贴图的模型数据结构
* @author		:  Oscar Shen
* @creat 		:  2017年2月21日21:34:41
* @revise 		:
************************************************************************
* Copyright @ OscarShen 2017. All rights reserved.
************************************************************************/
#pragma once
#ifndef ENG_MODEL_LOADER_H_
#define ENG_MODEL_LOADER_H_

#include <eng.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace eng {
	struct ModelData
	{
		std::shared_ptr<std::vector<Vector3f>> vertices;
		std::shared_ptr<std::vector<Vector3f>> normals;
		std::shared_ptr<std::vector<Vector2f>> uvs;
		std::shared_ptr<std::vector<unsigned>> indices;
		unsigned num_triangles;
		ModelData() :vertices(new std::vector<Vector3f>()), normals(new std::vector<Vector3f>()),
			uvs(new std::vector<Vector2f>()), indices(new std::vector<unsigned>()),
			num_triangles(0) {}
	};

	class ModelLoader
	{
	public:
		// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
		static void loadModel(const std::string &path, ModelData &mesh);

	private:
		// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
		static void processNode(ModelData &mesh, aiNode *node, const aiScene *scene);

		static void procesMesh(ModelData &mesh, aiMesh *assimp_mesh, const aiScene *scene);
	};

}


#endif // !SLOTH_MODEL_LOADER_H_
