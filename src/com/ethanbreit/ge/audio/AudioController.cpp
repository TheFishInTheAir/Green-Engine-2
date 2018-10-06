#include <ge/audio/AudioController.h>


#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <ge/console/Log.h>


namespace ge
{
    namespace Audio
    {
        ALCcontext* context;
        ALCdevice*  device;
        
        void alcErrCheck(std::string msg)
        {
            int e;
            if((e=alcGetError(device))!=ALC_NO_ERROR)
            {
                Log::critErr("AudioController", msg+" (err code: "+std::to_string(e)+")");
            }
        }

        void setListinerPos(glm::vec3 pos)
        {
            alListenerfv(AL_POSITION, &(pos[0]));
        }

        void setListinerOrientation(glm::vec3 up, glm::vec3 dir)
        {
            ALfloat orient[] = {dir.x, dir.y, dir.z, up.x, up.y, up.z};
            alListenerfv(AL_ORIENTATION, orient);
        }
        void setListinerOrientation(glm::quat dir)
        {
            //NOTE: wrong, actually maybe not...
            //glm::vec3 rot = glm::eulerAngles(dir);
            //Log::dbg(std::to_string(rot.y));
            //setListinerOrientation(glm::vec3(0,1,0), glm::vec3(sin(rot.y),0,cos(rot.y)));
            glm::vec3 tDir = -glm::vec3(0,0,1)*dir;

            setListinerOrientation(glm::vec3(0,1,0), glm::vec3(-tDir.x,0,tDir.z)); //ok this doesn't really work, but it's good enough for now...
        }

        void standardInit()
        {
            device = alcOpenDevice(nullptr);
            alcErrCheck("Couldn't Open Device");

			ALCint attribs[] = { ALC_HRTF_SOFT, 1, 0 };

            context = alcCreateContext(device, attribs);
            alcErrCheck("Couldn't Create Context");

            makeContextCurrent();
        }

        void makeContextCurrent()
        {
            alcMakeContextCurrent(context);
        }
    }
}