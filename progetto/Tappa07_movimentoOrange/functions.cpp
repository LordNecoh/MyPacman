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

void drawMap(sf::RenderWindow& window) {
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

void checkTunnel(const sf::Vector2f position, Character& character) {
    sf::Vector2i nextPosition = getGridCoords(getNextPosition(position, character.direction));

    if ( nextPosition.y == 9 && nextPosition.x == -1) { //Left tunnel
        manageTunnel(character, TunnelSide::LEFT);
    } else if (nextPosition.y == 9 && nextPosition.x == 21) { //Right tunnel
        manageTunnel(character, TunnelSide::RIGHT);
    }
}


/*  Assumes that the character is in the tunnel (row == 9) and col == 0 or col == 20 
    And teleports it to the other side  */
void manageTunnel(Character& character, TunnelSide tunnelSide){
   if(tunnelSide == TunnelSide::LEFT) {
        character.sprite.setPosition(sf::Vector2f(20 * GRID_TILE, 9 * GRID_TILE)); //Teleport to the right side
    }else if(tunnelSide == TunnelSide::RIGHT) {
        character.sprite.setPosition(sf::Vector2f(0 * GRID_TILE, 9 * GRID_TILE)); //Teleport to the left side
    }
}

sf::Vector2f getNextPosition(const sf::Vector2f& position, const sf::Vector2f& direction) {
    return position + (direction * static_cast<float>(GRID_TILE));
}

//Checks collision with walls and tunnel
bool canWalk(sf::Vector2f position, const sf::Vector2f& direction) {
    sf::Vector2i nextPosition = getGridCoords(getNextPosition(position, direction));

    return level[nextPosition.y][nextPosition.x] != WALL; 
}

std::vector<Pellet> placeDots(const sf::Texture& texture) {
    std::vector<Pellet> dots;

    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            if (level[row][col] == DOT) {  

                sf::Sprite dotSprite(texture);
                sf::IntRect dotRect(DOT_TEXTURE, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
                dotSprite.setTextureRect(dotRect);
                dotSprite.setScale(sf::Vector2f(SPRITES_SCALE, SPRITES_SCALE));
                dotSprite.setPosition(sf::Vector2f(col * GRID_TILE, row * GRID_TILE));
                Pellet dot(dotSprite, false);
                dots.push_back(dot);

            }else if(level[row][col] == SUPER_DOT) { 

                sf::Sprite dotSprite(texture);
                sf::IntRect dotRect(SUPERDOT_TEXTURE, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
                dotSprite.setTextureRect(dotRect);
                dotSprite.setScale(sf::Vector2f(SPRITES_SCALE, SPRITES_SCALE));
                dotSprite.setPosition(sf::Vector2f(col * GRID_TILE, row * GRID_TILE));
                Pellet dot(dotSprite, true);
                dots.push_back(dot);

            }
        }
    }
    return dots;
}

void manageDots(State& state, const sf::Vector2i& pacmanCoords) {
    for(int i=0; i<state.dots.size(); ++i) {
        if (doCollide(pacmanCoords, state.dots[i].sprite)) {
            state.dots.erase(state.dots.begin() + i); 
            if(state.dots.empty()) {
                state.gameState = GameState::GAME_WON;
            }
            break; //To avoid deleting more than one dot at a time
        }
    }
}

void checkCollisions(State& state){
    checkTunnel(state.pacman.sprite.getPosition(), state.pacman);

    //Design choice ahead: less elegant but more efficient (calculates pacmanCoords only once)
    sf::Vector2i pacmanCoords = getGridCoords(state.pacman.sprite.getPosition());
    manageDots(state, pacmanCoords);
    ghostsCollisions(state, pacmanCoords);
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

            sf::Text controls{state.font, "Use WASD to move, P to pause, R to reset", 12};
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
    state.pacman = initializePacman(texture);
    state.ghosts = Enemies(texture);
    state.dots = placeDots(texture);
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
            resetGame(state, state.mainTexture);
            state.gameState = GameState::PLAYING;
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

void drawAll(sf::RenderWindow& window, State& state) {
    drawMap(window);
    for (const auto& dot : state.dots) {
        window.draw(dot.sprite);
    }
    window.draw(state.pacman.sprite);  
    window.draw(state.ghosts.Red.sprite);
    window.draw(state.ghosts.Pink.sprite);
    window.draw(state.ghosts.Green.sprite);
    window.draw(state.ghosts.Orange.sprite);
}

void ghostsCollisions(State& state, const sf::Vector2i& pacmanCoords) {
    if (doCollide(pacmanCoords, state.ghosts.Pink.sprite) ||
        doCollide(pacmanCoords, state.ghosts.Red.sprite) ||
        doCollide(pacmanCoords, state.ghosts.Green.sprite) ||
        doCollide(pacmanCoords, state.ghosts.Orange.sprite)) {
        state.gameState = GameState::GAME_OVER;
    }
}

bool doCollide(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
    sf::Vector2i gridCoords1 = getGridCoords(sprite1.getPosition());
    sf::Vector2i gridCoords2 = getGridCoords(sprite2.getPosition());

    return gridCoords1 == gridCoords2;
}

bool doCollide(const sf::Vector2i& pacmanCoords, const sf::Sprite& collidingSprite) {
    //This function is preferred to the one above because it is more efficient
    //The downside is that it makes for a less elegant code
    sf::Vector2i collidingCoords = getGridCoords(collidingSprite.getPosition());
    return pacmanCoords == collidingCoords;
}

//Deprecated function, not used in the code
std::vector<sf::Vector2f> getPossibleDirections(const Ghost& ghost) {
    if (ghost.lastDirection == UP) {
        return {UP, LEFT, RIGHT};
    } else if (ghost.lastDirection == DOWN) {
        return {DOWN, LEFT, RIGHT};
    } else if (ghost.lastDirection == LEFT) {
        return {LEFT, UP, DOWN};
    } else if (ghost.lastDirection == RIGHT) {
        return {RIGHT, UP, DOWN};
    } else {
        return {UP, DOWN, LEFT, RIGHT}; // No last direction
    }
}

//Deprecated function, not used in the code
sf::Vector2f getGhostDirection(Ghost& ghost, const sf::Vector2f& target) {
    std::vector<sf::Vector2f> possibleDirections = getPossibleDirections(ghost);
    int minDistance = INT_MAX;
    sf::Vector2f bestDirection = ghost.direction; // Default to current direction

    for (const auto& direction : possibleDirections) {
        if (canWalk(ghost.sprite.getPosition(), direction)) {
            int distance = manhattanDistance(getGridCoords(ghost.sprite.getPosition()), getGridCoords(target));
            if (distance < minDistance) {
                minDistance = distance;
                bestDirection = direction;
            }
        }
    }
    return bestDirection;

}

void moveGhost(Ghost& ghost, const sf::Vector2f& target, float deltaTime) {
    
    if(isAlignedToGrid(ghost.sprite.getPosition())) {
        ghost.direction = getNextStepFromMap(getGridCoords(ghost.sprite.getPosition()), bfsDistanceMap(getGridCoords(target)));
    }else{
        ghost.direction = ghost.lastDirection;
    }

    checkTunnel(ghost.sprite.getPosition(), ghost); 
    ghost.sprite.move(ghost.direction * GHOST_SPEED * deltaTime); 
    ghost.lastDirection = ghost.direction; 
}

sf::Vector2f getPinkTaget(const sf::Vector2f& pacmanPos, const sf::Vector2f& pacmanDirection, const sf::Vector2f & pinkPos) {
    sf::Vector2f target = pacmanPos + (pacmanDirection * 4.f * static_cast<float>(GRID_TILE));

    sf::Vector2i targetCoords = getGridCoords(target);
    if (targetCoords.x < 0 || targetCoords.y < 0 || targetCoords.x >= MAP_WIDTH || targetCoords.y >= MAP_HEIGHT) {
        return pacmanPos; // Return pacman position if out of bounds
    }
    sf::Vector2i pinkCoords = getGridCoords(pinkPos);
    if (manhattanDistance(pinkCoords, targetCoords) <= 3) {
        return pacmanPos;  //Finalise the chase if already flanked pacman
    }

    return target; // Return the calculated target if valid
}

sf::Vector2f getOrangeTarget(const sf::Vector2f& pacmanPos, const sf::Vector2f& orangePos) {
    if(distance(pacmanPos, orangePos) > 5.f * GRID_TILE) {
        return pacmanPos; // Orange follows pacman
    }else{
        return HOME_CORNER; 
    }
}

void manageEnemies(State& state, float deltaTime){
    sf::Vector2f target;

    //Red follows the player
    target = state.pacman.sprite.getPosition();
    moveGhost(state.ghosts.Red, target, deltaTime);

    //Pink tries to get ahead of the player
    target = getPinkTaget(state.pacman.sprite.getPosition(), state.pacman.direction, state.ghosts.Pink.sprite.getPosition());
    moveGhost(state.ghosts.Pink, target, deltaTime);

    //Orange follows the player if he is close, otherwise runs away
    target = getOrangeTarget(state.pacman.sprite.getPosition(), state.ghosts.Orange.sprite.getPosition());
    moveGhost(state.ghosts.Orange, target, deltaTime);
}


//   ----   Functions for pathfinding   ----   //

float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

int manhattanDistance(const sf::Vector2i& a, const sf::Vector2i& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<std::vector<int>> bfsDistanceMap(sf::Vector2i goal) {
    std::vector<std::vector<int>> dist(MAP_HEIGHT, std::vector<int>(MAP_WIDTH, -1));
    std::queue<sf::Vector2i> q;

    q.push(goal);
    dist[goal.y][goal.x] = 0;

    std::vector<sf::Vector2i> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    while (!q.empty()) {
        sf::Vector2i current = q.front(); 
        q.pop();

        for (auto dir : directions) {
            sf::Vector2i next = current + dir;

            // Tunnel handling
            if (current.y == 9 && current.x == 0 && dir == sf::Vector2i{-1, 0}) {
                next = sf::Vector2i(20, 9); // from (0,9) to (20,9)
            } else if (current.y == 9 && current.x == 20 && dir == sf::Vector2i{1, 0}) {
                next = sf::Vector2i(0, 9); // from (20,9) to (0,9)
            }

            if (next.x < 0 || next.y < 0 || next.x >= MAP_WIDTH || next.y >= MAP_HEIGHT)
                continue;

            if (level[next.y][next.x] == WALL || dist[next.y][next.x] != -1)
                continue;

            dist[next.y][next.x] = dist[current.y][current.x] + 1;
            q.push(next);
        }
    }

    return dist;
}

sf::Vector2f getNextStepFromMap(sf::Vector2i ghostPos, const std::vector<std::vector<int>>& distMap) {
    sf::Vector2i bestDir = {0, 0};
    int bestDist = INT_MAX;

    std::vector<sf::Vector2i> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    for (auto dir : directions) {
        sf::Vector2i next = ghostPos + dir;
        if (next.x < -1 || next.y < 0 || next.x > MAP_WIDTH || next.y >= MAP_HEIGHT)
            continue;

        int d = distMap[next.y][next.x];
        if (d != -1 && d < bestDist) {
            bestDist = d;
            bestDir = dir;
        }
    }

    return sf::Vector2f(bestDir.x, bestDir.y);
}

