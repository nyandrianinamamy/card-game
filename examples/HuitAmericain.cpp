//
// Created by Kaou Thaar on 02/01/2021.
//

#include <iostream>
#include "HuitAmericain.h"



void HuitAmericain::lets_play(std::vector<std::string> players) {
    // Players
    board.create_players(players);
    GameTemplate::lets_play();
}


void HuitAmericain::initialization() {
    std::cout << "Launching HuitAmericain " << std::endl;

    // Configuration des cartes
    std::vector<std::string> suits{"spade", "club", "diamond", "heart"};
    std::vector<int> range{1,2,3,4,5,6,7,8,9,10,11,12,13};
    DeckBuilder deck_builder;
    std::unique_ptr<Deck> deck = deck_builder.create()
            .with_suits(suits)
            .with_range(range)
            .build();
    // ajouter deux cartes des joker

    deck->add_card("joker", "", 0);
    deck->add_card("joker", "", 0);

    assert(deck->get_nbcards() == 54);

    board.set_deck(std::move(deck));

    // Shuffle deck

    board.shuffle_deck();


    // Split cards to players
    std::vector<std::unique_ptr<Deck>> decks;
    board.get_deck().distribute(board.get_players().size(),7,decks);
    board.affect_decks_toplayers(decks);

}

void HuitAmericain::next_turn() {
    if (board.get_round()==0) { // si tour 0
        first_turn();
    }
    // si non on execute un tour
    excute_round();

}

void HuitAmericain::first_turn(){
    // le tour 0 ie avant de commencer
    // donneur pioche une carte
    Card card = board.get_deck().watch_front_card();
    while (isSpecialCard(card)){ // elle doit pas etre spéciale
        board.get_deck().shuffle();
        card = board.get_deck().watch_front_card();
    }
    // la carte pioché qui doit etre récouverte apres est dans temp deck
    board.get_temp_deck().add_card(board.get_deck().take_front_card());
    board.set_round(1);
}



void HuitAmericain::excute_round() {
    // afficher les cartes de chaque joueur et la carte a récouvrire
    displayPlayerStat(board.get_turn());
    if (board.get_deck().isEmpty()) { // si la pioche est vide
        // on garde juste la première cartes
        std::unique_ptr<Card> card = board.get_temp_deck().take_front_card();
        // on prend le reste des cartes déja jouées pour former de nouveau la pioche
        for (int i=0; i < board.get_temp_deck().get_nbcards();i++) {
            board.get_deck().add_card(board.get_temp_deck().take_front_card());
            board.shuffle_deck();
        }
        board.get_temp_deck().add_card(std::move(card));
    }
    bool ok = chooseCard(); // le joueur courant choisis une carte
    if(!ok){ // si y a pas une carte jouable dans sa main
        // si la carte piochée est jouable il la joue directement
        if (validCard(board.get_deck().watch_front_card()) || isSpecialCard(board.get_deck().watch_front_card())){
            board.get_temp_deck().add_card(board.get_deck().take_front_card());
            std::cout << "carte piochéé est jouée : " << board.get_temp_deck().watch_front_card() << std::endl ;
            // si la dernière carté joué est spécial (traitement spécial)
            if (isSpecialCard(board.get_temp_deck().watch_front_card())) specialProcess ();
            board.next_round();
        }
        else {// sinon il garde la carte dans la main et passe son tour
            board.get_players()[board.get_turn()]->get_deck().add_card(board.get_deck().take_front_card());
            board.next_round();
        }
    }
}



bool HuitAmericain::chooseCard() {
    // pour choisir une carte a jouer
    for (int i= 0; i< board.get_players()[board.get_turn()]->get_deck().get_nbcards();i++)
    {// chercher d'abord s'il ya une carte special ( priviligiée )
        if (isSpecialCard(board.get_players()[board.get_turn()]->get_deck().watch_card_at(i))){
            board.get_temp_deck().add_card(board.get_players()[board.get_turn()]->get_deck().take_card_at(i));
            specialProcess ();
            board.next_round();
            std::cout << "carte jouee : " << board.get_temp_deck().watch_front_card() << std::endl;
            return true;
        }
    }
    for (int i= 0; i< board.get_players()[board.get_turn()]->get_deck().get_nbcards();i++)
    { // si non chercher une carte de meme couleur ou de meme valeur
        if (validCard(board.get_players()[board.get_turn()]->get_deck().watch_card_at(i))){
            board.get_temp_deck().add_card(board.get_players()[board.get_turn()]->get_deck().take_card_at(i));
            board.next_round();
            std::cout << "carte jouee : " << board.get_temp_deck().watch_front_card() << std::endl;
            return true;
        }
    }
    return false;
}


bool HuitAmericain::isSpecialCard(const Card card) {
    // retourne true si la carte est spéciale
    for(int i = 0; i < (int)special_cards.size(); i++){
        if (special_cards[i] == card.get_value()) {
            return true;
        }
    }
    return false;
}


