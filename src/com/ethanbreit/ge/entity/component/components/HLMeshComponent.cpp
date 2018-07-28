#include <ge/entity/component/components/HLMeshComponent.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/graphics/Camera.h>
#include <ge/engine/scene/Scene.h>
#include <ge/graphics/UniformAutomator.h>

namespace ge
{

    Component* _constructor_HLMeshComponent(Entity* ent)
    {
        return new HLMeshComponent(ent);
    }

    ComponentConstructorRegistry::StartupHook HLMeshComponent::_hook("HLMeshComponent", _constructor_HLMeshComponent);


    HLMeshComponent::HLMeshComponent(Entity* ent) : ge::Component(ent)
    {
        Log::dbg("test1");
        mesh = new HLMesh();
        init();
    }

    HLMeshComponent::HLMeshComponent(Entity* ent, HLMesh* mesh) : ge::Component(ent)
    {
        this->mesh = mesh;
        init();
    }
    
    void HLMeshComponent::init()
    {

        addPublicVar("Sketch Material",  {DataType::MATERIAL, &sketchMatTag});
        addPublicVar("Sketch Mesh",  {DataType::MESH_DATA, &sketchMeshTag});
    }
    
	void HLMeshComponent::defaultInit()
	{
		//TODO: Make this good
	}
    
    void HLMeshComponent::insertToDefaultBatch()
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
    
    void HLMeshComponent::cycle()
    {

        if(sketchCachedMeshTag != sketchMeshTag && sketchMeshTag!="NULL")
        {
            mesh->setMeshData(sketchMeshTag);
            sketchCachedMeshTag = sketchMeshTag;
        }
        if(sketchCachedMatTag != sketchMatTag && sketchMatTag!="NULL")
        {
            mesh->setMaterial(sketchMatTag); 
            sketchCachedMatTag = sketchMatTag;
        }

        if(mesh->getMesh()==nullptr)
            return;

        if(mesh->getMesh()->isComplete())
        {

            //Inneficient TODO: make more gooder
            if(ent->components.count("TransformComponent"))
            {
				//TODO: have good thing for good stuff. (have a seperate thing to handle these uniforms)
				if(mesh->getMesh()->containsUniform(DBL_STRINGIFY(MVP)))
					mesh->getMesh()->setUniform(DBL_STRINGIFY(MVP), ge::Uniform::UniformContent(ent->getComponent<TransformComponent>("TransformComponent")->getMVP()));

				if(mesh->getMesh()->containsUniform(DBL_STRINGIFY(MVP_M)))
                {
                    //Log::dbg(ent->name);
					mesh->getMesh()->setUniform(DBL_STRINGIFY(MVP_M), ge::Uniform::UniformContent(ent->getComponent<TransformComponent>("TransformComponent")->getModel()));
                }
            }

            //NOTE: THIS STUFF SHOULD NOT BE HERE BUT IT IS EASY AND CONVENIENT FOR TESTS
			if (mesh->getMesh()->containsUniform(DBL_STRINGIFY(MVP_VP)) && Camera::displayCamera != nullptr)
				mesh->getMesh()->setUniform(DBL_STRINGIFY(MVP_VP), ge::Uniform::UniformContent(Camera::displayCamera->vp));

			if (mesh->getMesh()->containsUniform(DBL_STRINGIFY(MVP_V)) && Camera::displayCamera != nullptr)
				mesh->getMesh()->setUniform(DBL_STRINGIFY(MVP_V), ge::Uniform::UniformContent(Camera::displayCamera->view));
			
			if (mesh->getMesh()->containsUniform(DBL_STRINGIFY(MVP_P)) && Camera::displayCamera != nullptr)
				mesh->getMesh()->setUniform(DBL_STRINGIFY(MVP_P), ge::Uniform::UniformContent(Camera::displayCamera->proj));

			UniformAutomator::cycledUniformSetup(mesh->getMesh());
            //Log::dbg("RENDER");
            mesh->render();

        }
		else
		{
			Log::dbg(ent->name+": GOOD t   " + std::to_string(mesh->getMesh()->isComplete()));
			Log::dbg(ent->name+": GOOD ibo " + std::to_string(mesh->getMesh()->getIndexBuffer() == nullptr));
			Log::dbg(ent->name+": GOOD sg  " + std::to_string(mesh->getMesh()->getShaderGroup() == nullptr));
		}
    }


    void HLMeshComponent::setMeshData(std::string md)
    {
        sketchMeshTag = md;
        sketchCachedMeshTag = md;
        mesh->setMeshData(sketchMeshTag);
    }

    void HLMeshComponent::setMaterial(std::string md)
    {
        sketchMatTag = md;
        sketchCachedMatTag = md;
        mesh->setMaterial(sketchMatTag);
    }
    
    void HLMeshComponent::destroy()
    {
        delete mesh;
    }
    
    std::string HLMeshComponent::getTypeName()
    {
        return "HLMeshComponent";
    }
}


