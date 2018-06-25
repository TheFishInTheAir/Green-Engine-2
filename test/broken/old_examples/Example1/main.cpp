//
// Created by Ethan Breit on 2017-07-22.
//
//#include "../../ExampleController.h"


#ifdef Enable_Example1
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

    /**
     * Initialise Graphics
     */
    ge::GraphicsCore *gc = new ge::GraphicsCore(ge::GraphicsApi::OpenGL);

    ge_Error_ADDTRACE(
            gc->window->init(ge::WindowConstructorInfo()));

    /**
     * Error Test
     */

    {
        ge::Error err = test();
        ge_Error_ADDTRACE(err);
    }

    /**
     * Generate Shaders
     */
    ge::ResourceUtil::getRawStrResource("../res/shaders/debug/PosColour.frag", &fragSrc);
    gc->shaderFactory->genShader(fragSrc,ge::ShaderType::Shader_Fragment,&frag);

    ge::ResourceUtil::getRawStrResource("../res/shaders/debug/PosColour.vert", &vertSrc);
    gc->shaderFactory->genShader(vertSrc,ge::ShaderType::Shader_Vertex,&vert);




    /**
     * Generate Index Buffer
     */

    ge::IndexBuffer *ib = gc->bufferFactory->genIndexBuffer();

    ib->bufferData(sizeof(vertices),vertices,ge::BufferMemoryType::Static);

    ib->length = 3;

    /**
     * Generate Vertex Buffer and set vertex buffer data
     */

    ge::VertexBuffer *vb =gc->bufferFactory->genVertexBuffer();
    vb->offset = 0;
    vb->normalized = false;
    vb->dataType = ge::DataType::Float;
    vb->attributeId = 0;
    vb->sizePerAttrib = 3;
    vb->bufferData(sizeof(g_vertex_buffer_data), g_vertex_buffer_data, ge::BufferMemoryType::Static);

    /**
     * Generate Shader Group (Program)
     */

    ge::ShaderGroup *sg;

    ge_Error_ADDTRACE(
            gc->shaderFactory->genShaderGroup({vert, frag}, &sg));

    /**
     * Generate Mesh and add the shader group, index buffer, and the vertex buffer
     */

    ge::BaseTriangleMesh *ro = gc->meshFactory->newTriangleMesh();
    ro->setShaderGroup(sg);
    ro->setIndexBuffer(ib);

    ro->registerVertexBuffer("index", vb);

    ro->rebuffer(); /// This binds the buffers to the vao

    /**
     * Get the uniforms
     */

    ro->registerUniform("model"); //TODO: better syntax
    u = ro->getUniform("model");  //TODO: better syntax


    gc->window->setClearColour({0.2f, 0.3f, 0.3f}); /// self explanatory



    /**
     * Render Loop
     */

    while(!gc->window->shouldClose())
    {
        gc->window->clear(); /// Clear
        u->setData(glm::vec3(0.0f,0.0f,0.7f)); /// set the uniforms data (tint colour)
        ro->render(); ///render mesh
        gc->window->poll(); /// poll window events
        gc->window->swap(); /// swap buffers
    }
    gc->window->cleanup(); /// cleanup and terminate window


    delete ro;      ///Delete Mesh
    delete vb;      ///Delete Vertex Buffer
    delete sg;      ///Delete Shader Group
    delete frag;    ///Delete Fragment Shader
    delete vert;    ///Delete Vertex Shader

}

#endif
