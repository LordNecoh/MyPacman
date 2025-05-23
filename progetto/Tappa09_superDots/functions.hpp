#ifndef SFML_INCLUDES
#define SFML_INCLUDES
#endif

#include "gameStructures_fwd.hpp"

//      ---    GRID AND MAP  ---    //
sf::Vector2i getGridCoords(const sf::Vector2f& pos);

bool isAlignedToGrid(const sf::Vector2f& position);

void drawMap(sf::RenderWindow& window);

void checkTunnel(const sf::Vector2f nextPosition, Character& character);

void manageTunnel(Character& character, TunnelSide tunnelSide);

sf::Vector2f getNextPosition(const sf::Vector2f& position, const sf::Vector2f& direction);

bool canWalk(sf::Vector2f position, const sf::Vector2f& direction);

std::vector<Pellet> placeDots(const sf::Texture& texture);

void manageDots(State& state, const sf::Vector2i& pacmanCoords);

//   ----   MENU   ----   //

void centerOrigin(sf::Text& text);

void drawMenu(sf::RenderWindow& window, State& state);

void resetGame(State& state, const sf::Texture& texture);

void pauseGame(State& state);

void resumeGame(State& state);

void manageEnter(State& state);

//    ---- INITIALIZATION ----    //

Character initializePacman(const sf::Texture& texture);

sf::IntRect initializeGhostRect(GhostType ghostType);

sf::Vector2f initializeGhostSpawn(GhostType ghostType);

void drawAll(sf::RenderWindow& window, State& state);

//    ----   COLLISIONS   ----   //

void checkCollisions(State& state);

void manageGhostCollision(Ghost& ghost, State& state);

void enemiesCollisions(State& state);

bool doCollide(const sf::Sprite& sprite1, const sf::Sprite& sprite2);

bool doCollide(const sf::Vector2i& pacmanCoords, const sf::Sprite& collidingSprite);

bool doesGhostCollide(const Ghost& ghost, const Character& pacman);

//   ----   GHOSTS  ----   //

std::vector<sf::Vector2f> getPossibleDirections(const Ghost& ghost);

//Deprecated function, not used anymore
sf::Vector2f getGhostDirection(Ghost& ghost, const sf::Vector2f& target);

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

sf::Vector2f getNextStepFromMap(sf::Vector2i ghostPos, const std::vector<std::vector<int>>& distMap);

bool isTargetValid(const sf::Vector2i& target);