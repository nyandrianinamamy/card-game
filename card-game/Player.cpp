//
// Created by Ny Andrianina Mamy on 12/12/2020.
//

#include "Player.h"

std::string Player::get_name() const {
    return name;
}

Deck& Player::get_deck() {
    return *deck;
}

void Player::set_deck(std::unique_ptr<Deck>& _deck) {
    deck = std::move(_deck);
}

int Player::get_score() const {
    return score;
}

void Player::set_score(int n) {
    score = n;
}

std::ostream & operator << (std::ostream & out, const Player& player) {
    out << player.get_name();
    return out;
}

void Player::increase_score_by(int n) {
    set_score(score + n);
}
