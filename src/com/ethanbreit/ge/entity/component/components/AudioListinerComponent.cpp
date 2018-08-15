#include <ge/entity/component/components/AudioListinerComponent.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/audio/AudioController.h>
namespace ge
{


    Component* _constructor_AudioListinerComponent(Entity* ent)
    {
        return new AudioListinerComponent(ent);
    }

    ComponentConstructorRegistry::StartupHook AudioListinerComponent::_hook("AudioListinerComponent", _constructor_AudioListinerComponent);


    AudioListinerComponent::AudioListinerComponent(Entity* e) : Component(e)
    {
        //TODO: fill out
    }

    void AudioListinerComponent::defaultInit()
    {
        //TODO: probably should remove this.
    }

    void AudioListinerComponent::insertToDefaultBatch() //TODO: add ingroup priorities
    {

        if(!ComponentManager::containsComponentBatch("DefaultComponentBatch", getTypeName()))
        {
            DefaultComponentBatch* cmp = new DefaultComponentBatch();
            cmp->setComponentType(getTypeName());

            ComponentManager::registerComponentBatch(cmp);
        }

        ComponentManager::getComponentBatch("DefaultComponentBatch", getTypeName())->softInsert(this);
    }

    void AudioListinerComponent::cycle()
    {
        if(transformComponent == nullptr)
        {

            if(ent->components.count("TransformComponent"))
            {
                transformComponent = (TransformComponent*) ent->components.at("TransformComponent");
                
                Log::dbg("good test");
                Log::dbg("good test2");

            }
            else
            {
                Log::wrn("AudioListinerComponent", "An AudioListinerComponent requires a TransformComponent.");
                return;
            }
        }
        Audio::setListinerPos(transformComponent->getPosition());
        Audio::setListinerOrientation(transformComponent->getRotation());   

    }



    void AudioListinerComponent::destroy()
    {
    }

    std::string AudioListinerComponent::getTypeName()
    {
        return "AudioListinerComponent";
    }
}