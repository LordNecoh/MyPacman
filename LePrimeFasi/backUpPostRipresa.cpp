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
        window.draw(state.pacman);
        

        window.display();
    }

    return 0;
}