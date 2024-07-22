#include <vector>
#include <list>
#include <string>
#include <SFML/Network.hpp>

#pragma once

#ifndef GAMESOCKET_HPP
#define GAMESOCKET_HPP

enum class SocketType { stNone, stServer, stClient };

struct GameClient
{
	bool enabled;
	sf::IpAddress ipAddress;
	unsigned short port;
};

struct GameData
{
	std::vector<std::string> strings;
	std::vector<float> floats;
};

class GameSocket
{
	private:
		SocketType socketType;
		unsigned short port;
		sf::UdpSocket socket;
		std::list<GameClient> clients;
		std::string serverAddress;

		const bool sendTarget(GameData &data, GameClient target);
		const bool sendBroadcast(GameData& data);
		const bool addClient(GameClient client);
		const bool clear();

	public:
		GameSocket();
		~GameSocket();

		const bool start(SocketType socketType, unsigned short port, std::string address = "");
		const bool end();

		const GameData receive();
		const bool send(GameData &data);

		const SocketType getType();
};

#endif