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

void InitSubState(WindowManager* windowManager){ } 
void UpdateEventSubState(WindowManager* windowManager, sfEvent* evt) { }
void UpdateSubState(WindowManager* windowManager) { 
	printf("SubState\n"); 
	if (KEY_DOWN(A))
	{
		PopSubState();
	}
}
void RenderSubState(WindowManager* windowManager) { } 
void UIRenderSubState(WindowManager* windowManager) { } 
void DestroySubState(WindowManager* windowManager) { }
REGISTER_C_STATE(SubState)

sfCircleShape* CreateCircle(float radius, sfVector2f position, sfColor color)
{
	sfCircleShape* circle = sfCircleShape_create();
	sfCircleShape_setRadius(circle, radius);
	sfCircleShape_setPosition(circle, position);
	sfCircleShape_setFillColor(circle, color);
	return circle;
}

sfCircleShape* circle;

void Init___(WindowManager* window)
{
	RegisterSubState("SubState", window, sfTrue, sfTrue);
	circle = CreateCircle(80, sfVector2f_Create(100, 100), CreateColor(255, 0, 0, 255));

}
void UpdateEvent___(WindowManager* window, sfEvent* event)
{
}
void Update___(WindowManager* window)
{
	printf("MainState\n");

	if(KEY_DOWN(A))
	{
		PushSubState("SubState");
	}
}
void Render___(WindowManager* window)
{
	sfRenderWindow_drawCircleShape(window->GetWindow(window), circle, NULL);
}
void UIRender___(WindowManager* window)
{
}
void Destroy___(WindowManager* window)
{
		sfCircleShape_destroy(circle);

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
	/*Animation* anim = CreateAnimationFromFile("D:\\GitCours\\C\\C-Manager\\Animator\\Export\\test.anim", sfTexture_createFromFile("D:\\Cpp-Manager-for-SFML\\New_Manager\\Ressources\\ALL\\TEXTURES\\porte_intro_1224x855.png", NULL));
	sfRectangleShape_setPosition(anim->GetRenderer(anim), sfVector2f_Create(500, 250));
	StartGame(CreateWindowManager(1080, 720, "Test", sfDefaultStyle, NULL), "___", anim, "KeyAnim");*/
	ReportLeaks();
}