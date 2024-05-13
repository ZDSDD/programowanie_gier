//
// Created by kamil on 13.05.2024.
//

#ifndef PROJEKT1_GAME_H
#define PROJEKT1_GAME_H


// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
public:
    // game state
    GameState    State;
    bool         Keys[1024];
    unsigned int Width, Height;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};


#endif //PROJEKT1_GAME_H
