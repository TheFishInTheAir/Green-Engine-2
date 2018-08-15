#include <ge/audio/AudioSource.h>
#include <ge/console/Log.h>
#include <OpenAL/al.h>
namespace ge
{
    namespace Audio
    {
        const std::string LOG_TAG = "AudioSource";

        void AudioSource::setClip(AudioClip* clip)
        {
            this->clip = clip;
            if(!sIsCreated)
                Log::critErr(LOG_TAG, "Attempt to set clip without AudioSource being initialised(created).");

            alSourcei(sourceId, AL_BUFFER, clip->getALId());
        }

        AudioClip* AudioSource::getClip()
        {
            return clip;
        }

        void AudioSource::create()
        {
            alGenSources(1, &sourceId);
            sIsCreated = true;
        }
        
        bool AudioSource::isCreated()
        {
            return sIsCreated;
        }

        void AudioSource::update()
        {
            if(!sIsCreated)
                return; //NOTE: maybe we should show a warning or something...

            alSourcefv(sourceId, AL_POSITION, &position[0]);
            alSourcefv(sourceId, AL_VELOCITY, &velocity[0]);
            alSourcei(sourceId,   AL_LOOPING, loop);
        }

        void AudioSource::play()
        {
            if(clip==nullptr)
            {
                Log::err(LOG_TAG, "AudioClip is not set when trying to be played. Returning without throwing.");
                return;
            }
            if(!sIsCreated)
            {
                Log::err(LOG_TAG, "Attempt to play clip without AudioSource being initialised(created). Returning without throwing.");
                return;
            }

            alSourcePlay(sourceId);
        }

        void AudioSource::pause()
        {
            if(clip==nullptr)
            {
                Log::err(LOG_TAG, "AudioClip is not set when trying to be paused. Returning without throwing.");
                return;
            }
            if(!sIsCreated)
            {
                Log::err(LOG_TAG, "Attempt to pause clip without AudioSource being initialised(created). Returning without throwing.");
                return;
            }
            
            alSourcePause(sourceId);
        }

        void AudioSource::stop()
        {
            if(clip==nullptr)
            {
                Log::err(LOG_TAG, "AudioClip is not set when trying to be stopped. Returning without throwing.");
                return;
            }
            if(!sIsCreated)
            {
                Log::err(LOG_TAG, "Attempt to stop clip without AudioSource being initialised(created). Returning without throwing.");
                return;
            }
            
            alSourceStop(sourceId);
        }

        bool AudioSource::isPlaying()// NOTE: must be run on context thread
        {
            int state;
            alGetSourcei(sourceId, AL_SOURCE_STATE, &state);

            return state == AL_PLAYING;
        }

        void AudioSource::destroy()
        {
            //TODO: implement
        }
    }
}