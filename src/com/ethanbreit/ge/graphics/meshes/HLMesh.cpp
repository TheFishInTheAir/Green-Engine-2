#include <ge/graphics/meshes/HLMesh.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/engine/scene/Scene.h>
#include <ge/graphics/UniformAutomator.h>

namespace ge
{
    HLMesh::HLMesh()
    {
        //mesh = GraphicsCore::ctx->meshFactory->newTriangleMesh();
    }

    HLMesh::~HLMesh()
    {
        destroy();
    }

    void HLMesh::render()
    {
        if(mesh == nullptr)
            return;
        if(mat.shader_group == "NULL" || mat.name == "NULL")
            return;

        mesh->render();
        
    }

    TriangleMesh* HLMesh::getMesh()
    {
        return mesh;
    }

    void HLMesh::setMaterial(std::string mat)
    {
        this->mat = Scene::currentScene->materials[mat];
        applyMat();
    }

    void HLMesh::setMeshData(std::string tag)
    {
        if(mesh!=nullptr)
        {
            mesh->deepDestroy();
            delete mesh;
        }
        mesh = GraphicsCore::ctx->meshFactory->newTriangleMesh(*Scene::currentScene->meshes[tag].get());
        mesh->rebuffer();
        applyMat();
    }

    void HLMesh::applyMat()
    {

        //Early returns
        if(mesh == nullptr)
            return;
        if(mat.shader_group == "NULL" || mat.name == "NULL")
            return;
    
        mesh->setShaderGroup(Scene::currentScene->shaderGroups[mat.shader_group].get());

        for(auto texTag : mat.textures)
            mesh->registerTexture(Scene::currentScene->textures[texTag.second.first].get(), texTag.second.second);

        for(auto texTag : mat.cubemaps)
        {
            mesh->registerCubeMap(Scene::currentScene->cubemaps[texTag.second.first].get(), texTag.second.second);
        }

        UniformAutomator::initUniformSetup(mesh);

        for(auto u : mat.uniform_values)
        {
            mesh->setUniform(u.second.first, u.second.second);
        }
    }


    void HLMesh::destroy()
    {
        if(mesh!=nullptr)
        {
            mesh->deepDestroy();
            delete mesh;
        }
    }
}