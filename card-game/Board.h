//
// Created by Ny Andrianina Mamy on 21/12/2020.
//

#ifndef CARD_GAME_BOARD_H
#define CARD_GAME_BOARD_H

#include <vector>
#include <memory>
#include "DeckBuilder.h"
#include "Player.h"

class Board {
private:
    std::unique_ptr<Deck> deck; //52
    std::vector<std::unique_ptr<Player>> players; // 2
    std::unique_ptr<Deck> temp_deck; // Used to hold temporary cards for score computation
    int round;
    int turn;

public:
    explicit Board(): deck{new Deck()}, players{}, temp_deck{new Deck()}, round{0}, turn{0} {
    }
    virtual ~Board() = default;

    void set_deck(std::unique_ptr<Deck> _deck);

    Deck& get_deck();

    Deck& get_temp_deck();

    void increase_round();

    int get_round() const;

    int get_turn() const;

    void set_turn(int _turn);

    void shuffle_deck();

    void create_players(std::vector<string>& _players);

    void add_player(std::unique_ptr<Player>& player);

    void affect_decks_toplayers(std::vector<std::unique_ptr<Deck>>& decks);

    std::vector<std::unique_ptr<Player>>& get_players();

};

#endif //CARD_GAME_BOARD_H
