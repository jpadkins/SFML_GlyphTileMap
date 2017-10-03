# GlyphTileMap

An SFML-based drawable/transformable class for displaying a grid of characters
with colored backgrounds and several spacing options. Useful for roguelikes or
similar applications.

## Installation

Just copy `src/GlyphTileMap.h` and `src/GlyphTileMap.cpp` into your project.


Note that some C++11 features are used, so you'll need to compile with at
least that version of the standard or newer.

## Documentation

Doxygen documentation can be found inline in [src/GlyphTileMap.h](src/GlyphTileMap.h).

## Example

```cpp
...
sf::Font font;
font.loadFromFile("font.ttf");

///////////////////////////////////////////////////////////////////////////////
/// Create a GlyphTileMap with the following arguments:
///
/// font        The loaded sf::Font to use for sf::Glyph data
/// {40, 30}    sf::Vector2u containing the dimensions of the map (in tiles)
/// {16, 16}    sf::Vector2u containing the dimensions of each tile (in pixels)
/// 16          sf::Uint32 character size of the sf::Glyphs for each tile
///////////////////////////////////////////////////////////////////////////////
GlyphTileMap tileMap(font, {40, 30}, {16, 16}, 16);

///////////////////////////////////////////////////////////////////////////////
/// Create a GlyphTileMap::Tile, a struct containing all the data required to
/// populate a tile of a GlyphTileMap.
///
/// L'╬'                                wchar_t of the character to display
/// GlyphTileMap::Tile::Type::Exact     Spacing of the character within the
///                                     tile, either Text, Exact, Floor, or
///                                     Center
/// sf::Color::White                    Color of the tile's character
/// sf::Color::Black                    Color of the tile's background
/// {3, -4}                             Optional exact spacing of the tile's
///                                     character (relative to the center)
///////////////////////////////////////////////////////////////////////////////
GlyphTileMap::Tile tile(
    L'╬',
    GlyphTileMap::Tile::Type::Exact,
    sf::Color::White,
    sf::Color::Black,
    {3, -4}
    );

///////////////////////////////////////////////////////////////////////////////
/// Update a tile on the GlyphTileMap to values from the GlyphTileMap::Tile
///////////////////////////////////////////////////////////////////////////////
tileMap.setTile({0, 0}, tile);

///////////////////////////////////////////////////////////////////////////////
/// Tiles on the GlyphTileMap can be updated without a GlyphTileMap::Tile
///////////////////////////////////////////////////////////////////////////////
tileMap.setBackground({0, 1}, sf::Color::Blue);
tileMap.setCharacter({1, 0}, L'!', GlyphTileMap::Tile::Type::Center);

...

///////////////////////////////////////////////////////////////////////////////
/// Draw/Transform the GlyphTileMap like any other SFML drawable/transformable
///////////////////////////////////////////////////////////////////////////////
tileMap.setScale(0.5f, 0.5f);
tileMap.setPosition(100, 100);

window.clear();
window.draw(tileMap);
window.display();
...
```

#### A 50x36 grid of 16px random characters with random backgrounds and even spacing:
![example output](res/images/example0.png?raw=true)

#### An arbitrary number of layers can be added. Character size and spacing is adjustable:
![example output](res/images/example1.png?raw=true)

#### There are many options for spacing individual characters:
![example output](res/images/example2.png?raw=true)
