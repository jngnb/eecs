// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include "Player.h"
#include <algorithm>
#include <vector>

using namespace std;

class SimplePlayer : public Player {
  private:
    string name;
    vector<Card> hand;

  public: 
    SimplePlayer(const string &name_in) : name(name_in){};

    const std::string & get_name() const;

    void add_card(const Card &c);

    bool make_trump(const Card &upcard, bool is_dealer,
      int round, std::string &order_up_suit) const;

    void add_and_discard(const Card &upcard);

    Card lead_card(const std::string &trump);

    Card play_card(const Card &led_card, const std::string &trump);

};

const std::string & SimplePlayer::get_name() const{
  return name;
}

void SimplePlayer::add_card(const Card &c){
  assert(hand.size() < MAX_HAND_SIZE);
  hand.push_back(c);
}

bool SimplePlayer::make_trump(const Card &upcard, bool is_dealer,
      int round, std::string &order_up_suit) const{
        assert(round == 1 || round == 2);
        const string upcard_suit = upcard.get_suit();
        if(round == 1){
          int face_count = 0;
          for(int i = 0; i < static_cast<int>(hand.size()); ++i){
            if(hand[i].is_left_bower(upcard_suit)||hand[i].is_right_bower(upcard_suit)
              || (hand[i].is_face() && hand[i].is_trump(upcard_suit))){
              ++face_count;
            } 
          }
          if(face_count > 1){
            order_up_suit = upcard_suit;
            return true;
          }
          else if(face_count <= 1){
            return false;
          }
        }
        
        else if(round == 2){
          int face_count = 0;
          string upcard_suit2 = Suit_next(upcard_suit);
          if(is_dealer){
            order_up_suit = upcard_suit2;
            return true;
          }
          for(int i = 0; i < static_cast<int>(hand.size()); ++i){
            if(hand[i].is_left_bower(upcard_suit2)||hand[i].is_right_bower(upcard_suit2)
            || (hand[i].is_face() && hand[i].is_trump(upcard_suit2))){
              ++face_count;
            } 
          }
          if(face_count > 0){
            order_up_suit = upcard_suit2;
            return true;
          }
          else if(face_count == 1){
            return false;
          }
        }
        return false;
  }

  void SimplePlayer::add_and_discard(const Card &upcard){
    assert(hand.size() > 0);
    string upcard_suit = upcard.get_suit();
    int min_index = 0;
    Card min_card = hand[0];

    for(int i = 0; i < static_cast<int>(hand.size()) /*- 1*/; ++i){
      if(Card_less(hand[i], min_card, upcard_suit)){
        min_index = i;
        min_card = hand[i];
      }
    }
    if(Card_less(min_card, upcard, upcard_suit)){
      hand[min_index] = upcard;
    }
  }

  Card SimplePlayer::lead_card(const std::string &trump){
  assert(hand.size() > 0);
  //sort(hand.begin(), hand.end());
  Card high = hand[0];
  int high_idx = 0;

  int non_trump_count = 0;
  for(int i = 0; i < static_cast<int>(hand.size()); ++i){
    if(!hand[i].is_trump(trump)){
      ++non_trump_count;
    }
  }

  if(non_trump_count > 0){
    int i = 0;
    while(hand[i].is_trump(trump)){
      ++i;
    }
    high = hand[i];
    high_idx = i;
    for(; i < static_cast<int>(hand.size()); ++i){
      if(high < hand[i] && !hand[i].is_trump(trump)){
        high = hand[i];
        high_idx = i;
      }
    }
    hand.erase(hand.begin()+high_idx);
    return high;
  }

  else{
    for(int i = 0; i < static_cast<int>(hand.size()); ++i){
      if(Card_less(high,hand[i],trump)){
        high = hand[i];
        high_idx = i;
      }
    }
    hand.erase(hand.begin()+high_idx);
    return high;
  }

}


  Card SimplePlayer::play_card(const Card &led_card, const std::string &trump){
    assert(hand.size() > 0);

    Card card_played = hand[0];
    int led_suit_count = 0;
    int idx = 0;
    
    if (card_played.get_suit(trump) == led_card.get_suit(trump)){
          led_suit_count++;
    }
        
    for (int i = 0; i < static_cast<int>(hand.size()) /*- 1*/; ++i){
      if ((hand[i].get_suit(trump) == led_card.get_suit(trump))
            && ((Card_less(card_played, hand[i],trump))||
                card_played.get_suit(trump)!=led_card.get_suit(trump))){
        
        card_played = hand[i];
        led_suit_count++;
        idx = i;

      }
    }

    if (led_suit_count==0){
      for (int i = 0; i < static_cast<int>(hand.size()) /*- 1*/; ++i){
        if (Card_less(hand[i], card_played, trump)){

          card_played = hand[i];
          idx = i;

        }
      }
      
    }
    hand.erase(hand.begin() + idx);
    return card_played;
    
  }

  class HumanPlayer : public Player{
    private:
      string name;
      vector<Card> hand;

    public: 
    HumanPlayer (const string &name_in) : name(name_in){};

    const std::string & get_name() const;

    void add_card(const Card &c);

    bool make_trump(const Card &upcard, bool is_dealer,
      int round, std::string &order_up_suit) const;

    void add_and_discard(const Card &upcard);

    Card lead_card(const std::string &trump);

    Card play_card(const Card &led_card, const std::string &trump);

  };

