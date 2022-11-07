// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef stock_hpp
#define stock_hpp

#include <getopt.h>
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>
#include <vector>
#include <queue>


using namespace std;

enum class TimeState{
    NoTrade,
    CanBuy,
    Completed,
    Potential
};

class Order{
    size_t ts;
    size_t tID;
    size_t sID;
    //bool buy
    size_t lim;
    mutable size_t quantity;
    
public:
    Order(size_t ts_in, size_t tID_in, size_t sID_in, /*bool buy_in,*/ size_t lim_in, size_t quantity_in):
        ts(ts_in), tID(tID_in), sID(sID_in), /*buy(buy_in),*/ lim(lim_in), quantity(quantity_in){};
    
    const size_t& getTS() const{
        return ts;
    }
    const size_t& getTID() const{
        return tID;
    }
    const size_t& getSID() const{
        return sID;
    }
    const size_t& getLim() const{
        return lim;
    }
    size_t& getQuantity() const{
        return quantity;
    }
    
};

class CompareBuyer{
public:
    bool operator()(const Order& lhs, const Order& rhs) {
        if (lhs.getLim() == rhs.getLim()){
            return (lhs.getTS() > rhs.getTS());
        }
        return (lhs.getLim() < rhs.getLim());
    }
};

class CompareSeller{
public:
    bool operator()(const Order& lhs, const Order& rhs) {
        if (lhs.getLim() == rhs.getLim()){
            return (lhs.getTS() > rhs.getTS());
        }
        return (lhs.getLim() > rhs.getLim());
    }
};

class Market{
    
    struct Stock;
    struct timeTrStock;
    
    size_t num_stocks;
    size_t num_traders;

    bool verbose = false;
    bool median = false;
    bool tr_info = false;
    bool time_tr = false;
    
    size_t trades_completed = 0;
    size_t current_ts = 0;
    
    size_t bought_at = 0;
    
    vector<Stock> stocks;
    
    vector<priority_queue<size_t>> median_pqs_1;
    vector<priority_queue<size_t, vector<size_t>, greater<size_t>>> median_pqs_2;//does greater<size_t> work as intended?
    
    vector<timeTrStock> ttStocks;
    
    // first size_t = stocks bought
    // second size_t = stocks sold
    vector<pair<int, pair<size_t, size_t>>> traders;
    
public:
    
    void getOpt(int argc, char **argv);
    void runMarket();
    void processOrder(istream &stream);

    bool findMatch(const Order &order, bool buy);
    
    void storeMedian(const size_t &sID, const size_t &price);
    void medianOutput(size_t ts);
    
    void storeTrInfo(const size_t &buyer_tID, const size_t &seller_tID, const size_t quantity);
    void trInfoOutput();
    
    void storeTimeTr(const size_t &sID, const size_t &lim, const size_t &ts, bool buy);
    void timeTrOutput();
    
};

struct Market::Stock{
    priority_queue<Order, vector<Order>, CompareBuyer> buyers;
    priority_queue<Order, vector<Order>, CompareSeller> sellers;
};

struct Market::timeTrStock{
    
    TimeState state = TimeState::NoTrade;
    
    size_t ts1 = 0;
    size_t ts2 = 0;
    size_t sold = 0;
    size_t bought = 0;
    size_t p_ts1 = 0;
    size_t p_sold = 0;
};

#endif /* stock_hpp */
