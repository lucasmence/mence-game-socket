
#include <iostream>
#include <list>
#include <string>
#include <windows.h>
#include <SFML/System.hpp>
#include "GameSocket.hpp"

int main()
{

	sf::Clock clock;
	sf::Time timer = clock.getElapsedTime();

	std::cout << "Nickname / address / port: ";
	std::string name = "", address = "";
	unsigned short port = 0;
	std::cin >> name >> address >> port;

	GameSocket socket;

	if (name != "server")
	{
		std::cout << "Nice to meet you " + name + "!";

		float duration = 60.f, interval = 0.f;
		clock.restart();

		//inicia	
		socket.start(SocketType::stClient, 54123, address != "" ? address : "mence.ddns.net");

		while (duration > 0.f)
		{
			sf::Time timer = clock.getElapsedTime();
			clock.restart();
			float time = timer.asSeconds();
			duration -= time;

			//recebe
			GameData data = socket.receive();
			std::string message = data.strings.size() > 0 ? data.strings.at(0) : "";
			if (message != "")
				std::cout << "Got: " << message << std::endl;

			interval += time;
			if (interval >= 1.f)
			{
				//envia
				interval = 0.f;
				data = { {name + " sent hi!"}, {} };
				socket.send(data);
			}
		}
	}
	else
	{
		float duration = 600.f, interval = 0.f;
		clock.restart();

		std::cout << "Server ON...\n";

		socket.start(SocketType::stServer, port > 0 ? port : 54123);

		while (duration > 0.f)
		{
			sf::Time timer = clock.getElapsedTime();
			clock.restart();
			float time = timer.asSeconds();
			duration -= time;

			//recebe
			GameData data = socket.receive();
			std::string message = data.strings.size() > 0 ? data.strings.at(0) : "";

			if (message != "")
			{
				//imprime
				std::cout << "Got: " << message << std::endl;
				//envia
				data = { {message}, {} };
				socket.send(data);
			}
		}
	}

	return 0;
}
