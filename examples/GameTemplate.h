//
// Created by Ny Andrianina Mamy on 12/12/2020.
//

#ifndef CARD_GAME_GAMETEMPLATE_H
#define CARD_GAME_GAMETEMPLATE_H


#include "../card-game/Board.h"

#define SKIPLINE std::cout<<std::endl;

class GameTemplate { // Abstract

protected:
    Board board;

    virtual void initialization();
    virtual void first_turn();
    virtual bool is_the_end();
    virtual void next_turn();
    virtual void end_of_game();
    // Utils
    template<typename T>
    T ask_player(const std::string& title)
    {
        T x = 0;
        std::cout << title;
        while (!( std::cin >> x))
        {
            std::cin.clear();
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Veuillez entrer un nombre/mot correct: " << endl;
            std::cout << title;
        }
        std::cin.clear();
        std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return (x);
    }

public:
    virtual void lets_play();
};


#endif //CARD_GAME_GAMETEMPLATE_H
