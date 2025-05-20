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
    PLAYING,
    GAME_OVER,
    GAME_WON
    //Maybe add pause later on
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

    Character(const sf::Sprite& sprite) : sprite(sprite) {
        direction = sf::Vector2f(0.f, 0.f);
        nextDirection = sf::Vector2f(0.f, 0.f);
        velocity = sf::Vector2f(0.f, 0.f);
    }
};

struct State {
    //sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Pacman Game");
    Character pacman;
    sf::Clock clock;
    std::vector<Pellet> dots;
    GameState gameState = GameState::GAME_WON;
    const sf::Font font{"progetto/RisorseProgetto/Emulogic.ttf"}; 
    sf::Texture mainTexture; 

    State(const sf::Sprite& pacmanSprite) 
        : pacman(pacmanSprite) {}
};