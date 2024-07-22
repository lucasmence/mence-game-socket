
#include <iostream>
#include "GameSocket.hpp"

GameSocket::GameSocket()
{
	this->socket.setBlocking(false);
	this->clear();
}

GameSocket::~GameSocket()
{
	this->end();
}

const bool GameSocket::start(SocketType socketType, unsigned short port, std::string address)
{
	this->clear();

	this->port = port;
	this->socketType = socketType;
	this->serverAddress = address;
	
	if (this->socketType == SocketType::stNone || this->socketType == SocketType::stServer ? this->socket.bind(this->port) != sf::Socket::Done : false)
		return false;

	return true;
}
const bool GameSocket::end()
{
	this->clear();

	return true;
}

const GameData GameSocket::receive()
{
	sf::Packet packet;
	unsigned short receivePort;
	sf::IpAddress sender;

	this->socket.receive(packet, sender, receivePort);

	GameData data(GameData{ {}, {} });

	while (!packet.endOfPacket())
	{
		float floatValue = 0.f;
		std::string stringValue = "";

		if (packet >> stringValue)
			data.strings.emplace_back(stringValue);
		else if (packet >> floatValue)
			data.floats.emplace_back(floatValue);
	}

	this->addClient(GameClient{true, sender, receivePort});

	return data;
}
const bool GameSocket::send(GameData &data)
{
	switch (this->getType()) 
	{
		case (SocketType::stClient):
		{
			GameClient gameClient;
			gameClient.enabled = false;
			return this->sendTarget(data, gameClient);
		}

		case (SocketType::stServer):
		{
			return this->sendBroadcast(data);
		}
	}

	return false;
}

const SocketType GameSocket::getType()
{
	return this->socketType;
}

const bool GameSocket::sendTarget(GameData &data, GameClient target)
{
	sf::Packet packet;

	for (auto& floatIndex : data.floats)
		packet << floatIndex;
	for (auto& stringIndex : data.strings)
		packet << stringIndex;

	return target.enabled ? this->socket.send(packet, target.ipAddress, target.port) : this->socket.send(packet, this->serverAddress, this->port) == sf::Socket::Done;
}

const bool GameSocket::sendBroadcast(GameData& data)
{
	for (auto& client : this->clients)
		this->sendTarget(data, client);
	return true;
}

const bool GameSocket::addClient(GameClient client)
{
	if (this->socketType != SocketType::stServer)
		return false;

	auto clientIterator = std::find_if(this->clients.begin(), this->clients.end(), [client] (const GameClient clientIndex) {return clientIndex.ipAddress.toString() == client.ipAddress && clientIndex.port == client.port; });
	if (this->clients.end() == clientIterator)
	{
		this->clients.emplace_back(client);
		return true;
	}

	return false;
}

const bool GameSocket::clear()
{
	this->socket.unbind();
	this->socketType = SocketType::stNone;
	this->port = 55189;
	this->clients = {};
	this->serverAddress = "";

	return true;
}
