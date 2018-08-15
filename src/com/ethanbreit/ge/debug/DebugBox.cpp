#include <ge/debug/DebugBox.h>
#include <ge/graphics/meshes/TriangleMesh.h>
#include <ge/graphics/Camera.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/default_geom/Cube.h>
namespace ge
{
    namespace Debug
    {
        namespace DebugBox
        {
            const std::string _vert_src = std::string()+
            "#version 330\n"+
            "layout(location = 0) in vec3 vpos;\n"+
            "uniform mat4 mvp;\n"+
            "void main()\n"+
            "{\n"+
            "gl_Position = mvp * vec4(vpos, 1);\n"+
            "}\n";

            const std::string _frag_src = std::string()+
            "#version 330\n"+
            "out vec3 colour;\n"+
            "uniform vec3 inC;\n"+
            "void main()\n"+
            "{\n"+
            "colour = inC;\n"+
            "}\n";

            bool isInit = false;
            TriangleMesh* mesh;
            void init()
            {
                if(isInit)
                    return;
                
                mesh = GraphicsCore::ctx->meshFactory->newTriangleMesh(ge::dgeo::cube::getMeshData());

                std::shared_ptr<ge::Shader> v;
                std::shared_ptr<ge::Shader> f;

                GraphicsCore::ctx->shaderFactory->genShader(_vert_src, ge::ShaderType::Shader_Vertex, &v);
                GraphicsCore::ctx->shaderFactory->genShader(_frag_src, ge::ShaderType::Shader_Fragment, &f);

                ge::ShaderGroup* sg;
                GraphicsCore::ctx->shaderFactory->genShaderGroup({v, f}, &sg);

                sg->uniforms.insert({"inC", ge::Uniform(ge::Uniform::UniformType::FVEC3)});
                sg->uniforms.insert({"mvp", ge::Uniform(ge::Uniform::UniformType::MAT4 )});


                mesh->setShaderGroup(sg);
                mesh->rebuffer();

                isInit=true;
            }

            void draw(glm::vec3 colour, glm::vec3 pos, glm::vec3 scale, glm::mat4 VP)
            {
                init();
                mesh->setUniform("inC", colour);
                mesh->setUniform("mvp", glm::scale(glm::translate(VP, pos), scale));
                mesh->render();
            }

            void draw(glm::vec3 colour, glm::vec3 pos, glm::vec3 scale)
            {
                draw(colour, pos,scale, Camera::displayCamera->vp);
            }
        }
    }
}