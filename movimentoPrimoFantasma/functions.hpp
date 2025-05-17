#ifndef SFML_INCLUDES
#define SFML_INCLUDES

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <vector>
#include <iostream>

#endif
#include "gameStructures_fwd.hpp"

sf::Vector2i getGridCoords(const sf::Vector2f& pos);

bool isAlignedToGrid(const sf::Vector2f& position);

void drawMap(sf::RenderWindow& window);

void checkTunnel(const sf::Vector2f nextPosition, Character& character);

void manageTunnel(Character& character);

sf::Vector2f getNextPosition(const sf::Vector2f& position, const sf::Vector2f& direction);

bool canWalk(sf::Vector2f position, const sf::Vector2f& direction);

std::vector<Pellet> placeDots(const sf::Texture& texture);

void manageDots(State& state, const sf::Vector2i& pacmanCoords);

void checkCollisions(State& state);

void centerOrigin(sf::Text& text);

void drawMenu(sf::RenderWindow& window, State& state);

void resetGame(State& state, const sf::Texture& texture);

void pauseGame(State& state);

void resumeGame(State& state);

void manageEnter(State& state);

Character initializePacman(const sf::Texture& texture);

sf::IntRect initializeGhostRect(GhostType ghostType);

sf::Vector2f initializeGhostSpawn(GhostType ghostType);

void drawAll(sf::RenderWindow& window, State& state);

void ghostsCollisions(State& state, const sf::Vector2i& pacmanCoords);

bool doCollide(const sf::Sprite& sprite1, const sf::Sprite& sprite2);

bool doCollide(const sf::Vector2i& pacmanCoords, const sf::Sprite& collidingSprite);

int manhattanDistance(const sf::Vector2i& a, const sf::Vector2i& b);

std::vector<sf::Vector2f> getPossibleDirections(const Ghost& ghost);

void moveGhost(Ghost& ghost, const sf::Vector2f& target, float deltaTime);