/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientMetadata.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:15:53 by messkely          #+#    #+#             */
/*   Updated: 2025/05/31 22:14:36 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ClientMetadata_HPP
# define ClientMetadata_HPP

#include <string>

class ClientMetadata
{
    public:
        std::string nick;
        int socket;

        struct GameData
    	{
            std::string answer;
            std::string category;
            int attemptsLeft;
            bool inGame;

            GameData() : attemptsLeft(5), inGame(false) {}
        };

        GameData game;
    	ClientMetadata(const std::string& nickname, int sock);
        void sendMessage(const std::string& msg) const;
    	void resetGame();
};



#endif
