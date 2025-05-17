#ifndef SFML_INCLUDES
#define SFML_INCLUDES

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <vector>
#include <iostream>

#endif

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

