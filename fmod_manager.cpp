//Windows includes
#include "lib/universal_include.h"
#include "lib/debug_utils.h"
#include "lib/input.h"
#include <Windows.h>
#include <stdio.h>
//FMod includes

#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Programmers API Windows/api/inc/fmod.hpp"
#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Programmers API Windows/api/inc/fmod_errors.h"

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
		ReleaseAssert( false, "FMOD_MANAGER.CPP SAYS - FMOD HAS CRASHED, BLAME BONCEY'S FMOD CODE.");
    }
}

int main(int argc, char *argv[])
{
    FMOD::System     *system;
    FMOD::Sound      *sound1, *sound2, *sound3;
    FMOD::Channel    *channel = 0;
    FMOD_RESULT       result;
    int               key;
    unsigned int      version;

    /*
        Create a System object and initialize.
    */
    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 0;
    }

    result = system->init(32, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);

	result = system->createSound("../sounds/goy.wav", FMOD_HARDWARE, 0, &sound1);
    ERRCHECK(result);
	
	result = sound1->setMode(FMOD_LOOP_OFF);
    ERRCHECK(result);

	if (g_keyDowns[KEY_CONTROL, KEY_T])
		result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &channel);
}