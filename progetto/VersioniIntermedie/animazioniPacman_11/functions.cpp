#ifndef SFML_INCLUDES
#define SFML_INCLUDES
#endif

#include "functions.hpp"

//      ---    GRID AND MAP  ---    //
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

void checkSuper(State& state) {
    if(state.isPacmanSuper) {
        if(state.superClock.getElapsedTime().asSeconds() > SUPER_TIME) {
            state.ghosts.Red.setChase();
            state.ghosts.Pink.setChase();
            state.ghosts.Green.setChase();
            state.ghosts.Orange.setChase();
            state.isPacmanSuper = false; 
        }
    }
}

sf::Vector2f updatePacmanDirection(const Character& pacman) {
    if (canWalk(pacman.sprite.getPosition(), pacman.nextDirection)) {
        return pacman.nextDirection;
    } else {
        return NO_DIRECTION; //Block movement if there is a wall
    }
}

//   ----   MENU   ----   //

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
    state.superClock.restart();
    state.isPacmanSuper = false;
    state.ghosts.Red.mood = GhostMood::CHASE;
    state.ghosts.Pink.mood = GhostMood::CHASE;
    state.ghosts.Green.mood = GhostMood::CHASE;
    state.ghosts.Orange.mood = GhostMood::CHASE;
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

//    ---- INITIALIZATION ----    //

