#include "lib/universal_include.h"
#include "sound/sound_system.h"

#include "lib/binary_stream_readers.h"
#include "lib/sound_library_2d.h"


SoundSystem g_soundSystem;
SoundStreamDecoder *g_sample = NULL;


void SoundCallBack(StereoSample *buf, unsigned int numSamples)
{
    static short *monoBuf = new short[20000];
    if (numSamples > 20000)
        numSamples = 20000;
    
    if (g_sample)
    {
        g_sample->Read(monoBuf, numSamples);
    
        for (int i = 0; i < numSamples; i++)
        {
            buf[i].m_left = monoBuf[i];
            buf[i].m_right = monoBuf[i];
        }
    }
}


SoundSystem::SoundSystem()
{
    g_soundLibrary2d = new SoundLibrary2d();
    g_soundLibrary2d->SetCallback(&SoundCallBack);
}


void SoundSystem::Advance()
{
    g_soundLibrary2d->TopupBuffer();
}


int SoundSystem::PlayLoop(char const *sampleName)
{
    return 0;
}


void SoundSystem::PlayOnce(char const *sampleName)
{
    BinaryFileReader sampFile(sampleName);
    g_sample = new SoundStreamDecoder(&sampFile);
}
