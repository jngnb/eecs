// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350
//
//  main.hpp
//  P3

#ifndef main_hpp
#define main_hpp

#include <unordered_map>
#include <deque>
#include <queue>
#include <vector>

using namespace std;

class CompareLogByIdx;

class LogManager{
        
public:
    struct LogEntry{
        size_t ID;
        string ts;
        string cat;
        string msg;
    };
    
    class CompareLog{
    private:
        LogManager* logman;

    public:
        CompareLog(LogManager *lm) : logman(lm){}
        CompareLog(): logman(nullptr){}
        
        bool operator()(const size_t &_lhs, const size_t &_rhs){
        
            LogEntry lhs = logman->masterLog[_lhs];
            LogEntry rhs = logman->masterLog[_rhs];
            CompareLog comp;
            
            return comp(lhs, rhs);
        }

        bool operator()(const LogEntry &lhs, const LogEntry &rhs) {
            const unsigned long long ts1 = ts_convert(lhs.ts);
            const unsigned long long ts2 = ts_convert(rhs.ts);
            
            if (ts1 == ts2){
                string cat1 = lhs.cat;
                string cat2 = rhs.cat;
                
                transform(cat1.begin(), cat1.end(), cat1.begin(), ::tolower);
                transform(cat2.begin(), cat2.end(), cat2.begin(), ::tolower);
                
                if (cat1 == cat2){
                    return lhs.ID < rhs.ID;
                }
                return lhs.cat < rhs.cat;
            }

            return ts1 < ts2;
        }

    };
    
    class BoundCompare{
        
    public:
        bool operator()(const LogEntry &lhs, const LogEntry &rhs) {
            return ts_convert(lhs.ts) < ts_convert(rhs.ts);
        }
    };
    
private:
    
    bool ranged = false;
    bool searchMade = false;
    
    friend CompareLogByIdx;
    
    vector<LogEntry> masterLog = {};
    vector<size_t> translator;
    
    unordered_map<string, vector<size_t>> catHash;
    unordered_map<string, vector<size_t>> keyHash;
    deque<size_t> excerptList;
    queue<size_t> history;
        
public:
        
    //storage
    void store_masterLog(const string &filename);
    void store_cat();
    void store_kw();
    
    //command processing
    void timestamps_search(const string &ts1, const string &ts2);
    void matching_search(const string &ts);
    void category_search(string &cat);
    void keyword_search(const string &kws);
    
    void print_EL();
    void print_history();
    
    void append_history_EL();
    void append_pos_EL(const size_t &pos);
    void delete_entry_EL(const size_t &pos);
    void move_begin_EL(const size_t &pos);
    void move_end_EL(const size_t &pos);
    void clear_EL();
    void sort_EL();
    
    //helper functions
    static unsigned long long ts_convert(const string &ts_str);
    
};



#endif /* main_hpp */
