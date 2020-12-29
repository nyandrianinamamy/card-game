//
// Created by Ny Andrianina Mamy on 12/12/2020.
//

#include "Deck.h"
#include <algorithm>
#include <random>

void Deck::add_card(std::string suit, int value) {
    cards.push_back(std::unique_ptr<Card>(new Card(suit, value)));
}

void Deck::sort() { //trier un paquet
//    for(int i=0;i<cards.size();i++)
//    {
//        for(int j=i+1;j<cards.size()+1;j++)
//        {
//            if(cards[i]->compare(reinterpret_cast<Card &>(cards[j]))==1)
//            {
//                cards[i]->exchange(reinterpret_cast<Card &>(cards[j]));
//            }
//        }
//    }
}

void Deck::shuffle(){ //mélanger un paquet
    std::shuffle(
            std::begin(cards),
            std::end(cards),
            std::default_random_engine{
                    std::random_device{}()
            }
            );
}

unique_ptr<Card> Deck::pick_random() { // prendre une carte du paquet au hasard
    int indice = rand()%(cards.size());
    return move(cards[indice]);
}

unique_ptr<Card>& Deck::pick_card(int pos) { // prendre une carte d'une position précise
    return cards.at(pos);
}

Card& Deck::draw() //pour tirer une carte de la main
{
    Card *cd = new Card(*this->pick_card(cards.size()-1));//en memoirise la carte a tirer
    delete(&cards[cards.size()-1]);
    cards.erase(cards.begin()+cards.size()-1);
    return *cd;//retourner la carte choisie
}

int Deck::get_nbcards() { //nombre de cartes restant dans le paquet
    return cards.size();
}

bool Deck::isEmpty() {
    return this->get_nbcards()==0;
}

void Deck::split_half(vector<std::unique_ptr<Deck>> &decks) {
    decks.push_back(std::unique_ptr<Deck>(new Deck()));
    decks.push_back(std::unique_ptr<Deck>(new Deck()));
    split(decks, 2);
}

void Deck::add_card(std::string label, std::string suit, int value) {
    cards.push_back(std::unique_ptr<Card>(new Card(label, suit, value)));
}
