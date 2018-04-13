#ifndef GUI_UTILS_H_
#define GUI_UTILS_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>


// Button widget

typedef struct Button Button;

Button* Button_Create(SDL_Renderer *renderer,
                      const char *image,
                      SDL_Rect location,
					  void (*OnPreRender)(Button *button, const void *args),
                      void* (*OnClick)(void*));

Button* Button_Destroy(Button* button);

void Button_SetEnabled(Button* button, bool toEnable);

void Button_Render(Button *button, const void *args);

void* Button_HandleEvent(Button *button, SDL_Event *event, void *args);

// Pane widget - used as a buttons container

typedef struct Pane Pane;

Pane* Pane_Create(SDL_Renderer *renderer,
				  SDL_Rect location,
				  Button **buttons,
				  unsigned int numOfButtons,
				  void (*OnPreRender)(Pane *pane, const void *args),
				  void* (*OnClick)(void*));

Pane* Pane_Destroy(Pane* pane);

void Pane_Render(Pane *pane, const void *args);

void* Pane_HandleEvent(Pane *pane, SDL_Event *event, void *args);

#endif
