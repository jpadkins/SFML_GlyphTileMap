///////////////////////////////////////////////////////////////////////////////
/// Author:         Jacob P Adkins (github.com/jpadkins)
/// Filename:       GlyphTileMap.cpp
/// License:        MIT
/// Description:    An SFML-based drawable/transformable class for displaying
///                 a grid of characters with colored backgrounds and several
///                 spacing options.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Headers
///////////////////////////////////////////////////////////////////////////////

#include "GlyphTileMap.h"

///////////////////////////////////////////////////////////////////////////////
GlyphTileMap::Tile::Tile()
    : type(Type::Center)
    , character(L'?')
    , offset(0, 0)
    , foreground(sf::Color::White)
    , background(sf::Color::Black)
{}

///////////////////////////////////////////////////////////////////////////////
GlyphTileMap::Tile::Tile(wchar_t character, Type type,
    const sf::Color& foreground, const sf::Color& background,
    const sf::Vector2i& offset)
    : type(type)
    , character(character)
    , offset(offset)
    , foreground(foreground)
    , background(background)
{}

///////////////////////////////////////////////////////////////////////////////
GlyphTileMap::GlyphTileMap(sf::Font& font, const sf::Vector2u& area,
    const sf::Vector2u& spacing, sf::Uint32 characterSize)
    : m_font(font)
    , m_area(area)
    , m_spacing(spacing)
    , m_characterSize(characterSize)
    , m_foreground(sf::Quads, area.x * area.y * 4)
    , m_background(sf::Quads, area.x * area.y * 4)
{}

///////////////////////////////////////////////////////////////////////////////
const sf::Vector2u& GlyphTileMap::getArea() const
{
    return m_area;
}

///////////////////////////////////////////////////////////////////////////////
const sf::Vector2u& GlyphTileMap::getSpacing() const
{
    return m_spacing;
}