Character initializePacman(const sf::Texture& texture) {
    sf::Sprite pacmanSprite(texture);
    sf::IntRect pacmanRect(PACMAN_TEXTURE_DOWN1, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
    pacmanSprite.setTextureRect(pacmanRect);
    pacmanSprite.setScale(sf::Vector2f(SPRITES_SCALE, SPRITES_SCALE));
    pacmanSprite.setPosition(PACMAN_SPAWN);

    return Character(pacmanSprite);
}

sf::IntRect initializeGhostRect(GhostType ghostType) {
    switch (ghostType) {
        case GhostType::RED:
            return sf::IntRect(RED_GHOST_TEXTURE_DOWN, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
        case GhostType::PINK:
            return sf::IntRect(PINK_GHOST_TEXTURE_DOWN, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
        case GhostType::GREEN:
            return sf::IntRect(GREEN_GHOST_TEXTURE_DOWN, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
        case GhostType::ORANGE:
            return sf::IntRect(ORANGE_GHOST_TEXTURE_DOWN, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE));
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

//    ----   COLLISIONS   ----   //

void checkCollisions(State& state){
    checkTunnel(state.pacman.sprite.getPosition(), state.pacman);

    //Design choice ahead: less elegant but more efficient (calculates pacmanCoords only once)
    sf::Vector2i pacmanCoords = getGridCoords(state.pacman.sprite.getPosition());
    manageDots(state, pacmanCoords);
    checkSuper(state);
    enemiesCollisions(state);
}

void manageGhostCollision(Ghost& ghost, State& state) {
    if(doesGhostCollide(ghost, state.pacman)) {
        if(ghost.mood == GhostMood::CHASE){
            state.gameState = GameState::GAME_OVER;
        }else if(ghost.mood == GhostMood::FRIGHTENED) {
            ghost.mood = GhostMood::ORB; 
            ghost.sprite.setTextureRect(sf::IntRect(ORB_TEXTURE, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE)));
        }
    }
}

void enemiesCollisions(State& state) {
    manageGhostCollision(state.ghosts.Red, state);
    manageGhostCollision(state.ghosts.Pink, state);
    manageGhostCollision(state.ghosts.Green, state);
    manageGhostCollision(state.ghosts.Orange, state);
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

bool doesGhostCollide(const Ghost& ghost, const Character& pacman) {
    //Does not use grid coordinates, but the actual position of the sprites to make the collision check more precise
    sf::Vector2f ghostPos = ghost.sprite.getPosition();
    sf::Vector2f pacmanPos = pacman.sprite.getPosition();

    if(ghostPos.x < pacmanPos.x + COLLISION_TILE  &&
       ghostPos.x + COLLISION_TILE > pacmanPos.x &&
       ghostPos.y < pacmanPos.y + COLLISION_TILE &&
       ghostPos.y +COLLISION_TILE > pacmanPos.y) {
        return true; // Collision detected
    }
    return false; // No collision
}

void manageDots(State& state, const sf::Vector2i& pacmanCoords) {
    for(int i=0; i<state.dots.size(); ++i) {
        if (doCollide(pacmanCoords, state.dots[i].sprite)) {
            if(state.dots[i].isSuper) {
                state.ghosts.Red.setFrightened();
                state.ghosts.Pink.setFrightened();
                state.ghosts.Green.setFrightened();
                state.ghosts.Orange.setFrightened();
                state.isPacmanSuper = true;
                state.superClock.restart(); 
            }
            state.dots.erase(state.dots.begin() + i); 
            if(state.dots.empty()) {
                state.gameState = GameState::GAME_WON;
            }
            break; //To avoid deleting more than one dot at a time
        }
    }
}

//   ----   GHOSTS  ----   //

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

sf::Vector2f getAlternateTarget(const sf::Vector2f& pacmanPos, const sf::Vector2f& pacmanDirection, const Ghost& ghost) {
    // Try to predict Pacman's curves by considering his current direction and possible turns
    std::vector<sf::Vector2f> alternativeDirections = getPossibleDirections(ghost);

    for (const auto& altDir : alternativeDirections) {
        sf::Vector2f alternativeTarget = pacmanPos +  (pacmanDirection * 2.f * static_cast<float>(GRID_TILE));
        alternativeTarget += altDir * 2.f * static_cast<float>(GRID_TILE); // Move in the alternative direction
        sf::Vector2i alternativeCoords = getGridCoords(alternativeTarget);

        if (isTargetValid(alternativeCoords)) {
            return alternativeTarget; // Return the first valid alternative target
        }
    }

    return pacmanPos; // If no valid alternative is found, return Pacman's position
}

sf::Vector2f getPinkTarget(const sf::Vector2f& pacmanPos, const sf::Vector2f& pacmanDirection, const Ghost& pinkGhost) {    
    sf::Vector2f target = pacmanPos + (pacmanDirection * 4.f * static_cast<float>(GRID_TILE));

    sf::Vector2i targetCoords = getGridCoords(target);
    if (!isTargetValid(targetCoords)) {
        return getAlternateTarget(pacmanPos, pacmanDirection, pinkGhost); // If out of bounds, use the alternate target function
    }

    sf::Vector2i pinkCoords = getGridCoords(pinkGhost.sprite.getPosition());
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

sf::Vector2f getGreenTarget( const sf::Vector2f& pacmanPos, const sf::Vector2f& pacmanDir, const sf::Vector2f& blinkyPos, const Ghost& greenGhost) {
    sf::Vector2f offset = pacmanDir * 2.f * static_cast<float>(GRID_TILE);
    sf::Vector2f inFrontOfPacman = pacmanPos + offset;

    sf::Vector2f vec = inFrontOfPacman - blinkyPos;
    sf::Vector2f target = blinkyPos + vec * 2.f;

    if(!isTargetValid(getGridCoords(target))) {
        return getAlternateTarget(pacmanPos, pacmanDir, greenGhost); // If out of bounds, use the alternate target function
    }
    if(manhattanDistance(getGridCoords(blinkyPos), getGridCoords(inFrontOfPacman)) <= 3) {
        return pacmanPos; // Finalise the chase if already flanked pacman
    }

    return target;
}

sf::Vector2f getGhostSpawn(GhostType ghostType) {
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

void respawnGhost(Ghost& ghost) {
    sf::Vector2f spawnPosition = getGhostSpawn(ghost.type);
    if (distance(ghost.sprite.getPosition(), spawnPosition) < 3.0f) { // Allow small tolerance
        ghost.mood = GhostMood::CHASE; 
        ghost.sprite.setTextureRect(initializeGhostRect(ghost.type)); 
    }
}

void manageGhost(Ghost& ghost, const sf::Vector2f& target, float deltaTime) {
    switch (ghost.mood) {
        case GhostMood::CHASE:
            moveGhost(ghost, target, deltaTime);
            break;
        case GhostMood::FRIGHTENED:
            moveScaredGhost(ghost, deltaTime);
            break;
        case GhostMood::ORB:
            moveGhost(ghost, getGhostSpawn(ghost.type), deltaTime); // Move to home corner
            respawnGhost(ghost); //Reset ghosts if they are in the spawn point
            break;
    }
}

void manageEnemies(State& state, float deltaTime){
    sf::Vector2f target;

    //Red follows the player 
    target = state.pacman.sprite.getPosition();
    manageGhost(state.ghosts.Red, target, deltaTime);
    
    //Pink tries to get ahead of the player
    target = getPinkTarget(state.pacman.sprite.getPosition(), state.pacman.direction, state.ghosts.Pink);
    manageGhost(state.ghosts.Pink, target, deltaTime);

    //Orange follows the player if he is close, otherwise runs away
    target = getOrangeTarget(state.pacman.sprite.getPosition(), state.ghosts.Orange.sprite.getPosition());
    if(state.dots.size() > 120){
        moveGhost(state.ghosts.Orange, getGhostSpawn(state.ghosts.Orange.type), deltaTime) ; //Orange starts after yhe player has eaten 30 dots
    }else{
        manageGhost(state.ghosts.Orange, target, deltaTime);
    }
    
    //Green tries to flank the player coordinating with red
    target = getGreenTarget(state.pacman.sprite.getPosition(), state.pacman.direction, state.ghosts.Red.sprite.getPosition(), state.ghosts.Green);
    if(state.dots.size() > 90){
        moveGhost(state.ghosts.Green, getGhostSpawn(state.ghosts.Green.type), deltaTime); //Green starts after the player has eaten 60 dots
    } else{
        manageGhost(state.ghosts.Green, target, deltaTime);
    }

    animateEnemies(state);
}

void moveScaredGhost(Ghost& ghost, float deltaTime) {
    if (isAlignedToGrid(ghost.sprite.getPosition())) {
        ghost.direction = chooseScaredDirection(ghost);
    } else {
        ghost.direction = ghost.lastDirection;
    }

    checkTunnel(ghost.sprite.getPosition(), ghost); 
    ghost.sprite.move(ghost.direction * SCARED_SPEED * deltaTime); 
    ghost.lastDirection = ghost.direction; 
}

sf::Vector2f chooseScaredDirection(Ghost& ghost){
    //Choose a random direction from the possible ones
    std::vector<sf::Vector2f> possibleDirections = getPossibleDirections(ghost);
    sf::Vector2f direction = ghost.direction; // Default to current direction

    for (int i = possibleDirections.size() -1; i >= 0; --i) {
        if (!canWalk(ghost.sprite.getPosition(), possibleDirections[i])) {
            //Removes the direction if it is not valid
            possibleDirections.erase(possibleDirections.begin() +i);
        }
    }
    if (!possibleDirections.empty()) {
        // Choose a random direction from the remaining possible ones
        int randomIndex = rand() % possibleDirections.size();
        direction = possibleDirections[randomIndex];
    } 

    return direction;
}

//   ---   PATHFIDING   ---   //

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
    
        if (next.y == 9) {
            if (next.x == -1 && dir == sf::Vector2i{-1, 0}) next = {21, 9};
            if (next.x == 21 && dir == sf::Vector2i{ 1, 0}) next = {0, 9};
        }
    
        if (next.x < 0 || next.y < 0 || next.x >= MAP_WIDTH || next.y >= MAP_HEIGHT)
            continue;
    
        int d = distMap[next.y][next.x];
        if (d != -1 && d < bestDist) {
            bestDist = d;
            bestDir = dir;
        }
    }
    

    return sf::Vector2f(bestDir.x, bestDir.y);
}

bool isTargetValid(const sf::Vector2i& target) {
    //Checks if target is inside of the map, not a wall and not a void tile outside of the map
    if(target.x < 0 || target.y < 0 || target.x >= MAP_WIDTH || target.y >= MAP_HEIGHT) {
        return false; // Out of bounds
    }
    if(level[target.y][target.x] == WALL || level[target.y][target.x] == UNREACHABLE) {
        return false; // Wall or unreachable tile
    }
    return true; // Valid target
}


//   ----   ANIMATIONS   ----   //

sf::Vector2i getRedGhostTexture(Ghost& ghost) {
    if(ghost.direction == DOWN) {
        return RED_GHOST_TEXTURE_DOWN;
    }else if(ghost.direction == LEFT) {
        return RED_GHOST_TEXTURE_LEFT;
    }else if(ghost.direction == RIGHT) {
        return RED_GHOST_TEXTURE_RIGHT;
    }
    return RED_GHOST_TEXTURE_UP; // Default case
}

sf::Vector2i getPinkGhostTexture(Ghost& ghost) {
    if(ghost.direction == DOWN) {
        return PINK_GHOST_TEXTURE_DOWN;
    }else if(ghost.direction == LEFT) {
        return PINK_GHOST_TEXTURE_LEFT;
    }else if(ghost.direction == RIGHT) {
        return PINK_GHOST_TEXTURE_RIGHT;
    }
    return PINK_GHOST_TEXTURE_UP; // Default case
}

sf::Vector2i getGreenGhostTexture(Ghost& ghost) {
    if(ghost.direction == DOWN) {
        return GREEN_GHOST_TEXTURE_DOWN;
    }else if(ghost.direction == LEFT) {
        return GREEN_GHOST_TEXTURE_LEFT;
    }else if(ghost.direction == RIGHT) {
        return GREEN_GHOST_TEXTURE_RIGHT;
    }
    return GREEN_GHOST_TEXTURE_UP; // Default case
}

sf::Vector2i getOrangeGhostTexture(Ghost& ghost) {
    if(ghost.direction == DOWN) {
        return ORANGE_GHOST_TEXTURE_DOWN;
    }else if(ghost.direction == LEFT) {
        return ORANGE_GHOST_TEXTURE_LEFT;
    }else if(ghost.direction == RIGHT) {
        return ORANGE_GHOST_TEXTURE_RIGHT;
    }
    return ORANGE_GHOST_TEXTURE_UP; // Default case
}

sf::Vector2i getFrightenedGhostTexture(Ghost& ghost) {
    if(ghost.direction == DOWN) {
        return FRIGHTENED_GHOST_TEXTURE_DOWN;
    }else if(ghost.direction == LEFT) {
        return FRIGHTENED_GHOST_TEXTURE_LEFT;
    }else if(ghost.direction == RIGHT) {
        return FRIGHTENED_GHOST_TEXTURE_RIGHT;
    }
    return FRIGHTENED_GHOST_TEXTURE_UP; // Default case
}

void animateGhost(Ghost& ghost) {
    if(ghost.mood == GhostMood::CHASE) {
        switch (ghost.type) {
            case GhostType::RED:
                ghost.sprite.setTextureRect(sf::IntRect(getRedGhostTexture(ghost), sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE)));
                break;
            case GhostType::PINK:
                ghost.sprite.setTextureRect(sf::IntRect(getPinkGhostTexture(ghost), sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE)));
                break;
            case GhostType::GREEN:
                ghost.sprite.setTextureRect(sf::IntRect(getGreenGhostTexture(ghost), sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE)));
                break;
            case GhostType::ORANGE:
                ghost.sprite.setTextureRect(sf::IntRect(getOrangeGhostTexture(ghost), sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE)));
                break;
        }
    }else if(ghost.mood == GhostMood::FRIGHTENED) {
        ghost.sprite.setTextureRect(sf::IntRect(getFrightenedGhostTexture(ghost), sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE)));
    }else if(ghost.mood == GhostMood::ORB) {
        ghost.sprite.setTextureRect(sf::IntRect(ORB_TEXTURE, sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE)));
    }
}