const std::string & HumanPlayer::get_name() const{
  return name;
}

void HumanPlayer::add_card(const Card &c){
  assert(hand.size() < MAX_HAND_SIZE);
  hand.push_back(c);
  sort(hand.begin(), hand.end());
}

bool HumanPlayer::make_trump(const Card &upcard, bool is_dealer,
      int round, std::string &order_up_suit) const{

  string intro = "Human player " + get_name();

  for (int i = 0; i < static_cast<int>(hand.size()); ++i){
    cout << intro << "'s hand: [" << to_string(i) << "] " << hand[i] << endl;
  }

  cout << intro << ", please enter a suit, or \"pass\":" << endl;

  string decision;
  cin >> decision;

  if (decision == "pass") {return false;}
  else {order_up_suit = decision; return true;}

}

void HumanPlayer::add_and_discard(const Card &upcard){
  string up_suit;
   
  //if (make_trump(upcard, true, 1, up_suit)){

    string intro = "Human player " + get_name();

    for (int i = 0; i < static_cast<int>(hand.size()); ++i){
      cout << intro << "'s hand: [" << to_string(i) << "] " << hand[i] << endl;
    }

    cout << "Discard upcard: [-1]" << endl;
    cout << intro << ", please select a card to discard:" << endl;   

    int decision;
    cin >> decision;
    
    if (decision == -1) {}
    else {
      hand[decision] = upcard;
    }
    
  //}
}

Card HumanPlayer::lead_card(const std::string &trump){
  //sort(hand.begin(), hand.end());
  string intro = "Human player " + get_name();

  for (int i = 0; i < static_cast<int>(hand.size()); ++i){
    cout << intro << "'s hand: [" << to_string(i) << "] " << hand[i] << endl;
  }
  cout << intro << ", please select a card:" << endl;  
  
  int decision;
  cin >> decision;

  Card led = hand[decision];

  //cout << led << " led by " << get_name();

  hand.erase(hand.begin()+decision);

  return led;
}

Card HumanPlayer::play_card(const Card &led_card, const std::string &trump){
  //sort(hand.begin(), hand.end());
  string intro = "Human player " + get_name();

  for (int i = 0; i < static_cast<int>(hand.size()); ++i){
    cout << intro << "'s hand: [" << to_string(i) << "] " << hand[i] << endl;
  }
  cout << intro << ", please select a card:" << endl;  
  
  int decision;
  cin >> decision;

  Card played = hand[decision];
  
  hand.erase(hand.begin()+decision);

  return played;
}


Player * Player_factory(const std::string &name, const std::string &strategy) {
  if (strategy == "Simple"){
    return new SimplePlayer(name);
  }
  else if (strategy == "Human"){
    return new HumanPlayer(name);
  }

  assert(false);
  return nullptr;
  
}

std::ostream & operator<<(std::ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}

