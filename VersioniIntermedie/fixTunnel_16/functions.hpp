#ifndef SFML_INCLUDES
#define SFML_INCLUDES
#endif

#include "gameStructures_fwd.hpp"

//      ---    GRID AND MAP  ---    //
sf::Vector2i getGridCoords(const sf::Vector2f& pos);

bool isAlignedToGrid(const sf::Vector2f& position);

void checkTunnel(const sf::Vector2f nextPosition, Character& character);

void manageTunnel(Character& character, TunnelSide tunnelSide);

sf::Vector2f getNextPosition(const sf::Vector2f& position, const sf::Vector2f& direction);

bool canWalk(sf::Vector2f position, const sf::Vector2f& direction);

std::vector<Pellet> placeDots(const sf::Texture& texture);

void spawnFruit(State& state);

void checkFruitDespawn(State& state);

//   ----   PACMAN   ----   //

void checkSuper(State& state);

void disableSuper(State& state);

sf::Vector2f updatePacmanDirection(const Character& pacman);

void damagePacman(State& state);

void managePacman(State& state, float deltaTime);

//   ----   MENU   ----   //

void centerOrigin(sf::Text& text);

void pauseGame(State& state);

void resumeGame(State& state);

void manageEnter(State& state);

void setGameOver(State& state);

void setGameWon(State& state);

//    ---- INITIALIZATION ----    //

Character initializePacman(const sf::Texture& texture);

sf::IntRect initializeGhostRect(GhostType ghostType);

sf::Vector2f initializeGhostSpawn(GhostType ghostType);

void initializeDot(sf::Vector2i textureVector, sf::Vector2i position, std::vector<Pellet>& dots, const sf::Texture& mainTexture);

void saveHighScore(int score);

int loadHighScore();

//    ----   COLLISIONS   ----   //

void checkCollisions(State& state);

void manageGhostCollision(Ghost& ghost, State& state);

void enemiesCollisions(State& state);

bool doCollide(const sf::Sprite& sprite1, const sf::Sprite& sprite2);

bool doCollide(const sf::Vector2i& pacmanCoords, const sf::Sprite& collidingSprite);

bool doesGhostCollide(const Ghost& ghost, const Character& pacman);

void manageDots(State& state, const sf::Vector2i& pacmanCoords);

void fruitCollision(State& state);

//   ----   GHOSTS  ----   //

std::vector<sf::Vector2f> getPossibleDirections(const Ghost& ghost);

void moveGhost(Ghost& ghost, const sf::Vector2f& target, float deltaTime);

sf::Vector2f getAlternateTarget(const sf::Vector2f& pacmanPos, const sf::Vector2f& pacmanDirection, const Ghost& ghost);

sf::Vector2f getPinkTarget(const sf::Vector2f& pacmanPos, const sf::Vector2f& pacmanDirection, const Ghost& pinkGhost);

sf::Vector2f getOrangeTarget(const sf::Vector2f& pacmanPos, const sf::Vector2f& orangePos);

sf::Vector2f getGreenTarget(const sf::Vector2f& pacmanPos, const sf::Vector2f& pacmanDir, const sf::Vector2f& blinkyPos, const Ghost& greenGhost);

sf::Vector2f getGhostSpawn(GhostType ghostType);

void respawnGhost(Ghost& ghost);

void manageGhost(State& state, float deltaTime);

void manageEnemies(State& state, float deltaTime);

void moveScaredGhost(Ghost& ghost, float deltaTime);

sf::Vector2f chooseScaredDirection(Ghost& ghost);

//   ---   PATHFIDING   ---   //

float distance(const sf::Vector2f& a, const sf::Vector2f& b);

int manhattanDistance(const sf::Vector2i& a, const sf::Vector2i& b);

std::vector<std::vector<int>> bfsDistanceMap(sf::Vector2i goal);

sf::Vector2f getNextStepFromMap(const Ghost& ghost, const std::vector<std::vector<int>>& distMap);

bool isTargetValid(const sf::Vector2i& target);

//   ----   ANIMATIONS   ----   //

sf::Vector2i getRedGhostTexture(Ghost& ghost);

sf::Vector2i getPinkGhostTexture(Ghost& ghost);

sf::Vector2i getGreenGhostTexture(Ghost& ghost);

sf::Vector2i getOrangeGhostTexture(Ghost& ghost);

sf::Vector2i getFrightenedGhostTexture(Ghost& ghost);

void animateGhost(Ghost& ghost);

void animateEnemies(State& state);

void animateSuperPacmanDirection(State& state);

void animatePacmanDirection(State& state);

void animatePacmanMovement(State& state);

//   ----   DRAWING   ----   //

void drawMap(sf::RenderWindow& window);

void drawMenu(sf::RenderWindow& window, State& state);

void drawMainMenu(sf::RenderWindow& window, State& state);

void drawMenuHighscore(sf::RenderWindow& window, State& state);

void drawScore(sf::RenderWindow& window, const State& state);

void drawLives(sf::RenderWindow& window, const State& state);

void drawAll(sf::RenderWindow& window, State& state);