#include "GUIPanes.h"


// struct LoadPane {
//     Pane *pane;
// };

// Pane* LoadPane_Create(SDL_Renderer *renderer) {
//     Button *buttons[] = {
//         Button_Create(renderer,
//                       SRC_BUTTON_BACK,
//                       (SDL_Rect){ .x = 25, .y = 25, .w = BUTTON_W, .h = BUTTON_H },
//                       handleBackButton),
//         Button_Create(renderer,
//                       SRC_BUTTON_SLOT1,
//                       (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
//                       NULL),
//         Button_Create(renderer,
//                       SRC_BUTTON_SLOT2,
//                       (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
//                       NULL),
//         Button_Create(renderer,
//                       SRC_BUTTON_SLOT3,
//                       (SDL_Rect){ .x = 25, .y = 250, .w = BUTTON_W, .h = BUTTON_H },
//                       NULL),
//         Button_Create(renderer,
//                       SRC_BUTTON_SLOT4,
//                       (SDL_Rect){ .x = 25, .y = 325, .w = BUTTON_W, .h = BUTTON_H },
//                       NULL),
//         Button_Create(renderer,
//                       SRC_BUTTON_SLOT5,
//                       (SDL_Rect){ .x = 25, .y = 400, .w = BUTTON_W, .h = BUTTON_H },
//                       NULL),
//         Button_Create(renderer,
//                       SRC_BUTTON_LOAD,
//                       (SDL_Rect){ .x = 25, .y = 475, .w = BUTTON_W, .h = BUTTON_H },
//                       handleLoadButton),
//     };
//     return Pane_Create(renderer,
//                        (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
//                        buttons,
//                        7,
//                        NULL);
// }
