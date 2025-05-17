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
    const float epsilon = 1.5f; //Float imprecision tollerance
    return std::fmod(position.x, GRID_TILE) < epsilon &&
           std::fmod(position.y, GRID_TILE) < epsilon;
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
                initializeDot(DOT_TEXTURE, sf::Vector2i(row, col), dots, texture);
            }else if(level[row][col] == SUPER_DOT) { 
                initializeDot(SUPERDOT_TEXTURE, sf::Vector2i(row, col), dots, texture);
            }
        }
    }
    return dots;
}

void spawnFruit(State& state) {
    state.fruitClock.restart();
    state.fruit.isActive = true;  
}

void checkFruitDespawn(State& state) {
    if(state.fruitClock.getElapsedTime().asSeconds() > FRUIT_TIME) {
        state.fruit.isActive = false;  
    }
}

//   ----   PACMAN   ----   //

void checkSuper(State& state) {
    if(state.isPacmanSuper) {
        if(state.superClock.getElapsedTime().asSeconds() > SUPER_TIME) {
            disableSuper(state);
        }
    }
}

void disableSuper(State& state) {
    state.isPacmanSuper = false; 
    state.ghosts.Red.setChase();
    state.ghosts.Pink.setChase();
    state.ghosts.Green.setChase();
    state.ghosts.Orange.setChase();
    state.soundManager.setMusic(MAIN_THEME);
    state.killStreak = 0;
}

void enableSuper(State& state) {
    state.isPacmanSuper = true; 
    state.ghosts.Red.setFrightened();
    state.ghosts.Pink.setFrightened();
    state.ghosts.Green.setFrightened();
    state.ghosts.Orange.setFrightened();
    state.soundManager.setMusic(CHASE_THEME);
    state.superClock.restart();
}

sf::Vector2f updatePacmanDirection(const Character& pacman) {
    if (canWalk(pacman.sprite.getPosition(), pacman.nextDirection)) {
        return pacman.nextDirection;
    } else {
        return NO_DIRECTION; //Block movement if there is a wall
    }
}

void damagePacman(State& state){
    if(state.lives > 1) {
        state.lives -= 1;
        state.respawn(); //Reset pacman and ghosts
    }else{
        setGameOver(state); 
    }
}

void managePacman(State& state, float deltaTime) {
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
}

//   ----   MENU   ----   //

void centerOrigin(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.size / 2.f);
}

void pauseGame(State& state) {
    state.gameState = GameState::PAUSE;
    state.clock.restart();
    state.pacman.velocity = NO_DIRECTION; 
}