bool HuitAmericain::validCard(Card &card) {
    // une carte valide ( peut etre jouée si elle est de meme couleur ou de meme valeur
    if (card.get_value() ==board.get_temp_deck().watch_front_card().get_value() ||
        card.isSameCol(board.get_temp_deck().watch_front_card())) {
        return true;
    }
    else {return false;}
}



void HuitAmericain::specialProcess() { // pour les cartes spéciales
        switch (board.get_temp_deck().watch_front_card().get_value()) {
            case 1: { // cas d'un ace
                board.reverse_direction(); // inverser la direction
                break;
            }
            case 11: {// cas du valet : passer le tour du joueur suivant
                board.set_turn(board.compute_next_turn());
                break;
            }
            case 2: { // le joueur suivant doit piocher 2 cartes
                for (int i = 0; i < 2; i++) {
                    if (board.get_deck().isEmpty()) {
                        std::unique_ptr<Card> card = board.get_temp_deck().take_front_card();
                        for (int i=0; i < board.get_temp_deck().get_nbcards();i++) {
                            board.get_deck().add_card(board.get_temp_deck().take_front_card());
                            board.shuffle_deck();
                        }
                        board.get_temp_deck().add_card(std::move(card));
                    }
                    board.get_players()[board.compute_next_turn()]->get_deck().add_card(board.get_deck().take_front_card());
                    }
                break;
            }
            case 0 : { // le joueur suivant doit piocher 4 cartes
                for (int i = 0; i < 4; i++) {
                    if (board.get_deck().isEmpty()) {
                        std::unique_ptr<Card> card = board.get_temp_deck().take_front_card();
                        for (int i=0; i < board.get_temp_deck().get_nbcards();i++) {
                            board.get_deck().add_card(board.get_temp_deck().take_front_card());
                            board.shuffle_deck();
                        }
                        board.get_temp_deck().add_card(std::move(card));
                    }
                    board.get_players()[board.compute_next_turn()]->get_deck().add_card(board.get_deck().take_front_card());
                }
                break;
            }
        }
}


bool HuitAmericain::is_the_end() {
    // si un joueur finit toutes les carte dans sa main, il gagne fin de la partie.
    return std::any_of(
            board.get_players().begin(),
            board.get_players().end(),
            [](const std::unique_ptr<Player>& player) {
                return player->get_deck().isEmpty();
            });
}

void HuitAmericain::end_of_game() { // afficher le gagnant
    std::cout << "Fin du jeu " << std::endl;
    for (int i = 0; i < (int) board.get_players().size(); i++) {
        if (board.get_players()[i]->get_deck().isEmpty()) {
            std::cout << board.get_players()[i]->get_name() << " a gagné" << std::endl;
            board.get_players().erase(board.get_players().begin()+i);
        }
    }
    // si on a plus de 2 joueur et que un gagne ,
    // on calcule les points entre le reste des joueurs pour savoir le gagnant entre eux
    if (board.get_players().size() > 1) {
        std::vector<int> scores;
        for (int i = 0; i < (int) board.get_players().size(); i++) {
            std::cout << "le joueur : " << board.get_players()[i]->get_name() << " a comme cartes " << std::endl;
            std::cout << board.get_players()[i]->get_deck();
            std::cout << std::endl;
            int score = 0;
            for (int j = 0; j < board.get_players()[i]->get_deck().get_nbcards(); j++) {
                score += count_card(board.get_players()[i]->get_deck().watch_card_at(j));
            }
            scores.push_back(score);
        }
        std::vector<bool> winners(board.get_players().size());
        for (int j = 0; j < (int) scores.size(); j++) {
            bool ok = true;
            for (int i = 0; i < (int) scores.size(); i++) {
                if (i != j) {
                    if (scores[j] > scores[i]) {
                        ok = false;
                        break;
                    }
                }
            }
            winners[j] = ok;
        }
        for (int j = 0; j < (int) scores.size(); j++) {
            std::cout << "le joueur : " << board.get_players()[j]->get_name() << " a eu le score "
                      << scores[j] << std::endl;
            if (winners[j]) std::cout << " c'est le gagant " << std::endl;
        }

    }
}


void HuitAmericain::displayPlayerStat(int pos) {
        std::cout << "tour numero : "<<board.get_round() << std::endl;
        std::cout << "c'est le tour de joueur " << pos +1 << std::endl;
        std::cout << "le joueur : " << board.get_players()[pos]->get_name() << " a comme cartes " << std::endl;
        std::cout << board.get_players()[pos]->get_deck();
        std::cout << std::endl;
        std::cout << "la carte à recouvrir est : " << board.get_temp_deck().watch_front_card() << std::endl;
}

int HuitAmericain::count_card(Card & card){
    switch (card.get_value()) {
        case 1:{
            return 20;
        }
        case 2:{
            return 20;
        }
        case 11:{
            return 20;
        }
        case 12:{
            return 10;
        }
        case 13:{
            return 10;
        }
        case 8:{
            return 50;
        }
        case 0:{
            return 50;
        }
        default: {
            return card.get_value();
        }   // output number
    }
}


