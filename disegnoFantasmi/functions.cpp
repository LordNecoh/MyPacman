#ifndef SFML_INCLUDES
#define SFML_INCLUDES

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <vector>
#include <iostream>

#endif

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
    
                case GHOST_DOOR:
                    tile.setFillColor(sf::Color(255, 0, 255)); // Purple
                break;

                default:
                    tile.setFillColor(sf::Color::Black);
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
        case GameState::MAIN_MENU:{
            title.setString("My Pacman Game");
            restart.setString("Press Enter to start");

            sf::Text controls{state.font, "Use WASD to move, P to pause", 12};
            centerOrigin(controls);
            controls.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 80.f));
            window.draw(controls);

            sf::Text credits{state.font, "By: Leonardo Necordi", 12};
            centerOrigin(credits);
            credits.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 150.f));
            window.draw(credits);
            break;
        }

        case GameState::PAUSE:
            title.setString("Paused");
            restart.setString("Press Enter to continue");
            break;
            
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

void pauseGame(State& state) {
    state.gameState = GameState::PAUSE;
    state.clock.restart();
    state.pacman.velocity = NO_DIRECTION; 
}

void resumeGame(State& state) {
    state.gameState = GameState::PLAYING;
    state.clock.restart();
    state.pacman.velocity = NO_DIRECTION; 
}

void manageEnter(State& state) {
    switch (state.gameState) {
        case GameState::MAIN_MENU:
            state.gameState = GameState::PLAYING;
            resetGame(state, state.mainTexture);
            break;
        case GameState::PAUSE:
            resumeGame(state);
            break;
        case GameState::GAME_OVER:
            state.gameState = GameState::MAIN_MENU;
            break;
        case GameState::GAME_WON:
            state.gameState = GameState::MAIN_MENU;
            break;
        default:
            break;
    }
}

Character initializePacman(const sf::Texture& texture) {
    sf::Sprite pacmanSprite(texture);
    sf::IntRect pacmanRect(PACMAN_TEXTURE, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
    pacmanSprite.setTextureRect(pacmanRect);
    pacmanSprite.setScale(sf::Vector2f(SPRITES_SCALE, SPRITES_SCALE));
    pacmanSprite.setPosition(PACMAN_SPAWN);

    return Character(pacmanSprite);
}

sf::IntRect initializeGhostRect(GhostType ghostType) {
    switch (ghostType) {
        case GhostType::RED:
            return sf::IntRect(RED_GHOST_TEXTURE, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
        case GhostType::PINK:
            return sf::IntRect(PINK_GHOST_TEXTURE, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
        case GhostType::GREEN:
            return sf::IntRect(GREEN_GHOST_TEXTURE, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
        case GhostType::ORANGE:
            return sf::IntRect(ORANGE_GHOST_TEXTURE, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
    }
    return sf::IntRect(); // Default case
}

sf::Vector2f initializeGhostSpawn(GhostType ghostType){
    switch (ghostType) {
        case GhostType::RED:
            return RED_SPAWN;
        case GhostType::PINK:
            return PINK_SPAWN;
        case GhostType::GREEN:
            return GREEN_SPAWN;
        case GhostType::ORANGE:
            return ORANGE_SPAWN;
    }
    return sf::Vector2f(); // Default case
}

Character initializeGhost(const sf::Texture& texture, GhostType ghostType) {
    sf::Sprite ghostSprite(texture);
    sf::IntRect ghostRect = initializeGhostRect(ghostType);

    ghostSprite.setTextureRect(ghostRect);
    ghostSprite.setScale(sf::Vector2f(SPRITES_SCALE, SPRITES_SCALE));
    ghostSprite.setPosition(initializeGhostSpawn(ghostType));

    return Character(ghostSprite);
}

Enemies initializeEnemies(const sf::Texture& texture) {
    return(
        Enemies(
            initializeGhost(texture, GhostType::RED),
            initializeGhost(texture, GhostType::PINK),
            initializeGhost(texture, GhostType::GREEN),
            initializeGhost(texture, GhostType::ORANGE)
        )
    );
}

