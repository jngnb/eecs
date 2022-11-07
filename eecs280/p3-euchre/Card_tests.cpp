// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>
#include <fstream>

using namespace std;


TEST(test_card_ctor) {
    Card c(Card::RANK_ACE, Card::SUIT_HEARTS);
    ASSERT_EQUAL(Card::RANK_ACE, c.get_rank());
    ASSERT_EQUAL(Card::SUIT_HEARTS, c.get_suit());

    Card c2("Ace", "Hearts");
    ASSERT_EQUAL(Card::RANK_ACE, c2.get_rank());
    ASSERT_EQUAL(Card::SUIT_HEARTS, c2.get_suit());
}

// Add more test cases here
TEST(test_card_default_ctor){
    Card c;
    ASSERT_EQUAL(Card::RANK_TWO, c.get_rank());
    ASSERT_EQUAL(Card::SUIT_SPADES, c.get_suit());
}

TEST(test_card_get_suit_trump){
    Card c(Card::RANK_JACK, Card::SUIT_CLUBS);
    ASSERT_EQUAL(Card::RANK_JACK, c.get_rank());
    ASSERT_EQUAL(Card::SUIT_SPADES, c.get_suit(Card::SUIT_SPADES));

    Card c2(Card::RANK_KING, Card::SUIT_CLUBS);
    ASSERT_EQUAL(Card::SUIT_CLUBS, c2.get_suit(Card::SUIT_SPADES));

    Card c3(Card::RANK_JACK, Card::SUIT_HEARTS);
    ASSERT_EQUAL(Card::SUIT_DIAMONDS, c3.get_suit(Card::SUIT_DIAMONDS));

    Card c4(Card::RANK_THREE, Card::SUIT_HEARTS);
    ASSERT_EQUAL(Card::SUIT_HEARTS, c4.get_suit(Card::SUIT_DIAMONDS));

}

TEST(test_card_is_face){
    Card c(Card::RANK_TWO, Card::SUIT_DIAMONDS);
    Card d(Card::RANK_ACE, Card::SUIT_SPADES);
    ASSERT_FALSE(c.is_face());
    ASSERT_TRUE(d.is_face());
}

