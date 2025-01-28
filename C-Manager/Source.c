#include "SpriteManager.h"

#include "ThreadManager.h"
#include "WindowManager.h"
#include "Game.h"
#include "MemoryManagement.h"
#include "Animation.h"
#include "Particles.h"


Animation* anim;
sfCircleShape* uiCircle;

StateInfo menu;

Particles* particles;

void Init___(WindowManager* window)
{

	ParticleParam param = CreateDefaultParam(LIFE_TIME, sfVector2f_Create(100, 100), 0, 100);

	particles = CreateTextureParticles(param, GetTexture("test"), (sfIntRect) { 0, 0, 0, 0 });


}
void UpdateEvent___(WindowManager* window, sfEvent* event)
{
}
void Update___(WindowManager* window)
{
	if (KEY_UP(A))
	{
		printf("Up\n");
	}
	if (KEY_DOWN(A))
	{
		printf("Down\n");
	}

	UpdateParticles(particles, DeltaTime);
}
void Render___(WindowManager* window)
{
	sfRenderWindow_drawParticles(window->GetWindow(window), particles, NULL);
}
void UIRender___(WindowManager* window)
{
}
void Destroy___(WindowManager* window)
{
}

void setfpsmax(const WindowManager* window, void* fps_max)
{
	sfRenderWindow_setFramerateLimit(window->GetWindow(window), *(unsigned int*)fps_max);
}


REGISTER_C_STATE(___)

int main(void)
{
	// WindowManager* window_manager = CreateWindowManager(1080,720,"SFML_Works",sfDefaultStyle,NULL);
	// unsigned int fps = 400;
	// window_manager->AddNewCustomParam(window_manager, "FPS", setfpsmax,  &fps, sizeof(unsigned int));
	// fps = 200;
	// window_manager->SetCustomParam(window_manager,"FPS", &fps);
	// unsigned int* t = window_manager->GetCustomParam(window_manager,"FPS");
	// printf_d("%u",*t);
	//// free_d(calloc_d(int,0));
	//// free_d(calloc_d(int,0));
	//// free_d(calloc_d(int,0));
	////WindowManager* window_manager = CreateWindowManager(1080, 720, "SFML_Works", sfDefaultStyle, NULL);
	////unsigned int fps = 400;
	////window_manager->AddNewCustomParam(window_manager, "FPS", setfpsmax, &fps, sizeof(unsigned int));
	////fps = 200;
	////window_manager->SetCustomParam(window_manager, "FPS", &fps);
	////unsigned int* t = window_manager->GetCustomParam(window_manager, "FPS");
	////printf_d("%u", *t);
	////free_d(calloc_d(int, 0));
	////free_d(calloc_d(int, 0));
	////free_d(calloc_d(int, 0));


	//int a = 55;
	//printf_d("Test %d", a);
	////int a = 55;
	////printf_d("Test %d", a);

	//SpriteManager* sprite_manager = CreateSpriteManager();
	//sprite_manager->push_back(sprite_manager, "BG_1", NULL, sfTrue);


	//StateInfo menu;



	InitResourcesManager("../Ressources");
	StartGame(CreateWindowManager(1080, 720, "Test", sfDefaultStyle, NULL), "___", NULL, NULL);
	Animation* anim = CreateAnimationFromFile("D:\\GitCours\\C\\C-Manager\\Animator\\Export\\test.anim", sfTexture_createFromFile("D:\\Cpp-Manager-for-SFML\\New_Manager\\Ressources\\ALL\\TEXTURES\\porte_intro_1224x855.png", NULL));
	sfRectangleShape_setPosition(anim->GetRenderer(anim), sfVector2f_Create(500, 250));
	StartGame(CreateWindowManager(1080, 720, "Test", sfDefaultStyle, NULL), "___", anim, "KeyAnim");
	ReportLeaks();
}