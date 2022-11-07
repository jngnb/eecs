// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6
//  stock.cpp
//  P2A
//
//  Created by Jeongin Bae on 5/13/22.
//

#include <getopt.h>
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>
#include <vector>
#include <queue>
#include <sstream>
#include "stock.hpp"
#include "xcode_redirect.hpp"
#include "P2random.h"


void Market::getOpt(int argc, char **argv){
    
    opterr = false;
    int choice;
    int option_index = 0;
    option long_options[] = {
        { "verbose",        no_argument, nullptr, 'v' },
        { "median",         no_argument, nullptr, 'm' },
        { "trader_info",    no_argument, nullptr, 'i' },
        { "time_travelers", no_argument, nullptr, 't' },
        { nullptr,          0,           nullptr, '\0' }
    };
    
    while ((choice = getopt_long(argc, argv, "vmit", long_options, &option_index)) != -1){
        
        switch (choice){
            case 'v':{
                verbose = true;
                break;
            }
            case 'm':{
                median = true;
                break;
            }
            case 'i':{
                tr_info = true;
                break;
            }
            case 't':{
                time_tr = true;
                break;
            }
                
            default:{
                cerr << "Unknown command line option\n";
                exit(1);
                
            }
        }
    }
}

void Market::runMarket(){
    
    cout << "Processing orders...\n";
    
    string junk;
    getline(cin, junk);
    
    string mode;
    
    unsigned int seed;
    unsigned int num_orders;
    unsigned int rate;
    
    cin >> junk >> mode
        >> junk >> num_traders
        >> junk >> num_stocks;
    
    stringstream ss;
    
    if (mode == "PR") {
        cin >> junk >> seed
            >> junk >> num_orders
            >> junk >> rate;
        P2random::PR_init(ss, seed, (unsigned int) num_traders, (unsigned int) num_stocks, num_orders, rate);
    }
    
    stocks.resize(num_stocks);
    
    if (median){
        median_pqs_1.resize(num_stocks);
        median_pqs_2.resize(num_stocks);
    }
    if (tr_info){
        traders.resize(num_traders);
    }
    if (time_tr){
        ttStocks.resize(num_stocks);
    }
    
    //size_t ts = 0;
    //size_t id = 0;
    
    if (mode == "PR"){
        processOrder(ss);
    }
    else{
        processOrder(cin);
    }
}

void Market::processOrder(istream &stream){
    
    int ts;
    size_t id = 0;
    
    while (stream >> ts){
        
        if (ts < 0){
            cerr << "Error: Negative timestamp\n";
            exit(1);
        }
        else if (ts < (signed)current_ts){
            cerr << "Error: Decreasing timestamp\n";
            exit(1);
        }
        
        if (median && (unsigned)ts - 1 != current_ts){
            medianOutput((unsigned)ts - 1);
            current_ts = (unsigned)ts - 1;
        }
        
        char junk;
        bool buy = false;
        int tID;
        int sID;
        int lim;
        int quantity;
        
        size_t identifier = id++;
        
        stream >> junk;
        if (junk == 'B'){
            buy = true;
            stream >> junk >> junk;
        }
        else{
            stream >> junk >> junk >> junk;
        }
        
        stream  >> junk >> tID
                >> junk >> sID
                >> junk >> lim
                >> junk >> quantity;
        
        if (tID < 0 || tID >= (signed)num_traders){
            cerr << "Error: Invalid trader ID\n";
            exit(1);
        }
        if (sID < 0 || sID >= (signed)num_stocks){
            cerr << "Error: Invalid stock ID\n";
            exit(1);
        }
        if (lim <= 0){
            cerr << "Error: Invalid price\n";
            exit(1);
        }
        if (quantity <= 0){
            cerr << "Error: Invalid quantity\n";
            exit(1);
        }
        
        Order current_order(identifier, (unsigned)tID, (unsigned)sID, (unsigned)lim, (unsigned)quantity);
        
        findMatch(current_order, buy);
        
        if (time_tr){
            storeTimeTr((unsigned)sID, (unsigned)lim, (unsigned)ts, buy);
        }
        
    }//while there is input
    
    if (median){
        medianOutput((unsigned)ts);
    }
    cout << "---End of Day---\n"
         << "Trades Completed: " << trades_completed << '\n';
    
    if (tr_info){
        trInfoOutput();
    }
    if (time_tr){
        timeTrOutput();
    }
    
}

