#include <ge/loader/LoadAudioClip.h>
#include <ge/util/ResourceUtil.h>

#define STB_VORBIS_HEADER_ONLY
#include <stb/stb_vorbis.c>
namespace ge
{
    namespace AudioClipLoader
    {
        Audio::AudioClip* loadAudioClip(std::string url, bool isResource)
        {
            Audio::AudioClip* ac = new Audio::AudioClip();
            ac->samples = stb_vorbis_decode_filename((isResource ? ResourceUtil::getResPath(url) : url).c_str(), &(ac->channels), &(ac->sampleRate), &(ac->data));
			ac->url = url;
            return ac;
        }
    }
}