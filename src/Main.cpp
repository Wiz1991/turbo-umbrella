#include "Platform/Platform.hpp"
#include "SAT.hpp"

int main()
{
	util::Platform platform;

#if defined(_DEBUG)
	std::cout << "Hello World!" << std::endl;
#endif

	sf::RenderWindow window;
	//in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	window.create(sf::VideoMode(800.0f * screenScalingFactor, 600.0f * screenScalingFactor), "SFML works!");
	platform.setIcon(window.getSystemHandle());

	sf::CircleShape shape(window.getSize().x / 2);
	shape.setFillColor(sf::Color::White);

	sf::ConvexShape objects[2];
    for(auto& obj : objects)
    {
        obj.setOutlineColor(sf::Color::Black);
        obj.setOutlineThickness(-2.0f);
        obj.setFillColor(sf::Color::Transparent);
    }
    objects[0].setOrigin(50, 25);
    objects[0].setPointCount(3);
    objects[0].setPoint(0, sf::Vector2f(0, 0));
    objects[0].setPoint(1, sf::Vector2f(0, 50));
    objects[0].setPoint(2, sf::Vector2f(100, 50));
    objects[0].setPosition(100, 100);

    objects[1].setOrigin(50, 25);
    objects[1].setPointCount(5);
    objects[1].setPoint(0, sf::Vector2f(0, 0));
    objects[1].setPoint(1, sf::Vector2f(0, 50));
    objects[1].setPoint(2, sf::Vector2f(100, 50));
    objects[1].setPoint(3, sf::Vector2f(100, 30));
    objects[1].setPoint(4, sf::Vector2f(50, 0));
    objects[1].setPosition(200, 200);

    int selected_object = 0;
    bool moving = true;
    sf::Vector2f rotation_center;
	sf::Clock clock;
	sf::Time dT;
    while(window.isOpen())
    {
		dT = clock.restart();
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed)
                if(event.key.code == sf::Keyboard::Tab)
                    selected_object = 1 - selected_object;
            if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    moving = false;
                    rotation_center = objects[selected_object].getPosition();
                }
            }

            if(event.type == sf::Event::Closed)
                window.close();
        }
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) objects[selected_object].move(0,-200*dT.asSeconds());
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))	objects[selected_object].move(-200 * dT.asSeconds(),0);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))	objects[selected_object].move(0,200*dT.asSeconds());
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))	objects[selected_object].move(200*dT.asSeconds(),0);
        window.clear(sf::Color::White);
		float overlap;
        if(collision(objects[0], objects[1]))
        {
            objects[0].setFillColor(sf::Color::Red);
            objects[1].setFillColor(sf::Color::Red);
			sf::Vector2f d = { objects[1].getPosition().x - objects[0].getPosition().x,objects[1].getPosition().y - objects[0].getPosition().y };
			float s = std::sqrt(d.x*d.x + d.y*d.y);
			sf::Vector2f newpos = objects[0].getPosition();
			newpos.x-=overlap*d.x/s;
			newpos.y-=overlap*d.x/s;
			objects[0].setPosition(newpos);
        }
        else
        {
            objects[0].setFillColor(sf::Color::Transparent);
            objects[1].setFillColor(sf::Color::Transparent);
        }

        for(const auto& obj : objects)
            window.draw(obj);
        window.display();
    }
}
