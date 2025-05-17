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

//      ---    STRUCTURES   ---    //

//Pacman classical map, handmade:
//Here, height is 21 and WIDTH is 21
int level[MAP_HEIGHT][MAP_WIDTH] = {
    // 0 = empty, 1 = wall, 2 = pellet, 3 = power pellet, 4 = ghost door
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,1,0},
    {0,1,3,1,1,2,1,1,1,2,1,2,1,1,1,2,1,1,3,1,0},
    {0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,1,2,1,1,2,1,2,1,1,1,1,1,2,1,2,1,1,2,1,0},
    {0,1,2,2,2,2,1,2,2,2,1,2,2,2,1,2,2,2,2,1,0},
    {0,1,1,1,1,2,1,1,1,0,1,0,1,1,1,2,1,1,1,1,0},
    {0,0,0,0,1,2,1,0,0,0,0,0,0,0,1,2,1,0,0,0,0},
    {1,1,1,1,1,2,1,0,1,1,4,1,1,0,1,2,1,1,1,1,1},
    {0,0,0,0,0,2,0,0,1,0,0,0,1,0,0,2,0,0,0,0,0},
    {1,1,1,1,1,2,1,0,1,1,1,1,1,0,1,2,1,1,1,1,1},
    {0,0,0,0,1,2,1,0,0,0,0,0,0,0,1,2,1,0,0,0,0},
    {0,1,1,1,1,2,1,0,1,1,1,1,1,0,1,2,1,1,1,1,0},
    {0,1,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,1,0},
    {0,1,2,1,1,2,1,1,1,2,1,2,1,1,1,2,1,1,2,1,0},
    {0,1,3,2,1,2,2,2,2,2,0,2,2,2,2,2,1,2,3,1,0},
    {0,1,1,2,1,2,1,2,1,1,1,1,1,2,1,2,1,2,1,1,0},
    {0,1,2,2,2,2,1,2,2,2,1,2,2,2,1,2,2,2,2,1,0},
    {0,1,2,1,1,1,1,1,1,2,1,2,1,1,1,1,1,1,2,1,0},
    {0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0} 
};

enum class GameState{
    PLAYING,
    GAME_OVER,
    GAME_WON
    //Maybe add pause later on
};

struct Pellet {
    sf::Sprite sprite;
    bool isSuper = false;
    Pellet(const sf::Sprite& sprite, bool isSuper) : sprite(sprite), isSuper(isSuper) {}
};

struct Character {
    sf::Vector2f direction;
    sf::Vector2f nextDirection; //Direction to be set when aligned to the grid
    sf::Vector2f velocity;
    sf::Sprite sprite;
    bool traversingTunnel = false; //For smoother movement in the tunnel

    Character(const sf::Sprite& sprite) : sprite(sprite) {
        direction = sf::Vector2f(0.f, 0.f);
        nextDirection = sf::Vector2f(0.f, 0.f);
        velocity = sf::Vector2f(0.f, 0.f);
    }
};

struct State {
    //sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Pacman Game");
    Character pacman;
    sf::Clock clock;
    std::vector<Pellet> dots;
    GameState gameState = GameState::GAME_WON;  //For testing purposes, set to GAME_WON
    const sf::Font font{"../Progetto/Emulogic.ttf"}; 
    sf::Texture mainTexture; 

    State(const sf::Sprite& pacmanSprite) 
        : pacman(pacmanSprite) {}
};

//      ---    FUCNTIONS   ---    //

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
    
                case 1: // Wall
                    tile.setFillColor(sf::Color::Blue);
                    break;
    
                case 2: // Dot
                    tile.setFillColor(sf::Color::Black);
                    break;
    
                case 3: // Super Dot
                    tile.setFillColor(sf::Color::Black);
                break;
    
                case 4: // Ghost Door
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

    return level[row][col] != 1; // 1 = wall
}

void placeDots(State& state, const sf::Texture& texture) {
    state.dots.clear();

    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            if (level[row][col] == 2) {     //Normal dot
                sf::Sprite dotSprite(texture);
                sf::IntRect dotRect(DOT_TEXTURE, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
                dotSprite.setTextureRect(dotRect);
                dotSprite.setScale(sf::Vector2f(SPRITES_SCALE, SPRITES_SCALE));
                dotSprite.setPosition(sf::Vector2f(col * GRID_TILE, row * GRID_TILE));
                Pellet dot(dotSprite, false);
                state.dots.push_back(dot);
            }else if(level[row][col] == 3) { //Super dot
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


#include "handlers.cpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Pacman Game");

    sf::Texture mainTexture;
    if (!mainTexture.loadFromFile("../Progetto/pacmanSprites.png"))  return -1;

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
    -Più pallini eliminati con una singola collisione
    -Resize da fixare (Togliere? Sennò fare in modo che combaci bene con la mappa)  edit: sembra funzionare
    
    Prossime cose da implementare:
    -Game over

    -Presenza fantasmi
    -Gestione collisioni con i fantasmi
    -Gestione animazioni
    -Gestione punteggio (facoltativa)
    -Gestione suoni (facoltativa)
    -Gestione pause e menù (facoltativa)
    -Gestione livelli (molto facoltativa)
*/