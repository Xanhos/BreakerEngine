#include "SpriteManager.h"

#include "ThreadManager.h"
#include "WindowManager.h"
#include "Game.h"
#include "MemoryManagement.h"
#include "Animation.h"
#include "Particles.h"
#include "UI.h"
#include "Export.h"

Animation* anim;
sfCircleShape* uiCircle;

StateInfo menu;
Particles* particles;

UIObjectManager* uiManager;

void callback(UIObject* object, WindowManager* window)
{
	if (object->isHover)
	{
		object->setColor(object, CreateColor(0, 0, 255, 255));
	}
	else
	{
		object->setColor(object, CreateColor(255, 0, 0, 255));
	}

	if (object->isClicked)
	{
		PopSubState();
	}
}


void InitSubState(WindowManager* windowManager)
{
	LoadScene("Menu");
	uiManager = CreateUIObjectManager();
	

	uiManager->Add(uiManager, CreateUIObjectFromCircleShape(NULL, "Play", sfMouseLeft, -1));
	uiManager->Add(uiManager, CreateUIObjectFromCircleShape(NULL, "Quit", sfMouseLeft, -1));

	UIObject* button = uiManager->GetFromName(uiManager, "Play");
	button->setTexture(button, GetTexture("t"), sfTrue);
	button->setPosition(button, sfVector2f_Create(400, 200));
	button->setSize(button, sfVector2f_Create(100, 100));
	button->setColor(button, CreateColor(255, 0, 0, 255));
	button->setUpdateFunction(button, &callback);

	button = uiManager->GetFromName(uiManager, "Quit");
	button->setTexture(button, GetTexture("t"), sfTrue);
	button->setPosition(button, sfVector2f_Create(400, 400));
	button->setSize(button, sfVector2f_Create(100, 100));
	button->setColor(button, CreateColor(255, 0, 0, 255));
	button->setUpdateFunction(button, &callback);

	return;
}

void UpdateEventSubState(WindowManager* windowManager, sfEvent* evt) { }

void UpdateSubState(WindowManager* windowManager) 
{ 
	uiManager->Update(uiManager, windowManager);
	UIObject* playButton = uiManager->GetFromName(uiManager, "Play");



	if (KEY_DOWN(E))
	{
		PopSubState();
	}
}
void RenderSubState(WindowManager* windowManager) 
{
	uiManager->Render(uiManager, windowManager);
}
void UIRenderSubState(WindowManager* windowManager)
{ 
} 

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
	RegisterSubState("SubState", window, sfTrue, sfFalse);
	circle = CreateCircle(80, sfVector2f_Create(100, 100), CreateColor(255, 0, 0, 255));

}
void UpdateEvent___(WindowManager* window, sfEvent* event)
{
}
void Update___(WindowManager* window)
{
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
	InitResourcesManager("../Ressources");
	StartGame(CreateWindowManager(1080, 720, "Test", sfDefaultStyle, NULL), "___", NULL, NULL);
	ReportLeaks();
}