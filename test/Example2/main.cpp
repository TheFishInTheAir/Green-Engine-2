//
// Created by Ethan Breit on 2017-07-28.
//
#include "../ExampleController.h"

#ifdef Enable_Example2
#include <gl/glew.h>
#include <error/Error.h>
#include <console/ConsoleIO.h>
#include <util/ResourceUtil.h>
#include <graphics/GraphicsCore.h>

ge::Error test()
{
    ge::Error err("testing");
    ge_Error_ADDTRACE(err);
    return err;
}

ge::Shader* vert;
ge::Shader* frag;

ge::Uniform* u;

std::string fragSrc;
std::string vertSrc;

static const float g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
};

static const unsigned int vertices[] = {
        0,1,2
};

int main()
{

    ge::GraphicsCore *gc = new ge::GraphicsCore(ge::GraphicsApi::API_OpenGL);


    {
        ge::Error err = test();
        ge_Error_ADDTRACE(err);
    }
    ge_Error_ADDTRACE(
            gc->window->init(ge::WindowConstructorInfo())
    );


    ge::ResourceUtil::getRawStrResource("PosColour.frag", &fragSrc);
    gc->shaderFactory->genShader(fragSrc,ge::ShaderType::Shader_Fragment,&frag);

    ge::ResourceUtil::getRawStrResource("PosColour.vert", &vertSrc);
    gc->shaderFactory->genShader(vertSrc,ge::ShaderType::Shader_Vertex,&vert);





    ge::IndexBuffer *ib = gc->bufferFactory->genIndexBuffer();

    ib->bufferData(sizeof(vertices),vertices,ge::BufferMemoryType::Static);

    ib->length = 3;

    ge::VertexBuffer *vb =gc->bufferFactory->genVertexBuffer();
    vb->offset = 0;
    vb->normalized = false;
    vb->dataType = ge::DataType::Float;
    vb->attributeId = 0;
    vb->sizePerAttrib = 3;
    vb->bufferData(sizeof(g_vertex_buffer_data), g_vertex_buffer_data, ge::BufferMemoryType::Static);



    ge::ShaderGroup *sg;
    {
        ge_Error_ADDTRACE(
        gc->shaderFactory->genShaderGroup({vert, frag}, &sg)
        );
    }


    ge::RenderObject *ro = gc->renderObjectFactory->newRenderObject();
    ro->setShaderGroup(sg);
    ro->setIndexBuffer(ib);

    ro->registerVertexBuffer("index", vb);

    ro->rebuffer();

    ro->registerUniform("tint");
    u = ro->getUniform("tint");


    gc->window->setClearColour({0.2f, 0.3f, 0.3f});

    while(!gc->window->shouldClose())
    {
        gc->window->clear();
        u->setData(glm::vec3(0.0f,0.0f,0.7f));
        ro->render();
        gc->window->poll();
        gc->window->swap();
    }
    gc->window->cleanup();


    delete ro;
    delete vb;
    delete sg;
    delete frag;
    delete vert;

}

#endif