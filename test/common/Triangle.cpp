//
// Created by Ethan Breit on 2017-08-13.
//

#include <common/Triangle.h>
#include <util/ResourceUtil.h>
#include <memory/GlobalMemory.h>


static const float g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
};

static const unsigned int vertices[] = {
        0,1,2
};

static Uniform* u;
static IndexBuffer *ib;
static VertexBuffer *vb;
static ShaderGroup *sg;
static TriangleMesh *mesh;

static GraphicsCore* core;

static std::shared_ptr<Shader> vert;
static std::shared_ptr<Shader> frag;

static std::string fragSrc;
static std::string vertSrc;



void Triangle::render()
{
    u->setData(camera->vp*model);
    mesh->render();

	//ConsoleIO::print("RENDERING \n");
}

void Triangle::update()
{

	//ConsoleIO::print("UPDATING \n");


    if(shouldRotate)
    {
        if(rotationState)
        {
            rotation += 1.0f;
            model = glm::rotate(model,glm::radians(1.0f), {0.0f,1.0f,0.0f});
            if(rotation>360.0f)
                rotationState=false;
        }
        else
        {
            rotation -= 1.0f;
            model = glm::rotate(model,glm::radians(1.0f), {0.0f,-1.0f,0.0f});
            if(rotation<1.0f)
                rotationState=true;
        }
    }

    if(shouldHover)
    {
        if(positionState)
        {
            position += 0.01;
            model = glm::translate(model, {0.0f,0.01f,0.0f});
            if(position>1)
                positionState=false;
        }
        else
        {
            position -= 0.01;
            model = glm::translate(model, {0.0f,-0.01f,0.0f});
			if (position < -1) {
				positionState = true;
			}
        }
    }
}
bool isInitialised = false;
Triangle::Triangle(bool shouldRotate, bool shouldHover, Camera *c)
{
	ge::GlobalRuntime::ge_REGISTER_RUNTIME_HANDLER;

    ///Static setup
    static bool shouldSetup = true;
    if(shouldSetup)
    {
        shouldSetup = false;
		GlobalMemory::get("ge_render_context_runtime").getRawData<Runtime>()->enqueFunctionStatic(setup);
    }
	while (!isInitialised);
    ///Triangle Instance Setup
    this->shouldRotate = shouldRotate;
    this->shouldHover  = shouldHover;


    camera = c;



}

void Triangle::setup()
{

    /**
     *
     * One Time Mesh Setup (we only need one mesh because we are only changing uniforms)
     *
     */

    core = GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>(); ///Get Current Instance of graphics core

    ge::ResourceUtil::getRawStrResource("../res/shaders/debug/PosColour3D.frag", &fragSrc); ///Get Shader
    core->shaderFactory->genShader(fragSrc,ge::ShaderType::Shader_Fragment,&frag); ///Generate the shader

    ge::ResourceUtil::getRawStrResource("../res/shaders/debug/PosColour3D.vert", &vertSrc); ///Get Shader
    core->shaderFactory->genShader(vertSrc,ge::ShaderType::Shader_Vertex,&vert); ///Generate the shader

    /**
    * Generate Index Buffer
    */


    ib = core->bufferFactory->genIndexBuffer();

	ib->data = new Empty::IndexBuffer;


	ib->data->size = sizeof(vertices);

	ib->data->data = vertices;

	ib->data->memoryType = ge::BufferMemoryType::Static;

	ib->data->length = 3;

	ib->bufferData();


    /**
     * Generate Vertex Buffer and set vertex buffer data
     */

    vb = core->bufferFactory->genVertexBuffer();

	vb->data = new Empty::VertexBuffer;

	vb->data->offset = 0;
	vb->data->normalized = false;
	vb->data->dataType = ge::DataType::Float;
	vb->data->attributeId = 0;
	vb->data->sizePerAttrib = 3;

	vb->data->size = sizeof(g_vertex_buffer_data);

	vb->data->data = g_vertex_buffer_data;

	vb->data->memoryType = ge::BufferMemoryType::Static;

    vb->bufferData();

    /**
     * Generate Shader Group (Program)
     */


    ge_Error_ADDTRACE(
            core->shaderFactory->genShaderGroup({vert, frag}, &sg));

    /**
    * Generate Mesh and add the shader group, index buffer, and the vertex buffer
    */

    mesh = core->meshFactory->newTriangleMesh();
    mesh->setShaderGroup(sg);
    mesh->setIndexBuffer(ib);

    mesh->registerVertexBuffer("vertex", vb);

    mesh->rebuffer(); /// This binds the buffers to the vao

    /**
     * Get the uniforms
     */

    mesh->registerUniform("mvp"); ///register uniform
    u = mesh->getUniform("mvp");  ///acquire uniform
	isInitialised = true;
}

void Triangle::cleanup()
{
    delete mesh;    ///Delete Mesh
    delete vb;      ///Delete Vertex Buffer
    delete ib;      ///Delete Index Buffer
    delete sg;      ///Delete Shader Group
    //delete frag;    ///Delete Fragment Shader
    //delete vert;    ///Delete Vertex Shader
}
