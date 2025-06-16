/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:18:24 by messkely          #+#    #+#             */
/*   Updated: 2025/05/30 22:33:57 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>
#include "ClientMetadata.hpp"

struct AQInventory{
    std::string answer;
    std::string category;
};

class Game {
	private:
    	std::vector<AQInventory> data;
    	void loadDataFile(const std::string& filename);
    	AQInventory getRandomQA() const;
		std::string drawRaw(int len);
		void displayInventory(ClientMetadata& client, const char *playerAnswer);

	public:
    	Game();
    	void start(ClientMetadata& client);
    	void handleGuess(ClientMetadata& client, const std::string& guess);
    	void sendHint(ClientMetadata& client);

};



#endif
