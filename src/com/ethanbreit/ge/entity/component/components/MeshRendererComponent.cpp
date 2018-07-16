#include <ge/entity/component/components/MeshRendererComponent.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
namespace ge
{
    MeshRendererComponent::MeshRendererComponent(Entity* ent) : ge::Component(ent)
    {
        mesh = GraphicsCore::ctx->meshFactory->newTriangleMesh();
    }

    MeshRendererComponent::MeshRendererComponent(Entity* ent, TriangleMesh* mesh) : ge::Component(ent)
    {
        this->mesh = mesh;
    }

    
	void MeshRendererComponent::defaultInit()
	{
		//TODO: Make this good
	}
    
    void MeshRendererComponent::insertToDefaultBatch()
    {
            //TODO: make a batched rendering component
        if(!ComponentManager::containsComponentBatch("DefaultComponentBatch", getTypeName()))
        {
            ComponentBatch* cmp = new DefaultComponentBatch();
            cmp->setComponentType(getTypeName());
            
            ComponentManager::registerComponentBatch(cmp);
        }
        
        ComponentManager::getComponentBatch("DefaultComponentBatch", getTypeName())->softInsert(this);
    }
    
    void MeshRendererComponent::cycle()
    {
        /*{
            Log::dbg("GOOD t   " + std::to_string(mesh->isComplete()));
            Log::dbg("GOOD ibo " + std::to_string(mesh->getIndexBuffer()==nullptr));
            Log::dbg("GOOD sg  " + std::to_string(mesh->getShaderGroup()==nullptr));
        }*/
        if(mesh->isComplete())
        {

            //Inneficient TODO: make more gooder
            if(ent->components.count("TransformComponent") && mesh->containsUniform(DBL_STRINGIFY(MVP)))
            {
                mesh->setUniform(DBL_STRINGIFY(MVP), ge::Uniform::UniformContent(ent->getComponent<TransformComponent>("TransformComponent")->getMVP()));

            }
            
            mesh->render();

        }
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
