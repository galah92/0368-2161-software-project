#include <stdio.h>
#include "GUIUtils.h"

#define SDL_ERROR_STRING    "SDL Error: %s\n"


struct Widget {
	void *data;
	void *renderer;
	void (*render)(Widget *widget);
	void (*handleEvent)(Widget *widget, SDL_Event *event);
	Widget* (*destroy)(Widget *widget);
};

void Widget_Render(Widget *widget) {
	if (!widget || !widget->render) return;
	widget->render(widget);
}

void Widget_HandleEvent(Widget *widget, SDL_Event *event) {
	if (!widget || !widget->handleEvent) return;
	widget->handleEvent(widget, event);
}

Widget* Widget_Destroy(Widget *widget) {
	if (!widget) return NULL;
	return widget->destroy(widget);
}

struct Button {
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect location;
	void (*action)();
};

Button* Button_Create(SDL_Renderer *renderer, const char *image, SDL_Rect location, void (*action)()) {
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
    const char *error = SDL_GetError();
    if (strcmp(error, "")) printf(SDL_ERROR_STRING, error);
    if (!button) return NULL;
	if (button->texture) SDL_DestroyTexture(button->texture);
	free(button);
    return NULL;
}

void Button_Render(Button *button) {
	SDL_RenderCopy(button->renderer, button->texture, NULL, &button->location);
}

void Button_HandleEvent(Button *button, SDL_Event *event) {
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point = { .x = event->button.x, .y = event->button.y };
		if (SDL_PointInRect(&point, &button->location) && button->action) {
			button->action();
		}
	}
}
