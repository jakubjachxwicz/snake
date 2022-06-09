#include "header.h"


Win::Win()
{
	window = new sf::RenderWindow(sf::VideoMode(576, 576), "Snake");
	window->setFramerateLimit(5);

	event = sf::Event();

	initAssets();
}

Win::~Win()
{
	delete window;
}

const bool Win::isOpen() const
{
	return window->isOpen();
}


void Win::eventHandler()
{
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();


		case sf::Event::KeyPressed:
		{
			if (!gameOver)
			{
				if (readyToMove)
				{
					if (event.key.code == sf::Keyboard::Up && dirLabel != 'u' && dirLabel != 'd')
					{
						direction.x = 0.f;
						direction.y = -48.f;
						dirLabel = 'u';
						readyToMove = false;
					}
					else if (event.key.code == sf::Keyboard::Right && dirLabel != 'l' && dirLabel != 'r')
					{
						direction.x = 48.f;
						direction.y = 0.f;
						dirLabel = 'r';
						readyToMove = false;
					}
					else if (event.key.code == sf::Keyboard::Down && dirLabel != 'u' && dirLabel != 'd')
					{
						direction.x = 0.f;
						direction.y = 48.f;
						dirLabel = 'd';
						readyToMove = false;
					}
					else if (event.key.code == sf::Keyboard::Left && dirLabel != 'l' && dirLabel != 'r')
					{
						direction.x = -48.f;
						direction.y = 0.f;
						dirLabel = 'l';
						readyToMove = false;
					}
				}
			}
			else
			{
				if (event.key.code == sf::Keyboard::Space) reset();
			}
		}	break;
		}
	}
}

void Win::initAssets()
{
	gameOver = false;
	readyToMove = true;

	direction.x = 0.f;
	direction.y = 0.f;
	dirLabel = 0;

	coin.x = 336.f;
	coin.y = 192.f;
	coinSprt.setPosition(coin);
	coinCircle.setFillColor(sf::Color::Blue);
	coinCircle.setRadius(10.f);
	coinCircle.setPosition(sf::Vector2f(coin.x + 12.f, coin.y + 12.f));

	tempSegment = Segment();
	lastSegment.x = 0.f;
	lastSegment.y = 0.f;

	snake.push_back(tempSegment);
	snake[0].setPos(sf::Vector2f(240.f, 240.f));
	snake[0].rect.setFillColor(sf::Color::Cyan);

	coinT.loadFromFile("assets/coin.png");

	coinSprt.setTexture(coinT);

	if (pointBuffer.loadFromFile("assets/point.wav")) pointSoundLoaded = true;
	else pointSoundLoaded = false;

	if (defeatBuffer.loadFromFile("assets/gameover.wav")) defeatSoundLoaded = true;
	else defeatSoundLoaded = false;

	sound.setBuffer(pointBuffer);
}


void Win::loop()
{
	eventHandler();

	if (!gameOver)
	{
		//moving snake
		for (int i = snake.size() - 1; i >= 0; i--)
		{
			if (!i)
			{
				snake[i].move(direction);
				break;
			}
			snake[i].setPos(snake[i - 1].position);
		}
		lastSegment = snake[snake.size() - 1].position;
		readyToMove = true;


		//collision check
		for (int i = 1; i < snake.size(); i++)
		{
			if (snake[0].position == snake[i].position)
			{
				if (pointSoundLoaded)
				{
					sound.setBuffer(defeatBuffer);
					sound.play();
				}

				gameOver = true;
				LOG("GAME OVER\t\tWcisnij spacje aby kontynuowac");
			}
		}


		//coin
		if (snake[0].position == coin)
		{
			if (pointSoundLoaded)
			{
				sound.play();
			}

			tempSegment.setPos(lastSegment);
			tempSegment.rect.setFillColor(
				sf::Color(rand() % 256, rand() % 256, rand() % 256)
			);
			snake.push_back(tempSegment);

			sf::Vector2f coinNewPosition;
			bool positionOK;
			do
			{
				positionOK = true;
				coinNewPosition.x = (float)((rand() % 12) * 48);
				coinNewPosition.y = (float)((rand() % 12) * 48);

				for (int i = 0; i < snake.size(); i++)
				{
					if (coinNewPosition == snake[i].position) positionOK = false;
				}
			} while (!positionOK);

			coin = coinNewPosition;
			coinSprt.setPosition(coinNewPosition);
			coinCircle.setPosition(sf::Vector2f(coinNewPosition.x + 12.f, coinNewPosition.y + 12.f));
		}
	}


	render();
}

void Win::render()
{
	window->clear(sf::Color::Black);

	for (int i = 0; i < snake.size(); i++)
	{
		window->draw(snake[i].rect);
	}

	window->draw(coinCircle);
	window->draw(coinSprt);

	window->display();
}


void Win::reset()
{
	gameOver = false;
	readyToMove = true;

	snake.clear();
	snake.push_back(tempSegment);
	snake[0].setPos(sf::Vector2f(240.f, 240.f));
	snake[0].rect.setFillColor(sf::Color::Cyan);

	dirLabel = 0;
	direction.x = 0.f;
	direction.y = 0.f;

	coin.x = 336.f;
	coin.y = 192.f;
	coinSprt.setPosition(coin);
	coinCircle.setPosition(sf::Vector2f(coin.x + 12.f, coin.y + 12.f));


	sound.setBuffer(pointBuffer);
}



Segment::Segment()
{
	position.x = 0.f;
	position.y = 0.f;

	rect.setPosition(this->position);
	//rect.setFillColor(sf::Color::White);
	rect.setFillColor(sf::Color
	(
		rand() % 256, rand() % 256, rand() % 256
	));
	rect.setSize(sf::Vector2f(48.f, 48.f));
}

void Segment::setPos(sf::Vector2f pos)
{
	position = pos;
	rect.setPosition(pos);
}

void Segment::move(sf::Vector2f vect)
{
	this->setPos(this->position + vect);

	if (this->position.x == 576.f) this->setPos(sf::Vector2f(0.f, this->position.y));
	if (this->position.y == 576.f) this->setPos(sf::Vector2f(this->position.x, 0.f));
	if (this->position.x < 0.f) this->setPos(sf::Vector2f(528.f, this->position.y));
	if (this->position.y < 0.f) this->setPos(sf::Vector2f(this->position.x, 528.f));
}