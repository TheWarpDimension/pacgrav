#ifndef INCLUDED_SOUND_SYSTEM
#define INCLUDED_SOUND_SYSTEM


#include "lib/hash_table.h"
#include "sound/sound_stream_decoder.h"


class SoundSystem
{
public:
    SoundSystem();

    int PlayLoop(char const *sampleName);
    void PlayOnce(char const *sampleName);

    void Advance();

private:
    HashTable <SoundStreamDecoder *> m_samples;
};


extern SoundSystem g_soundSystem;


#endif
