#ifndef GUI_UTILS_H_
#define GUI_UTILS_H_

#include <SDL.h>
#include <SDL_video.h>


// Widget (abstract) - currently unused

typedef struct Widget Widget;

void Widget_Render(Widget *widget);

void Widget_HandleEvent(Widget *widget, SDL_Event *event);

Widget* Widget_Destroy(Widget *widget);

// Button widget

typedef struct Button Button;

Button* Button_Create(SDL_Renderer *renderer, const char *image, SDL_Rect location, void (*action)());

Button* Button_Destroy(Button* button);

void Button_Render(Button *button);

void Button_HandleEvent(Button *button, SDL_Event *event);

// Pane widget

// typedef struct Pane Pane;

// Widget* Pane_Create(SDL_Renderer *renderer, Widget **buttons, unsigned int numOfButtons, SDL_Rect location, void (*action)());

// Widget* Pane_Destroy(Widget* widget);

// void Pane_Render(Widget *widget);

// void Pane_HandleEvent(Widget *widget, SDL_Event *event);

typedef struct MainPane {
    Button *newGameBtn;
    Button *loadBtn;
    Button *startBtn;
    Button *backBtn;
} MainPane;

typedef struct SettingsPane {
    int x;
} SettingsPane;

typedef struct GamePane {
    int x;
} GamePane;

typedef struct LoadPane {
    Button *newGameBtn;
    Button *slot1Btn;
    Button *slot2Btn;
    Button *slot3Btn;
} LoadPane;


#endif
