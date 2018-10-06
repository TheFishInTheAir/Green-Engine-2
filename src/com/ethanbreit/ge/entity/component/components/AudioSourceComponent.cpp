#include <ge/entity/component/components/AudioSourceComponent.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/engine/scene/Scene.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/audio/AudioController.h>
namespace ge
{


    Component* _constructor_AudioSourceComponent(Entity* ent)
    {
        return new AudioSourceComponent(ent);
    }

    ComponentConstructorRegistry::StartupHook AudioSourceComponent::_hook("AudioSourceComponent", _constructor_AudioSourceComponent);


    AudioSourceComponent::AudioSourceComponent(Entity* e) : Component(e)
    {
        source = new Audio::AudioSource();
        source->create(); //should be run on audio thread

		init();
    }
    AudioSourceComponent::AudioSourceComponent(Entity* e, Audio::AudioSource* source) : Component(e)
    {
        this->source = source;

		init();
        //TODO: fill out
    }

    void AudioSourceComponent::init()
    {
		addPublicVar("audio clip", { DataType::AUDIO_CLIP, &currentAudioSource });
        addPublicVar("loop", {DataType::BOOL, &(source->loop)});
        addPublicVar("play", {DataType::BOOL, &queuePlay});
        addPublicVar("pause", {DataType::BOOL, &queuePause});
        addPublicVar("stop", {DataType::BOOL, &queueStop});
    }


    void AudioSourceComponent::defaultInit()
    {
        //TODO: probably should remove this.
    }

    void AudioSourceComponent::insertToDefaultBatch() //TODO: add ingroup priorities
    {

        if(!ComponentManager::containsComponentBatch("DefaultComponentBatch", getTypeName()))
        {
            DefaultComponentBatch* cmp = new DefaultComponentBatch();
            cmp->setComponentType(getTypeName());

            ComponentManager::registerComponentBatch(cmp);
        }

        ComponentManager::getComponentBatch("DefaultComponentBatch", getTypeName())->softInsert(this);
    }

    void AudioSourceComponent::play()
    {
        queuePlay = true;
    }
    void AudioSourceComponent::pause()
    {
        queuePause = true;
    }
    void AudioSourceComponent::stop()
    {
        queueStop = true;
    }

    void AudioSourceComponent::cycle()
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
                Log::wrn("AudioSourceComponent", "An AudioSourceComponent requires a TransformComponent.");
                return;
            }
        }
		if (currentAudioSource != "NONE")
		{
			if (currentAudioSource != currentAudioSourceCACHED)
			{
				if (!Scene::currentScene->audioClips.count(currentAudioSource))
				{
					Log::critErr("AudioSourceComponent", "No Resource: " + currentAudioSource);
					return;
				}
				currentAudioSourceCACHED = currentAudioSource;
				source->setClip(Scene::currentScene->audioClips.at(currentAudioSource).get());
			}
		}

        if(!source->isCreated())
            source->create();
        if(source->getClip()!=nullptr)
        {
            if(queuePlay)
            {
                queuePlay=false;
                source->play();
            }
            if(queuePause)
            {
                queuePause=false;
                source->pause();
            }
            if(queueStop)
            {
                queueStop=false;
                source->stop();
            }

            source->position = transformComponent->getPosition();
            source->update();
        }
        
    }



    void AudioSourceComponent::destroy()
    {
    }

    std::string AudioSourceComponent::getTypeName()
    {
        return "AudioSourceComponent";
    }
}