///////////////////////////////////////////////////////////////////////////////
sf::Uint32 GlyphTileMap::getCharacterSize() const
{
    return m_characterSize;
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::setTile(const sf::Vector2u& coords, const Tile& tile)
{
    updateTile(coords, tile);
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::setTileCharacter(const sf::Vector2u& coords,
    wchar_t character, Tile::Type type, const sf::Vector2i& offset)
{
    updateCharacter(coords, character, type, offset);
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::setTileForeground(const sf::Vector2u& coords,
    const sf::Color& color)
{
    updateForegroundColor(coords, color);
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::setTileBackground(const sf::Vector2u& coords,
    const sf::Color& color)
{
    updateBackgroundColor(coords, color);
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::draw(sf::RenderTarget& target, sf::RenderStates states)
    const
{
    states.transform *= getTransform();
    states.texture = &m_font.getTexture(m_characterSize);
    target.draw(m_background, states);
    target.draw(m_foreground, states);
}

///////////////////////////////////////////////////////////////////////////////
sf::Uint32 GlyphTileMap::getIndex(const sf::Vector2u& coords) const
{
    return (coords.y * m_area.x) + coords.x;
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::updateTile(const sf::Vector2u& coords, const Tile& tile)
{
    const sf::Glyph& glyph = m_font.getGlyph(tile.character, m_characterSize,
        false);

    sf::Vector2i adjustedOffset = {0, 0};

    switch (tile.type) {
    case Tile::Text:
        {
        adjustedOffset.x = glyph.bounds.left;
        adjustedOffset.y = m_spacing.y + glyph.bounds.top;
        }
        break;
    case Tile::Exact:
        adjustedOffset.x = (m_spacing.x - glyph.textureRect.width) / 2;
        adjustedOffset.y = (m_spacing.y - glyph.textureRect.height) / 2;
        adjustedOffset.x += tile.offset.x;
        adjustedOffset.y += tile.offset.y;
        break;
    case Tile::Floor:
        adjustedOffset.x = (m_spacing.x - glyph.textureRect.width) / 2;
        adjustedOffset.y = m_spacing.y - glyph.textureRect.height;
        break;
    case Tile::Center:
        adjustedOffset.x = (m_spacing.x - glyph.textureRect.width) / 2;
        adjustedOffset.y = (m_spacing.y - glyph.textureRect.height) / 2;
        break;
    }

    updateForegroundPosition(coords, glyph.textureRect, adjustedOffset);
    updateForegroundColor(coords, tile.foreground);
    updateBackgroundPosition(coords);
    updateBackgroundColor(coords, tile.background);
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::updateCharacter(const sf::Vector2u& coords,
        wchar_t character, Tile::Type type, const sf::Vector2i& offset)
{
    const sf::Glyph& glyph = m_font.getGlyph(character, m_characterSize,
        false);

    sf::Vector2i adjustedOffset = {0, 0};

    switch (type) {
    case Tile::Text:
        {
        adjustedOffset.x = glyph.bounds.left;
        adjustedOffset.y = m_spacing.y + glyph.bounds.top;
        }
        break;
    case Tile::Exact:
        adjustedOffset.x = (m_spacing.x - glyph.textureRect.width) / 2;
        adjustedOffset.y = (m_spacing.y - glyph.textureRect.height) / 2;
        adjustedOffset.x += offset.x;
        adjustedOffset.y += offset.y;
        break;
    case Tile::Floor:
        adjustedOffset.x = (m_spacing.x - glyph.textureRect.width) / 2;
        adjustedOffset.y = m_spacing.y - glyph.textureRect.height;
        break;
    case Tile::Center:
        adjustedOffset.x = (m_spacing.x - glyph.textureRect.width) / 2;
        adjustedOffset.y = (m_spacing.y - glyph.textureRect.height) / 2;
        break;
    }

    updateForegroundPosition(coords, glyph.textureRect, adjustedOffset);
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::updateForegroundPosition(const sf::Vector2u& coords,
    const sf::IntRect& textureRect, const sf::Vector2i& offset)
{
    sf::Uint32 index = getIndex(coords) * 4;

    m_foreground[index].position = {
        static_cast<float>((coords.x * m_spacing.x) + offset.x),
        static_cast<float>((coords.y * m_spacing.y) + offset.y)
        };
    m_foreground[index + 1].position = {
        static_cast<float>((coords.x * m_spacing.x) + textureRect.width
            + offset.x),
        static_cast<float>((coords.y * m_spacing.y) + offset.y)
        };
    m_foreground[index + 2].position = {
        static_cast<float>((coords.x * m_spacing.x) + textureRect.width
            + offset.x),
        static_cast<float>((coords.y * m_spacing.y) + textureRect.height
            + offset.y)
        };
    m_foreground[index + 3].position = {
        static_cast<float>((coords.x * m_spacing.x) + offset.x),
        static_cast<float>((coords.y * m_spacing.y) + textureRect.height
            + offset.y)
        };
    m_foreground[index].texCoords = {
        static_cast<float>(textureRect.left),
        static_cast<float>(textureRect.top)
        };
    m_foreground[index + 1].texCoords = {
        static_cast<float>(textureRect.left + textureRect.width),
        static_cast<float>(textureRect.top)
        };
    m_foreground[index + 2].texCoords = {
        static_cast<float>(textureRect.left + textureRect.width),
        static_cast<float>(textureRect.top + textureRect.height)
        };
    m_foreground[index + 3].texCoords = {
        static_cast<float>(textureRect.left),
        static_cast<float>(textureRect.top + textureRect.height)
        };
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::updateForegroundColor(const sf::Vector2u& coords,
    const sf::Color& color)
{
    sf::Uint32 index = getIndex(coords) * 4;

    m_foreground[index].color = color;
    m_foreground[index + 1].color = color;
    m_foreground[index + 2].color = color;
    m_foreground[index + 3].color = color;
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::updateBackgroundPosition(const sf::Vector2u& coords)
{
    sf::Uint32 index = getIndex(coords) * 4;

    m_background[index].position = {
        static_cast<float>(coords.x * m_spacing.x),
        static_cast<float>(coords.y * m_spacing.y)
        };
    m_background[index + 1].position = {
        static_cast<float>((coords.x * m_spacing.x) + m_spacing.x),
        static_cast<float>(coords.y * m_spacing.y)
        };
    m_background[index + 2].position = {
        static_cast<float>((coords.x * m_spacing.x) + m_spacing.x),
        static_cast<float>((coords.y * m_spacing.y) + m_spacing.y)
        };
    m_background[index + 3].position = {
        static_cast<float>(coords.x * m_spacing.x),
        static_cast<float>((coords.y * m_spacing.y) + m_spacing.y)
        };
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::updateBackgroundColor(const sf::Vector2u& coords,
    const sf::Color& color)
{
    sf::Uint32 index = getIndex(coords) * 4;

    m_background[index].color = color;
    m_background[index + 1].color = color;
    m_background[index + 2].color = color;
    m_background[index + 3].color = color;
}

