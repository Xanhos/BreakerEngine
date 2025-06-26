#include "openGLParticles.h"


int main() {
	// Initialize SFML window
	sfVideoMode mode = { 1200, 800, 32 };
	sfRenderWindow* window = sfRenderWindow_create(mode, "GPU Particle System", sfResize | sfClose, NULL);

	if (!window) {
		printf("Failed to create SFML window\n");
		return -1;
	}

	sfRenderWindow_setActive(window, sfTrue);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW\n");
		return -1;
	}

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

	// Initialize particle system with different shapes
	ParticleSystem triangleSystem = createEmptyParticlesSystem(), squareSystem = createEmptyParticlesSystem(), circleSystem = createEmptyParticlesSystem();
	initParticleSystem(&triangleSystem, 3, 10240, window);  // Triangle
	initParticleSystem(&squareSystem, 4, 1024 * 10, window);    // Square
	initParticleSystem(&circleSystem, 5, 1024 * 10, window);   // Circle (16 sides)

	setParticleTexture(&squareSystem, "..\\Ressources\\ALL\\Textures\\loading.png");

	// Setup matrices

	sfClock* clock = sfClock_create();
	sfEvent event;

	sfView* view = sfView_create();
	sfView_setViewport(view, (sfFloatRect) { .left = 0.f, .top = 0.f, .width = 1.f, .height = 1.f });
	sfView_setSize(view, sfVector2f_Create(1200.f, 800.f));
	sfView_setCenter(view, DivideVector2f(sfVector2f_Create(1200.f, 800.f), 2.f));


	sfRenderWindow_setView(window, view);

	printf("Controls:\n");
	printf("1 - Triangle particles\n");
	printf("2 - Square particles\n");
	printf("3 - Circle particles\n");
	printf("Click to spawn particles\n");
	sfSprite* rect = sfSprite_create();
	sfSprite_setTexture(rect, sfTexture_createFromFile("..\\Ressources\\ALL\\Textures\\loading.png", NULL), sfTrue);
	int currentMode = 2; // Start with circles

	while (sfRenderWindow_isOpen(window)) {
		float deltaTime = sfTime_asSeconds(sfClock_restart(clock));

		// Handle events
		while (sfRenderWindow_pollEvent(window, &event)) {
			if (event.type == sfEvtClosed) {
				sfRenderWindow_close(window);
			}

			if (event.type == sfEvtKeyPressed) {
				if (event.key.code == sfKeyNum1) currentMode = 0;
				else if (event.key.code == sfKeyNum2) currentMode = 1;
				else if (event.key.code == sfKeyNum3) currentMode = 2;
			}

			//if (event.type == sfEvtMouseButtonPressed) {
			//	float x = (float)event.mouseButton.x;
			//	float y = (float)event.mouseButton.y;

			//	// Spawn particles based on current mode
			//	for (int i = 0; i < MAX_PARTICLES; i++) {
			//		if (currentMode == 0) {
			//		addParticle(&triangleSystem, x + (rand() % 400 - 20), y + (rand() % 400 - 20), 0);
			//	}
			//else if (currentMode == 1) {
			//	addParticle(&squareSystem, x + (rand() % 20 - 20), y + (rand() % 20 - 20), 0);
			//}
			//else {
			//	addParticle(&circleSystem, x + (rand() % 400 - 20), y + (rand() % 400 - 20), 0);
			//}
			//	}
			//}
		}
		static float timerFPS = 0.f;
		timerFPS += deltaTime;
		if (timerFPS > 0.2f)
		{
			timerFPS = 0.f;
			sfRenderWindow_setTitle(window, FloatToString(1.f / deltaTime, 0u));			
		}

		if (sfMouse_isButtonPressed(sfMouseLeft))
		{
			sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
			sfVector2f mousePos_ = sfRenderWindow_mapPixelToCoords(window, mousePos, view);
			if (currentMode == 0) {
				for (int i = triangleSystem.particleCount; i < triangleSystem.maxParticles; i++)
				{
					addParticle(&triangleSystem, mousePos_.x, mousePos_.y, 0);
				}
			}
			else if (currentMode == 1) {
				addParticle(&squareSystem, mousePos_.x, mousePos_.y, 0);
			}
			else {
				addParticle(&circleSystem, mousePos_.x, mousePos_.y, 0);
			}
		}

		if (KEY(R))
		{
			destroyParticleSystem(&triangleSystem);
		}

		static float zoomTimer = 0.f;
		zoomTimer += deltaTime;
		if (KEY(D))
		{
			sfView_setCenter(view, AddVector2f(sfView_getCenter(view), sfVector2f_Create(-100.f * deltaTime, 0.f)));
		}
		else if (KEY(Q))
		{
			sfView_setCenter(view, AddVector2f(sfView_getCenter(view), sfVector2f_Create(100.f * deltaTime, 0.f)));

		}
		if (KEY(Z))
		{
			sfView_setCenter(view, AddVector2f(sfView_getCenter(view), sfVector2f_Create(0.f, -100.f * deltaTime)));
		}
		else if (KEY(S))
		{
			sfView_setCenter(view, AddVector2f(sfView_getCenter(view), sfVector2f_Create(0.f, 100.f * deltaTime)));
		}
		if (KEY(A) && zoomTimer > .2f)
		{
			sfView_zoom(view, 1.1f);
			zoomTimer = 0.f;
		}
		else if (KEY(E) && zoomTimer > .2f)
		{
			sfView_zoom(view,.9f);
			zoomTimer = 0.f;
		}
		sfRenderWindow_setView(window, view);

		updateParticles(&triangleSystem, deltaTime);
		updateParticles(&squareSystem, deltaTime);
		updateParticles(&circleSystem, deltaTime);

		// Utiliser les particules mortes
		if (triangleSystem.deadParticlesList.count > 0) {
			printf("Frame: %d particules triangles mortes\n", triangleSystem.deadParticlesList.count);
			for (int i = 0; i < triangleSystem.deadParticlesList.count; i++) {
				printf("  Triangle mort à: (%.2f, %.2f, %.2f)\n",
					triangleSystem.deadParticlesList.particles[i].position[0],
					triangleSystem.deadParticlesList.particles[i].position[1],
					triangleSystem.deadParticlesList.particles[i].position[2]);
			}
		}

		if (squareSystem.deadParticlesList.count > 0) {
			printf("Frame: %d particules carrés mortes\n", squareSystem.deadParticlesList.count);
			for (int i = 0; i < squareSystem.deadParticlesList.count; i++) {
				printf("  Carré mort à: (%.2f, %.2f, %.2f)\n",
					squareSystem.deadParticlesList.particles[i].position[0],
					squareSystem.deadParticlesList.particles[i].position[1],
					squareSystem.deadParticlesList.particles[i].position[2]);
			}
		}

		if (circleSystem.deadParticlesList.count > 0) {
			printf("Frame: %d particules cercles mortes\n", circleSystem.deadParticlesList.count);
			for (int i = 0; i < circleSystem.deadParticlesList.count; i++) {
				printf("  Cercle mort à: (%.2f, %.2f, %.2f)\n",
					circleSystem.deadParticlesList.particles[i].position[0],
					circleSystem.deadParticlesList.particles[i].position[1],
					circleSystem.deadParticlesList.particles[i].position[2]);
			}
		}

		// Clear screen
		sfRenderWindow_clear(window, sfBlack);

		renderParticles(&triangleSystem, 0, window);
		sfRenderWindow_drawSprite(window, rect, NULL);
		renderParticles(&squareSystem, 1, window);

		renderParticles(&circleSystem, 0, window);



		// Display
		sfRenderWindow_display(window);
	}

	// Cleanup
	destroyParticleSystem(&triangleSystem);
	destroyParticleSystem(&squareSystem);
	destroyParticleSystem(&circleSystem);
	sfClock_destroy(clock);
	sfRenderWindow_destroy(window);

	return 0;
}