bool Market::findMatch(const Order &order, bool buy){
    Stock &temp_stock = stocks[order.getSID()];
    bool find_again = false;
        
    if (buy){
        if (temp_stock.sellers.empty()){
            temp_stock.buyers.push(order);
            return false;
        }
        
        const Order &temp_order = temp_stock.sellers.top();
        
        size_t buy_quantity = order.getQuantity();
        size_t sell_quantity = temp_order.getQuantity();
        size_t buy_lim = order.getLim();
        size_t sell_lim = temp_order.getLim();
        size_t tID = temp_order.getTID();
        
        if (buy_lim >= sell_lim){
            //verbose output
            if (buy_quantity > sell_quantity){
                order.getQuantity() =
                    buy_quantity - sell_quantity;

                buy_quantity = sell_quantity;
                find_again = true;
                
            }
            //else subtract the #sold from seller
            else if (buy_quantity < sell_quantity){
                temp_order.getQuantity() =
                    sell_quantity - buy_quantity;
                order.getQuantity() = 0;
            }
            //if all stocks of the seller are sold, pop the seller
            else{
                temp_stock.sellers.pop();
                order.getQuantity() = 0;
            }
            
            trades_completed++;
            
            if (verbose){
                cout << "Trader " << order.getTID()
                     << " purchased " << buy_quantity
                     << " shares of Stock " << order.getSID()
                     << " from Trader " << tID
                     << " for $" << sell_lim << "/share\n";
            }
            
            bought_at = sell_lim;
            
            if (median){
                storeMedian(order.getSID(), bought_at);
            }
            if (tr_info){
                storeTrInfo(order.getTID(), tID, buy_quantity);
            }
            
            
            if (find_again){
                temp_stock.sellers.pop();
                bool match_found = findMatch(order, buy);
                if (match_found && order.getQuantity() > 0){
                    temp_stock.buyers.push(order);
                }
                else if (!match_found){
                    return false;
                }
            }
            
            return true;
        }
        else {
            temp_stock.buyers.push(order);
            return false;
        }
    }
    
    else{
        
        if (temp_stock.buyers.empty()){
            temp_stock.sellers.push(order);
            return false;
        }
        
        const Order &temp_order = temp_stock.buyers.top();
        
        size_t sell_quantity = order.getQuantity();
        size_t buy_quantity = temp_order.getQuantity();
        size_t sell_lim = order.getLim();
        size_t buy_lim = temp_order.getLim();
        size_t tID = temp_order.getTID();
        
        if (sell_lim <= buy_lim){
            
            //subtract the #sold from seller
            if (sell_quantity > buy_quantity){
                order.getQuantity() =
                    sell_quantity - buy_quantity;
                find_again = true;

            }
            else if (sell_quantity < buy_quantity){
                temp_order.getQuantity() =
                    buy_quantity - sell_quantity;
                buy_quantity = sell_quantity;
                order.getQuantity() = 0;
                
            }
            //if buyer buys the stocks as requested, pop the buyer
            else{
                temp_stock.buyers.pop();
                order.getQuantity() = 0;
            }
            
            trades_completed++;
            
            if (verbose){
                cout << "Trader " << tID
                     << " purchased " << buy_quantity
                     << " shares of Stock " << order.getSID()
                     << " from Trader " << order.getTID()
                     << " for $" << buy_lim << "/share\n";
            }
            
            bought_at = buy_lim;
            
            if (median){
                storeMedian(order.getSID(), bought_at);
            }
            if (tr_info){
                storeTrInfo(tID, order.getTID(), buy_quantity);
            }
            
            if (find_again){
                temp_stock.buyers.pop();
                bool match_found = findMatch(order, buy);
                if (match_found && order.getQuantity() > 0){
                    temp_stock.sellers.push(order);
                }
                else if (!match_found){
                    return false;
                }
            }
            
            return true;
        }
        else{
            temp_stock.sellers.push(order);
            return false;
        }
    }

}

//Median match price of Stock <STOCK_ID> at time <TIMESTAMP> is $<MEDPRICE>
void Market::medianOutput(size_t ts){
    
    for (size_t i = 0; i < median_pqs_1.size(); ++i){
        
        priority_queue<size_t> &temp_pq1 = median_pqs_1[i];
        priority_queue<size_t, vector<size_t>, greater<size_t>> &temp_pq2 = median_pqs_2[i];
        
        size_t value;
        
        if (temp_pq1.size() + temp_pq2.size() > 0){
            
            if (temp_pq1.size() > temp_pq2.size()){
                value = temp_pq1.top();
            }
            else if (temp_pq2.size() > temp_pq1.size()){
                value = temp_pq2.top();
            }
            else{
                value = (temp_pq1.top() + temp_pq2.top())/2;
            }
            cout << "Median match price of Stock " << i
                 << " at time " << ts << " is $" << value << '\n';
        }
    }
}

