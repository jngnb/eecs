// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "Pack.h"
#include "Player.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
using namespace std;

class Game{
  private: 
    Pack pack;
    bool shuffle;

    Player * players[4];
    int dealer_index = 0;
    int player_ordered_up_index = 0;
    int trick_points[2] = {0, 0};

    vector<int> team1 = {0, 2};
    vector<int> team2 = {1, 3};

    int hand_num = 0;
    int team1_points = 0;
    int team2_points = 0;

  public:
    Game(Pack pack_file_in, bool shuffle_in):
        pack(pack_file_in), shuffle(shuffle_in){};

    const int get_t1_points();
    const int get_t2_points();
    
    void initialize_players(vector<string> names, vector<string> strategies);
    void shuffle_reset();
    void deal_cards();
    Card turn_upcard();
    string make_trump();
    int one_trick(int num, string trump);
    void run_tricks(string trump);
    void march_or_euchred();
    void reset_hands();
    void dealer_next();
    void player_next(int & next_player);
    void delete_players();
};

const int Game::get_t1_points(){
    return team1_points;
}
const int Game::get_t2_points(){
    return team2_points;
}

void Game::initialize_players(vector<string> names, vector<string> strategies){
    for (int i = 0; i < static_cast<int>(names.size()); i++){
        Player * player = Player_factory(names[i], strategies[i]);
        players[i] = player;
    }
}
void Game::shuffle_reset(){
    if (shuffle){
        pack.shuffle();
    }
    else if (!shuffle){
        pack.reset();
    }
}

void Game::deal_cards(){//change to dealer
    cout << "Hand " << hand_num << endl;

    cout << players[dealer_index]->get_name() << " deals" << endl;
    
    int arr[8] = {3, 2, 3, 2, 2, 3, 2, 3};
    int next_index = dealer_index+1;
    if(next_index == 4){ next_index=0; }//edge case
    
    for(int i = 0; i < 8; i++){
        int deal_count = arr[i];
        int j = 0;
        
        while(j<deal_count){
            players[next_index]->add_card(pack.deal_one());
            ++j;
        }
        player_next(next_index);
        next_index++;
    }
}

Card Game::turn_upcard(){
    Card upcard = pack.deal_one();
    cout << upcard << " turned up" << endl;
    return upcard;
}

string Game::make_trump(){
    Card upcard = turn_upcard();
    string up_suit;

    for (int round = 1; round <= 2; ++round){
        int next_index = dealer_index+1;
        if (next_index == 4){next_index = 0;}//edge case
        for (; next_index != dealer_index; ++next_index){
            bool made = players[next_index]->make_trump(upcard, false, round, up_suit);
            string name = players[next_index]->get_name();
            if (made && (round == 1)){
                cout << name << " orders up " << up_suit << endl << endl;
                players[dealer_index]->add_and_discard(upcard); 
                player_ordered_up_index = next_index;
                return up_suit;
            }
            else if (made){
                cout << name << " orders up " << up_suit << endl << endl;
                player_ordered_up_index = next_index;
                return up_suit;
            }
            else if (!made){
                cout << name << " passes" << endl;
                player_next(next_index);
            }
        }
        bool made = players[dealer_index]->make_trump(upcard, true, round, up_suit);
        if (made){
            cout << players[dealer_index]->get_name() << " orders up " << up_suit 
                << endl << endl;
            if (round == 1){
                players[dealer_index]->add_and_discard(upcard);
            }
            else if(round == 2){
                assert(up_suit != upcard.get_suit());
            }
            player_ordered_up_index = dealer_index;
            return up_suit;
        }
        else if(!made){
            assert(round==1);
            cout << players[dealer_index]->get_name() << " passes" << endl;
        }
    }
    return " ";
}

int Game::one_trick(int leader_index, string trump_in){

    Player* leader = players[leader_index];
    Card led = leader->lead_card(trump_in);
    cout << led << " led by " << leader->get_name() << endl;

    Card winning_card = led;
    int winner_index = leader_index;

    int next_index = leader_index+1;
    if (next_index == 4){next_index = 0;} //edge case

    for (; next_index != leader_index; ++next_index){
        Card played = players[next_index]->play_card(led, trump_in);
        cout << played << " played by " << players[next_index]->get_name() << endl;

        if (Card_less(winning_card, played, led, trump_in)){
            winning_card = played;
            winner_index = next_index;
        }

        player_next(next_index);
    }

    if(winner_index == team1[0] || winner_index == team1[1]){
        ++trick_points[0];
    }
    else if (winner_index == team2[0] || winner_index == team2[1]){
        ++trick_points[1];
    }

    cout << players[winner_index]->get_name() << " takes the trick" << endl;
    return winner_index;
}

