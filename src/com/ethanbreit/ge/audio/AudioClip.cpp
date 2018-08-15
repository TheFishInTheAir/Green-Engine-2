#include <ge/audio/AudioClip.h>
#include <OpenAL/al.h>
#include <ge/console/Log.h>
namespace ge
{
    namespace Audio
    {
        unsigned int getFormat(int channels)
        {
            if(channels==1)
                return AL_FORMAT_MONO16;
            if(channels==2)
                return AL_FORMAT_STEREO16;
            Log::critErr("AudioClip", "Unsupported number of channels: '"+std::to_string(channels)+"'.");
            return 0;
        }

        unsigned int AudioClip::getALId()
        {
            if(!bufferExists)
                Log::critErr("AudioClip", "Attempt to get OpenAL Buffer Id with buffer being generated.");
            return bufferId;
        }

        void AudioClip::upload()
        {
            if(!bufferExists)
            {
                alGenBuffers(1,&bufferId);
                bufferExists = true;
            }
            alBufferData(bufferId, getFormat(channels), data, samples*channels*sizeof(short), sampleRate);
        }

        bool AudioClip::isReady()
        {
            return bufferExists;
        }
    }
}