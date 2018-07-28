//
// Created by Ethan Breit on 2017-07-26.
//


#include <ge/graphics/abs/OpenGL/Meshes/GLTriangleMesh.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/console/Log.h>
#include <iostream>

namespace ge
{
	namespace GL
	{
        const std::string LOG_TAG = "GLTriangleMesh";
        
        void TriangleMesh::applyUniform(ge::Uniform u)
        {
            switch(u.getUniformType())
            {
                case ge::Uniform::FLOAT:
                    glUniform1f(u.descriptorId, u.fv1);
                    break;
                case ge::Uniform::INT:
                    glUniform1i(u.descriptorId, u.iv1);
                    break;
                case ge::Uniform::FVEC2:
                    glUniform2f(u.descriptorId, u.fv2.x, u.fv2.y);
                    break;
                case ge::Uniform::FVEC3:
                    glUniform3f(u.descriptorId, u.fv3.x, u.fv3.y, u.fv3.z);
                    break;
                case ge::Uniform::FVEC4:
                    glUniform4f(u.descriptorId, u.fv4.x, u.fv4.y, u.fv4.z, u.fv4.w);
                    break;
                case ge::Uniform::MAT2:
                    glUniformMatrix2fv(u.descriptorId, 1,GL_FALSE, &u.fm2[0][0]);
                    break;
                case ge::Uniform::MAT3:
                    glUniformMatrix3fv(u.descriptorId, 1,GL_FALSE, &u.fm3[0][0]);
                    break;
                case ge::Uniform::MAT4:
                    glUniformMatrix4fv(u.descriptorId, 1,GL_FALSE, &u.fm4[0][0]);
                    break;
            }
        }
        
		void  TriangleMesh::render()
		{
            if(cullBackface)
                glEnable(GL_CULL_FACE); //TODO: COMMENT OUT
            else
                glDisable(GL_CULL_FACE);

                
			startRender();

			glBindVertexArray(_vao);
            
            for(auto u : shaderGroup->uniforms)
            {
                applyUniform(u.second);
            }
            
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->id);
			for (auto tex : _textures)
			{
				//TODO: Check if Texture is already loaded (BATCHING) @UNFINISHED
				// Check if Texture is already loaded (will be part of batching system)
				{
					int currentID;
					glGetIntegerv(GL_TEXTURE0 + tex.first, &currentID); // Should not return anything below 0 so comparison between int and unsigned int SHOULD be safe
					if(tex.second->id == currentID)
					{
						continue;
					}
				}

				glActiveTexture(GL_TEXTURE0 + tex.first);
				glBindTexture(GL_TEXTURE_2D, tex.second->id);
			}

			for (auto cMap : _cMaps)
			{
				//glUniform1(glGetUniformLocation(shaderGroup->programID, DBL_STRINGIFY(CUBEMAP_0)), cMap.second->id);
				glActiveTexture(GL_TEXTURE0 + cMap.first);
				glBindTexture(GL_TEXTURE_CUBE_MAP, cMap.second->id);
			}

			glDrawElements(GL_TRIANGLES, indexBuffer->data->length, GL_UNSIGNED_INT, 0);

			isRendering = false;
		}

		void TriangleMesh::rebuffer()
		{
			glBindVertexArray(_vao);
			unsigned int i = 0;
			for (auto vb : _vertexBuffers)
			{
				glEnableVertexAttribArray(vb.second->data->attributeId);

				vb.second->bind();
				glVertexAttribPointer(vb.second->data->attributeId, vb.second->data->sizePerAttrib, EnumUtil::getTrueDataType(vb.second->data->dataType),
					(GLboolean)vb.second->data->normalized, 0, vb.second->data->offset);

				i++;
			}
		}

        bool TriangleMesh::isComplete()
        {
            if (shaderGroup==nullptr)
                return false;
            if(indexBuffer==nullptr)
                return false;
            if(_vertexBuffers.size()==0)
                return false;
                return true;
        }

		void TriangleMesh::registerUniform(std::string name)
		{
			/*//ge::GL::Uniform* uniform = new GL::Uniform;
			uniform->parentMesh = this;
			uniform->identifier = name;
			uniform->id = glGetUniformLocation(shaderGroup->programID, name.c_str());
			_uniforms.insert({ name, uniform });*/
		}

		ge::Uniform* TriangleMesh::getUniform(std::string s)
		{
			return _uniforms.find(s)->second;
		}
        
