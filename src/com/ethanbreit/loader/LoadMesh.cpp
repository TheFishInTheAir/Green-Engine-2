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

        Error loadTriangleMesh(std::string path, BaseTriangleMesh **outMesh)
        {

            const aiScene *scene = importer.ReadFile(ResourceUtil::getResPath(path).c_str(), aiProcess_Triangulate ); //TODO: UV flip maybe...

            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                ge_Error_GENERATE(std::string("Assimp:") + importer.GetErrorString());
            }

            if(scene->mNumMeshes!=1)
            {
                ge_Error_GENERATE("More than one mesh in file!");
            }

            /**
             *
             * This all assumes that there is only one mesh, one texture and one material.
             *
             */
            GraphicsCore* graphicsCore = GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>();
            *outMesh = graphicsCore->meshFactory->newTriangleMesh();

            const aiMesh *mesh = scene->mMeshes[0];

            std::vector<glm::vec3>  vertices;
            std::vector<unsigned int>        indices;
            std::vector<glm::vec3>  normals;
            std::vector<glm::vec2>  UVs;
            
            bool hasNormals = false;
            bool hasUVs     = false;



            if(mesh->HasNormals())
                hasNormals  = true;

            if(mesh->HasTextureCoords(0))
                hasUVs      = true;


            if(mesh->HasBones())
            {
                ConsoleIO::Print("WARNING: Currently no support for bones in meshes. ignoring.",MessageType::Warning); //TODO: Implement bones in engine
            }



            for (int i = 0; i < mesh->mNumVertices; ++i)
            {

                ///Get Vertices

                aiVector3D v = mesh->mVertices[i];
                
                vertices.push_back(
                        {
                                v.x,
                                v.y,
                                v.z
                        }
                );

                if(hasNormals)
                {
                    ///Get Normals

                    aiVector3D n = mesh->mNormals[i];

                    normals.push_back(
                            {
                                    n.x,
                                    n.y,
                                    n.z
                            }
                    );
                }

                if(hasUVs)
                {
                    ///Get UVs

                    aiVector3D u = mesh->mTextureCoords[0][i];
                    UVs.push_back(
                            {
                                    u.x,
                                    u.y,
                            }
                    );
                }

            }

            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            /**
             *
             * Generate Buffers
             *
             */

            VertexBuffer *vertexBuffer  = graphicsCore->bufferFactory->genVertexBuffer();
            IndexBuffer  *indexBuffer   = graphicsCore->bufferFactory->genIndexBuffer();


            vertexBuffer->dataType      = DataType::Float;
            vertexBuffer->attributeId   = 0;
            vertexBuffer->sizePerAttrib = 3;
            vertexBuffer->offset        = 0;
            vertexBuffer->normalized    = false;

            vertexBuffer->bufferData(vertices.size() * sizeof(glm::vec3),&vertices[0],BufferMemoryType::Static);

            indexBuffer->length         = (unsigned int) indices.size();
            indexBuffer->bufferData(indices.size() * sizeof(unsigned int),&indices[0],BufferMemoryType::Static);

            (*outMesh)->registerVertexBuffer("Vertices", vertexBuffer);
            (*outMesh)->setIndexBuffer(indexBuffer);


            if(hasNormals)
            {

                VertexBuffer *normalBuffer  = graphicsCore->bufferFactory->genVertexBuffer();

                normalBuffer->dataType      = DataType::Float;
                normalBuffer->attributeId   = 1;
                normalBuffer->sizePerAttrib = 3;
                normalBuffer->offset        = 0;
                normalBuffer->normalized    = false;

                normalBuffer->bufferData(normals.size() * sizeof(glm::vec3),&normals[0],BufferMemoryType::Static);

                (*outMesh)->registerVertexBuffer("Normals", normalBuffer);
            }
            if(hasUVs)
            {
                VertexBuffer *UVBuffer      = graphicsCore->bufferFactory->genVertexBuffer();
                UVBuffer    ->dataType      = DataType::Float;
                UVBuffer    ->attributeId   = 2;
                UVBuffer    ->sizePerAttrib = 2;
                UVBuffer    ->offset        = 0;
                UVBuffer    ->normalized    = false;

                UVBuffer->bufferData(UVs.size() * sizeof(glm::vec3),&UVs[0],BufferMemoryType::Static);

                (*outMesh)->registerVertexBuffer("UVs", UVBuffer);
            }

            (*outMesh)->rebuffer(); ///Mesh should be loaded

            return Error();
        }
    }
}
