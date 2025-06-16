/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:47:20 by messkely          #+#    #+#             */
/*   Updated: 2025/05/30 22:41:47 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <ctime>

Game::Game()
{
	std::srand(std::time(0));
	loadDataFile("bonus/data.txt");
}

void Game::loadDataFile(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string answer, category;
		if (std::getline(iss, answer, ',') && std::getline(iss, category))
		{
			AQInventory entry;
			entry.answer = answer;
			entry.category = category;
			data.push_back(entry);
		}
	}
}

std::string Game::drawRaw(int len)
{
	std::string rawHolder;
	for (int i = 0; i < len; ++i)
		rawHolder += " -";
	return (rawHolder);
}
#include <iostream>
void Game::displayInventory(ClientMetadata &client, const char *playerAnswer)
{
	std::string result;

	const char *answer = client.game.answer.c_str();
	size_t len = std::strlen(answer);
	for (size_t i = 0; i < len; ++i)
		result += (answer[i] == playerAnswer[i]) ? "|" + std::string(1, answer[i]) : "|X";
	result += "|";
	client.sendMessage(drawRaw(len));
	client.sendMessage(result);
	client.sendMessage(drawRaw(len));
}

AQInventory Game::getRandomQA() const
{
	AQInventory empty;
	if (data.empty())
		return empty;
	int index = std::rand() % data.size();
	return data[index];
}

void Game::start(ClientMetadata &client)
{
	if (client.game.inGame)
		return;

	AQInventory entry = getRandomQA();
	client.game.answer = entry.answer;
	client.game.category = entry.category;
	client.game.attemptsLeft = 5;
	client.game.inGame = true;

	client.sendMessage("Game started! You have 5 attempts to guess the correct object name.");
	client.sendMessage("Use !hint to discover the category the object belongs to.");
	client.sendMessage("Use !guess followed by a word of your choice.");
	client.sendMessage("You can always leave the game by sending !quit.");
	displayInventory(client, "");
}

void Game::sendHint(ClientMetadata &client)
{
	if (!client.game.inGame)
		return;
	client.sendMessage("Hint: The category is '" + client.game.category + "'");
}

void Game::handleGuess(ClientMetadata &client, const std::string &guess)
{
	if (!client.game.inGame)
		return;
	client.game.attemptsLeft--;
	if (guess == client.game.answer)
	{
		client.sendMessage("Correct! The word was '" + client.game.answer + "'. You won!");
		client.resetGame();
		return;
	}

	if (client.game.attemptsLeft == 0)
	{
		client.sendMessage("No guess left, The answer was '" + client.game.answer + "'.");
		client.resetGame();
		return;
	}

	displayInventory(client, guess.c_str());
	std::ostringstream oss;
	oss << "Wrong! You have " << client.game.attemptsLeft << " tries left.";
	client.sendMessage(oss.str());
}
