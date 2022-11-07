// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include "Pack.h"
using namespace std;


Pack::Pack(){

    int idx = 0;
    
    while (idx < PACK_SIZE){
        for (int i = 0; i < NUM_SUITS; ++i){
            for (int j = 7; j < NUM_RANKS; ++j){
                Card card(RANK_NAMES_BY_WEIGHT[j], SUIT_NAMES_BY_WEIGHT[i]);
                cards[idx] = card;
                idx++;
            }
        }
    }
    next = 0;

}

Pack::Pack(std::istream& pack_input){
    string rank, of_ph, suit;
    int idx = 0;
    
    while (idx < PACK_SIZE){
        pack_input >> rank >> of_ph >> suit;
        Card card(rank, suit);
        cards[idx] = card;
        idx++;
    }
    
    next = 0;
};

Card Pack::deal_one(){
    assert(!empty());
    Card card_to_return = cards[next];
    next++;
    return card_to_return;

};

void Pack::reset() {
    next = 0;
}

void Pack::shuffle(){

    std::array<Card, PACK_SIZE> cards_copy = cards;
    std::array<Card, PACK_SIZE> temp_cards;

    for (int count = 0; count < 7; ++count){

        int temp_idx = 0;

        while (temp_idx < PACK_SIZE - 1){
            for (int i = 0; i < PACK_SIZE/2; i++){
                temp_cards[temp_idx] = cards_copy[i];
                temp_cards[temp_idx+1] = cards_copy[i+(PACK_SIZE/2)];
                temp_idx+=2;
            }
        }
        cards_copy = temp_cards;

    }
    cards = temp_cards;

    next = 0;

};

bool Pack::empty() const{
    if (next == PACK_SIZE){
        return true;
    }
    return false;
};