void manageEnter(State& state) {
    int level = state.level;
    switch (state.gameState) {
        case GameState::MAIN_MENU:
            startGame(state);
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

void resumeGame(State& state) {
    state.gameState = GameState::PLAYING;
    state.clock.restart();
    state.pacman.velocity = NO_DIRECTION; 
}

void startGame(State& state) {
    int level = state.level;
    int score = state.score;
    state.resetGame();
    //Keep score and level
    state.level = level; 
    state.score = score; 
    levelSettings(state);
    state.gameState = GameState::PLAYING;
}

void setGameOver(State& state) {
    state.soundManager.playDeathSound();
    state.gameState = GameState::GAME_OVER;
    state.soundManager.setMusic(MENU_THEME); 
    saveHighScore(state.score);
}

void setGameWon(State& state) {
    state.gameState = GameState::GAME_WON;
    state.level += 1;
    state.soundManager.setMusic(MENU_THEME); 
    saveHighScore(state.score);
}

//    ---- INITIALIZATION ----    //

Character initializePacman(const sf::Texture& texture) {
    sf::Sprite pacmanSprite(texture);
    sf::IntRect pacmanRect(PACMAN_TEXTURE_DOWN1, CHARACTER_VECTOR);
    pacmanSprite.setTextureRect(pacmanRect);
    pacmanSprite.setScale(SCALE_VECTOR);
    pacmanSprite.setPosition(PACMAN_SPAWN);

    return Character(pacmanSprite);
}

sf::IntRect initializeGhostRect(GhostType ghostType) {
    switch (ghostType) {
        case GhostType::RED:
            return sf::IntRect(RED_GHOST_TEXTURE_DOWN, CHARACTER_VECTOR);
        case GhostType::PINK:
            return sf::IntRect(PINK_GHOST_TEXTURE_DOWN, CHARACTER_VECTOR);
        case GhostType::GREEN:
            return sf::IntRect(GREEN_GHOST_TEXTURE_DOWN, CHARACTER_VECTOR);
        case GhostType::ORANGE:
            return sf::IntRect(ORANGE_GHOST_TEXTURE_DOWN, CHARACTER_VECTOR);
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

void initializeDot(sf::Vector2i textureVector, sf::Vector2i position, std::vector<Pellet>& dots, const sf::Texture& mainTexture) {
    sf::Sprite dotSprite(mainTexture);
    sf::IntRect dotRect(textureVector, CHARACTER_VECTOR);
    dotSprite.setTextureRect(dotRect);
    dotSprite.setScale(SCALE_VECTOR);
    dotSprite.setPosition(sf::Vector2f(position.y * GRID_TILE, position.x * GRID_TILE));
    bool isSuper = (textureVector == SUPERDOT_TEXTURE);
    Pellet dot(dotSprite, isSuper);
    dots.push_back(dot);
}

void saveHighScore(int score) {
    if(loadHighScore() > score) return;

    const std::string filename = "highscore.txt";
    std::ofstream file(filename);
    if (file.is_open()) {
        file << score;
        file.close();
    }
}

int loadHighScore() {
    const std::string& filename = "highscore.txt";
    std::ifstream file(filename);
    int score = 0;
    if (file.is_open()) {
        file >> score;
        file.close();
    }
    return score;
}

void levelSettings(State &state){
    GHOST_SPEED = getGhostSpeed(state.level);
    SCARED_SPEED = GHOST_SPEED / 2;
    SUPER_TIME = getSuperTime(state.level); 
    state.fruit.sprite.setTextureRect(getFruitTextureRect(state.level));
}

float getGhostSpeed(int level) {
    if(level >= 1 && level <= 5){ //Reach 80 in level 5, gradually increasing speed
        return 70.0f + level * 2.0f;
    }
    if(level >5 && level <= 20){   //Reach 90 in level 20, gradually increasing speed
        return 80.0f + level * 0.5f;
    }
    return 91.0f;
}

float getSuperTime(int level) {
    //Supertime drecreases with the level, reaching 1 second in level 20
    //After 21 it stays at 0 seconds
    if(level > 21) return 0.0f;
    return SUPER_TIME - (level - 1) * 0.42f;
}

int getFruitPoints(int level) {
    if (level == 1 || level == 2) {
        return 100;
    } else if (level == 3 || level == 4 || level == 5) {
        return 300;
    } else if (level >= 6 && level <= 10) {
        return 1000;
    } else if (level >= 11 && level <= 15) {
        return 2500;
    } else if (level >= 16 && level <= 20) {
        return 5000;
    } else {
        return 10000; // For levels above 20
    }
}

sf::IntRect getFruitTextureRect(int level) {
    if (level == 1 || level == 2) {
        return sf::IntRect(CHERRY_TEXTURE, CHARACTER_VECTOR);
    } else if (level == 3 || level == 4 || level == 5) {
        return sf::IntRect(BANANA_TEXTURE, CHARACTER_VECTOR);
    } else if (level >= 6 && level <= 10) {
        return sf::IntRect(APPLE_TEXTURE, CHARACTER_VECTOR);
    } else if (level >= 11 && level <= 15) {
        return sf::IntRect(PINEAPPLE_TEXTURE, CHARACTER_VECTOR);
    } else if (level >= 16 && level <= 20) {
        return sf::IntRect(YELLOW_KEY_TEXTURE, CHARACTER_VECTOR);
    } else {
        return sf::IntRect(RED_POTION_TEXTURE, CHARACTER_VECTOR); // For levels above 20
    }
}

//    ----   COLLISIONS   ----   //

void checkCollisions(State& state){
    checkTunnel(state.pacman.sprite.getPosition(), state.pacman);

    manageDots(state);
    checkSuper(state);
    enemiesCollisions(state);
    fruitCollision(state);
    checkFruitDespawn(state);
}

void checkTunnel(const sf::Vector2f position, Character& character) {
    sf::Vector2i nextPosition = getGridCoords(getNextPosition(position, character.direction));

    if ( nextPosition.y == 9 && nextPosition.x == -1) { //Left tunnel
        manageTunnel(character, TunnelSide::LEFT);
    } else if (nextPosition.y == 9 && nextPosition.x == 21) { //Right tunnel
        manageTunnel(character, TunnelSide::RIGHT);
    }
}

void manageGhostCollision(Ghost& ghost, State& state) {
    if(doesGhostCollide(ghost, state.pacman)) {
        if(ghost.mood == GhostMood::CHASE){
            damagePacman(state);
        }else if(ghost.mood == GhostMood::FRIGHTENED) {
            killGhost(ghost, state);
        }
    }
}

void killGhost(Ghost& ghost, State& state) {
    state.soundManager.playWooshSound();
    ghost.mood = GhostMood::ORB; 
    ghost.sprite.setTextureRect(sf::IntRect(ORB_TEXTURE, CHARACTER_VECTOR));
    state.score += GHOST_SCORE * pow(2, state.killStreak);
    state.killStreak += 1;
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

void manageDots(State& state) {
    sf::Vector2i pacmanCoords = getGridCoords(state.pacman.sprite.getPosition());
    for(int i=0; i<state.dots.size(); ++i) {
        if (doCollide(pacmanCoords, state.dots[i].sprite)) {
            if(state.dots[i].isSuper) {
                enableSuper(state); 
                state.soundManager.playSuperDotSound();
                state.score += SUPER_DOT_SCORE;
            }else{
                state.soundManager.playEatingSound();
                state.score += DOT_SCORE;
            }
            state.dots.erase(state.dots.begin() + i); 
            if(state.dots.empty()) {
                setGameWon(state);
            }else if(state.dots.size() == 80 || state.dots.size() == 50){
                spawnFruit(state);
            }
            break; //To avoid deleting more than one dot at a time
        }
    }
}

void fruitCollision(State& state) {
    if (state.fruit.isActive && doCollide(state.pacman.sprite, state.fruit.sprite)) {
        state.soundManager.playEatingSound();
        state.fruit.isActive = false;
        state.score += getFruitPoints(state.level);
    }
}

//   ----   GHOSTS  ----   //

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
    if(state.dots.size() > 120){    //Orange starts after the player has eaten 30 dots
        manageGhost(state.ghosts.Orange, getGhostSpawn(state.ghosts.Orange.type), deltaTime) ; 
    }else{
        manageGhost(state.ghosts.Orange, target, deltaTime);
    }
    
    //Green tries to flank the player coordinating with red
    target = getGreenTarget(state.pacman.sprite.getPosition(), state.pacman.direction, state.ghosts.Red.sprite.getPosition(), state.ghosts.Green);
    if(state.dots.size() > 90){     //Green starts after the player has eaten 60 dots
        manageGhost(state.ghosts.Green, getGhostSpawn(state.ghosts.Green.type), deltaTime); 
    } else{
        manageGhost(state.ghosts.Green, target, deltaTime);
    }

    animateEnemies(state);
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
            ghost.direction = NO_DIRECTION; // Stop moving
            moveGhost(ghost, getGhostSpawn(ghost.type), deltaTime); // Move home
            respawnGhost(ghost); //Reset ghosts if they are in the spawn point
            break;
    }
}

void moveGhost(Ghost& ghost, const sf::Vector2f& target, float deltaTime) {
    if(isAlignedToGrid(ghost.sprite.getPosition())) {
        ghost.direction = getNextStepFromMap(ghost, bfsDistanceMap(getGridCoords(target)));
    }else{
        ghost.direction = ghost.lastDirection;
    }

    checkTunnel(ghost.sprite.getPosition(), ghost); 
    ghost.sprite.move(ghost.direction * GHOST_SPEED * deltaTime); 
    ghost.lastDirection = ghost.direction; 
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

//   ---   PATHFIDING   ---   //

sf::Vector2f getNextStepFromMap(const Ghost& ghost, const std::vector<std::vector<int>>& distMap) {
    sf::Vector2i bestDir = {0, 0};
    int bestDist = INT_MAX;

    std::vector<sf::Vector2i> directions = getPossibileDirectionsInt(ghost); 

    for (auto dir : directions) {
        sf::Vector2i next = getGridCoords(ghost.sprite.getPosition()) + dir;
    
        if (next.y == 9) {  // Tunnel handling
            if (next.x == -1 && dir == sf::Vector2i{-1, 0}) next = {21, 9};
            if (next.x == 21 && dir == sf::Vector2i{ 1, 0}) next = {0, 9};
        }
    
        //Border handling
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

std::vector<sf::Vector2f> getPossibleDirections(const Ghost& ghost) {
    if (ghost.direction == UP) {
        return {UP, LEFT, RIGHT};
    } else if (ghost.direction == DOWN) {
        return {DOWN, LEFT, RIGHT};
    } else if (ghost.direction == LEFT) {
        return {LEFT, UP, DOWN};
    } else if (ghost.direction == RIGHT) {
        return {RIGHT, UP, DOWN};
    } else {
        return {UP, DOWN, LEFT, RIGHT}; // No last direction
    }
}

std::vector<sf::Vector2i> getPossibileDirectionsInt(const Ghost& ghost) {
    if (ghost.direction == UP) { 
        return {UP_INT, LEFT_INT, RIGHT_INT};
    } else if (ghost.direction == DOWN) { 
        return {DOWN_INT, LEFT_INT, RIGHT_INT};
    } else if (ghost.direction == LEFT) { 
        return {LEFT_INT, UP_INT, DOWN_INT};
    } else if (ghost.direction == RIGHT) { 
        return {RIGHT_INT, UP_INT, DOWN_INT};
    } else {
        return {UP_INT, DOWN_INT, LEFT_INT, RIGHT_INT}; // No last direction
    }
}

std::vector<std::vector<int>> bfsDistanceMap(sf::Vector2i goal) {
    std::vector<std::vector<int>> dist(MAP_HEIGHT, std::vector<int>(MAP_WIDTH, -1));
    std::queue<sf::Vector2i> q;

    q.push(goal);
    dist[goal.y][goal.x] = 0;

    std::vector<sf::Vector2i> directions = {
        UP_INT, DOWN_INT, LEFT_INT, RIGHT_INT
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

float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

int manhattanDistance(const sf::Vector2i& a, const sf::Vector2i& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
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
                ghost.sprite.setTextureRect(sf::IntRect(getRedGhostTexture(ghost), CHARACTER_VECTOR));
                break;
            case GhostType::PINK:
                ghost.sprite.setTextureRect(sf::IntRect(getPinkGhostTexture(ghost), CHARACTER_VECTOR));
                break;
            case GhostType::GREEN:
                ghost.sprite.setTextureRect(sf::IntRect(getGreenGhostTexture(ghost), CHARACTER_VECTOR));
                break;
            case GhostType::ORANGE:
                ghost.sprite.setTextureRect(sf::IntRect(getOrangeGhostTexture(ghost), CHARACTER_VECTOR));
                break;
        }
    }else if(ghost.mood == GhostMood::FRIGHTENED) {
        ghost.sprite.setTextureRect(sf::IntRect(getFrightenedGhostTexture(ghost), CHARACTER_VECTOR));
    }else if(ghost.mood == GhostMood::ORB) {
        ghost.sprite.setTextureRect(sf::IntRect(ORB_TEXTURE, CHARACTER_VECTOR));
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

    state.pacman.sprite.setTextureRect(sf::IntRect(state.pacmanTextureVector[frameIndex], CHARACTER_VECTOR));
}


//  ----   DRAWING   ----   //

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

//Function to draw the GameOver, GameWon and Pause menu
void drawMenu(sf::RenderWindow& window, State& state) {
    sf::Text title{state.font, "", 18};
    sf::Text restart{state.font, "Press R to restart", 15};

    switch (state.gameState) {
        case GameState::MAIN_MENU:{
            title.setString("My Pacman Game");
            restart.setString("Press Enter to start!");

            drawMainMenu(window, state);
            break;
        }

        case GameState::PAUSE:
            title.setString("Paused");
            restart.setString("Press Enter to continue");
            break;
            
        case GameState::GAME_OVER:
            title.setString("Game Over");

            drawThanks(window, state);
            drawMenuHighscore(window, state);
            break;

        case GameState::GAME_WON:
            title.setString("You won!");

            drawThanks(window, state);
            drawNewLevel(window, state);
            drawMenuHighscore(window, state);
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

void drawMainMenu(sf::RenderWindow& window, State& state) {
    sf::Text controls1{state.font, "Use WASD or arrow keys to move", 12};
    centerOrigin(controls1);
    controls1.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 80.f));
    window.draw(controls1);

    sf::Text controls2{state.font, "Press P to Pause, R to reset", 12};
    centerOrigin(controls2);
    controls2.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 100.f));
    window.draw(controls2);

    sf::Text credits{state.font, "By: Leonardo Necordi", 12};
    centerOrigin(credits);
    credits.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 180.f));
    window.draw(credits);
}

void drawMenuHighscore(sf::RenderWindow& window, State& state) {
    sf::Text highScoreText{state.font, "High Score:", 18};
    sf::Text highScoreValue{state.font, std::to_string(loadHighScore()), 18};
    centerOrigin(highScoreText);
    centerOrigin(highScoreValue);
    highScoreText.setPosition(HIGHSCORE_TEXT_MENUPOSITION);
    highScoreValue.setPosition(HIGHSCORE_MENUPOSITION);

    window.draw(highScoreText);
    window.draw(highScoreValue);
}

void drawThanks(sf::RenderWindow& window, State& state) {
    sf::Text thanksText{state.font, "Thanks for playing!", 12};
    centerOrigin(thanksText);
    thanksText.setPosition(THANKS_POSITION);
    window.draw(thanksText);
}

void drawNewLevel(sf::RenderWindow& window, State& state) {
    sf::Text newLevelText{state.font, "Press Enter to go to level " + std::to_string(state.level), 15};
    centerOrigin(newLevelText);
    newLevelText.setPosition(NEWLEVEL_POSITION);
    window.draw(newLevelText);
}

void drawScore(sf::RenderWindow& window, const State& state) {
    sf::Text scoreText{state.font, "Score:" + std::to_string(state.score), 9};
    scoreText.setPosition(SCORE_POSITION);
    window.draw(scoreText);

    sf::RectangleShape scoreLine;
    scoreLine.setSize(sf::Vector2f(WINDOW_WIDTH, 2)); 
    scoreLine.setPosition(sf::Vector2f(0, WINDOW_HEIGHT - SCOREBOARD_HEIGHT + 5));
    scoreLine.setFillColor(sf::Color::White);
    window.draw(scoreLine);

    sf::Text highScoreText{state.font, "High Score: " + std::to_string(loadHighScore()), 9};
    highScoreText.setPosition(HIGHSCORE_POSITION);
    window.draw(highScoreText);
}

void drawLives(sf::RenderWindow& window, const State& state) {
    sf::Text livesText{state.font, "Lives: ",9};
    livesText.setPosition(LIFETEXT_POSITION);
    window.draw(livesText);

    sf::Sprite lifeSprite(state.mainTexture);
    sf::IntRect lifeRect(PACMAN_TEXTURE_DOWN1, CHARACTER_VECTOR);
    for(int i = 0; i < state.lives; ++i) {
        lifeSprite.setTextureRect(lifeRect);
        lifeSprite.setScale(sf::Vector2f(LIFE_SCALE, LIFE_SCALE));
        lifeSprite.setPosition(sf::Vector2f(LIFE_POSITION.x + i * LIFE_OFFSET, LIFE_POSITION.y));
        window.draw(lifeSprite);
    }
}

void drawLevel(sf::RenderWindow& window, const State& state) {
    sf::Text levelText{state.font, "Level: " + std::to_string(state.level), 12};
    centerOrigin(levelText);
    levelText.setPosition(LEVEL_POSITION);
    window.draw(levelText);
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
    drawLevel(window, state);
    drawScore(window, state);
    drawLives(window, state);
    if(state.fruit.isActive) window.draw(state.fruit.sprite);
}