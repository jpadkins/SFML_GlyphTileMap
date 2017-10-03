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
/// Static variables
///////////////////////////////////////////////////////////////////////////////

std::unordered_map<sf::Uint32, std::unordered_map<wchar_t, sf::Glyph>>
    GlyphTileMap::m_glyphs;

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
{
    if (m_glyphs.find(characterSize) == m_glyphs.end()) {
        m_glyphs.insert({
            characterSize,
            std::unordered_map<wchar_t, sf::Glyph>()
            });
    }
}

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
    setForegroundColor(coords, color);
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::setTileBackground(const sf::Vector2u& coords,
    const sf::Color& color)
{
    setBackgroundColor(coords, color);
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
void GlyphTileMap::checkCharacter(wchar_t character)
{
    if (m_glyphs[m_characterSize].find(character)
        == m_glyphs[m_characterSize].end()) {
        m_glyphs[m_characterSize].insert({
            character,
            m_font.getGlyph(character, m_characterSize, false)
            });
    }
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::updateTile(const sf::Vector2u& coords, const Tile& tile)
{
    checkCharacter(tile.character);

    sf::IntRect textureRect = m_glyphs[m_characterSize][tile.character]
        .textureRect;

    sf::Vector2i adjustedOffset = {0, 0};

    switch (tile.type) {
    case Tile::Type::Text:
        {
        sf::FloatRect bounds = m_glyphs[m_characterSize][tile.character]
            .bounds;
        adjustedOffset.x = bounds.left;
        adjustedOffset.y = m_spacing.y + bounds.top;
        }
        break;
    case Tile::Type::Exact:
        adjustedOffset.x = (m_spacing.x - textureRect.width) / 2;
        adjustedOffset.y = (m_spacing.y - textureRect.height) / 2;
        adjustedOffset.x += tile.offset.x;
        adjustedOffset.y += tile.offset.y;
        break;
    case Tile::Type::Floor:
        adjustedOffset.x = (m_spacing.x - textureRect.width) / 2;
        adjustedOffset.y = m_spacing.y - textureRect.height;
        break;
    case Tile::Type::Center:
        adjustedOffset.x = (m_spacing.x - textureRect.width) / 2;
        adjustedOffset.y = (m_spacing.y - textureRect.height) / 2;
        break;
    }

    setForegroundPosition(coords, textureRect, adjustedOffset);
    setForegroundColor(coords, tile.foreground);
    setBackgroundPosition(coords);
    setBackgroundColor(coords, tile.background);
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::updateCharacter(const sf::Vector2u& coords,
        wchar_t character, Tile::Type type, const sf::Vector2i& offset)
{
    checkCharacter(character);

    sf::IntRect textureRect = m_glyphs[m_characterSize][character]
        .textureRect;

    sf::Vector2i adjustedOffset = {0, 0};

    switch (type) {
    case Tile::Type::Text:
        {
        sf::FloatRect bounds = m_glyphs[m_characterSize][character].bounds;
        adjustedOffset.x = bounds.left;
        adjustedOffset.y = m_spacing.y + bounds.top;
        }
        break;
    case Tile::Type::Exact:
        adjustedOffset.x = (m_spacing.x - textureRect.width) / 2;
        adjustedOffset.y = (m_spacing.y - textureRect.height) / 2;
        adjustedOffset.x += offset.x;
        adjustedOffset.y += offset.y;
        break;
    case Tile::Type::Floor:
        adjustedOffset.x = (m_spacing.x - textureRect.width) / 2;
        adjustedOffset.y = m_spacing.y - textureRect.height;
        break;
    case Tile::Type::Center:
        adjustedOffset.x = (m_spacing.x - textureRect.width) / 2;
        adjustedOffset.y = (m_spacing.y - textureRect.height) / 2;
        break;
    }

    setForegroundPosition(coords, textureRect, adjustedOffset);
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::setForegroundPosition(const sf::Vector2u& coords,
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
void GlyphTileMap::setForegroundColor(const sf::Vector2u& coords,
    const sf::Color& color)
{
    sf::Uint32 index = getIndex(coords) * 4;

    m_foreground[index].color = color;
    m_foreground[index + 1].color = color;
    m_foreground[index + 2].color = color;
    m_foreground[index + 3].color = color;
}

///////////////////////////////////////////////////////////////////////////////
void GlyphTileMap::setBackgroundPosition(const sf::Vector2u& coords)
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
void GlyphTileMap::setBackgroundColor(const sf::Vector2u& coords,
    const sf::Color& color)
{
    sf::Uint32 index = getIndex(coords) * 4;

    m_background[index].color = color;
    m_background[index + 1].color = color;
    m_background[index + 2].color = color;
    m_background[index + 3].color = color;
}

