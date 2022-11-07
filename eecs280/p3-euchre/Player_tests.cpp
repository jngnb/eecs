// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}
TEST(test_add_card){
    Player * alice = Player_factory("Alice", "Simple");
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    alice->add_card(two_spades);

    ASSERT_EQUAL(alice->play_card(two_spades, Card::SUIT_HEARTS), two_spades);
    
    delete alice;
}

TEST(test_add_card2){
    Player * alice = Player_factory("Alice", "Simple");
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
    alice->add_card(king_hearts);
    ASSERT_EQUAL(alice->play_card(two_spades, Card::SUIT_HEARTS), king_hearts);
    
    delete alice;
}

TEST(test_play_card){
    Player * alice = Player_factory("Alice", "Simple");
    Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
    Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);

    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);

    alice->add_card(nine_spades);
    alice->add_card(ace_spades);

    ASSERT_EQUAL(alice->play_card(king_hearts, Card::SUIT_DIAMONDS), nine_spades);
    
    alice->add_card(nine_spades);

    ASSERT_EQUAL(alice->play_card(king_hearts, Card::SUIT_SPADES), nine_spades);

    alice->add_card(jack_hearts);

    ASSERT_EQUAL(alice->play_card(king_hearts, Card::SUIT_SPADES), jack_hearts);

    alice->add_card(jack_diamonds);

    ASSERT_EQUAL(alice->play_card(king_hearts, Card::SUIT_SPADES), jack_diamonds);
    
    delete alice;
}

TEST(test_add_and_discard){

    Player * alice = Player_factory("Alice", "Simple");

    Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);

    Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
    Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

    alice->add_card(king_hearts);
    alice->add_card(ten_spades);
    alice->add_card(ace_spades);
    alice->add_card(nine_spades);
    alice->add_card(king_diamonds);

    alice->add_and_discard(Card(Card::RANK_KING, Card::SUIT_CLUBS));

    Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);
    //king clubs led
    //discard nine spades
    //add king clubs

    //king hearts
    //ten spades
    //ace spades
    //king diamonds
    //king clubs

    ASSERT_EQUAL(alice->play_card(queen_hearts, Card::SUIT_DIAMONDS), king_hearts);
    ASSERT_EQUAL(alice->play_card(queen_hearts, Card::SUIT_DIAMONDS), ten_spades);
    ASSERT_EQUAL(alice->play_card(queen_hearts, Card::SUIT_DIAMONDS), king_clubs);
    ASSERT_EQUAL(alice->play_card(queen_hearts, Card::SUIT_DIAMONDS), ace_spades);
    ASSERT_EQUAL(alice->play_card(queen_hearts, Card::SUIT_DIAMONDS), king_diamonds);
    
    delete alice;
}

TEST(test2_add_and_discard){
    Player * bob = Player_factory("Bob", "Simple");

    Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);
    Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);
    Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);


    bob->add_card(jack_diamonds);
    bob->add_card(king_spades);
    bob->add_card(ace_spades);
    bob->add_card(jack_spades);
    bob->add_card(king_diamonds);

    bob->add_and_discard(Card(Card::RANK_JACK, Card::SUIT_CLUBS));
    
    Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);

    Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);

    ASSERT_EQUAL(bob->play_card(queen_clubs, Card::SUIT_DIAMONDS), jack_clubs);
    ASSERT_EQUAL(bob->play_card(ten_diamonds, Card::SUIT_HEARTS), king_diamonds);

    delete bob;

}

TEST(test_make_trump){
    Player * testc = Player_factory("Bob", "Simple");
    const Card upcard = Card(Card::RANK_TEN,Card::SUIT_DIAMONDS);
    string order_up_suit = Card::SUIT_DIAMONDS;
    int round = 1;
    const Card a = Card(Card::RANK_JACK,Card::SUIT_DIAMONDS);
    const Card b = Card(Card::RANK_JACK,Card::SUIT_HEARTS);
    const Card c = Card(Card::RANK_TEN,Card::SUIT_SPADES);
    testc->add_card(a);
    testc->add_card(b);
    testc->add_card(c);
    bool is_dealer = true;
    ASSERT_TRUE(testc->make_trump(upcard,is_dealer,round,order_up_suit));
    delete testc;
}

TEST(test_play_card2){
    Player * testd = Player_factory("Joe", "Simple");
    const Card led_card = Card(Card::RANK_KING,Card::SUIT_DIAMONDS);
    string trump = Card::SUIT_DIAMONDS;
    const Card a = Card(Card::RANK_ACE,Card::SUIT_DIAMONDS);
    const Card b = Card(Card::RANK_JACK,Card::SUIT_HEARTS);
    const Card c = Card(Card::RANK_TEN,Card::SUIT_SPADES);
    testd->add_card(a);
    testd->add_card(b);
    testd->add_card(c);

    ASSERT_EQUAL(testd->play_card(led_card,trump),b);

    delete testd;
}

TEST(test_lead_card){
    Player * teste = Player_factory("Joe", "Simple");
    string trump = Card::SUIT_HEARTS;
    const Card a = Card(Card::RANK_ACE,Card::SUIT_DIAMONDS);
    const Card b = Card(Card::RANK_JACK,Card::SUIT_DIAMONDS);
    const Card c = Card(Card::RANK_TEN,Card::SUIT_CLUBS);
    const Card d = Card(Card::RANK_TEN,Card::SUIT_SPADES);
    const Card e = Card(Card::RANK_NINE,Card::SUIT_DIAMONDS);
    teste->add_card(a);
    teste->add_card(b);
    teste->add_card(c);

    ASSERT_EQUAL(teste->lead_card(trump), a);

    delete teste;
}

TEST(test2_lead_card){
    Player * boba = Player_factory("boba", "Simple");
    string trump = Card::SUIT_DIAMONDS;
    
    const Card a = Card(Card::RANK_QUEEN,Card::SUIT_DIAMONDS);
    const Card b = Card(Card::RANK_JACK,Card::SUIT_DIAMONDS);
    const Card c = Card(Card::RANK_KING,Card::SUIT_DIAMONDS);
    const Card d = Card(Card::RANK_ACE,Card::SUIT_DIAMONDS);
    const Card e = Card(Card::RANK_NINE,Card::SUIT_DIAMONDS);
    
    boba->add_card(a);
    boba->add_card(b);
    boba->add_card(c);
    boba->add_card(d);
    boba->add_card(e);

    ASSERT_EQUAL(boba->lead_card(trump), b);

    delete boba;
}

// Add more tests here

TEST_MAIN()
