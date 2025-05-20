#ifndef SFML_INCLUDES
#define SFML_INCLUDES

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <vector>
#include <iostream>

#endif

void handle_close(sf::RenderWindow& window) {
    window.close();
}

void handle_resize(const sf::Event::Resized& resized, sf::RenderWindow& window) {
    // Constrain aspect ratio and map always the same portion of the world
    float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
    sf::Vector2u ws = resized.size;
    float new_aspect = static_cast<float>(ws.x) / static_cast<float>(ws.y);
    if (new_aspect < aspect)
        ws = {ws.x, static_cast<unsigned>(ws.x / aspect)};
    else
        ws = {static_cast<unsigned>(ws.y * aspect), ws.y};
    window.setSize(ws);
}

template <typename T>
void handle(T& event, State& state) {}

void handle(const sf::Event::KeyPressed& key, State& state) {
    if(state.gameState == GameState::PLAYING){
        switch (key.scancode) {
            case sf::Keyboard::Scancode::W:
                state.pacman.nextDirection = sf::Vector2f(0.f, -1.f);
                break;
            case sf::Keyboard::Scancode::S:
                state.pacman.nextDirection = sf::Vector2f(0.f, 1.f);
                break;
            case sf::Keyboard::Scancode::A:
                state.pacman.nextDirection = sf::Vector2f(-1.f, 0.f);
                break;
            case sf::Keyboard::Scancode::D:
                state.pacman.nextDirection = sf::Vector2f(1.f, 0.f);
                break;
            default:
                break;
        }
    }else{
        switch (key.scancode) {
            case sf::Keyboard::Scancode::R:
                state.gameState = GameState::PLAYING;
                resetGame(state, state.mainTexture);
                break;
            default:
                break;
        }
    }
    
}
