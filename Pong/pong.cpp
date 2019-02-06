#include <SFML/Graphics.hpp>
#include <random>
#include <time.h>
#include <SFML/Audio.hpp>
#define WIDTH 800
#define HEIGHT 600
int main() {
	srand(static_cast<unsigned int>(time(NULL)));

	sf::RenderWindow mWindow(sf::VideoMode(WIDTH,HEIGHT,32), "Pong recreation", sf::Style::Titlebar | sf::Style::Close);
	mWindow.setVerticalSyncEnabled(true);
	mWindow.setFramerateLimit(60);
	bool isPlaying = false;
	sf::SoundBuffer sound;
	sf::Font font;
	sf::RectangleShape leftPaddle;
	sf::Vector2f paddleSize(26, 100);
	sf::RectangleShape rightPaddleAI;
	float paddleSpeed = 300.f;
	sf::Vector2f pauseMessagePos(200, 300);
	sf::CircleShape ball;
	const float ballRadius = 10.f;
	double ballSpeed=270.f;
	double ballAngle = 0.f;
	sf::Vector2f leftPaddlePosition(20.f, HEIGHT / 2.f);
	sf::Vector2f rightPaddlePosition((WIDTH  - 20.f), (HEIGHT / 2.f));

	


	

	//Load in the sound and font
	if (!sound.loadFromFile("resources/ball.wav"))
		return EXIT_FAILURE;
	sf::Sound ballSFX(sound);
	if (!font.loadFromFile("resources/sansation.ttf"))
		return EXIT_FAILURE;

	//Initiate pause message
	sf::Text pauseMessage;
	pauseMessage.setFont(font);
	pauseMessage.setString("Press SPACE to begin playing");
	pauseMessage.setPosition(pauseMessagePos);
	
	//Initiate left paddle

	leftPaddle.setPosition(leftPaddlePosition);
	leftPaddle.setOutlineThickness(3.5);
	leftPaddle.setOutlineColor(sf::Color::Black);
	leftPaddle.setFillColor(sf::Color::Red);
	leftPaddle.setSize(paddleSize);
	leftPaddle.setOrigin(paddleSize.x/2,paddleSize.y/2);

	//Initiate right paddle
	rightPaddleAI.setPosition(rightPaddlePosition);
	rightPaddleAI.setOutlineThickness(3.5);
	rightPaddleAI.setOutlineColor(sf::Color::Black);
	rightPaddleAI.setFillColor(sf::Color::Magenta);
	rightPaddleAI.setSize(paddleSize);
	rightPaddleAI.setOrigin(paddleSize.x / 2, paddleSize.y / 2);

	//Initiate ball 
	ball.setFillColor(sf::Color::Blue);
	ball.setPosition(WIDTH / 2, HEIGHT / 2);
	ball.setRadius(ballRadius);
	ball.setOutlineThickness(0.3f);
	ball.setOutlineColor(sf::Color::Black);
	ball.setOrigin(ballRadius / 2, ballRadius / 2);
	sf::Clock AITimer;
	const sf::Time AIReaction = sf::seconds(0.1f);
	float rightPaddleSpeed = 0.f;

	sf::Event event;
	sf::Clock clock;
	while (mWindow.isOpen())
	{
		while (mWindow.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				mWindow.close();
			if (event.type == sf::Event::Closed)
				mWindow.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
				if (!isPlaying) {
					isPlaying = true;

					leftPaddle.setPosition(leftPaddlePosition);
					rightPaddleAI.setPosition(rightPaddlePosition);
					ball.setPosition(WIDTH / 2, HEIGHT / 2);
					do {



						ballAngle = (rand() % 360) * 2 * 3.16 / 360;

					} while (std::abs(std::cos(ballAngle) < 0.7));

					

				}
			}

		}
		if (isPlaying) {
			mWindow.clear(sf::Color::White);
			mWindow.draw(leftPaddle);
			mWindow.draw(rightPaddleAI);
			mWindow.draw(ball);
		}
		else {
			mWindow.clear(sf::Color::Green);
			mWindow.draw(pauseMessage);
		}

			if (isPlaying) {
						//Player movement
				
				float timeElapsed = clock.restart().asSeconds();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {

					leftPaddle.move(0.f, -paddleSpeed*timeElapsed);

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {

					leftPaddle.move(0.f, paddleSpeed * timeElapsed);

				}
				//Ball movement
				double factor = timeElapsed * ballSpeed;
				ball.move(std::cos(ballAngle)*factor, std::sin(ballAngle)*factor);
				//Ball boundaries
				if (ball.getPosition().y - ballRadius < 0.f)
				{
					ballSFX.play();
					ballAngle = -ballAngle;
					ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
				}
				if (ball.getPosition().y + ballRadius > HEIGHT)
				{
					ballSFX.play();
					ballAngle = -ballAngle;
					ball.setPosition(ball.getPosition().x, HEIGHT - ballRadius - 0.1f);
				}
				//Ball losing/winning boundaries
				if (ball.getPosition().x + ballRadius  > WIDTH) {
					isPlaying = false;
					pauseMessage.setString("You won!\nPress space to restart or \nescape to exit");
				}
				if (ball.getPosition().x - ballRadius < 0.f) {
					isPlaying = false;
					pauseMessage.setString("You lost!\nPress space to restart or\nescape to exit");
				}

				//AI movement
				if (((rightPaddleSpeed < 0.f) && (rightPaddleAI.getPosition().y - paddleSize.y / 2 > 5.f)) ||
					((rightPaddleSpeed > 0.f) && (rightPaddleAI.getPosition().y + paddleSize.y / 2 < HEIGHT - 5.f))){
					
						rightPaddleAI.move(0.f, rightPaddleSpeed * timeElapsed);
				}

					if (AITimer.getElapsedTime() > AIReaction)
					{
						AITimer.restart();
						if (ball.getPosition().y + ballRadius > rightPaddleAI.getPosition().y + paddleSize.y / 2)
							rightPaddleSpeed = paddleSpeed;
						else if (ball.getPosition().y - ballRadius < rightPaddleAI.getPosition().y - paddleSize.y / 2)
							rightPaddleSpeed = -paddleSpeed;
						else
							rightPaddleSpeed = 0.f;
					}




				//Paddle and ball collision
				if (ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
					ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
					ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
					ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2)
				{
					if (ball.getPosition().y > leftPaddle.getPosition().y)
						ballAngle = 3.14 - ballAngle + (std::rand() % 20) * 3.14 / 180;
					else
						ballAngle = 3.14 - ballAngle - (std::rand() % 20) * 3.14 / 180;

					ballSFX.play();
					ball.setPosition(leftPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.01f, ball.getPosition().y);
				}
				if (ball.getPosition().x + ballRadius > rightPaddleAI.getPosition().x - paddleSize.x / 2 &&
					ball.getPosition().x + ballRadius < rightPaddleAI.getPosition().x &&
					ball.getPosition().y + ballRadius >= rightPaddleAI.getPosition().y - paddleSize.y / 2 &&
					ball.getPosition().y - ballRadius <= rightPaddleAI.getPosition().y + paddleSize.y / 2)
				{
					if (ball.getPosition().y > rightPaddleAI.getPosition().y)
						ballAngle = 3.14 - ballAngle + (std::rand() % 20) * 3.14 / 180;
					else
						ballAngle = 3.14 - ballAngle - (std::rand() % 20) * 3.14 / 180;

					ballSFX.play();
					ball.setPosition(rightPaddleAI.getPosition().x - ballRadius - paddleSize.x / 2 - 0.01f, ball.getPosition().y);
				}

			}
			

		mWindow.display();
	}



	

	
}