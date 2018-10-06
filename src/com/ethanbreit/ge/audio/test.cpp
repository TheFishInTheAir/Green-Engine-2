#include <ge/audio/test.h>

#include <AL/al.h>
#include <AL/alc.h>

#define STB_VORBIS_HEADER_ONLY
#include <stb/stb_vorbis.c>

#include <ge/console/Log.h>
namespace ge
{
    void runOpenALTest()
    {
        //stb_vorbis
        int channels, sample_rate, samples;
        short* data;

        samples = stb_vorbis_decode_filename("/Users/garthbreit/Desktop/ge_test.ogg", &channels, &sample_rate, &data);

        Log::dbg("Channels: "+std::to_string(channels));
        Log::dbg("Sample Rate: "+std::to_string(sample_rate));
        Log::dbg("Samples: "+std::to_string(samples));
        

        //OpenAL
        ALuint buffer, source;
        ALCdevice *device = nullptr;
        ALCcontext *context = nullptr;
        
        device = alcOpenDevice(NULL);
        if(device==nullptr)
            Log::critErr("couldn't create OpenAL Device");

        context = alcCreateContext((ALCdevice*)device, NULL);
        if(context==nullptr)
            Log::critErr("couldn't create OpenAL Context");


        alcMakeContextCurrent(context); //VERY IMPORTANT

        alGenBuffers(1, &buffer);
        

        alBufferData(buffer, AL_FORMAT_STEREO16, data, samples*2*sizeof(short), sample_rate);
        
        ALfloat SourcePos[] = { 3.0, 0.0, 0.0 };
        ALfloat ListinerPos[] = { 0.0, 0.0, 0.0 };


        ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };
        alListenerfv(AL_POSITION,    ListinerPos);
        alListenerfv(AL_VELOCITY,    ListinerPos);
        alListenerfv(AL_ORIENTATION, ListenerOri);
                
        alGenSources(1, &source);



        alSourcei (source, AL_BUFFER, buffer);
        alSourcef (source, AL_PITCH, 1.0f);
        alSourcef (source, AL_GAIN,  1.0f);
        alSourcefv(source, AL_POSITION, SourcePos);
        alSourcefv(source, AL_VELOCITY, ListinerPos);
        alSourcei (source, AL_LOOPING,  true     );


        //alSourceQueueBuffers(source, 1, &buffer);

        alSourcePlay(source);
        

        int i = 0;

        while(true)
        {
            
            //alGetSourcei(source, AL_SOURCE_STATE, &i);
            //Log::critErr("Source State:"+std::to_string(i) +", play_state:"+std::to_string(AL_PLAYING)+", stop_state"+std::to_string(AL_STOPPED));
        }
        Log::critErr("sound finished");
    }
}