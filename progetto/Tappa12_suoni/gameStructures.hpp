//Pacman classical map, handmade:
//Here, height is 21 and WIDTH is 21
int level[MAP_HEIGHT][MAP_WIDTH] = {
    // 0 = empty, 1 = wall, 2 = dot, 3 = super dot, 4 = ghost door, 5 = unreachable tile
    //150 total dots
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5},
    {5,1,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,1,5},
    {5,1,3,1,1,2,1,1,1,2,1,2,1,1,1,2,1,1,3,1,5},
    {5,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,5},
    {5,1,2,1,1,2,1,2,1,1,1,1,1,2,1,2,1,1,2,1,5},
    {5,1,2,2,2,2,1,2,2,2,1,2,2,2,1,2,2,2,2,1,5},
    {5,1,1,1,1,2,1,1,1,0,1,0,1,1,1,2,1,1,1,1,5},
    {5,5,5,5,1,2,1,0,0,0,0,0,0,0,1,2,1,5,5,5,5},
    {1,1,1,1,1,2,1,0,1,1,4,1,1,0,1,2,1,1,1,1,1},
    {5,5,5,5,5,2,0,0,1,0,0,0,1,0,0,2,5,5,5,5,5},
    {1,1,1,1,1,2,1,0,1,1,1,1,1,0,1,2,1,1,1,1,1},
    {5,5,5,5,1,2,1,0,0,0,0,0,0,0,1,2,1,5,5,5,5},
    {5,1,1,1,1,2,1,0,1,1,1,1,1,0,1,2,1,1,1,1,5},
    {5,1,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,1,5},
    {5,1,2,1,1,2,1,1,1,2,1,2,1,1,1,2,1,1,2,1,5},
    {5,1,3,2,1,2,2,2,2,2,0,2,2,2,2,2,1,2,3,1,5},
    {5,1,1,2,1,2,1,2,1,1,1,1,1,2,1,2,1,2,1,1,5},
    {5,1,2,2,2,2,1,2,2,2,1,2,2,2,1,2,2,2,2,1,5},
    {5,1,2,1,1,1,1,1,1,2,1,2,1,1,1,1,1,1,2,1,5},
    {5,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,5},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5} 
};

enum class TunnelSide{
    LEFT,
    RIGHT 
};

enum TileType {
    EMPTY = 0,
    WALL = 1,
    DOT = 2,
    SUPER_DOT = 3,
    GHOST_DOOR = 4,
    UNREACHABLE = 5
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

enum class GhostMood{
    CHASE,
    FRIGHTENED,
    ORB
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

    Character(const sf::Sprite& sprite) : sprite(sprite) {
        direction = sf::Vector2f(0.f, 0.f);
        nextDirection = sf::Vector2f(0.f, 0.f);
        velocity = sf::Vector2f(0.f, 0.f);
    }

    virtual ~Character() = default; 
};

struct Ghost : public Character {
    GhostType type;
    GhostMood mood = GhostMood::CHASE;
    sf::Vector2i target = {0, 0}; //Target position for the ghost to move towards
    sf::Vector2f lastDirection = RIGHT;

    Ghost(const sf::Texture & texture, GhostType type) : Character(sf::Sprite(texture)), type(type) {
        sprite.setTextureRect(initializeGhostRect(type));
        sprite.setScale(sf::Vector2f(SPRITES_SCALE, SPRITES_SCALE));
        sprite.setPosition(initializeGhostSpawn(type));
    }

    int setFrightened() {
        if(mood != GhostMood::CHASE) return 1;
        mood = GhostMood::FRIGHTENED;
        return 0;
    }

    int setChase() {
        if(mood != GhostMood::FRIGHTENED) return 1;
        mood = GhostMood::CHASE;
        return 0;
    }
};

struct Enemies{
    Ghost Red;
    Ghost Pink;
    Ghost Green;
    Ghost Orange;

    Enemies(const Ghost& red, const Ghost& pink, const Ghost& green, const Ghost& orange)
        : Red(red), Pink(pink), Green(green), Orange(orange) {}

    Enemies(const sf::Texture& texture)
        : Red(texture, GhostType::RED), Pink(texture, GhostType::PINK), 
        Green(texture, GhostType::GREEN), Orange(texture, GhostType::ORANGE) {}
};

struct SoundManager{
    sf::Music activeMusic;
    sf::SoundBuffer eatingBuffer;
    sf::Sound eatingSound;
    sf::SoundBuffer superDotBuffer;
    sf::Sound superDotSound;
    sf::SoundBuffer wooshBuffer;
    sf::Sound wooshSound;
    sf::SoundBuffer deathBuffer;
    sf::Sound deathSound;

    SoundManager(const std::string& soundPath):
        eatingBuffer(soundPath), eatingSound(eatingBuffer), 
        superDotBuffer(SUPERDOT_SOUND), superDotSound(superDotBuffer),
        wooshBuffer(WOOSH_SOUND), wooshSound(wooshBuffer),
        deathBuffer(DEATH_SOUND), deathSound(deathBuffer) {
        eatingSound.setVolume(45.f); 
        superDotSound.setVolume(50.f); 
        wooshSound.setVolume(100.f); 
        deathSound.setVolume(45.f);
    } 
};

struct State {
    Character pacman;
    Enemies ghosts;
    sf::Clock clock;
    sf::Clock superClock;
    std::vector<Pellet> dots;
    GameState gameState = GameState::MAIN_MENU;
    const sf::Font font{"progetto/RisorseProgetto/Emulogic.ttf"}; 
    sf::Texture mainTexture; 
    bool isPacmanSuper = false; //True if pacman has eaten a super dot
    std::vector<sf::Vector2i> pacmanTextureVector = PACMAN_TEXTURE_DOWN; 
    SoundManager soundManager;

    State(const Character& pacman, const Enemies& ghosts) 
        : pacman(pacman), ghosts(ghosts), soundManager(EATING_SOUND) {}

    State(const sf::Texture& texture) 
        : mainTexture(texture), 
        pacman(initializePacman(texture)),
        ghosts(texture),
        dots(placeDots(texture)),
        soundManager(EATING_SOUND) {}
};

