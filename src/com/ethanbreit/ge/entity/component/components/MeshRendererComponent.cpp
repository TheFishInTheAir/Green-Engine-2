#include <ge/entity/component/components/MeshRendererComponent.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/graphics/Camera.h>
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
            if(ent->components.count("TransformComponent"))
            {
				//TODO: have good thing for good stuff. (have a seperate thing to handle these uniforms)
				if(mesh->containsUniform(DBL_STRINGIFY(MVP)))
					mesh->setUniform(DBL_STRINGIFY(MVP), ge::Uniform::UniformContent(ent->getComponent<TransformComponent>("TransformComponent")->getMVP()));

				if(mesh->containsUniform(DBL_STRINGIFY(MVP_M)))
					mesh->setUniform(DBL_STRINGIFY(MVP_M), ge::Uniform::UniformContent(ent->getComponent<TransformComponent>("TransformComponent")->getModel()));
            }

			if (mesh->containsUniform(DBL_STRINGIFY(MVP_VP)) && Camera::displayCamera != nullptr)
				mesh->setUniform(DBL_STRINGIFY(MVP_VP), ge::Uniform::UniformContent(Camera::displayCamera->vp));

			if (mesh->containsUniform(DBL_STRINGIFY(MVP_V)) && Camera::displayCamera != nullptr)
				mesh->setUniform(DBL_STRINGIFY(MVP_V), ge::Uniform::UniformContent(Camera::displayCamera->view));
			
			if (mesh->containsUniform(DBL_STRINGIFY(MVP_P)) && Camera::displayCamera != nullptr)
				mesh->setUniform(DBL_STRINGIFY(MVP_P), ge::Uniform::UniformContent(Camera::displayCamera->proj));
			
            mesh->render();

        }
		else
		{
			Log::dbg("GOOD t   " + std::to_string(mesh->isComplete()));
			Log::dbg("GOOD ibo " + std::to_string(mesh->getIndexBuffer() == nullptr));
			Log::dbg("GOOD sg  " + std::to_string(mesh->getShaderGroup() == nullptr));
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
