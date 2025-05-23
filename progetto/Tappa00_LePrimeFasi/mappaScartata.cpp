/*
    ---  !!! ATTENZIONE !!!  ---
    Questo file è uno "screesnshot" del progetto originale, non è più in uso.
    In particolare, qui abbiamo l'ultima versione prima del cambio di approccio con la mappa
    Probabilmente diverse cose cambieranno nelle versioni più aggiornate, segnalo i punti principali:

    Cose tolte subito dopo il reset:
    Struct level ed enum Tile
    Funzioni getTileCode, autoTileMap, getWallTextureRect
    Nel main() MAP CREATION e for nella parte di DRAW
*/



#ifndef SFML_INCLUDES
#define SFML_INCLUDES

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>

#endif


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int TILE_SIZE = 17; // Size of each tile in pixels
const int TILE_PADDING = 20;
const float MOVE_SPEED = 80.0f; //Pixels per second
const float SPRITES_SCALE = 2.0f; //Constant to scale sprites

const int GRID_TILE = TILE_SIZE * SPRITES_SCALE; // 17 * 2 = 34
const int MAP_WIDTH = 28;
const int MAP_HEIGHT = 31;

//  MAP
/*
0 = vuoto

1 = muro generico

2 = pallino

3 = superpallino

4 = porta dei fantasmi
*/
int level[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,2,1,1,1,1,1,1,2,1,1,1,2,1,1,1,1,2,1},
    {1,3,1,0,0,1,2,1,0,1,2,1,0,0,0,0,1,2,1,0,1,2,1,0,0,1,3,1},
    {1,2,1,1,1,1,2,1,1,1,2,1,1,1,1,1,1,2,1,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,2,1,1,1,1,1,4,4,4,4,1,1,2,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,1,4,4,4,4,1,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,2,1,1,1,1,1,1,2,1,1,1,2,1,1,1,1,2,1},
    {1,3,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,3,2,1},
    {1,1,1,1,2,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,2,1,1,1,1},
    {0,0,0,1,2,2,2,1,1,2,2,2,0,0,0,0,2,2,2,1,1,2,2,2,1,0,0,0},
    {1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,2,1,1,1,1},
    {1,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,1},
    {1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,1,2,1,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,0,1,2,1,1,0,1,1,2,1,2,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,1,0,1,2,1,1,0,1,1,2,1,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,0,1,2,5,6,0,5,6,2,1,2,1,1,1,1,1,1,1,2,1},
    {1,3,2,2,2,1,2,2,0,2,2,2,2,0,2,2,2,2,2,1,2,2,2,2,2,3,2,1},
    {1,1,1,1,2,1,2,1,0,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,1,1,1,1},
    {0,0,0,1,2,2,2,1,0,2,2,2,2,2,2,2,2,2,2,1,2,2,2,1,0,0,0,0},
    {1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


enum Tile {
    EMPTY = 0,
    DOT = 2,
    WALL_TOP = 10,
    WALL_BOTTOM = 11,
    WALL_LEFT = 12,
    WALL_RIGHT = 13,
    WALL_HORIZONTAL = 14,
    WALL_VERTICAL = 15,
    WALL_CORNER_TL = 16,
    WALL_CORNER_TR = 17,
    WALL_CORNER_BL = 18,
    WALL_CORNER_BR = 19,
    WALL_CROSS = 20,
    WALL_T_UP = 21,
    WALL_T_DOWN = 22,
    WALL_T_LEFT = 23,
    WALL_T_RIGHT = 24
};

struct State {
    sf::Vector2f velocity;
    sf::Vector2f direction;
    sf::Vector2f nextDirection;
    sf::Sprite pacman;
    sf::Clock clock;

    State(const sf::Sprite pacmanSprite): velocity(0.f, 0.f), direction(0.f, 0.f), pacman(pacmanSprite) {}
};

bool isAlignedToGrid(const sf::Vector2f& position) {
    const float epsilon = 2.0f; //Float imprecision tollerance
    return std::fmod(position.x, GRID_TILE) < epsilon &&
           std::fmod(position.y, GRID_TILE) < epsilon;
}

int getTileCode(int y, int x) {
    bool up    = (y > 0 && level[y-1][x] == 1);
    bool down  = (y < MAP_HEIGHT-1 && level[y+1][x] == 1);
    bool left  = (x > 0 && level[y][x-1] == 1);
    bool right = (x < MAP_WIDTH-1 && level[y][x+1] == 1);

    if (up && down && left && right) return WALL_CROSS;
    if (up && down && left)          return WALL_T_RIGHT;
    if (up && down && right)         return WALL_T_LEFT;
    if (up && left && right)         return WALL_T_DOWN;
    if (down && left && right)       return WALL_T_UP;

    if (up && down)   return WALL_VERTICAL;
    if (left && right) return WALL_HORIZONTAL;

    if (down && right) return WALL_CORNER_TL;
    if (down && left)  return WALL_CORNER_TR;
    if (up && right)   return WALL_CORNER_BL;
    if (up && left)    return WALL_CORNER_BR;

    if (up)    return WALL_BOTTOM;
    if (down)  return WALL_TOP;
    if (left)  return WALL_RIGHT;
    if (right) return WALL_LEFT;

    return 1; //Fallback to generic wall
}

void autoTileMap() {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (level[y][x] == 1) {
                level[y][x] = getTileCode(y, x);
            }
        }
    }
}