void Game::run_tricks(string trump_in){
    int leader_idx = dealer_index+1;
    if (leader_idx == 4){
        leader_idx = 0;
    }
    for (int i = 0; i < 5; ++i){
        leader_idx = one_trick(leader_idx, trump_in);
        cout << endl;
    }
    dealer_next();
}

void Game::march_or_euchred(){
    vector<int> winning_team = team1;
    vector<int> losing_team = team2;
    int winning_trick_points = trick_points[0];
    int * team_to_inc = &team1_points;

    if (trick_points[1] > trick_points[0]){
        winning_team = team2;
        losing_team = team1;
        winning_trick_points = trick_points[1];
        team_to_inc = &team2_points;
    }
    string win_mem1 = players[winning_team[0]]->get_name();
    string win_mem2 = players[winning_team[1]]->get_name();

    int pou_idx = player_ordered_up_index;
    cout << win_mem1 << " and " << win_mem2 << " win the hand" << endl;
    
    if ((pou_idx == winning_team[0] || pou_idx == winning_team[1])
        && winning_trick_points == 5){
        (*team_to_inc) += 2;
        cout << "march!" << endl;
    }
    else if ((pou_idx != winning_team[0] && pou_idx != winning_team[1])){
        (*team_to_inc) += 2;
        cout << "euchred!" << endl;
    }
    else{
        (*team_to_inc)++;
    }
    cout << players[team1[0]]->get_name() <<  " and " 
        << players[team1[1]]->get_name() << " have " 
        << team1_points << " points" << endl;
    cout << players[team2[0]]->get_name() << " and " 
        << players[team2[1]]->get_name() << " have " 
        <<  team2_points << " points" << endl << endl;

    trick_points[0] = 0;
    trick_points[1] = 0;
}

void Game::dealer_next(){
    dealer_index++;
    if (dealer_index == 4){
        dealer_index = 0;
    }
    hand_num++;
}

void Game::player_next(int & next_idx){
    if (next_idx == 3){
        next_idx = -1;
    }
    else if (next_idx == 4){
        next_idx = 0;
    }
}

void Game::delete_players(){
    for (int i = 0; i < 4; ++i) {
        delete players[i];
    }
}

//NOT PART OF GAME CLASS
Pack initialize_pack(istream& pack_in){
    Pack pack(pack_in);
    return pack;
}

bool play_euchre(Game euchre, int win_points){
    while (euchre.get_t1_points() < win_points && euchre.get_t2_points() < win_points){
        euchre.shuffle_reset();
        euchre.deal_cards();
        string trump_suit = euchre.make_trump();
        euchre.run_tricks(trump_suit);
        euchre.march_or_euchred();
    }
    return (euchre.get_t1_points() > euchre.get_t2_points());
}

int main(int argc, char ** argv){
    for (int i = 0; i < argc; ++i){ cout << argv[i] << " "; } 
    cout << endl;
    ifstream pack_file(argv[1]);
    if (!pack_file.is_open()){cout << "Error opening " << argv[1] << endl; return 1;}

    bool shuffle;
    if (strcmp(argv[2], "noshuffle")==0){shuffle = false;}
    else if(strcmp(argv[2], "shuffle")==0){shuffle = true;}

    int win_points = atoi(argv[3]);

    vector<string> names;
    for(int i = 4; i < 11; i+=2){ names.push_back(argv[i]); }
    vector<string> strategies;
    for(int i = 5; i < 12; i+=2){ strategies.push_back(argv[i]); }

    int x = 0;

    for (int i = 0; i < static_cast<int>(strategies.size()); ++i){
        if (strategies[i] != "Simple" && strategies[i] != "Human"){x++;}
    }
    if (argc != 12 || x > 0 || win_points < 1 || win_points > 100 ||
        ((strcmp(argv[2],"noshuffle")!=0) && (strcmp(argv[2],"shuffle")!=0))){
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
    }

    Game euchre(initialize_pack(pack_file), shuffle);//initializing game
    euchre.initialize_players(names, strategies); // initializing players
    bool who_won = play_euchre(euchre, win_points);
    
    if(who_won){cout << names[0] << " and " << names[2] << " win!" << endl;}
    else if (!who_won){cout << names[1] << " and " << names[3] << " win!" << endl;}
    euchre.delete_players();
}
