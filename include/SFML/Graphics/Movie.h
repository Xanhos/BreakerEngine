#ifndef CSFEMOVIE_MOVIE_H
#define CSFEMOVIE_MOVIE_H

#include "Types.h"
#include "Export.h"
#include "SFML/System/Time.h"
#include "SFML/System/Vector2.h"
#include "SFML/Graphics/Rect.h"
#include "SFML/Graphics/Texture.h"
#include "SFML/Graphics/RenderStates.h"
#include "SFML/Audio/SoundStatus.h"

CSFML_GRAPHICS_API sfeMovie* sfeMovie_createFromFile(const char* filename);

CSFML_GRAPHICS_API void sfeMovie_destroy(sfeMovie* movie);

CSFML_GRAPHICS_API void sfeMovie_play(sfeMovie* movie);

CSFML_GRAPHICS_API void sfeMovie_pause(sfeMovie* movie);

CSFML_GRAPHICS_API void sfeMovie_stop(sfeMovie* movie);

CSFML_GRAPHICS_API void sfeMovie_update(sfeMovie* movie);

CSFML_GRAPHICS_API void sfeMovie_setVolume(sfeMovie* movie, float volume);

CSFML_GRAPHICS_API float sfeMovie_getVolume(const sfeMovie* movie);

CSFML_GRAPHICS_API sfTime sfeMovie_getDuration(const sfeMovie* movie);

CSFML_GRAPHICS_API sfVector2f sfeMovie_getSize(const sfeMovie* movie);

CSFML_GRAPHICS_API void sfeMovie_fit(sfeMovie* movie, sfFloatRect frame, sfBool preserveRatio);

CSFML_GRAPHICS_API float sfeMovie_getFramerate(const sfeMovie* movie);

CSFML_GRAPHICS_API unsigned int sfeMovie_getSampleRate(sfeMovie* movie);

CSFML_GRAPHICS_API unsigned int sfeMovie_getChannelCount(sfeMovie* movie);

CSFML_GRAPHICS_API sfSoundStatus sfeMovie_getStatus(sfeMovie* movie);

CSFML_GRAPHICS_API sfTime sfeMovie_getPlayingOffset(sfeMovie* movie);

CSFML_GRAPHICS_API sfBool sfeMovie_setPlayingOffset(sfeMovie* movie, sfTime timeOffset);

CSFML_GRAPHICS_API sfTexture* sfeMovie_getCurrentImage(sfeMovie* movie);

CSFML_GRAPHICS_API void sfeMovie_setLoop(sfeMovie* movie, sfBool loop);

CSFML_GRAPHICS_API void sfRenderWindow_drawMovie(sfRenderWindow* renderWindow, const sfeMovie* movie, const sfRenderStates* states);


#endif CSFEMOVIE_MOVIE_H