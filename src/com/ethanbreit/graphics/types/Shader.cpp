#include <graphics/types/Shader.h>
#include "memory/GlobalMemory.h"
#include "graphics/GraphicsCore.h"
#include <engine/global_settings.pre>

ge::Shader::~Shader()
{
	
	ge::GlobalMemory::get(DBL_STRINGIFY(RENDERER_INSTANCE)).getData<GraphicsCore>().shaderFactory->destroyShader(this);
	
}
