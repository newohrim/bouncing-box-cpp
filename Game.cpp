#include "Engine.h"
#include <stdlib.h>
#include <memory.h>

#include "GameLevel.h"

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()

GameLevel* gameLvl = nullptr;

// initialize game data in this function
void initialize()
{
    gameLvl = new GameLevel();
    GameParams params;
    params.worldWidth = SCREEN_WIDTH;
    params.worldHeight = SCREEN_HEIGHT;
    gameLvl->Init(params);
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    if (is_key_pressed(VK_ESCAPE)) {
        schedule_quit_game();
        return;
    }
    {
        InputState state = InputState::NONE;
        state |= is_key_pressed(VK_SPACE) ? InputState::KEY_SPACE : InputState::NONE;
        state |= is_key_pressed(VK_LEFT) ? InputState::KEY_LEFT : InputState::NONE;
        state |= is_key_pressed(VK_RIGHT) ? InputState::KEY_RIGHT : InputState::NONE;
        state |= is_key_pressed(VK_UP) ? InputState::KEY_UP : InputState::NONE;
        state |= is_key_pressed(VK_DOWN) ? InputState::KEY_DOWN : InputState::NONE;
        gameLvl->ProceedInput(state, dt);
    }
    if (!gameLvl->Update(dt)) {
        // reload level?
        finalize();
        initialize();
    }
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

  gameLvl->Draw(&buffer[0][0], SCREEN_WIDTH, SCREEN_HEIGHT);
}

// free game data in this function
void finalize()
{
    delete gameLvl;
}

