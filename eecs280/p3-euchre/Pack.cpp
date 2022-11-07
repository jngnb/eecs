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
}

Card Pack::deal_one(){
    assert(!empty());
    Card card_to_return = cards[next];
    next++;
    return card_to_return;

}

void Pack::reset() {
    next = 0;
}

void Pack::shuffle(){
    
    for(int i = 0; i < 7; ++i){
        int idx = 0;
        int idx1 = 0;
        int idx2 = 12;
        int idx3 = 1;
        int idx4 = 0;

        std::array<Card, PACK_SIZE> newcards = cards;

        while(idx < 12){
            cards[idx1] = newcards[idx2]; 
            cards[idx3] = newcards[idx4];
        
            idx1 += 2;
            idx2++;
            idx3 += 2;
            idx4++;

            ++idx;
        }
    }
    next = 0;
}


bool Pack::empty() const{

    if (next == PACK_SIZE){
        return true;
    }
    return false;
}


