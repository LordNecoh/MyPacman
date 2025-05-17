#ifndef SFML_INCLUDES
#define SFML_INCLUDES

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <queue>
#include <limits.h> 
#include <random>

#endif

//      ---    GLOBAL VARIABLES   ---    //
const int CHARACTER_SIZE = 17; // Size of each character in pixels (space without padding in the texture sheet)
const int TILE_SIZE = 20;
const float PACMAN_SPEED = 80.0f; //Pixels per second
const float GHOST_SPEED = PACMAN_SPEED; //Pixels per second
const float SCARED_SPEED = GHOST_SPEED / 2; //Half speed when scared
const float SPRITES_SCALE = 1.5f; //Constant for scaling
const int GRID_TILE = CHARACTER_SIZE * SPRITES_SCALE;
const int COLLISION_TILE = GRID_TILE - 1; //Slightly smaller for more permissive collision detection
const float SUPER_TIME = 10.0f; //In seconds

const int MAP_HEIGHT = 21;
const int MAP_WIDTH = 21;

const int FRAME_RATE = 60;
const int WINDOW_WIDTH = MAP_WIDTH * GRID_TILE;
const int WINDOW_HEIGHT = MAP_HEIGHT * GRID_TILE;

const sf::Vector2f NO_DIRECTION(0.f, 0.f);
const sf::Vector2f UP(0.f, -1.f);
const sf::Vector2f DOWN(0.f, 1.f);
const sf::Vector2f LEFT(-1.f, 0.f);
const sf::Vector2f RIGHT(1.f, 0.f);

const sf::Vector2f PACMAN_SPAWN(10 * GRID_TILE, 15 * GRID_TILE);
const sf::Vector2f RED_SPAWN(10 * GRID_TILE, 7 * GRID_TILE);
const sf::Vector2f PINK_SPAWN(10 * GRID_TILE, 9 * GRID_TILE);
const sf::Vector2f GREEN_SPAWN(9 * GRID_TILE, 9 * GRID_TILE);
const sf::Vector2f ORANGE_SPAWN(11 * GRID_TILE, 9 * GRID_TILE);
const sf::Vector2f HOME_CORNER(3 * GRID_TILE, 15 * GRID_TILE); 

const sf::Vector2f TITLE_POSITION(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3 - 10);
const sf::Vector2f RESTART_POSITION(TITLE_POSITION.x , TITLE_POSITION.y + 100);

#include "textures.hpp"
#include "functions.hpp"
#include "gameStructures.hpp"
#include "functions.cpp"
#include "handlers.cpp"


int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Pacman Game");

    sf::Texture mainTexture;
    if (!mainTexture.loadFromFile("src/progetto/RisorseProgetto/pacmanSprites.png"))  return -1;

    window.setFramerateLimit(FRAME_RATE);

    State state(mainTexture);
    state.clock.restart();  
   
    //  ---   GAME LOOP   ---   //
    while (window.isOpen()) {
        window.handleEvents(
            [&window](const sf::Event::Closed&) { handle_close(window); },
            [&window](const sf::Event::Resized& event) { handle_resize(event, window); },
            [&state, &window](const auto& event) { handle(event, state); }
        );

        window.clear();

        if(state.gameState != GameState::PLAYING) {
            drawMenu(window, state); 

            window.display();
            continue; //If the game is not playing, skip the rest of the loop
        }

        float deltaTime = state.clock.restart().asSeconds();
        
        //    PLAYER MOVEMENT    //
        if (isAlignedToGrid(state.pacman.sprite.getPosition())) {
            checkCollisions(state); 
            state.pacman.direction = updatePacmanDirection(state.pacman); 
            if(state.isPacmanSuper){
                animateSuperPacmanDirection(state);
            }else{
                animatePacmanDirection(state); 
            }
        }
        
        state.pacman.velocity = state.pacman.direction * PACMAN_SPEED;
        state.pacman.sprite.move(state.pacman.velocity * deltaTime);
        animatePacmanMovement(state);

        manageEnemies(state, deltaTime);

        drawAll(window, state);
        window.display();
    }

    return 0;
}


/*
    Problemi attuali:
    -Tunnel ogni tanto non funziona per i fantasmi (forse risolto)
    
    Prossime cose da implementare:
    -Gestione animazioni

    -Gestione punteggio 
    -Gestione suoni (facoltativa)
    -Gestione livelli (molto facoltativa)
*/