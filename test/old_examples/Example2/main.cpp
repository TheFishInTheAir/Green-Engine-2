//
// Created by Ethan Breit on 2017-07-28.
//
#include "../../ExampleController.h"

#ifdef Enable_Example2
#include <gl/glew.h>
#include <error/Error.h>
#include <console/ConsoleIO.h>
#include <util/ResourceUtil.h>
#include <graphics/GraphicsCore.h>
#include <graphics/Camera.h>
#include <debug/FreeMove.h>

ge::Shader* vert;
ge::Shader* frag;

ge::Uniform* u;

glm::mat4 model(1);

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
	gc->window->makeCurrentThread();

    /**
     * Generate and Configure Camera
     */

    ge::Camera camera = ge::Camera();
    camera.nearCull = 0.1f;
    camera.farCull  = 100.0f;
    camera.aspectRatio = 1.6f;
    //camera.fov = 90.0f/100.0f; ///Divide by 100 to convert fov to radians.
    camera.fov   = glm::radians(45.0f);
    ///initialising data
    camera.up = glm::vec3(0,1,0);
    camera.pos = glm::vec3(0,0,0);
    camera.dir = glm::vec3(0,0,-1);



    /**
     * Generate and Configure Debug FreeMove TODO: Add Debug to namespace
     */

    ge::FreeMove fm = ge::FreeMove();
    fm.speed = 0.05f;
    fm.lookSpeed = 0.002f;
    fm._position = glm::vec3(0);

    ///initialising data
    fm._horizontalAngle = 0;
    fm._verticalAngle = 0;



    /**
     * Generate Shaders
     */
    ge::ResourceUtil::getRawStrResource("../res/shaders/debug/PosColour3D.frag", &fragSrc);
    gc->shaderFactory->genShader(fragSrc,ge::ShaderType::Shader_Fragment,&frag);

    ge::ResourceUtil::getRawStrResource("../res/shaders/debug/PosColour3D.vert", &vertSrc);
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

    ro->registerUniform("mvp"); //TODO: better syntax
    u = ro->getUniform("mvp");  //TODO: better syntax


    gc->window->setClearColour({0.2f, 0.3f, 0.3f}); /// self explanatory


    /**
     *
     * Create Model Position and Orientation
     *
     */

    glm::translate(model,glm::vec3(0,1,0));

    /**
     * Render Loop
     */

    while(!gc->window->shouldClose())
    {
        gc->window->clear(); /// Clear

        fm.update(&camera);

        camera.update();

        u->setData(camera.proj*camera.view); /// set the uniforms data (matrix view projection)


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