#include "Game.h"
#include "LoadingState.h"
#include "Particles.h"
#include <time.h>

int main(void)
{
	//sfRenderStates states;
	//states.shader = sfShader_createFromFile(NULL, NULL, "shader.frag");
	//states.transform = sfTransform_Identity;
	//states.blendMode = sfBlendAlpha;
	//states.texture = NULL;
	//srand(time(NULL));
	//sfShader_setVec2Uniform(states.shader, "rect_pos", (sfGlslVec2) { 500.f, 500.f });
	//sfShader_setVec2Uniform(states.shader, "rect_size", (sfGlslVec2) { 256.f, 256.f * 2 });

	//sfTexture* texture = sfTexture_createFromFile("E:\\BreakerEngine\\Ressources\\ALL\\Textures\\loading.png", NULL);
	//sfTexture_setRepeated(texture, sfFalse);

	//sfShader_setTextureUniform(states.shader, "texture", texture);

	//sfRenderWindow* window = sfRenderWindow_create((sfVideoMode_getDesktopMode()), "Shader", sfDefaultStyle, NULL);
	//sfEvent evt;
	//sfRectangleShape* rect = sfRectangleShape_create();
	//sfRectangleShape_setSize(rect, sfVector2f_Create(1920, 1080));
	//while (sfRenderWindow_isOpen(window))
	//{
	//	while (sfRenderWindow_pollEvent(window, &evt))
	//	{
	//		if (evt.type == sfEvtClosed)
	//		{
	//			sfRenderWindow_close(window);
	//			break;
	//		}
	//	}

	//	sfRenderWindow_clear(window, sfBlack);
	//	int count = rand_int(1, 256);
	//	sfShader_setIntUniform(states.shader, "count", count);
	//	for (size_t i = 0; i < count; i++)
	//	{
	//		char* particle = AddChar("particles[", AddChar(IntToString(i), "].rect_pos"));
	//		sfVector2f size = sfVector2f_Create(50.f, 50.f);
	//		sfShader_setVec2Uniform(states.shader, particle, sfVector2f_Create(rand_float(0, sfVideoMode_getDesktopMode().width - size.x), rand_float(0, sfVideoMode_getDesktopMode().height - size.y)));

	//		particle = AddChar("particles[", AddChar(IntToString(i), "].rect_size"));
	//		sfShader_setVec2Uniform(states.shader, particle, size);
	//		CleanUpTempMemory();
	//	}

	//	sfRenderWindow_drawRectangleShape(window, rect, &states);
	//	sfRenderWindow_display(window);
	//	Sleep(200000);
	//}







	InitResourcesManager("../Ressources");
	StartGame(CreateWindowManager(1920, 1080, "BreakerEngine", sfDefaultStyle, NULL), "MainMenu", "Loading", &ResetLoadingState);
}