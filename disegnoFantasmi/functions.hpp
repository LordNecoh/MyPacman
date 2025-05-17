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

void createMap(sf::RenderWindow& window);

void manageTunnel(Character& character);

bool canWalk(Character& character, const sf::Vector2f& direction);

void placeDots(State& state, const sf::Texture& texture);

void manageDots(State& state);

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

Character initializeGhost(const sf::Texture& texture, GhostType type);

Enemies initializeEnemies(const sf::Texture& texture);