#include "GUIPanes.h"


Widget* Button_Create(SDL_Renderer *renderer, const char *image, SDL_Rect location, void (*action)()) {
	Widget *widget = malloc(sizeof(Widget));
	if (!widget) return Button_Destroy(widget);
	Button *button = malloc(sizeof(Button));
    if (!button) return Button_Destroy(widget);
	SDL_Surface *surface = SDL_LoadBMP(image);
    if (!surface) return Button_Destroy(widget);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) return Button_Destroy(widget);
	SDL_FreeSurface(surface);
	button->texture = texture;
	button->location = location;
	button->action = action;
	widget->data = button;
	widget->destroy = Button_Destroy;
	widget->render = Button_Render;
	widget->handleEvent = Button_HandleEvent;
	return widget;
}

Widget* Button_Destroy(Widget* widget) {
    if (!widget) return NULL;
	Button *button = (Button*)widget->data;
    if (button) {
        if (button->texture) SDL_DestroyTexture(button->texture);
        free(button);
    }
	free(widget);
    return NULL;
}

void Button_HandleEvent(Widget *widget, SDL_Event *event) {
	if (event->type == SDL_MOUSEBUTTONUP) {
		Button *button = (Button*)widget->data;
		SDL_Point point = { .x = event->button.x, .y = event->button.y };
		if (SDL_PointInRect(&point, &button->location)) {
			button->action();
		}
	}
}

void Button_Render(Widget *widget, SDL_Renderer *renderer) {
	Button* button = (Button*)widget->data;
	SDL_RenderCopy(renderer, button->texture, NULL, &button->location);
}
