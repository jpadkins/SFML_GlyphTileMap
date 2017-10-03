///////////////////////////////////////////////////////////////////////////////
/// Author:         Jacob P Adkins (github.com/jpadkins)
/// Filename:       GlyphTileMap.h
/// License:        MIT
/// Description:    An SFML-based drawable/transformable class for displaying
///                 a grid of characters with colored backgrounds and several
///                 spacing options.
///////////////////////////////////////////////////////////////////////////////

#ifndef GLYPH_TILE_MAP_H
#define GLYPH_TILE_MAP_H

///////////////////////////////////////////////////////////////////////////////
/// Headers
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <unordered_map>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class GlyphTileMap : public sf::Drawable, public sf::Transformable {
public:

    struct Tile {
        ///////////////////////////////////////////////////////////////////////
        /// Tile::Type is used to determine how a Tile's character should be
        /// placed within its bounds. The types are:
        ///
        /// Text:   Spacing is based on font kerning (e.g. characters like g
        ///         and y will intrude partially on the Tile below them)
        /// Exact:  Spacing is based on the Tile's offset values, which are
        ///         relative to the coords of Type::Center
        /// Floor:  The Tile's character is centered horizontally but aligned
        ///         vertically with the bottom of the tile
        /// Center: The Tile's character is centered both horizontally and
        ///         vertically
        ///////////////////////////////////////////////////////////////////////
        enum class Type { Text, Exact, Floor, Center };

        ///////////////////////////////////////////////////////////////////////
        /// \brief Default Tile constructor
        ///
        /// The default values of a GlyphTileMap::Tile are:
        ///
        /// character:  L' '
        /// type:       Type::Center
        /// foreground: sf::Color::White
        /// background: sf::Color::Black
        /// offset:     {0, 0}
        ///////////////////////////////////////////////////////////////////////
        Tile();

        ///////////////////////////////////////////////////////////////////////
        /// \brief Constructor for a Tile
        ///
        /// \param character    Character of the Tile
        /// \param type         Type of the Tile (determines spacing)
        /// \param foreground   Color of the Tile's character
        /// \param background   Color to display behind the Tile's character
        /// \param offset       Manual spacing offset values of the Tile
        ///////////////////////////////////////////////////////////////////////
        Tile(wchar_t character, Type type, const sf::Color& foreground,
            const sf::Color& background, const sf::Vector2i& offset = {0, 0});

        ///////////////////////////////////////////////////////////////////////
        Type type;
        wchar_t character;
        sf::Vector2i offset;
        sf::Color foreground;
        sf::Color background;
    };

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    /// \param font             Reference to a loaded sf::Font to use for
    //                          glyph data
    /// \param area             Width and height of the GlyphTileMap in # of
    //                          tiles
    /// \param spacing          Width and height of each tile in pixels
    /// \param characterSize    Size of each glyph
    ///////////////////////////////////////////////////////////////////////////
    GlyphTileMap(sf::Font& font, const sf::Vector2u& area,
        const sf::Vector2u& spacing, sf::Uint32 characterSize);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Returns a const reference to the area of the GlyphTileMap
    ///
    /// \return a const reference to the area of the GlyphTileMap
    ///////////////////////////////////////////////////////////////////////////
    const sf::Vector2u& getArea() const;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Returns a const reference to the spacing of the GlyphTileMap
    ///
    /// \return a const reference to the spacing of the GlyphTileMap
    ///////////////////////////////////////////////////////////////////////////
    const sf::Vector2u& getSpacing() const;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Returns the character size of the GlyphTileMap
    ///
    /// \return the character size of the GlyphTileMap
    ///////////////////////////////////////////////////////////////////////////
    sf::Uint32 getCharacterSize() const;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Updates the GlyphTileMap at a coords with data from a Tile
    ///
    /// \param coords   Coordinates in the GlyphTileMap to update
    /// \param tile     GlyphTileMap::Tile to update the GlyphTileMap with
    ///////////////////////////////////////////////////////////////////////////
    void setTile(const sf::Vector2u& coords, const Tile& tile);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Updates the character at a coords in the GlyphTileMap
    ///
    /// \param coords       Coordinates in the GlyphTileMap to update
    /// \param character    New character for the tile
    /// \param type         Tile::Type of the new character
    ///////////////////////////////////////////////////////////////////////////
    void setTileCharacter(const sf::Vector2u& coords, wchar_t character,
        Tile::Type type, const sf::Vector2i& offset = {0, 0});

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Updates the foreground color at a coords in the GlyphTileMap
    ///
    /// \param coords   Coordinates in the GlyphTileMap to update
    /// \param color    New foreground color for the tile
    ///////////////////////////////////////////////////////////////////////////
    void setTileForeground(const sf::Vector2u& coords, const sf::Color& color);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Updates the background color at a coords in the GlyphTileMap
    ///
    /// \param coords   Coordinates in the GlyphTileMap to update
    /// \param color    New background color for the tile
    ///////////////////////////////////////////////////////////////////////////
    void setTileBackground(const sf::Vector2u& coords, const sf::Color& color);

private:

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    sf::Uint32 getIndex(const sf::Vector2u& coords) const;

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void checkCharacter(wchar_t character);

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void updateTile(const sf::Vector2u& coords, const Tile& tile);

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void updateCharacter(const sf::Vector2u& coords, wchar_t character,
        Tile::Type type, const sf::Vector2i& offset);

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void setForegroundPosition(const sf::Vector2u& coords,
        const sf::IntRect& textureRect, const sf::Vector2i& offset);

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void setForegroundColor(const sf::Vector2u& coords,
        const sf::Color& color);

    ///////////////////////////////////////////////////////////////////////////
    ///
    /////////////////////////////////////////////////////////////////////////// 
    void setBackgroundPosition(const sf::Vector2u& coords);

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void setBackgroundColor(const sf::Vector2u& coords,
        const sf::Color& color);

    ///////////////////////////////////////////////////////////////////////////
    sf::Font& m_font;
    sf::Vector2u m_area;
    sf::Vector2u m_spacing;
    sf::Uint32 m_characterSize;
    sf::VertexArray m_foreground;
    sf::VertexArray m_background;
    static std::unordered_map<sf::Uint32,
        std::unordered_map<wchar_t, sf::Glyph>> m_glyphs;
};

#endif
