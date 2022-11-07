// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "Card.h"
using namespace std;

// add any necessary #include or using directives here

// rank and suit names -- do not remove these
constexpr const char* const Card::RANK_TWO;
constexpr const char* const Card::RANK_THREE;
constexpr const char* const Card::RANK_FOUR;
constexpr const char* const Card::RANK_FIVE;
constexpr const char* const Card::RANK_SIX;
constexpr const char* const Card::RANK_SEVEN;
constexpr const char* const Card::RANK_EIGHT;
constexpr const char* const Card::RANK_NINE;
constexpr const char* const Card::RANK_TEN;
constexpr const char* const Card::RANK_JACK;
constexpr const char* const Card::RANK_QUEEN;
constexpr const char* const Card::RANK_KING;
constexpr const char* const Card::RANK_ACE;

constexpr const char* const Card::SUIT_SPADES;
constexpr const char* const Card::SUIT_HEARTS;
constexpr const char* const Card::SUIT_CLUBS;
constexpr const char* const Card::SUIT_DIAMONDS;

// add your code below

Card::Card() {
    rank = RANK_TWO;
    suit = SUIT_SPADES;
}

Card::Card(const std::string &rank_in, const std::string &suit_in){
    assert( rank_in == RANK_TWO || rank_in == RANK_THREE || rank_in == RANK_FOUR ||
    rank_in == RANK_FIVE || rank_in == RANK_SIX || rank_in == RANK_SEVEN || 
    rank_in == RANK_EIGHT || rank_in == RANK_NINE || rank_in == RANK_TEN || 
    rank_in == RANK_JACK || rank_in == RANK_QUEEN || rank_in == RANK_KING || 
    rank_in == RANK_ACE);
    
    assert(suit_in == SUIT_DIAMONDS || suit_in == SUIT_HEARTS 
    || suit_in == SUIT_SPADES || suit_in == SUIT_CLUBS);


    rank = rank_in;
    suit = suit_in;
}

std::string Card::get_rank() const{
    return rank;
}

std::string Card::get_suit() const{
    return suit;
}

std::string Card::get_suit(const std::string &trump) const{
    if(is_left_bower(trump) == true){
        return trump;
    }
    else{
        return suit;
    }
}

bool Card::is_face() const{
    
    if(rank == RANK_JACK || rank == RANK_QUEEN
    || rank == RANK_KING || rank == RANK_ACE){
        return true;
    }

    return false;
}

bool Card::is_right_bower(const std::string &trump) const{
    
    if(rank == RANK_JACK && suit == trump){
        return true;
    }
    
    return false;
}

bool Card::is_left_bower(const std::string &trump) const{
    
    if(rank == RANK_JACK){
        if(
        (trump == SUIT_CLUBS && suit == SUIT_SPADES) || 
        (trump == SUIT_SPADES && suit == SUIT_CLUBS) || 
        (trump == SUIT_HEARTS && suit == SUIT_DIAMONDS) ||
        (trump == SUIT_DIAMONDS && suit == SUIT_HEARTS)){
            return true;
        }
    }

    return false;
}

bool Card::is_trump(const std::string &trump) const{

    if(trump == suit || is_left_bower(trump)){
        return true;
    }

    return false; 
}

//returns indices of cards' ranks in array
vector<int> helper_index(const Card &left, const Card &right){
    vector<int> helper_vec;
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;

    while(left.get_rank() != RANK_NAMES_BY_WEIGHT[i]){
        ++i;
    }

    while(right.get_rank() != RANK_NAMES_BY_WEIGHT[j]){
        ++j;
    }

    while(left.get_suit() != SUIT_NAMES_BY_WEIGHT[k]){
        ++k;
    };

    while(right.get_suit() != SUIT_NAMES_BY_WEIGHT[l]){
        ++l;
    };

    helper_vec.push_back(i);
    helper_vec.push_back(j);
    helper_vec.push_back(k);
    helper_vec.push_back(l);

    return helper_vec;
}

bool operator<(const Card &lhs, const Card &rhs){

    vector<int> vec = helper_index(lhs, rhs);

    if (vec[0] < vec[1]){
        return true;
    }
    else if (vec[0] == vec[1]){   
        if (vec[2] < vec[3]){
            return true;
        }
        else{
        }
    }
    else{
    }
    return false;

}

bool operator<=(const Card &lhs, const Card &rhs){
    
    vector<int> vec = helper_index(lhs, rhs);

    if (vec[0] < vec[1]){
        return true;
    }
    else if (vec[0] == vec[1]){   
        if (vec[2] <= vec[3]){
            return true;
        }
        else{
        }
    }
    else{
    }
    return false;

}

bool operator>(const Card &lhs, const Card &rhs){
    
    vector<int> vec = helper_index(lhs, rhs);

    if (vec[0] > vec[1]){
        return true;
    }
    else if (vec[0] == vec[1]){   
        if (vec[2] > vec[3]){
            return true;
        }
        else{
        }
    }
    else{
    }
    return false;
}

bool operator>=(const Card &lhs, const Card &rhs){
    
    vector<int> vec = helper_index(lhs, rhs);

    if (vec[0] > vec[1]){
        return true;
    }
    else if (vec[0] == vec[1]){   
        if (vec[2] >= vec[3]){
            return true;
        }
        else{
        }
    }
    else{
    }
    return false;
}

bool operator==(const Card &lhs, const Card &rhs){
    
    vector<int> vec = helper_index(lhs, rhs);

    if (vec[0] == vec[1] && vec[2] == vec[3]){
        return true;
    }

    return false;

}

bool operator!=(const Card &lhs, const Card &rhs){
        
    vector<int> vec = helper_index(lhs, rhs);

    if (vec[0] != vec[1] || vec[2] != vec[3]){
        return true;
    }

    return false;

}

std::string Suit_next(const std::string &suit){
    if (suit == Card::SUIT_SPADES){
        return Card::SUIT_CLUBS;
    }
    else if (suit == Card::SUIT_CLUBS){
        return Card::SUIT_SPADES;
    }
    else if (suit == Card::SUIT_HEARTS){
        return Card::SUIT_DIAMONDS;
    }
    else{
        return Card::SUIT_HEARTS;
    }
}

std::ostream & operator<<(std::ostream &os, const Card &card){
    os << card.get_rank() << " of " << card.get_suit();
    return os;
}

bool Card_less(const Card &a, const Card &b, const std::string &trump){
    if (a.is_trump(trump) && !(b.is_trump(trump))){
        return false;
    }
    else if (a.is_trump(trump) && b.is_trump(trump)){
        if(a.is_right_bower(trump)){
            return false;
        }
        else if(b.is_right_bower(trump)){
            return true;
        }
        else{
            if(b.is_left_bower(trump)){
                return true;
            }
            if(a.is_left_bower(trump)){
                return false;
            }
            return (a < b);
        }
    }
    else if(!(a.is_trump(trump)) && b.is_trump(trump)){
        return true;
    }
    else{
        return (a < b);
    }
    
}

bool Card_less(const Card &a, const Card &b, const Card &led_card,
                const std::string &trump){

    if(!(a.is_trump(trump)) && !(b.is_trump(trump))){
        if (a.get_suit() == led_card.get_suit() &&
                (b.get_suit() != led_card.get_suit())){
            return false;
        }
        else if (a.get_suit() != led_card.get_suit() &&
                (b.get_suit() == led_card.get_suit())){
            return true;
        }
    }
    return Card_less(a,b,trump);
    
}




// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec (see the appendices) before implementing
// the following operator overload functions:
//   operator<<
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=
