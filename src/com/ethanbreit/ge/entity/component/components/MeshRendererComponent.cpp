#include <ge/entity/component/components/MeshRendererComponent.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/entity/component/batches/PipelineComponentBatch.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/graphics/Camera.h>
#include <ge/engine/scene/Scene.h>
#include <ge/graphics/UniformAutomator.h>
namespace ge
{

    Component* _constructor_MeshRendererComponent(Entity* ent)
    {
        return new MeshRendererComponent(ent);
    }

    ComponentConstructorRegistry::StartupHook MeshRendererComponent::_hook("MeshRendererComponent", _constructor_MeshRendererComponent);




    MeshRendererComponent::MeshRendererComponent(Entity* ent) : ge::Component(ent)
    {
        mesh = GraphicsCore::ctx->meshFactory->newTriangleMesh();
        init();
    }

    MeshRendererComponent::MeshRendererComponent(Entity* ent, TriangleMesh* mesh) : ge::Component(ent)
    {
        this->mesh = mesh;
        init();
    }
    
    void MeshRendererComponent::init()
    {
        addPublicVar("Cull Backface",  {DataType::BOOL, &mesh->cullBackface});

    }
    
	void MeshRendererComponent::defaultInit()
	{
		//TODO: Make this good
	}
    
    void MeshRendererComponent::insertToDefaultBatch()
    {
            //TODO: make a batched rendering component
        if(!ComponentManager::containsComponentBatch("PipelineComponentBatch", getTypeName()))
        {
            ComponentBatch* cmp = new PipelineComponentBatch();
            cmp->setComponentType(getTypeName());
            
            ComponentManager::registerComponentBatch(cmp);
        }
        
        ComponentManager::getComponentBatch("PipelineComponentBatch", getTypeName())->softInsert(this);
    }
    
    void MeshRendererComponent::cycle()
    {

        if(GraphicsCore::ctx->currentPipeline->getState()!=PipelineState::Render)
            return;
        if(GraphicsCore::ctx->currentPipeline->getCurrentStage()->type!=PipelineDrawType::Default)
            return;
        /*{
            Log::dbg("GOOD t   " + std::to_string(mesh->isComplete()));
            Log::dbg("GOOD ibo " + std::to_string(mesh->getIndexBuffer()==nullptr));
            Log::dbg("GOOD sg  " + std::to_string(mesh->getShaderGroup()==nullptr));
        }*/
        PipelineStage* cps = GraphicsCore::ctx->currentPipeline->getCurrentStage();

        if(mesh->isComplete())
        {
            if(cps->overrideCamera)
            {
                if(ent->components.count("TransformComponent"))
                {
				//TODO: have good thing for good stuff. (have a seperate thing to handle these uniforms)
				    if(mesh->containsUniform(DBL_STRINGIFY(MVP)))
					    mesh->setUniform(DBL_STRINGIFY(MVP), ge::Uniform::UniformContent(cps->cam->vp*ent->getComponent<TransformComponent>("TransformComponent")->getModel()));

				    if(mesh->containsUniform(DBL_STRINGIFY(MVP_M)))
                    {
                    //Log::dbg(ent->name);
    					mesh->setUniform(DBL_STRINGIFY(MVP_M), ge::Uniform::UniformContent(ent->getComponent<TransformComponent>("TransformComponent")->getModel()));
                    }
                }
                
			    if (mesh->containsUniform(DBL_STRINGIFY(MVP_VP)))
    				mesh->setUniform(DBL_STRINGIFY(MVP_VP), ge::Uniform::UniformContent(cps->cam->vp));

			    if (mesh->containsUniform(DBL_STRINGIFY(MVP_V)))
    				mesh->setUniform(DBL_STRINGIFY(MVP_V), ge::Uniform::UniformContent(cps->cam->view));
			
			    if (mesh->containsUniform(DBL_STRINGIFY(MVP_P)))
    				mesh->setUniform(DBL_STRINGIFY(MVP_P), ge::Uniform::UniformContent(cps->cam->proj));
            }
            else
            {
            //Inneficient TODO: make more gooder
                if(ent->components.count("TransformComponent"))
                {
				//TODO: have good thing for good stuff. (have a seperate thing to handle these uniforms)
				    if(mesh->containsUniform(DBL_STRINGIFY(MVP)))
					    mesh->setUniform(DBL_STRINGIFY(MVP), ge::Uniform::UniformContent(ent->getComponent<TransformComponent>("TransformComponent")->getMVP()));

				    if(mesh->containsUniform(DBL_STRINGIFY(MVP_M)))
                    {
                    //Log::dbg(ent->name);
    					mesh->setUniform(DBL_STRINGIFY(MVP_M), ge::Uniform::UniformContent(ent->getComponent<TransformComponent>("TransformComponent")->getModel()));
                    }
                }
            
           
                //NOTE: THIS STUFF SHOULD NOT BE HERE BUT IT IS EASY AND CONVENIENT FOR TESTS
			    if (mesh->containsUniform(DBL_STRINGIFY(MVP_VP)) && Camera::displayCamera != nullptr)
    				mesh->setUniform(DBL_STRINGIFY(MVP_VP), ge::Uniform::UniformContent(Camera::displayCamera->vp));

			    if (mesh->containsUniform(DBL_STRINGIFY(MVP_V)) && Camera::displayCamera != nullptr)
    				mesh->setUniform(DBL_STRINGIFY(MVP_V), ge::Uniform::UniformContent(Camera::displayCamera->view));
			
			    if (mesh->containsUniform(DBL_STRINGIFY(MVP_P)) && Camera::displayCamera != nullptr)
    				mesh->setUniform(DBL_STRINGIFY(MVP_P), ge::Uniform::UniformContent(Camera::displayCamera->proj));
            }

			UniformAutomator::cycledUniformSetup(mesh);
            //Log::dbg("RENDER");
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
