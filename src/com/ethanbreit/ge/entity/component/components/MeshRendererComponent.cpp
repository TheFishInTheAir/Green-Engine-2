#include <ge/entity/component/components/MeshRendererComponent.h>
#include <ge/graphics/GraphicsCore.h>
namespace ge
{
    MeshRendererComponent::MeshRendererComponent(Entity* ent) : ge::Component(ent)
    {
        mesh = GraphicsCore::ctx->meshFactory->newTriangleMesh();
    }

	void MeshRendererComponent::defaultInit()
	{
		//TODO: Make this good
	}
    
    void MeshRendererComponent::insertToDefaultBatch()
    {
        //TODO: make BatchedRendererComponent.
    }
    
    void MeshRendererComponent::cycle()
    {
        if(mesh->isComplete())
            mesh->render();
    }
    
    void MeshRendererComponent::destroy()
    {
        delete mesh;
    }
    
    std::string MeshRendererComponent::getTypeName()
    {
        return "MeshRendererComponent";
    }
}
