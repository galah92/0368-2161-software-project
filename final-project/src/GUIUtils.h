#ifndef GUI_UTILS_H_
#define GUI_UTILS_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>
#include "GameManager.h"

#define GUI_BOARD_MOVE_ARGS   4


typedef struct Button Button;

/**
 * Create new Button instance.
 * @param   renderer    the SDL_Renderer to render to button to
 * @param   image       the Button's image
 * @param   location    the Button's position on the window
 * @param   OnPreRender the callback method to call before rendering
 * @param   OnClick     the callback method to call when event detected
 * @return  NULL if malloc failed
 *          Button* instance otherwise
 */
Button* Button_Create(SDL_Renderer *renderer,
                      const char *image,
                      SDL_Rect location,
					  void (*OnPreRender)(Button *button, const void *args),
                      void (*OnClick)(void*));

/**
 * Free all resources for a given Button instance.
 * @param   button      the instance to destroy
 * @return  NULL
 */
Button* Button_Destroy(Button* button);

/**
 * Set a given image path to a given Button instance
 * @param   button      the instance to update it's image
 * @param   image       the new image path
 */
void Button_SetImage(Button *button, const char *image);

/**
 * Update a given Button instace state - whether it's enabled,
 * and thus raising the OnClick callback when needed.
 * Also affect the button color.
 * @param   button      the instance to update it's image
 * @param   isEnabled   the new button state
 */
void Button_SetEnabled(Button* button, bool isEnabled);

/**
 * Update a given Button instace state - whether it's toggled,
 * and thus the button color is set to 'toggled' color.
 * @param   button      the Button instance
 * @param   isToggled   the new button state
 */
void Button_SetToggled(Button* button, bool isToggled);

/**
 * Render a given Button instance to it's SDL_Renderer,
 * delegating the args to the OnPreRender callback.
 * @param   button      the Button instance
 * @param   args        the args to pass to OnPreRender
 */
void Button_Render(Button *button, const void *args);

/**
 * Make a given Button instance react to SDL_Event. Specifically,
 * raise the OnClick callback if needed.
 * @param   button      the Button instance
 * @param   event       the event to react for
 * @param   args        the args to pass to OnClick
 */
void Button_HandleEvent(Button *button, SDL_Event *event, void *args);


typedef struct Pane Pane;

/**
 * Create new Pane instance.
 * @param   renderer    the SDL_Renderer to render to pane to
 * @param   location    the Pane's position on the window
 * @param   buttons     the buttons belong to the Pane
 * @param   numOfButton the number of given buttons
 * @return  NULL if malloc failed
 *          Pane* instance otherwise
 */
Pane* Pane_Create(SDL_Renderer *renderer,
				  SDL_Rect location,
				  Button **buttons,
				  unsigned int numOfButtons);

/**
 * Free all resources for a given Pane instance.
 * @param   pane        the instance to destroy
 * @return  NULL
 */
Pane* Pane_Destroy(Pane* pane);

/**
 * Render a given Pane instance to it's SDL_Renderer,
 * delegating the args to it's buttons OnPreRender callback.
 * @param   pane        the Pane instance
 * @param   args        the args to pass to the button's OnPreRender
 */
void Pane_Render(Pane *pane, const void *args);

/**
 * Delegate a given SDL_Event to a given Pane instance's buttons,
 * with the given args.
 * @param   pane        the Pane instance
 * @param   event       the event to delegate
 * @param   args        the args to delegate
 */
void Pane_HandleEvent(Pane *pane, SDL_Event *event, void *args);


typedef struct BoardEventArgs {
    int move[GUI_BOARD_MOVE_ARGS];
    bool isRightClick;
} BoardEventArgs;

typedef struct Board Board;

/**
 * Create new Board instance.
 * @param   renderer    the SDL_Renderer to render to button to
 * @param   action      the callback method to call when event detected
 * @return  NULL if malloc failed
 *          Pane* instance otherwise
 */
Board* Board_Create(SDL_Renderer *renderer, void (*action)(BoardEventArgs*, void*));

/**
 * Free all resources for a given Board instance.
 * @param   board       the instance to destroy
 * @return  NULL
 */
Board* Board_Destroy(Board* board);

/**
 * Render a given Board instance to it's SDL_Renderer.
 * @param   board       the Board instance
 * @param   manager     the GameManager to relay on it's chess board
 * @param   commType    the GameCommandType to relay on
 */
void Board_Render(Board *board, const GameManager *manager, GameCommandType commType);

/**
 * Make a given Board instance react to SDL_Event. Specifically,
 * raise the action callback if needed.
 * @param   board       the Board instance
 * @param   event       the event to react for
 * @param   args        the args to pass to action
 */
void* Board_HandleEvent(Board *board, SDL_Event *event, void *args);

#endif
