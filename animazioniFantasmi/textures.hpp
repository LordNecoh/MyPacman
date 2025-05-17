#ifndef SFML_INCLUDES
#define SFML_INCLUDES
#endif

//This file contains the texture coordinates for the sprites in the texture sheet

const sf::Vector2i PACMAN_TEXTURE(TILE_SIZE * 6 + 9, TILE_SIZE*4+ 7); // 8th column (index 7), 5th row (index 4)
const sf::Vector2i DOT_TEXTURE(TILE_SIZE * 13 -4, +2); //13th cloumn ,1st row
const sf::Vector2i SUPERDOT_TEXTURE(TILE_SIZE * 14 -4, + 2); //13th cloumn ,2nd row

//    ---  CHASING GHOSTS   ---    //
//Credo per allineare sia: y + (nriga + 4) , x -(ncolonna +4)

const sf::Vector2i RED_GHOST_TEXTURE_LEFT(TILE_SIZE * 9-9, TILE_SIZE * 2+5);
const sf::Vector2i RED_GHOST_TEXTURE_RIGHT(TILE_SIZE * 10-9, TILE_SIZE * 2+5); 
const sf::Vector2i RED_GHOST_TEXTURE_DOWN(TILE_SIZE * 11-7, TILE_SIZE * 2+5); 
const sf::Vector2i RED_GHOST_TEXTURE_UP(TILE_SIZE * 12-7, TILE_SIZE * 2+5);

const sf::Vector2i PINK_GHOST_TEXTURE_LEFT(TILE_SIZE * 5 - 13, TILE_SIZE +4);
const sf::Vector2i PINK_GHOST_TEXTURE_RIGHT(TILE_SIZE * 6 - 13, TILE_SIZE +4);
const sf::Vector2i PINK_GHOST_TEXTURE_DOWN(TILE_SIZE * 7 - 11, TILE_SIZE +4);
const sf::Vector2i PINK_GHOST_TEXTURE_UP(TILE_SIZE * 8 - 11, TILE_SIZE +4);

const sf::Vector2i GREEN_GHOST_TEXTURE_LEFT(3, TILE_SIZE * 2+5); 
const sf::Vector2i GREEN_GHOST_TEXTURE_RIGHT(TILE_SIZE + 3, TILE_SIZE * 2+5); 
const sf::Vector2i GREEN_GHOST_TEXTURE_DOWN(TILE_SIZE * 2+5, TILE_SIZE * 2+5); 
const sf::Vector2i GREEN_GHOST_TEXTURE_UP(TILE_SIZE * 3+5, TILE_SIZE * 2+5); 

const sf::Vector2i ORANGE_GHOST_TEXTURE_LEFT(TILE_SIZE * 5-13, TILE_SIZE * 2+5);
const sf::Vector2i ORANGE_GHOST_TEXTURE_RIGHT(TILE_SIZE * 6-13, TILE_SIZE * 2+5);
const sf::Vector2i ORANGE_GHOST_TEXTURE_DOWN(TILE_SIZE * 7-11, TILE_SIZE * 2+5);
const sf::Vector2i ORANGE_GHOST_TEXTURE_UP(TILE_SIZE * 8-11, TILE_SIZE * 2+5);

//   ---  FRIGHTENED GHOSTS   ---    //
const sf::Vector2i FRIGHTENED_GHOST_TEXTURE_LEFT(3,TILE_SIZE +4);
const sf::Vector2i FRIGHTENED_GHOST_TEXTURE_RIGHT(TILE_SIZE + 3,TILE_SIZE +4);
const sf::Vector2i FRIGHTENED_GHOST_TEXTURE_DOWN(TILE_SIZE * 2 + 5,TILE_SIZE +4); 
const sf::Vector2i FRIGHTENED_GHOST_TEXTURE_UP(TILE_SIZE * 3 + 5,TILE_SIZE +4);



const sf::Vector2i ORB_TEXTURE(TILE_SIZE * 9 -10, +2);