sf::IntRect getWallTextureRect(int tileType) {
    switch(tileType) {
        case WALL_TOP:         return sf::IntRect(sf::Vector2i(TILE_PADDING * 11, TILE_PADDING * 14), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_BOTTOM:      return sf::IntRect(sf::Vector2i(TILE_PADDING * 10, TILE_PADDING * 14), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_LEFT:        return sf::IntRect(sf::Vector2i(TILE_PADDING * 10, TILE_PADDING * 15), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_RIGHT:       return sf::IntRect(sf::Vector2i(TILE_PADDING * 11, TILE_PADDING * 15), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_HORIZONTAL:  return sf::IntRect(sf::Vector2i(TILE_PADDING * 7, TILE_PADDING * 15), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_VERTICAL:    return sf::IntRect(sf::Vector2i(TILE_PADDING * 7, TILE_PADDING * 14), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_CORNER_TL:   return sf::IntRect(sf::Vector2i(TILE_PADDING * 7, TILE_PADDING * 11), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_CORNER_TR:   return sf::IntRect(sf::Vector2i(TILE_PADDING * 9, TILE_PADDING * 11), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_CORNER_BL:   return sf::IntRect(sf::Vector2i(TILE_PADDING * 7, TILE_PADDING * 13), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_CORNER_BR:   return sf::IntRect(sf::Vector2i(TILE_PADDING * 9, TILE_PADDING * 13), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_CROSS:       return sf::IntRect(sf::Vector2i(TILE_PADDING * 8, TILE_PADDING * 12), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_T_UP:        return sf::IntRect(sf::Vector2i(TILE_PADDING * 8, TILE_PADDING * 11), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_T_DOWN:      return sf::IntRect(sf::Vector2i(TILE_PADDING * 8, TILE_PADDING * 13), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_T_LEFT:      return sf::IntRect(sf::Vector2i(TILE_PADDING * 7, TILE_PADDING * 12), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        case WALL_T_RIGHT:     return sf::IntRect(sf::Vector2i(TILE_PADDING * 9, TILE_PADDING * 12), sf::Vector2i(TILE_SIZE, TILE_SIZE));
        default:               return sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(TILE_SIZE, TILE_SIZE)); // fallback
    }
}





#include "handlers.cpp"


int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Pacman Game");

    window.setFramerateLimit(60);

    sf::Texture mainTexture;

    if (!mainTexture.loadFromFile("../Progetto/pacmanSprites.png")) {
        return -1; // Handle error
    }

    // Create game objects (Pacman, ghosts, etc.) here
    sf::Sprite pacman(mainTexture); 
    sf::IntRect pacmanRect(sf::Vector2i(TILE_PADDING * 6 + 9, TILE_PADDING*4+ 7), sf::Vector2i(TILE_SIZE,TILE_SIZE)); // 8th column (index 7), 5th row (index 4)
    pacman.setTextureRect(pacmanRect);
    pacman.setScale(sf::Vector2f(SPRITES_SCALE,SPRITES_SCALE));

    // Initialize game state
    State state(pacman);
    state.velocity = sf::Vector2f(0.f, 0.f);
    state.pacman = pacman;

    //Set the clock
    state.clock.restart();

    //  ---   MAP CREATION    --- //
    autoTileMap();
    
    std::vector<sf::Sprite> mapTiles;

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int tileType = level[y][x];
            
            if (tileType == 0) continue;    //Empty Space
    
            sf::Sprite tile(mainTexture);
    
           
            if (tileType != 2 && tileType != 3 && tileType != 4) {  //Wall
                tile.setTextureRect(getWallTextureRect(tileType));
            }
            else if (tileType == 2) { //Dot
                tile.setTextureRect(sf::IntRect(sf::Vector2i(TILE_SIZE, 0), sf::Vector2i(TILE_SIZE, TILE_SIZE)));
            }
            else if (tileType == 3) { //Superdot
                tile.setTextureRect(sf::IntRect(sf::Vector2i(TILE_PADDING * 15, 0), sf::Vector2i(TILE_SIZE, TILE_SIZE)));
            }
            else if (tileType == 4) { //Ghost Door
                tile.setTextureRect(sf::IntRect(sf::Vector2i(TILE_PADDING * 16, 0), sf::Vector2i(TILE_SIZE, TILE_SIZE)));
            }
    
            tile.setScale(sf::Vector2f(SPRITES_SCALE, SPRITES_SCALE));
            tile.setPosition(sf::Vector2f(x * GRID_TILE, y * GRID_TILE));
            
            mapTiles.push_back(tile);
        }
    }


   
    // Main game loop
    while (window.isOpen()) {
        window.handleEvents(
            [&window](const sf::Event::Closed&) { handle_close(window); },
            [&window](const sf::Event::Resized& event) { handle_resize(event, window); },
            [&state](const auto& event) { handle(event, state); }
        );

        float deltaTime = state.clock.restart().asSeconds();
        if (isAlignedToGrid(state.pacman.getPosition())) {
            state.direction = state.nextDirection;
        }
        
        state.velocity = state.direction * MOVE_SPEED;
        state.pacman.move(state.velocity * deltaTime);


        window.clear();

        //   ---      DRAWS    ---     //
        for (const auto& tile : mapTiles) {
            window.draw(tile);
        }
        window.draw(state.pacman);
        

        window.display();
    }

    return 0;
}