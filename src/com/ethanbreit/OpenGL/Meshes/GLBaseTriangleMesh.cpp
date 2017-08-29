//
// Created by Ethan Breit on 2017-07-26.
//


#include <OpenGL/Meshes/GLBaseTriangleMesh.h>
#include <memory/GlobalMemory.h>
#include <console/ConsoleIO.h>
#include <iostream>

namespace ge
{
    namespace GL
    {

        void TriangleMesh::render()
        {
			startRender();
            glBindVertexArray(_vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->id);

            glDrawElements(GL_TRIANGLES, indexBuffer->length, GL_UNSIGNED_INT, 0);

			isRendering = false;
        }

        void TriangleMesh::rebuffer()
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
            for(auto tex : _textures)
            {
                if(tex.first <= GlobalMemory::get("ge_max_texture_units").getData<int>())
                {
                    glActiveTexture(GL_TEXTURE0+tex.first);
                    glBindTexture(GL_TEXTURE_2D, tex.second->id);
                }
                else
                {
                    ConsoleIO::Print("GL: texture unit ID("+std::to_string(tex.first)+") exceeds maximum("+std::to_string(GlobalMemory::get("ge_max_texture_units").getData<int>())+")!",MessageType::Error);
                }
            }
        }
        void TriangleMesh::registerUniform(std::string name)
        {
            ge::GL::Uniform* uniform = new GL::Uniform;
			uniform->parentMesh = this;
            uniform->identifier = name;
            uniform->id = glGetUniformLocation(shaderGroup->programID, name.c_str());
            _uniforms.insert({name, uniform});
        }

        ge::Uniform* TriangleMesh::getUniform(std::string s)
        {
            return _uniforms.find(s)->second;
        }

        ge::VertexBuffer* TriangleMesh::getVertexBuffer(std::string s)
        {
            return _vertexBuffers.find(s)->second;
        }

        void TriangleMesh::registerVertexBuffer(std::string s, ge::VertexBuffer* vertexBuffer)
        {
            _vertexBuffers.insert({s, (GL::VertexBuffer *) vertexBuffer});
        }

        void TriangleMesh::setShaderGroup(ge::ShaderGroup* sg)
        {
            shaderGroup = (GL::ShaderGroup *) sg;
        }

        ge::ShaderGroup* TriangleMesh::getShaderGroup()
        {
            return shaderGroup;
        }

        TriangleMesh::~TriangleMesh()
        {
            for(auto u : _uniforms)
            {
                delete u.second;
            }

            glDeleteVertexArrays(1, &_vao);

        }

        TriangleMesh::TriangleMesh()
        {
        	ConsoleIO::print(std::to_string(_vao) + std::string("\n"));
            glGenVertexArrays(1,&_vao);
			ConsoleIO::print(std::to_string(_vao) + std::string("\n"));
        }

        void TriangleMesh::setIndexBuffer(ge::IndexBuffer *ib)
        {
            indexBuffer = (IndexBuffer *) ib;
        }

        ge::IndexBuffer *TriangleMesh::getIndexBuffer()
        {
            return indexBuffer;
        }

        void TriangleMesh::registerTexture(ge::Texture* tex, unsigned int unitId)
        {
            _textures.insert({unitId, (GL::Texture*)tex});
        }

	    void TriangleMesh::startRender()
	    {
			if (!isRendering)
			{
				isRendering = true;

				glUseProgram(shaderGroup->programID);

			}

	    }
    }
}