TEST(test_is_bower){
    Card c(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card d(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    Card e(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card f(Card::RANK_KING, Card::SUIT_DIAMONDS);
    ASSERT_TRUE(e.is_right_bower(Card::SUIT_DIAMONDS));
    ASSERT_FALSE(f.is_right_bower(Card::SUIT_DIAMONDS));
    ASSERT_TRUE(c.is_left_bower(Card::SUIT_HEARTS));
    ASSERT_FALSE(d.is_left_bower(Card::SUIT_HEARTS));
}

TEST(test_is_trump){
    Card c(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card d(Card::RANK_NINE, Card::SUIT_CLUBS);
    ASSERT_TRUE(c.is_trump(Card::SUIT_HEARTS));
    ASSERT_FALSE(d.is_trump(Card::SUIT_SPADES));
}

TEST(test_card_operators){
    Card two_spades(Card::RANK_TWO, Card::SUIT_SPADES);

    Card three_spades(Card::RANK_THREE, Card::SUIT_SPADES);
    ASSERT_TRUE(two_spades<three_spades);
    ASSERT_FALSE(two_spades>three_spades);
    ASSERT_TRUE(three_spades>two_spades);
    ASSERT_FALSE(three_spades<two_spades);

    ASSERT_TRUE(two_spades!=three_spades);
    ASSERT_FALSE(two_spades==three_spades);

    Card two_hearts(Card::RANK_TWO, Card::SUIT_HEARTS);
    ASSERT_TRUE(two_spades<two_hearts);
    ASSERT_FALSE(two_spades>two_hearts);
    ASSERT_TRUE(two_hearts>two_spades);
    ASSERT_FALSE(two_hearts<two_spades);

    Card two_diamonds(Card::RANK_TWO, Card::SUIT_DIAMONDS);    
    ASSERT_TRUE(two_spades<two_diamonds);
    ASSERT_TRUE(two_spades<=two_diamonds);
    ASSERT_TRUE(two_diamonds>two_spades);
    ASSERT_TRUE(two_diamonds>=two_spades);

    Card ace_spades(Card::RANK_ACE, Card::SUIT_SPADES);
    ASSERT_TRUE(two_spades<ace_spades);
    ASSERT_TRUE(ace_spades>two_spades);

    Card ace_diamonds(Card::RANK_ACE, Card::SUIT_DIAMONDS);
    ASSERT_TRUE(ace_diamonds>ace_spades && ace_diamonds>=ace_spades);
    ASSERT_TRUE(ace_diamonds>two_spades);

}

TEST(test_Suit_next){
    Card c(Card::RANK_KING, Card::SUIT_DIAMONDS);
    Card d(Card::RANK_THREE, Card::SUIT_SPADES);

    ASSERT_EQUAL(Suit_next(Card::SUIT_SPADES), Card::SUIT_CLUBS);
    ASSERT_EQUAL(Suit_next(Card::SUIT_DIAMONDS), Card::SUIT_HEARTS);

}

TEST(test_card_print){
    Card ace_diamonds(Card::RANK_ACE, Card::SUIT_DIAMONDS);
    ostringstream oss;
    oss << ace_diamonds;
    ASSERT_EQUAL(oss.str(), "Ace of Diamonds");
}

TEST(test_card_less_1){
    Card c(Card::RANK_KING, Card::SUIT_DIAMONDS);
    Card d(Card::RANK_KING, Card::SUIT_SPADES);
    Card e(Card::RANK_ACE, Card::SUIT_DIAMONDS);
    Card f(Card::RANK_TWO, Card::SUIT_HEARTS);
    ASSERT_TRUE(Card_less(c,d,Card::SUIT_SPADES));
    ASSERT_TRUE(Card_less(e,f,Card::SUIT_HEARTS));
}

TEST(test_card_card_less_trump_led){

    Card two_diamonds(Card::RANK_TWO, Card::SUIT_DIAMONDS);
    Card three_diamonds(Card::RANK_THREE, Card::SUIT_DIAMONDS);

    Card two_spades(Card::RANK_TWO, Card::SUIT_SPADES);
    Card two_hearts(Card::RANK_TWO, Card::SUIT_HEARTS);
    Card two_clubs(Card::RANK_TWO, Card::SUIT_CLUBS);
    
    Card three_spades(Card::RANK_THREE, Card::SUIT_SPADES);
    ASSERT_TRUE(Card_less(two_spades, three_spades, two_diamonds, Card::SUIT_HEARTS));
    ASSERT_TRUE(Card_less(two_spades, three_spades, two_spades, Card::SUIT_HEARTS));
    ASSERT_TRUE(Card_less(two_spades, two_diamonds, two_hearts, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(two_spades, two_diamonds, three_spades, Card::SUIT_HEARTS));
    ASSERT_TRUE(Card_less(two_spades, three_diamonds, two_hearts, Card::SUIT_HEARTS));
    ASSERT_TRUE(Card_less(two_spades, two_diamonds, three_spades, Card::SUIT_DIAMONDS));
    ASSERT_FALSE(Card_less(two_spades, three_diamonds, two_diamonds, Card::SUIT_SPADES));

    Card jack_spades(Card::RANK_JACK, Card::SUIT_SPADES);
    Card king_spades(Card::RANK_KING, Card::SUIT_SPADES);
    Card jack_clubs(Card::RANK_JACK, Card::SUIT_CLUBS);

    ASSERT_TRUE(Card_less(jack_clubs, jack_spades, two_hearts, Card::SUIT_SPADES));
    ASSERT_TRUE(Card_less(jack_clubs, jack_spades, two_spades, Card::SUIT_SPADES));
    ASSERT_TRUE(Card_less(jack_clubs, jack_spades, two_clubs, Card::SUIT_SPADES));
    ASSERT_FALSE(Card_less(jack_clubs, jack_spades, two_hearts, Card::SUIT_CLUBS));

    ASSERT_FALSE(Card_less(jack_spades, king_spades, two_hearts, Card::SUIT_CLUBS));
    ASSERT_TRUE(Card_less(jack_spades, king_spades, two_spades, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(jack_spades, king_spades, two_spades, Card::SUIT_SPADES));
    ASSERT_FALSE(Card_less(jack_spades, king_spades, two_spades, Card::SUIT_CLUBS));


}




TEST_MAIN()
