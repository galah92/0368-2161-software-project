#include <string.h>
#include <SDL.h>
#include <SDL_video.h>
#include "GUIUtils.h"


struct Button {
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect location;
	bool isEnabled;
	void (*OnPreRender)(Button *button, const void *args);
	void* (*OnClick)(void*);
};

Button* Button_Create(SDL_Renderer *renderer,
					  const char *image,
					  SDL_Rect location,
					  void (*OnPreRender)(Button *button, const void *args),
					  void* (*OnClick)(void*)) {
	Button *button = malloc(sizeof(Button));
    if (!button) return Button_Destroy(button);
	SDL_Surface *surface = SDL_LoadBMP(image);
    if (!surface) return Button_Destroy(button);
	button->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!button->texture) return Button_Destroy(button);
	SDL_FreeSurface(surface);
	button->renderer = renderer;
	button->location = location;
	button->isEnabled = true;
	button->OnPreRender = OnPreRender;
	button->OnClick = OnClick;
	return button;
}

Button* Button_Destroy(Button* button) {
    if (!button) return NULL;
	if (button->texture) SDL_DestroyTexture(button->texture);
	free(button);
    return NULL;
}

void Button_SetEnabled(Button* button, bool toEnable) {
	if (!button) return;
	button->isEnabled = toEnable;
}

void Button_Render(Button *button, const void *args) {
	if (!button) return;
	if (button->OnPreRender) button->OnPreRender(button, args);
	SDL_RenderCopy(button->renderer, button->texture, NULL, &button->location);
	if (!button->isEnabled) {
		SDL_SetRenderDrawColor(button->renderer, 255, 255, 255, 128);
		SDL_RenderFillRect(button->renderer, &button->location);		
	}
}

void* Button_HandleEvent(Button *button, SDL_Event *event, void *args) {
	if (!button || !button->isEnabled) return NULL;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point = { .x = event->button.x, .y = event->button.y };
		if (SDL_PointInRect(&point, &button->location) && button->OnClick) {
			return button->OnClick(args);
		}
	}
	return NULL;
}

struct Pane {
	SDL_Renderer *renderer;
	SDL_Rect location;
	Button **buttons;
	unsigned int numOfButtons;
	void (*OnPreRender)(Pane *pane, const void *args);
	void* (*OnClick)(void*);
};

Pane* Pane_Create(SDL_Renderer *renderer,
				  SDL_Rect location,
				  Button **buttons,
				  unsigned int numOfButtons,
				  void (*OnPreRender)(Pane *pane, const void *args),
				  void* (*OnClick)(void*)) {
	Pane *pane = malloc(sizeof(Pane));
    if (!pane) return Pane_Destroy(pane);
	pane->buttons = malloc(sizeof(Button) * numOfButtons);
	if (!pane->buttons) return Pane_Destroy(pane);
	memcpy(pane->buttons, buttons, sizeof(Button) * numOfButtons);
	pane->renderer = renderer;
	pane->location = location;
	pane->numOfButtons = numOfButtons;
	pane->OnPreRender = OnPreRender;
	pane->OnClick = OnClick;
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

void Pane_Render(Pane *pane, const void *args) {
	if (!pane) return;
	if (pane->OnPreRender) pane->OnPreRender(pane, args);
	for (unsigned int i = 0; i < pane->numOfButtons; i++) {
		Button_Render(pane->buttons[i], args);
	}
}

void* Pane_HandleEvent(Pane *pane, SDL_Event *event, void *args) {
	if (!pane) return NULL;
	for (unsigned int i = 0; i < pane->numOfButtons; i++) {
		void *result = Button_HandleEvent(pane->buttons[i], event, args);
		if (result) return result;
	}
	return NULL;
}
