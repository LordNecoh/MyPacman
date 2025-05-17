#ifndef SFML_INCLUDES
#define SFML_INCLUDES

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <vector>
#include <iostream>

#endif

#include "functions.hpp"

sf::Vector2i getGridCoords(const sf::Vector2f& pos) {
    return sf::Vector2i(
        static_cast<int>(pos.x / GRID_TILE),
        static_cast<int>(pos.y / GRID_TILE)
    );
}

bool isAlignedToGrid(const sf::Vector2f& position) {
    const float epsilon = 2.0f; //Float imprecision tollerance
    return std::fmod(position.x, GRID_TILE) < epsilon &&
           std::fmod(position.y, GRID_TILE) < epsilon;
}

void createMap(sf::RenderWindow& window) {
    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            if (level[row][col] == 0) continue; // Skip void tiles
            sf::RectangleShape tile(sf::Vector2f(GRID_TILE, GRID_TILE));
            tile.setPosition(sf::Vector2f(col * GRID_TILE, row * GRID_TILE));
    
            switch (level[row][col]) {
    
                case WALL:
                    tile.setFillColor(sf::Color::Blue);
                    break;
    
                case DOT: 
                    tile.setFillColor(sf::Color::Black);
                    break;
    
                case SUPER_DOT:
                    tile.setFillColor(sf::Color::Black);
                break;
    
                case GHOST_DOOR:
                    tile.setFillColor(sf::Color(255, 0, 255)); // Purple
                    break;
            }
    
            window.draw(tile);
        }
    }
}

/*  Assumes that the character is in the tunnel (row == 9) and col == 0 or col == 20 
    And teleports it to the other side  */
void manageTunnel(Character& character){
    if (character.direction.x < 0) {
        character.sprite.setPosition(sf::Vector2f((MAP_WIDTH - 1) * GRID_TILE, character.sprite.getPosition().y));
    } else if (character.direction.x > 0) {
        character.sprite.setPosition(sf::Vector2f(0, character.sprite.getPosition().y));
    }
}

//Checks collision with walls and tunnel
bool canWalk(Character& character, const sf::Vector2f& direction) {

    sf::Vector2f nextPosition = character.sprite.getPosition() + (direction  * static_cast<float>(GRID_TILE));

    int row = static_cast<int>(nextPosition.y / GRID_TILE);
    int col = static_cast<int>(nextPosition.x / GRID_TILE);

    if ( row == 9 && (col == 0 || col == 20)) {     //Checks if pacman is in the tunnel
        character.traversingTunnel = true;
        return true; 
    }

    return level[row][col] != WALL; 
}

void placeDots(State& state, const sf::Texture& texture) {
    state.dots.clear();

    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            if (level[row][col] == DOT) {  
                   
                sf::Sprite dotSprite(texture);
                sf::IntRect dotRect(DOT_TEXTURE, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
                dotSprite.setTextureRect(dotRect);
                dotSprite.setScale(sf::Vector2f(SPRITES_SCALE, SPRITES_SCALE));
                dotSprite.setPosition(sf::Vector2f(col * GRID_TILE, row * GRID_TILE));
                Pellet dot(dotSprite, false);
                state.dots.push_back(dot);

            }else if(level[row][col] == SUPER_DOT) { 

                sf::Sprite dotSprite(texture);
                sf::IntRect dotRect(SUPERDOT_TEXTURE, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
                dotSprite.setTextureRect(dotRect);
                dotSprite.setScale(sf::Vector2f(SPRITES_SCALE, SPRITES_SCALE));
                dotSprite.setPosition(sf::Vector2f(col * GRID_TILE, row * GRID_TILE));
                Pellet dot(dotSprite, true);
                state.dots.push_back(dot);

            }
        }
    }
}

void manageDots(State& state){
    sf::Vector2i pacmanGridCoords = getGridCoords(state.pacman.sprite.getPosition());

    for(int i=0; i<state.dots.size(); ++i) {
        sf::Vector2i dotGridCoords = getGridCoords(state.dots[i].sprite.getPosition());
        if (pacmanGridCoords == dotGridCoords) {
            state.dots.erase(state.dots.begin() + i); 
            if(state.dots.empty()) {
                std::cout << "All dots collected!" << std::endl;
                state.gameState = GameState::GAME_WON;
            }
            break; //To avoid deleting more than one dot at a time
        }
    }
}

void checkCollisions(State& state){
    if(state.pacman.traversingTunnel){
        manageTunnel(state.pacman);
        state.pacman.traversingTunnel = false;
    }
    manageDots(state);
}

void centerOrigin(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.size / 2.f);
}

//Function to draw the GameOver, GameWon and Pause menu
void drawMenu(sf::RenderWindow& window, State& state) {
    sf::Text title{state.font, "", 18};
    sf::Text restart{state.font, "Press R to restart", 18};

    switch (state.gameState) {
        case GameState::GAME_OVER:
            title.setString("Game Over");
            break;
        case GameState::GAME_WON:
            title.setString("You won!");
            break;
        default:
            break;
    }
    centerOrigin(title);
    centerOrigin(restart);

    title.setPosition(TITLE_POSITION);
    restart.setPosition(RESTART_POSITION);

    window.draw(title);
    window.draw(restart);
}

void resetGame(State& state, const sf::Texture& texture) {
    //Pacman
    state.pacman.sprite.setPosition(PACMAN_SPAWN);
    state.pacman.direction = NO_DIRECTION;
    state.pacman.nextDirection = NO_DIRECTION;
    state.pacman.velocity = NO_DIRECTION;
    state.pacman.traversingTunnel = false;

    //Dots, state and clock
    placeDots(state, texture);
    state.gameState = GameState::PLAYING;
    state.clock.restart();
}