#include <ge/debug/debug_entities/ColouredBox.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/util/ResourceUtil.h>
#include <ge/graphics/empty_types/IndexBuffer.h>

namespace ge
{
	namespace Debug {

		float vertecies[] =
		{
			-1.0f,-1.0f,-1.0f, // triangle 1 : begin
			-1.0f,-1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, // triangle 1 : end
			1.0f, 1.0f,-1.0f, // triangle 2 : begin
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f, // triangle 2 : end
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f
		};

		int indicies[] =
		{
			0,
			1,
			2,
			3,
			4,
			5,
			6,
			7,
			8,
			9,
			10,
			11,
			12,
			13,
			14,
			15,
			16,
			17,
			18,
			19,
			20,
			21,
			22,
			23,
			24,
			25,
			26,
			27,
			28,
			29,
			30,
			31,
			32,
			33,
			34,
			35
		};



		IndexBuffer* ib;
		VertexBuffer* vb;
		Empty::VertexBuffer vbe;
		Empty::IndexBuffer ibe;

		ShaderGroup* shaders;
		std::shared_ptr<Shader>			frag;
		std::shared_ptr<Shader>			vert;

		void setup()
		{
			GraphicsCore* gc = GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>();

			ib = gc->bufferFactory->genIndexBuffer();
			vb = gc->bufferFactory->genVertexBuffer();
			


			vbe.data = &vertecies;
			vbe.attributeId = VERTEX_POSITION;
			vbe.dataType = DataType::Float;
			vbe.memoryType = BufferMemoryType::Static;
			vbe.normalized = false;
			vbe.offset = 0;
			vbe.sizePerAttrib = 3;
			vbe.size = 36 * 3 * sizeof(float);

			ibe.memoryType = BufferMemoryType::Static;
			ibe.data = &indicies;
			ibe.length = 36;
			ibe.size = 36 * sizeof(int);

			vb->data = &vbe;
			ib->data = &ibe;

			vb->bufferData();
			ib->bufferData();

			std::string fragRaw;
			std::string vertRaw;

			ResourceUtil::getRawStrResource("debug/SolidColour/solid.frag", &fragRaw);
			ResourceUtil::getRawStrResource("debug/SolidColour/solid.vert", &vertRaw);

			gc->shaderFactory->genShader(fragRaw, ShaderType::Shader_Fragment, &frag);
			gc->shaderFactory->genShader(vertRaw, ShaderType::Shader_Vertex, &vert);

			gc->shaderFactory->genShaderGroup({vert,frag}, &shaders);
			

		}


		DebugColouredBox::DebugColouredBox(Camera* camera)
		{
			ge_REGISTER_RUNTIME_HANDLER;

			this->camera = camera;
			
		}

		DebugColouredBox::DebugColouredBox(Camera* camera, glm::vec3 pos)
		{
			ge_REGISTER_RUNTIME_HANDLER;

			this->camera = camera;
			model = glm::translate(model, pos);
		}

		void DebugColouredBox::render()
		{
			static bool onet_initialised = false;
			if(!onet_initialised)
			{
				onet_initialised = true;
				setup();
			}

			if(!initialised)
			{
				initialised = true;
				GraphicsCore* gc = GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>();
				mesh = gc->meshFactory->newTriangleMesh();

				mesh->registerVertexBuffer("vertex", vb);
				mesh->setIndexBuffer(ib);
				mesh->setShaderGroup(shaders);
				mesh->rebuffer();

				mesh->registerUniform("mvp");
				mesh->registerUniform(DBL_STRINGIFY(UNIFORM_SOLID_COL));

				mvp_u = mesh->getUniform("mvp");
				colour_u = mesh->getUniform(DBL_STRINGIFY(UNIFORM_SOLID_COL));

			}
			else
			{
				mvp_u->setData(camera->vp * model);
				colour_u->setData(colour);

				mesh->render();
			}
		}


	}
}
