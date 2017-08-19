//
// Created by Ethan Breit on 2017-08-15.
//
#include "ExampleModel.h"
#include <util/ResourceUtil.h>
#include <memory/GlobalMemory.h>
#include <error/Error.h>
#include <graphics/enums/BufferMemoryType.h>
#include <loader/LoadMesh.h>
#include <graphics/GraphicsCore.h>


static const float g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
};

static const unsigned int vertices[] = {
        0,1,2
};



static GraphicsCore* core;

static Shader* vert;
static Shader* frag;

static std::string fragSrc;
static std::string vertSrc;

static ShaderGroup* sg;

void ExampleModel::render()
{
    u->setData(camera->vp*model);
    mesh->render();
}

void ExampleModel::update()
{

}

ExampleModel::ExampleModel(std::string p, Camera *c)
{
    static bool first = true;
    if(first)
    {
        first = false;
        ge::GlobalRuntime::ge_REGISTER_RUNTIME_HANDLER;

        core = GlobalMemory::get(
                "ge_renderer_instance").getRawData<GraphicsCore>(); ///Get Current Instance of graphics core

        ge::ResourceUtil::getRawStrResource("PosColour3D.frag", &fragSrc); ///Get Shader
        core->shaderFactory->genShader(fragSrc, ge::ShaderType::Shader_Fragment, &frag); ///Generate the shader

        ge::ResourceUtil::getRawStrResource("PosColour3D.vert", &vertSrc); ///Get Shader
        core->shaderFactory->genShader(vertSrc, ge::ShaderType::Shader_Vertex, &vert); ///Generate the shader

        core->shaderFactory->genShaderGroup({vert,frag},&sg);



    }




    ge::MeshLoader::loadTriangleMesh(p,&mesh);
    camera = c;

    mesh->setShaderGroup(sg);

    mesh->registerUniform("mvp"); ///register uniform
    u = mesh->getUniform("mvp");  ///acquire uniform

}
