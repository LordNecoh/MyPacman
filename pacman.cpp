#ifndef SFML_INCLUDES
#define SFML_INCLUDES

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <queue>
#include <limits.h> 
#include <random>
#include <string>
#include <fstream>

#endif

//      ---    GLOBAL VARIABLES   ---    //

//In pixels
const int CHARACTER_SIZE = 17; // Size of each character (space without padding in the texture sheet)
const int TILE_SIZE = 20;
const float LIFE_OFFSET = 20.f; //Offset between lives

//Constants for scaling
const float SPRITES_SCALE = 1.5f; 
const float LIFE_SCALE = 0.8f;

const int GRID_TILE = CHARACTER_SIZE * SPRITES_SCALE;
const int COLLISION_TILE = GRID_TILE - 1; //Slightly smaller for more permissive collision detection

//Speed is expressed in pixels per second
const float PACMAN_SPEED = 80.0f; 
float GHOST_SPEED = 70.0f; //Default speed for the ghosts
float SCARED_SPEED = GHOST_SPEED / 2; //Half speed when scared

//Time is expressed in seconds
float SUPER_TIME = 9.0f; 
const float FRUIT_TIME = 9.0f; 

const int MAP_HEIGHT = 21;
const int MAP_WIDTH = 21;

const int FRAME_RATE = 60;
const int SCOREBOARD_HEIGHT = 40;
const int WINDOW_WIDTH = MAP_WIDTH * GRID_TILE;
const int WINDOW_HEIGHT = MAP_HEIGHT * GRID_TILE + SCOREBOARD_HEIGHT;
const int SCOREBOARD_Y = WINDOW_HEIGHT - SCOREBOARD_HEIGHT + 18;

//Directions float
const sf::Vector2f NO_DIRECTION(0.f, 0.f);
const sf::Vector2f UP(0.f, -1.f);
const sf::Vector2f DOWN(0.f, 1.f);
const sf::Vector2f LEFT(-1.f, 0.f);
const sf::Vector2f RIGHT(1.f, 0.f);

//Directions int
const sf::Vector2i NO_DIRECTION_INT(0, 0);
const sf::Vector2i UP_INT(0, -1);
const sf::Vector2i DOWN_INT(0, 1);
const sf::Vector2i LEFT_INT(-1, 0);
const sf::Vector2i RIGHT_INT(1, 0);

//Map key locations
const sf::Vector2f PACMAN_SPAWN(10 * GRID_TILE, 15 * GRID_TILE);
const sf::Vector2f RED_SPAWN(10 * GRID_TILE, 7 * GRID_TILE);
const sf::Vector2f PINK_SPAWN(10 * GRID_TILE, 9 * GRID_TILE);
const sf::Vector2f GREEN_SPAWN(9 * GRID_TILE, 9 * GRID_TILE);
const sf::Vector2f ORANGE_SPAWN(11 * GRID_TILE, 9 * GRID_TILE);
const sf::Vector2f FRUIT_SPAWN(10 * GRID_TILE, 11 * GRID_TILE); 
const sf::Vector2f HOME_CORNER(3 * GRID_TILE, 15 * GRID_TILE); 

//Menu positions
const sf::Vector2f TITLE_POSITION(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 - 10);
const sf::Vector2f RESTART_POSITION(TITLE_POSITION.x , TITLE_POSITION.y + 125);
const sf::Vector2f HIGHSCORE_TEXT_MENUPOSITION(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 100.f);
const sf::Vector2f HIGHSCORE_MENUPOSITION(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 125.f);
const sf::Vector2f THANKS_POSITION(TITLE_POSITION.x, TITLE_POSITION.y + 30.f);
const sf::Vector2f NEWLEVEL_POSITION(RESTART_POSITION.x, RESTART_POSITION.y + 30.f);

//Game positions
const sf::Vector2f SCORE_POSITION(10, SCOREBOARD_Y);
const sf::Vector2f LIFETEXT_POSITION(180, SCOREBOARD_Y);
const sf::Vector2f LIFE_POSITION(245, SCOREBOARD_Y);
const sf::Vector2f HIGHSCORE_POSITION(350, SCOREBOARD_Y); 
const sf::Vector2f LEVEL_POSITION(WINDOW_WIDTH / 2, 10); //Willingly on top on the upper wall

//Vectors to initialize textures
const sf::Vector2i CHARACTER_VECTOR(CHARACTER_SIZE, CHARACTER_SIZE);
const sf::Vector2f SCALE_VECTOR(SPRITES_SCALE, SPRITES_SCALE); 

//  ---    SOUNDS   ---    //
const std::string MAIN_THEME = "src/progetto/RisorseProgetto/pacmanTheme.ogg";
const std::string MENU_THEME = "src/progetto/RisorseProgetto/menuMusic.mp3"; //Menu Music by SergeQuadrado -- https://freesound.org/s/786273/ -- License: Attribution NonCommercial 4.0
const std::string CHASE_THEME = "src/progetto/RisorseProgetto/chase.wav"; //Chase 8-Bit.wav by Volvion -- https://freesound.org/s/315717/ -- License: Creative Commons 0

const std::string EATING_SOUND = "src/progetto/RisorseProgetto/blop.flac"; //water_flicks_splash_057.flac by Department64 -- https://freesound.org/s/607803/ -- License: Attribution 4.0
const std::string SUPERDOT_SOUND = "src/progetto/RisorseProgetto/blipBlip.wav"; //BLIP BLIP.wav by Hacrown87 -- https://freesound.org/s/488257/ -- License: Creative Commons 0
const std::string WOOSH_SOUND = "src/progetto/RisorseProgetto/woosh.wav"; //electric woosh by HadaHector -- https://freesound.org/s/446383/ -- License: Attribution 4.0
const std::string DEATH_SOUND = "src/progetto/RisorseProgetto/death.wav"; //8 bit Death sound by MentosLat -- https://freesound.org/s/417486/ -- License: Creative Commons 0


#include "textures.hpp"
#include "functions.hpp"
#include "gameStructures.hpp"
#include "functions.cpp"
#include "handlers.cpp"


int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "My Pacman Game");
    sf::Texture mainTexture;
    if (!mainTexture.loadFromFile("src/progetto/RisorseProgetto/pacmanSprites.png"))  return -1;

    window.setFramerateLimit(FRAME_RATE);

    State state(mainTexture);
    state.soundManager.setMusic(MENU_THEME);  
   
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
        
        float deltaTime = state.clock.restart().asSeconds();    //Calculating only once for precision
        managePacman(state, deltaTime); 
        manageEnemies(state, deltaTime);

        drawAll(window, state);
        window.display();
    }
    return 0;
}