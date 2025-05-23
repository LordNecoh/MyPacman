//Pacman classical map, handmade:
//Here, height is 21 and WIDTH is 21
int level[MAP_HEIGHT][MAP_WIDTH] = {
    // 0 = empty, 1 = wall, 2 = dot, 3 = super dot, 4 = ghost door
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,1,0},
    {0,1,3,1,1,2,1,1,1,2,1,2,1,1,1,2,1,1,3,1,0},
    {0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,1,2,1,1,2,1,2,1,1,1,1,1,2,1,2,1,1,2,1,0},
    {0,1,2,2,2,2,1,2,2,2,1,2,2,2,1,2,2,2,2,1,0},
    {0,1,1,1,1,2,1,1,1,0,1,0,1,1,1,2,1,1,1,1,0},
    {0,0,0,0,1,2,1,0,0,0,0,0,0,0,1,2,1,0,0,0,0},
    {1,1,1,1,1,2,1,0,1,1,4,1,1,0,1,2,1,1,1,1,1},
    {0,0,0,0,0,2,0,0,1,0,0,0,1,0,0,2,0,0,0,0,0},
    {1,1,1,1,1,2,1,0,1,1,1,1,1,0,1,2,1,1,1,1,1},
    {0,0,0,0,1,2,1,0,0,0,0,0,0,0,1,2,1,0,0,0,0},
    {0,1,1,1,1,2,1,0,1,1,1,1,1,0,1,2,1,1,1,1,0},
    {0,1,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,1,0},
    {0,1,2,1,1,2,1,1,1,2,1,2,1,1,1,2,1,1,2,1,0},
    {0,1,3,2,1,2,2,2,2,2,0,2,2,2,2,2,1,2,3,1,0},
    {0,1,1,2,1,2,1,2,1,1,1,1,1,2,1,2,1,2,1,1,0},
    {0,1,2,2,2,2,1,2,2,2,1,2,2,2,1,2,2,2,2,1,0},
    {0,1,2,1,1,1,1,1,1,2,1,2,1,1,1,1,1,1,2,1,0},
    {0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0} 
};

enum TileType {
    EMPTY = 0,
    WALL = 1,
    DOT = 2,
    SUPER_DOT = 3,
    GHOST_DOOR = 4
};

enum class GameState{
    MAIN_MENU,
    PLAYING,
    PAUSE,
    GAME_OVER,
    GAME_WON
};

enum class GhostType{
    RED,
    PINK,
    GREEN,
    ORANGE
};

struct Pellet {
    sf::Sprite sprite;
    bool isSuper = false;
    Pellet(const sf::Sprite& sprite, bool isSuper) : sprite(sprite), isSuper(isSuper) {}
};

struct Character {
    sf::Vector2f direction;
    sf::Vector2f nextDirection; //Direction to be set when aligned to the grid
    sf::Vector2f velocity;
    sf::Sprite sprite;
    bool traversingTunnel = false; //For smoother movement in the tunnel
    bool isAlive = true; //For the ghosts

    Character(const sf::Sprite& sprite) : sprite(sprite) {
        direction = sf::Vector2f(0.f, 0.f);
        nextDirection = sf::Vector2f(0.f, 0.f);
        velocity = sf::Vector2f(0.f, 0.f);
    }
};

struct Enemies{
    Character Red;
    Character Pink;
    Character Green;
    Character Orange;

    Enemies(const sf::Sprite& redSprite, const sf::Sprite& pinkSprite, const sf::Sprite& greenSprite, const sf::Sprite& orangeSprite) 
        : Red(redSprite), Pink(pinkSprite), Green(greenSprite), Orange(orangeSprite) {}

    Enemies(const Character& red, const Character& pink, const Character& green, const Character& orange)
        : Red(red), Pink(pink), Green(green), Orange(orange) {}

};

struct State {
    Character pacman;
    Enemies ghosts;
    sf::Clock clock;
    std::vector<Pellet> dots;
    GameState gameState = GameState::MAIN_MENU;
    const sf::Font font{"progetto/RisorseProgetto/Emulogic.ttf"}; 
    sf::Texture mainTexture; 

    State(const sf::Sprite& pacmanSprite, const sf::Sprite& redSprite, const sf::Sprite& pinkSprite, const sf::Sprite& greenSprite, const sf::Sprite& orangeSprite) 
        : pacman(pacmanSprite), ghosts(redSprite, pinkSprite, greenSprite, orangeSprite) {
        // Initialize the ghosts with their respective sprites
    }

    State(const Character& pacman, const Enemies& ghosts) 
        : pacman(pacman), ghosts(ghosts) {}

    State(const sf::Texture& texture) 
        : mainTexture(texture), 
        pacman(initializePacman(texture)),
        ghosts(initializeEnemies(texture)) {}
};