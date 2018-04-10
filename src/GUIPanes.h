#ifndef GUI_PANES_H_
#define GUI_PANES_H_

#include <SDL.h>
#include <SDL_video.h>


typedef struct Widget Widget;
struct Widget {
	void *data;
	void (*render)(Widget *widget, SDL_Renderer *renderer);
	void (*handleEvent)(Widget *widget, SDL_Event *event);
	Widget* (*destroy)(Widget *widget);
};

typedef struct Button {
	SDL_Renderer *render;
	SDL_Texture *texture;
	SDL_Rect location;
	void (*action)();
} Button;

Widget* Button_Create(SDL_Renderer *renderer, const char *image, SDL_Rect location, void (*action)());

Widget* Button_Destroy(Widget* widget);

void Button_HandleEvent(Widget *widget, SDL_Event *event);

void Button_Render(Widget *widget, SDL_Renderer *renderer);

typedef struct MainPane {
    int x;
} MainPane;

typedef struct SettingsPane {
    int x;
} SettingsPane;

typedef struct GamePane {
    int x;
} GamePane;

typedef struct LoadPane {
    int x;
} LoadPane;


#endif
