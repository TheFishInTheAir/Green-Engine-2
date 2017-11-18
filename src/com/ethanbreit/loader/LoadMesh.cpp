//
// Created by Ethan Breit on 2017-08-10.
//

#include <loader/LoadMesh.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <memory/GlobalMemory.h>
#include <graphics/GraphicsCore.h>
#include "util/ResourceUtil.h"

namespace ge
{
	namespace MeshLoader
	{

		Assimp::Importer importer;

		Error loadTriangleMesh(std::string path, Empty::MeshData *outMesh)
		{

			const aiScene *scene = importer.ReadFile(ResourceUtil::getResPath(path).c_str(), aiProcess_Triangulate); //TODO: UV flip maybe...

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				ge_Error_GENERATE(std::string("Assimp:") + importer.GetErrorString());
			}

			if (scene->mNumMeshes != 1)
			{
				ge_Error_GENERATE("More than one mesh in file!");
			}

			/**
			*
			* This all assumes that there is only one mesh, one texture and one material.
			*
			*/
			//GraphicsCore* graphicsCore = GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>();

			const aiMesh *mesh = scene->mMeshes[0];
			//TODO: allocate to heap (like indices buffer)
			std::vector<glm::vec3>*  vertices	= new std::vector<glm::vec3>;
			std::vector<unsigned int>* indices	= new std::vector<unsigned int>;
			std::vector<glm::vec3>*  normals	= new std::vector<glm::vec3>;
			std::vector<glm::vec2>*  UVs		= new std::vector<glm::vec2>;

			bool hasNormals = false;
			bool hasUVs = false;



			if (mesh->HasNormals())
				hasNormals = true;

			if (mesh->HasTextureCoords(0))
				hasUVs = true;


			if (mesh->HasBones())
			{
				ConsoleIO::print("WARNING: Currently no support for bones in meshes. ignoring.", MessageType::Warning); //TODO: Implement bones in engine
			}



			for (int i = 0; i < mesh->mNumVertices; ++i)
			{

				///Get Vertices

				aiVector3D v = mesh->mVertices[i];

				vertices->push_back(
				{
					v.x,
					v.y,
					v.z
				}
				);

				if (hasNormals)
				{
					///Get Normals

					aiVector3D n = mesh->mNormals[i];

					normals->push_back(
					{
						n.x,
						n.y,
						n.z
					}
					);
				}

				if (hasUVs)
				{
					///Get UVs

					aiVector3D u = mesh->mTextureCoords[0][i];
					UVs->push_back(
					{
						u.x,
						u.y * -1,
					}
					);
				}

			}

			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices->push_back(face.mIndices[j]);
			}

			/**
			*
			* Generate Buffers
			*
			*/

			Empty::VertexBuffer *vertexBuffer = new Empty::VertexBuffer();
			Empty::IndexBuffer  *indexBuffer = new Empty::IndexBuffer();


			vertexBuffer->dataType = DataType::Float;
			vertexBuffer->attributeId = 0;
			vertexBuffer->sizePerAttrib = 3;
			vertexBuffer->offset = 0;
			vertexBuffer->normalized = false;
			vertexBuffer->size = vertices->size() * sizeof(glm::vec3);
			vertexBuffer->data = &(*vertices)[0];
			vertexBuffer->memoryType = BufferMemoryType::Static;

			indexBuffer->length = (unsigned int)indices->size();
			indexBuffer->size = indices->size() * sizeof(unsigned int);
			indexBuffer->data = &(*indices)[0];
			indexBuffer->memoryType = BufferMemoryType::Static;

			outMesh->vertexBuffers.push_back({ vertexBuffer, "vertex" });
			outMesh->indexBuffer = indexBuffer;
			outMesh->hasIndexBuffer = true;

			if (hasNormals)
			{

				Empty::VertexBuffer *normalBuffer = new Empty::VertexBuffer();

				normalBuffer->dataType = DataType::Float;
				normalBuffer->attributeId = 1;
				normalBuffer->sizePerAttrib = 3;
				normalBuffer->offset = 0;
				normalBuffer->normalized = false;

				normalBuffer->size = normals->size() * sizeof(glm::vec3);
				normalBuffer->data = &(*normals)[0];
				normalBuffer->memoryType = BufferMemoryType::Static;

				(*outMesh).vertexBuffers.push_back({ normalBuffer,"normals", });
			}
			if (hasUVs)
			{
				Empty::VertexBuffer *UVBuffer = new Empty::VertexBuffer();
				UVBuffer->dataType = DataType::Float;
				UVBuffer->attributeId = 2;
				UVBuffer->sizePerAttrib = 2;
				UVBuffer->offset = 0;
				UVBuffer->normalized = false;

				UVBuffer->size = UVs->size() * sizeof(glm::vec2);
				UVBuffer->data = &(*UVs)[0];
				UVBuffer->memoryType = BufferMemoryType::Static;


				(*outMesh).vertexBuffers.push_back({ UVBuffer,"UVs", });
			}

			
			return Error();
		}
	}
}