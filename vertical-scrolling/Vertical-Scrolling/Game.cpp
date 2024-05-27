

#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{

	m_scrollSpeed = 400.0f;
	m_player.setSize(sf::Vector2f(40.0f, 40.0f));
	m_player.setPosition(400.0f, 500.0f);
	m_player.setFillColor(sf::Color::Yellow);
	m_player.setOutlineColor(sf::Color::Black);
	m_player.setOutlineThickness(2);
	m_playerSpeed = 600.0f;
	for (int i = 0; i < terrainHeight; ++i)
	{
		for (int j = 0; j < terrainWidth; ++j)
		{
			int index = i * terrainWidth + j;
			if (levelData[index] == 1)
			{
				m_terrain[index].setSize(sf::Vector2f(80.0f, 80.0f));
				m_terrain[index].setPosition(80.0f * j, -80.0f * i);
				m_terrain[index].setFillColor(sf::Color::Black);
				m_terrain[index].setOutlineColor(sf::Color::Yellow);
				m_terrain[index].setOutlineThickness(2);
			}
			else if(levelData[index == 2])
			{
				/*m_terrain[index].setSize(sf::Vector2f(30.0f, 30.0f));
				float xPos = 80.0f * j;
				float yPos = 80.0f * i;
				m_terrain[index].setPosition(xPos, yPos);
				m_terrain[index].setFillColor(sf::Color::Red);*/
			}
		}
	}
}


Game::~Game()
{
}

void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type)
		{
			m_exitGame = true;
		}
		{
			processKeys(newEvent);
		}
		if (sf::Event::KeyPressed == newEvent.type)
		{
			processKeys(newEvent);
		}
	}
}



void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_gameOver)
	{
		restart();
	}
}


void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	if (m_gameOver)
	{
		
		return;
	}

	
	for (int i = 0; i < amountOfTiles; i++)
	{
		m_terrain[i].move(0.0f, m_scrollSpeed * t_deltaTime.asSeconds());

		if (m_player.getGlobalBounds().intersects(m_terrain[i].getGlobalBounds()))
		{
			sf::FloatRect intersection;
			m_player.getGlobalBounds().intersects(m_terrain[i].getGlobalBounds(), intersection);
			if (intersection.width > intersection.height)
			{
				if (m_player.getPosition().y < m_terrain[i].getPosition().y)
				{
					m_player.move(0.0f, -intersection.height);
				}
					
				else {
					m_player.move(0.0f, intersection.height);
				}
					
			}
			else
			{
				if (m_player.getPosition().x < m_terrain[i].getPosition().x)
				{
					m_player.move(-intersection.width, 0.0f);
				}
					
				else
				{
					m_player.move(intersection.width, 0.0f);
				}
					
			}
		}
	}

	bool isColliding = false;
	for (int i = 0; i < amountOfTiles; ++i)
	{
		if (m_player.getGlobalBounds().intersects(m_terrain[i].getGlobalBounds()))
		{
			isColliding = true;
			break;
		}
	}

	if (isColliding)
	{
		m_player.setFillColor(sf::Color::Red);
	}
	else
	{
		m_player.setFillColor(sf::Color::Yellow);
	}

	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_player.move(0.0f, -m_playerSpeed * t_deltaTime.asSeconds());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_player.move(0.0f, m_playerSpeed * t_deltaTime.asSeconds());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_player.move(-m_playerSpeed * t_deltaTime.asSeconds(), 0.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_player.move(m_playerSpeed * t_deltaTime.asSeconds(), 0.0f);
	}

	for (int i = 0; i < amountOfTiles; i++)
	{
		if (m_player.getGlobalBounds().intersects(m_terrain[i].getGlobalBounds()))
		{
			m_gameOver = true;
		}
	}

	if (m_gameOver)
	{
		gameOver();
	}
}

void Game::gameOver()
{
	sf::Font m_gameFont;
	if (!m_gameFont.loadFromFile("ASSETS/FONTS/ariblk.ttf"))
	{
		std::cout << "error loading font file." << std::endl;
	}

	sf::Text gameOverText;
	gameOverText.setFont(m_gameFont);
	gameOverText.setString("GAME OVER :(");
	gameOverText.setCharacterSize(50);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setPosition(m_window.getSize().x / 2 - gameOverText.getLocalBounds().width / 2, 
		m_window.getSize().y / 2 - gameOverText.getLocalBounds().height / 2);

	m_window.draw(gameOverText);
}


void Game::restart()
{
	m_player.setPosition(400.0f, 500.0f);
	m_gameOver = false;

	int terrainWidth = 10;
	int terrainHwight = amountOfTiles / 10;
	for (int i = 0; i < terrainHeight; i++)
	{
		for (int j = 0; j < terrainWidth; j++)
		{
			int index = i * terrainWidth + j;
			if (levelData[index] == 1)
			{
				m_terrain[index].setPosition(80.0f * j, -80.0f * i);
			}
			else if (levelData[index] == 2)
			{
				float xPos = 80.0f * j;
				float yPos = -80.0f * i;
			m_terrain[index].setPosition(xPos, yPos);
			}
		}
	}
}

void Game::render()
{
	m_window.clear(sf::Color(55, 55, 55));
	
	for (int i = 0; i < amountOfTiles; ++i)
	{
		m_window.draw(m_terrain[i]);
	}
	
	m_window.draw(m_player);
	if (m_gameOver)
	{
		gameOver();
	}
	m_window.display();
}