void Market::storeMedian(const size_t &sID, const size_t &price){
    
    priority_queue<size_t> &temp_pq1 = median_pqs_1[sID];
    priority_queue<size_t, vector<size_t>, greater<size_t>> &temp_pq2 = median_pqs_2[sID];

    if (temp_pq1.empty() ||
        price <= temp_pq1.top()){
        
        temp_pq1.push(price);
        if (temp_pq1.size() == temp_pq2.size()+2){
            temp_pq2.push(temp_pq1.top());
            temp_pq1.pop();
        }
    }
    else if (temp_pq2.empty() ||
             price >= temp_pq2.top()){
        
        temp_pq2.push(price);
        if (temp_pq2.size() == temp_pq1.size()+2){
            temp_pq1.push(temp_pq2.top());
            temp_pq2.pop();
        }
    }
    else {
        if (temp_pq1.size() <= temp_pq2.size()){
            temp_pq1.push(price);
        }
        else{
            temp_pq2.push(price);
        }
    }
}

void Market::storeTrInfo(const size_t &buyer_tID, const size_t &seller_tID, const size_t quantity){
    
    pair<int, pair<size_t, size_t>> &buyer_info = traders[buyer_tID];
    pair<int, pair<size_t, size_t>> &seller_info = traders[seller_tID];
    
    buyer_info.first += -1 * (int)(bought_at * quantity);
    buyer_info.second.first += quantity;
    
    seller_info.first += (int)(bought_at * quantity);
    seller_info.second.second += quantity;
}

//Trader <TRADER_ID> bought <NUMBER_BOUGHT> and sold <NUMBER_SOLD> for a net transfer of $<NET_VALUE_TRADED>(newline)
void Market::trInfoOutput(){
    cout << "---Trader Info---\n";
    for(size_t i = 0; i < traders.size(); ++i){
        cout << "Trader " << i << " bought " << traders[i].second.first
             << " and sold " << traders[i].second.second
             << " for a net transfer of $" << traders[i].first << '\n';
    }
    
}

void Market::storeTimeTr(const size_t &sID, const size_t &lim, const size_t &ts, bool buy){
    
    timeTrStock &stock = ttStocks[sID];
    
    // first seller of specific stock
    if (stock.state == TimeState::NoTrade){
        if (!buy){ //first SELLER enters
            stock.ts1 = ts;
            stock.sold = lim;
            stock.state = TimeState::CanBuy;
        }
    }
    // first ELIGIBLE buyer of specific stock
    else if (stock.state == TimeState::CanBuy){
        if (!buy && lim < stock.sold){
            stock.ts1 = ts;
            stock.sold = lim;
        }
        if (buy && lim > stock.sold){
            stock.ts2 = ts;
            stock.bought = lim;
            stock.state = TimeState::Completed;
        }
    }
    // better (higher) buyer enters --> state doesn't change
    // or better (lower) seller enters --> state = potential
    else if (stock.state == TimeState::Completed){
        if (buy && (lim > stock.bought)){
            stock.bought = lim;
            stock.ts2 = ts;
        }
        else if (!buy && (lim < stock.sold)){
            stock.p_ts1 = ts;
            stock.p_sold = lim;
            stock.state = TimeState::Potential;
        }
    }
    // better (produces more profit than current bought-sold) enters -->
    // state = completed
    else{
        if (!buy && (lim < stock.p_sold)){
            stock.p_ts1 = ts;
            stock.p_sold = lim;
        }
        else if (buy &&
            ((int)lim - (int)stock.p_sold) > (int)(stock.bought - stock.sold)){
            stock.ts1 = stock.p_ts1;
            stock.sold = stock.p_sold;
            stock.bought = lim;
            stock.ts2 = ts;
            stock.state = TimeState::Completed;
        }
    }
}

void Market::timeTrOutput(){
    cout << "---Time Travelers---\n";
    for(size_t i = 0; i < ttStocks.size(); ++i){
        
        const timeTrStock &stock = ttStocks[i];
        const TimeState &state = stock.state;
        
        if (state == TimeState::NoTrade ||
            state == TimeState::CanBuy) {
            cout << "A time traveler could not make a profit on Stock "
                 << i << '\n';
        }
        else if (state == TimeState::Completed ||
                 state == TimeState::Potential){
            cout << "A time traveler would buy Stock " << i << " at time "
                 << stock.ts1 << " for $" << stock.sold
                 << " and sell it at time " << stock.ts2
                 << " for $" << stock.bought << '\n';
        }
    }
}

int main(int argc, char** argv){
    
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    
    Market market;
    market.getOpt(argc, argv);
    market.runMarket();
    
    return 0;
}



