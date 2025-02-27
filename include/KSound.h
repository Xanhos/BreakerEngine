#pragma once

#ifdef KSOUND_EXPORTS
#define KSOUND_API  __declspec(dllexport)
#else 
#define KSOUND_API  __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef struct KSound KSound;
typedef unsigned char KBool;

typedef enum
{
    kStopped, ///< Sound / music is not playing
    kPaused,  ///< Sound / music is paused
    kPlaying  ///< Sound / music is playing
} KSoundStatues;


#define KTRUE 1
#define KFALSE 0

KSound KSOUND_API* KSound_createSoundFromFile(const char* path);

void KSOUND_API KSound_play(KSound* sound);

void KSOUND_API KSound_stop(KSound* sound);

void KSOUND_API KSound_pause(KSound* sound);

void KSOUND_API KSound_destroy(KSound* sound);

void KSOUND_API KSound_setLoop(KSound* sound, KBool loop);
KBool KSOUND_API KSound_getLoop(KSound* sound);

KSoundStatues KSOUND_API KSound_getStatues(KSound* sound);

void KSOUND_API KSound_setVolume(KSound* sound, float volume);

#ifdef __cplusplus
}
#endif