        void TriangleMesh::setUniform(std::string s, ge::Uniform::UniformContent value)
        {
            if(shaderGroup->uniforms.count(s))
            {
                switch(shaderGroup->uniforms.at(s).getUniformType())
                {
                    case ge::Uniform::FLOAT:
                        shaderGroup->uniforms.at(s).fv1 = value.fv1;
                        break;
                    case ge::Uniform::INT:
                        shaderGroup->uniforms.at(s).iv1 = value.iv1;
                        break;
                    case ge::Uniform::FVEC2:
                        shaderGroup->uniforms.at(s).fv2 = value.fv2;
                        break;
                    case ge::Uniform::FVEC3:
                        shaderGroup->uniforms.at(s).fv3 = value.fv3;
                        break;
                    case ge::Uniform::FVEC4:
                        shaderGroup->uniforms.at(s).fv4 = value.fv4;
                        break;
                    case ge::Uniform::MAT2:
                        shaderGroup->uniforms.at(s).fm2 = value.fm2;
                        break;
                    case ge::Uniform::MAT3:
                        shaderGroup->uniforms.at(s).fm3 = value.fm3;
                        break;
                    case ge::Uniform::MAT4:
                        shaderGroup->uniforms.at(s).fm4 = value.fm4;
                        break;
                }
            }
            else
            {
                Log::err("Uniform '"+s+"' is not available in Current Program.");
            }
        }

		ge::VertexBuffer* TriangleMesh::getVertexBuffer(std::string s)
		{
			return _vertexBuffers.find(s)->second;
		}

		void TriangleMesh::registerVertexBuffer(std::string s, ge::VertexBuffer* vertexBuffer)
		{
			_vertexBuffers[s] = (GL::VertexBuffer *) vertexBuffer;
		}

		void TriangleMesh::setShaderGroup(ge::ShaderGroup* sg)
		{
			shaderGroup = (GL::ShaderGroup *) sg;
            
            //NOTE: THIS SHOULD NOT BE HERE
            for(auto key : shaderGroup->uniforms)
            {
                shaderGroup->uniforms.at(key.first).descriptorId = glGetUniformLocation(shaderGroup->programID, key.first.c_str());
            }
		}
        
        bool TriangleMesh::containsUniform(std::string u)
        {
            return (bool) shaderGroup->uniforms.count(u);
        }


		ge::ShaderGroup* TriangleMesh::getShaderGroup()
		{
			return shaderGroup;
		}

		TriangleMesh::~TriangleMesh()
		{
			for (auto u : _uniforms)
			{
				free(u.second);
			}

			glDeleteVertexArrays(1, &_vao);

		}

		TriangleMesh::TriangleMesh()
		{
			glGenVertexArrays(1, &_vao);
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
			if (unitId <= GlobalMemory::get("ge_max_texture_units").getData<int>())
			{
				_textures[unitId] = (GL::Texture*)tex;
			}
			else
			{
                Log::critErr(LOG_TAG, "GL: texture unit ID(" + std::to_string(unitId) + ") exceeds maximum(" + std::to_string(GlobalMemory::get("ge_max_texture_units").getData<int>()) + ")!");
			}
        }
        std::vector<ge::Texture*> TriangleMesh::getTextures()
        {
            std::vector<ge::Texture*> outTexs;
            for(auto t : _textures)
                outTexs.push_back((ge::Texture*)t.second);
            return outTexs;
        }
		void TriangleMesh::registerCubeMap(ge::CubeMap* cMap, unsigned int unitId)
        {
			if (unitId <= GlobalMemory::get("ge_max_texture_units").getData<int>())
			{
				_cMaps[unitId] =  (GL::CubeMap*)cMap;
			}
			else
			{
				Log::critErr(LOG_TAG, "GL: texture unit ID(" + std::to_string(unitId) + ") exceeds maximum(" + std::to_string(GlobalMemory::get("ge_max_texture_units").getData<int>()) + ")!");
			}
        }

	    void TriangleMesh::startRender()
	    {
			if (!isRendering)
			{
				isRendering = true;
				glUseProgram(shaderGroup->programID);
			}
	    }




        void TriangleMesh::deepDestroy()
        {
            if(indexBuffer != nullptr)
                delete indexBuffer;
            for(auto v : _vertexBuffers)
                delete v.second;
            


            /*//NOTE: I just copied this from the destructor, is it safe for an object to delete itself? TODO: maybe delete this...
            for (auto u : _uniforms)
			{
				free(u.second);
			}

			glDeleteVertexArrays(1, &_vao);*/
        }
    }
}
