///
/// NOTE:
///
/// This Main.cpp file is mainly for testing/experimenting and may contain
/// errors.
///

#include <ctime>
#include <cstdlib>
#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "GlyphTileMap.h"

void setFrameScale(sf::Vector2f& frameScale, int windowWidth, int windowHeight,
    int frameWidth, int frameHeight)
{
    frameScale.x = static_cast<float>(windowWidth)
        / static_cast<float>(frameWidth);
    frameScale.y = static_cast<float>(windowHeight)
        / static_cast<float>(frameHeight);
}

sf::Color randColor()
{
    return sf::Color(
        std::rand() % 255,
        std::rand() % 255, 
        std::rand() % 255,
        255
        );
}

void randomizeTiles(GlyphTileMap& tileMap)
{
    for (sf::Uint32 i = 0; i < tileMap.getArea().x; ++i)
    for (sf::Uint32 j = 0; j < tileMap.getArea().y; ++j) {

        tileMap.setTile({i, j}, GlyphTileMap::Tile(
                static_cast<wchar_t>(std::rand()%65536),
                GlyphTileMap::Tile::Center,
                randColor(),
                randColor()
                ));
    }
}

int main()
{
    const int frameWidth = 640;
    const int frameHeight = 480;
    const sf::VideoMode windowMode(0, 0, 32);
    const char* windowTitle = "SFML Application";
    const sf::Uint32 windowStyle = sf::Style::Fullscreen;

    std::srand(std::time(0));

    sf::RenderWindow window(windowMode, windowTitle, windowStyle);

    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    window.setVerticalSyncEnabled(true);

    sf::RenderTexture frame;
    if (!frame.create(frameWidth, frameHeight)) {
        exit(EXIT_FAILURE);
    }
    window.setActive();

    sf::Vector2f frameScale;
    setFrameScale(frameScale, windowMode.width, windowMode.height, frameWidth,
        frameHeight);

    sf::Font unifont;
    if (!unifont.loadFromFile("res/fonts/unifont.ttf")) {
        exit(EXIT_FAILURE);
    }

    GlyphTileMap tileMap(unifont, {40, 30}, {16, 16}, 16);
    randomizeTiles(tileMap);

    sf::Clock timer;
    sf::Uint32 deltaTime = 0;

    while (window.isOpen()) {
        deltaTime = timer.restart().asMilliseconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                setFrameScale(frameScale, event.size.width, event.size.height,
                    frameWidth, frameHeight);
                break;
            default:
                break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            randomizeTiles(tileMap);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            //sf::Color color = randColor();
            for (sf::Uint32 i = 0; i < tileMap.getArea().x; ++i)
            for (sf::Uint32 j = 0; j < tileMap.getArea().y; ++j) {
                tileMap.setTileCharacter({i, j}, L'?');
            }
        }

        frame.clear();
        frame.draw(tileMap);
        frame.display();
        sf::Sprite frameSprite(frame.getTexture());
        frameSprite.setScale(frameScale);
        window.draw(frameSprite);
        window.display();
    }

    return EXIT_SUCCESS;
}
