#ifndef SFML_INCLUDES
#define SFML_INCLUDES

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <vector>
#include <iostream>

#endif

//      ---    GLOBAL VARIABLES   ---    //
const int CHARACTER_SIZE = 17; // Size of each character in pixels (space without padding in the texture sheet)
const int TILE_SIZE = 20;
const float MOVE_SPEED = 80.0f; //Pixels per second
const float SPRITES_SCALE = 1.5f; //Constant for scaling
const int GRID_TILE = CHARACTER_SIZE * SPRITES_SCALE;

const int MAP_HEIGHT = 21;
const int MAP_WIDTH = 21;

const int WINDOW_WIDTH = MAP_WIDTH * GRID_TILE;
const int WINDOW_HEIGHT = MAP_HEIGHT * GRID_TILE;

const sf::Vector2f NO_DIRECTION(0.f, 0.f);
const sf::Vector2f PACMAN_SPAWN(10 * GRID_TILE, 15 * GRID_TILE);

const sf::Vector2i PACMAN_TEXTURE(TILE_SIZE * 6 + 9, TILE_SIZE*4+ 7); // 8th column (index 7), 5th row (index 4)
const sf::Vector2i DOT_TEXTURE(TILE_SIZE * 13 -4, +2); //13th cloumn ,1st row
const sf::Vector2i SUPERDOT_TEXTURE(TILE_SIZE * 14 -4, + 2); //13th cloumn ,2nd row

const sf::Vector2f TITLE_POSITION(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3 - 10);
const sf::Vector2f RESTART_POSITION(TITLE_POSITION.x , TITLE_POSITION.y + 100);

#include "gameStructures.hpp"
#include "functions.cpp"
#include "handlers.cpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Pacman Game");

    sf::Texture mainTexture;
    if (!mainTexture.loadFromFile("progetto/RisorseProgetto/pacmanSprites.png"))  return -1;

    window.setFramerateLimit(60);

    //    ---    ENTITIES AND STATES   ---    //

    //Pacman
    sf::Sprite pacmanSprite(mainTexture); 
    sf::IntRect pacmanRect(PACMAN_TEXTURE, sf::Vector2i(CHARACTER_SIZE,CHARACTER_SIZE));
    pacmanSprite.setTextureRect(pacmanRect);
    pacmanSprite.setScale(sf::Vector2f(SPRITES_SCALE,SPRITES_SCALE));

    pacmanSprite.setPosition(PACMAN_SPAWN);

    //State and clock
    State state(pacmanSprite);
    state.mainTexture = mainTexture;
    state.clock.restart();

    //Dots
    placeDots(state, mainTexture);
    
   
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
            continue; //If the game is over, skip the rest of the loop
        }

        createMap(window);

        float deltaTime = state.clock.restart().asSeconds();
        
        //    PLAYER MOVEMENT    //
        if (isAlignedToGrid(state.pacman.sprite.getPosition())) {
            checkCollisions(state); 
            if (canWalk(state.pacman, state.pacman.nextDirection)) {
                state.pacman.direction = state.pacman.nextDirection;
            } else {
                state.pacman.direction = NO_DIRECTION; //Block movement if there is a wall
            }
        }
        
        state.pacman.velocity = state.pacman.direction * MOVE_SPEED;
        state.pacman.sprite.move(state.pacman.velocity * deltaTime);

        //   DRAW   //
        for (int i = 0; i < state.dots.size(); ++i) {
            window.draw(state.dots[i].sprite);
        }
        window.draw(state.pacman.sprite);  

        window.display();
    }

    return 0;
}


/*
    Problemi attuali:
    
    Prossime cose da implementare:
    -Gestione pause e menù (facoltativa)

    -Presenza fantasmi
    -Gestione collisioni con i fantasmi
    -Gestione animazioni
    -Gestione punteggio (facoltativa)
    -Gestione suoni (facoltativa)
    -Gestione livelli (molto facoltativa)
*/