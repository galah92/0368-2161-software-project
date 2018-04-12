#include <string.h>
#include <SDL.h>
#include <SDL_video.h>
#include "GUIUtils.h"


struct Button {
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect location;
	void* (*action)(void*);
};

Button* Button_Create(SDL_Renderer *renderer,
					  const char *image,
					  SDL_Rect location,
					  void* (*action)(void*)) {
	Button *button = malloc(sizeof(Button));
    if (!button) return Button_Destroy(button);
	SDL_Surface *surface = SDL_LoadBMP(image);
    if (!surface) return Button_Destroy(button);
	button->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!button->texture) return Button_Destroy(button);
	SDL_FreeSurface(surface);
	button->renderer = renderer;
	button->location = location;
	button->action = action;
	return button;
}

Button* Button_Destroy(Button* button) {
    if (!button) return NULL;
	if (button->texture) SDL_DestroyTexture(button->texture);
	free(button);
    return NULL;
}

void Button_Render(Button *button) {
	if (!button) return;
	SDL_RenderCopy(button->renderer, button->texture, NULL, &button->location);
}

void* Button_HandleEvent(Button *button, SDL_Event *event, void *args) {
	if (!button) return NULL;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point = { .x = event->button.x, .y = event->button.y };
		if (SDL_PointInRect(&point, &button->location) && button->action) {
			return button->action(args);
		}
	}
	return NULL;
}

struct Pane {
	SDL_Renderer *renderer;
	SDL_Rect location;
	Button **buttons;
	unsigned int numOfButtons;
	void* (*action)(void*);
};

Pane* Pane_Create(SDL_Renderer *renderer,
				  SDL_Rect location,
				  Button **buttons,
				  unsigned int numOfButtons,
				  void* (*action)(void*)) {
	Pane *pane = malloc(sizeof(Pane));
    if (!pane) return Pane_Destroy(pane);
	pane->buttons = malloc(sizeof(Button) * numOfButtons);
	if (!pane->buttons) return Pane_Destroy(pane);
	memcpy(pane->buttons, buttons, sizeof(Button) * numOfButtons);
	pane->renderer = renderer;
	pane->location = location;
	pane->numOfButtons = numOfButtons;
	pane->action = action;
	return pane;
}

Pane* Pane_Destroy(Pane* pane) {
    if (!pane) return NULL;
	for (unsigned int i = 0; i < pane->numOfButtons; i++) {
		Button_Destroy(pane->buttons[i]);
	}
	free(pane);
    return NULL;
}

void Pane_Render(Pane *pane) {
	if (!pane) return;
	for (unsigned int i = 0; i < pane->numOfButtons; i++) {
		Button_Render(pane->buttons[i]);
	}
}

void* Pane_HandleEvent(Pane *pane, SDL_Event *event, void *args) {
	if (!pane) return NULL;
	void *result;
	for (unsigned int i = 0; i < pane->numOfButtons; i++) {
		result = Button_HandleEvent(pane->buttons[i], event, args);
		if (result) return result;
	}
	return NULL;
}