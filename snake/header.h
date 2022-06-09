#pragma once

#include <iostream>
#include <vector>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define LOG(x) std::cout << x << std::endl


struct Segment
{
	Segment();
	void setPos(sf::Vector2f);
	void move(sf::Vector2f);

	sf::Vector2f position;
	sf::RectangleShape rect;
};


class Win
{
	sf::RenderWindow* window;
	sf::Event event;

	void initAssets();
	void eventHandler();
	void render();

	void reset();

	bool gameOver;

	sf::Vector2f direction;
	char dirLabel;
	bool readyToMove;

	sf::Texture coinT;
	sf::Sprite coinSprt;
	sf::Vector2f coin;
	sf::CircleShape coinCircle;

	Segment tempSegment;
	sf::Vector2f lastSegment;
	std::vector<Segment> snake;

	sf::SoundBuffer pointBuffer, defeatBuffer;
	bool pointSoundLoaded, defeatSoundLoaded;
	sf::Sound sound;

public:
	Win();
	~Win();

	const bool isOpen() const;
	void loop();
};