void animateEnemies(State& state) {
    animateGhost(state.ghosts.Red);
    animateGhost(state.ghosts.Pink);
    animateGhost(state.ghosts.Green);
    animateGhost(state.ghosts.Orange);
}

void animatePacmanDirection(State& state) {
    if (state.pacman.direction == DOWN) {
        state.pacmanTextureVector = PACMAN_TEXTURE_DOWN;
    } else if (state.pacman.direction == LEFT) {
        state.pacmanTextureVector = PACMAN_TEXTURE_LEFT;
    } else if (state.pacman.direction == UP) {
        state.pacmanTextureVector = PACMAN_TEXTURE_UP;
    } else if (state.pacman.direction == RIGHT) {
        state.pacmanTextureVector = PACMAN_TEXTURE_RIGHT;
    } else {
        state.pacmanTextureVector = PACMAN_TEXTURE_DOWN; // Default case
    }
}

void animateSuperPacmanDirection(State& state) {
    if (state.pacman.direction == DOWN) {
        state.pacmanTextureVector = SUPER_PACMAN_TEXTURE_DOWN;
    } else if (state.pacman.direction == LEFT) {
        state.pacmanTextureVector = SUPER_PACMAN_TEXTURE_LEFT;
    } else if (state.pacman.direction == UP) {
        state.pacmanTextureVector = SUPER_PACMAN_TEXTURE_UP;
    } else if (state.pacman.direction == RIGHT) {
        state.pacmanTextureVector = SUPER_PACMAN_TEXTURE_RIGHT;
    } else {
        state.pacmanTextureVector = SUPER_PACMAN_TEXTURE_DOWN; // Default case
    }
}

void animatePacmanMovement(State& state) {
    sf::Time elapsedTime = state.superClock.getElapsedTime();   //Using super because it resets less frequently than the clock
    int frameIndex = (elapsedTime.asMilliseconds() / PACMAN_ANIMATION_SPEED) % PACMAN_FRAMES; //Get the current frame index based on the elapsed time

    state.pacman.sprite.setTextureRect(sf::IntRect(state.pacmanTextureVector[frameIndex], sf::Vector2i(CHARACTER_SIZE, CHARACTER_SIZE)));
}




