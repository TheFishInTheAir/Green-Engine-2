//
// Created by Ethan Breit on 2017-07-26.
//

#include <string>

#include <OpenGL/types/GLRenderObject.h>
namespace ge
{
    namespace GL
    {

        void RenderObject::render()
        {
            glUseProgram( shaderGroup->programID );
            glBindVertexArray(_vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->id);

            glDrawElements(GL_TRIANGLES, indexBuffer->length, GL_UNSIGNED_INT, 0); //TODO:FIX


        }

        void RenderObject::rebuffer()
        {
            glBindVertexArray(_vao);
            unsigned int i = 0;
            for(auto vb : _vertexBuffers)
            {
                glEnableVertexAttribArray(vb.second->attributeId);

                vb.second->bind();
                glVertexAttribPointer(vb.second->attributeId, vb.second->sizePerAttrib, EnumUtil::getTrueDataType(vb.second->dataType),
                                      (GLboolean) vb.second->normalized, 0, vb.second->offset);

                i++;
            }
        }
        void RenderObject::registerUniform(std::string name)
        {
            ge::GL::Uniform* uniform = new GL::Uniform;
            uniform->identifier = name;
            uniform->id = glGetUniformLocation(shaderGroup->programID, name.c_str());
            _uniforms.insert({name, uniform});
        }

        ge::Uniform* RenderObject::getUniform(std::string s)
        {
            return _uniforms.find(s)->second;
        }

        ge::VertexBuffer* RenderObject::getVertexBuffer(std::string s)
        {
            return _vertexBuffers.find(s)->second;
        }

        void RenderObject::registerVertexBuffer(std::string s, ge::VertexBuffer* vertexBuffer)
        {
            _vertexBuffers.insert({s, (GL::VertexBuffer *) vertexBuffer});
        }

        void RenderObject::setShaderGroup(ge::ShaderGroup* sg)
        {
            shaderGroup = (GL::ShaderGroup *) sg;
        }

        ge::ShaderGroup* RenderObject::getShaderGroup()
        {
            return shaderGroup;
        }

        RenderObject::~RenderObject()
        {
            for(auto u : _uniforms)
            {
                delete u.second;
            }

            glDeleteVertexArrays(1, &_vao);

        }

        RenderObject::RenderObject()
        {
            glGenVertexArrays(1,&_vao);
        }

        void RenderObject::setIndexBuffer(ge::IndexBuffer *ib)
        {
            indexBuffer = (IndexBuffer *) ib;
        }

        ge::IndexBuffer *RenderObject::getIndexBuffer()
        {
            return indexBuffer;
        }

    }
}
