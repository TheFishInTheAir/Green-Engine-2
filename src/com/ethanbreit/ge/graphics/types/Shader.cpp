#include <ge/graphics/types/Shader.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/graphics/GraphicsCore.h>
#include <engine/global_settings.pre>
#include <ge/util/PreprocessorUtil.h>

ge::Shader::~Shader()
{
	
	ge::GlobalMemory::get(DBL_STRINGIFY(RENDERER_INSTANCE)).getData<GraphicsCore>().shaderFactory->destroyShader(this);
	
}
