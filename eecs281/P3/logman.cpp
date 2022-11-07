// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350
//
//  main.cpp
//  P3

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <strings.h>
#include <ctype.h>
#include <deque>
#include <queue>
#include <vector>

#include "logman.hpp"
#include "xcode_redirect.hpp"

using namespace std;

//TODO: O case sensitivity
//      O helper function for timestamps
//      O sort master log before umaps
//      O excerpt list with functor
//      O set intersection
//          O re-order history
//      O error checking
//      O translator vector
//      O sync with stdio

void LogManager::store_masterLog(const string &filename){
    ifstream fin;
    fin.open(filename);
    
    if (!fin.is_open()){
        cerr << "File failed to open\n";
        exit(1);
    }
    
    string ts, cat, msg;
    size_t i = 0;
    while (getline(fin, ts, '|')){
        getline(fin, cat, '|');
        getline(fin, msg, '\n');
        
        LogEntry temp;
        temp.ID = i++;
        temp.ts = ts;
        temp.cat = cat;
        temp.msg = msg;
                
        masterLog.push_back(temp);
    }
    
    cout << masterLog.size() << " entries read\n";
    
    //resize translating vector
    translator.resize(masterLog.size());
    
    //sort by timestamp
    sort(masterLog.begin(), masterLog.end(), CompareLog());
    
    //initializing translator
    for (size_t i = 0; i < masterLog.size(); ++i){
        translator[masterLog[i].ID] = i;
    }

}

void LogManager::store_cat(){
    for (size_t i = 0; i < masterLog.size(); ++i){
        
        string temp = masterLog[i].cat;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        catHash[temp].push_back(i);
    }
}

void LogManager::store_kw(){
    for (size_t i = 0; i < masterLog.size(); ++i){
        
        string &temp_cat = masterLog[i].cat;
        string &temp_msg = masterLog[i].msg;
        
        for (size_t j = 0; j < temp_cat.length(); ++j){
            string temp;
            
            //builds every word
            while (isalnum(temp_cat[j])){
                temp += temp_cat[j++];
            }
            transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
            
            //if not already in the vector add to vector
            if (!binary_search(keyHash[temp].begin(),
                               keyHash[temp].end(), i)){
                keyHash[temp].push_back(i);
            }
            
        }//category traversal
        
        for (size_t j = 0; j < temp_msg.length(); ++j){
            string temp;
            
            //builds every word
            
            while (isalnum(temp_msg[j])){
                temp += temp_msg[j++];
            }
            
            if (!temp.empty()){
                transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
                
                //if not already in the vector add to vector
                if (!binary_search(keyHash[temp].begin(),
                                   keyHash[temp].end(), i)){
                    keyHash[temp].push_back(i);
                }
            }
        }//message traversal
        
    }//masterlog traversal
}

void LogManager::timestamps_search(const string &ts1, const string &ts2){
    
    cout << "Timestamps search: ";
    ranged = true;
    searchMade = true;
    history = queue<size_t>{};
    
    LogEntry temp1;
    temp1.ts = ts1;
    auto _b = lower_bound(masterLog.begin(), masterLog.end(),
                         temp1, BoundCompare());
    
    LogEntry temp2;
    temp2.ts = ts2;
    auto _e = upper_bound(masterLog.begin(), masterLog.end(),
                         temp2, BoundCompare());
    
    size_t b = (unsigned) (_b - masterLog.begin());
    size_t e = (unsigned) (_e - masterLog.begin());
    
    history.push(b);
    history.push(e);
    
    cout << e - b << " entries found\n"; //+1????
    
}

void LogManager::matching_search(const string &ts){
    
    cout << "Timestamp search: ";
    ranged = true;
    searchMade = true;
    history = queue<size_t>{};
    
    LogEntry temp;
    temp.ts = ts;
    
    auto _b = lower_bound(masterLog.begin(), masterLog.end(),
                         temp, BoundCompare());
    auto _e = upper_bound(masterLog.begin(), masterLog.end(),
                         temp, BoundCompare());
    
    size_t b = (unsigned) (_b - masterLog.begin());
    size_t e = (unsigned) (_e - masterLog.begin());
    
    history.push(b);
    history.push(e);
    
    cout << e - b << " entries found\n"; //+1????
    
}

void LogManager::category_search(string &cat){
    
    cout << "Category search: ";
    ranged = false;
    searchMade = true;
    history = queue<size_t>{};
    
    transform(cat.begin(), cat.end(), cat.begin(), ::tolower);
    
    const vector<size_t> &temp = catHash[cat];
    
    for (size_t i = 0; i < temp.size(); ++i){
        history.push(temp[i]);
    }
    
    cout << temp.size() << " entries found\n";

}

void LogManager::keyword_search(const string &kws){
    
    cout << "Keyword search: ";
    ranged = false;
    searchMade = true;
    history = queue<size_t>{};
    
    queue<vector<size_t>> sets;
    
    for (size_t j = 0; j < kws.length(); ++j){
        string temp;
        
        //builds every word
        while (isalnum(kws[j])){
            temp += kws[j++];
        }
        
        if (!temp.empty()){
            transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
            sets.push(keyHash[temp]);
        }
    }
    
    while (sets.size() > 1){
        
        vector<size_t> set1 = sets.front();
        sets.pop();
        sort(set1.begin(), set1.end());
        
        vector<size_t> set2 = sets.front();
        sets.pop();
        sort(set2.begin(), set2.end());
        
        vector<size_t> set3(set1.size()+set2.size());
        
        auto it = set_intersection(set1.begin(), set1.end(),
                                   set2.begin(), set2.end(),
                                   set3.begin());
        
        set3.resize((unsigned)(it - set3.begin()));
        sets.push(set3);

    }
    
    for (size_t i : sets.front()){
        history.push(i);
    }
    
    cout << sets.front().size() << " entries found\n";
    
}

void LogManager::print_EL(){
    
    size_t i = 0;
    
    while (i < excerptList.size()){
        const LogEntry &entry = masterLog[excerptList.front()];
        cout << i++ << "|"
             << entry.ID << "|"
             << entry.ts << "|"
             << entry.cat << "|"
             << entry.msg << '\n';
        
        excerptList.push_back(excerptList.front());
        excerptList.pop_front();
    }
    
}

void LogManager::print_history(){
    if (history.size() == 0){
        cerr << "There are no recent search results to print.\n";
    }
    
    if (!ranged){
        queue<size_t> storage;
        
        while (!history.empty()){
            const LogEntry &entry = masterLog[history.front()];
            cout << entry.ID << "|"
                 << entry.ts << "|"
                 << entry.cat << "|"
                 << entry.msg << '\n';
            
            storage.push(history.front());
            history.pop();
        }//printing most recent search results until empty
        
        swap(history, storage); //copy-swap
    }//if not ranged
    
    else{
        
        size_t b = history.front(); history.pop();
        size_t e = history.front(); history.pop();
        
        history.push(b); history.push(e);
        
        while (b != e){
            const LogEntry &entry = masterLog[b++];
            cout << entry.ID << "|"
                 << entry.ts << "|"
                 << entry.cat << "|"
                 << entry.msg << '\n';
        }
        
    }//if ranged
}

//use iterators?
void LogManager::append_history_EL(){
    if (history.size() == 0 && searchMade){
        cerr << "There are no recent search results to append.\n";
    }
    else if (history.size() == 0 && !searchMade){
        return;
    }
    
    if (!ranged){
        queue<size_t> storage;
            
        while (!history.empty()){
            excerptList.push_back(history.front());
            storage.push(history.front());
            history.pop();
        }
        
        swap(history, storage);
        cout << history.size() << " log entries appended\n";
    }
    
    else{
        size_t b = history.front(); history.pop();
        size_t e = history.front(); history.pop();
        
        history.push(b); history.push(e);
        cout << e - b << " log entries appended\n";
        
        while (b != e){
            excerptList.push_back(b++);
        }
        
    }
}

void LogManager::append_pos_EL(const size_t &pos){
    if (pos >= masterLog.size()){
        cerr << pos << " is not a valid position in master log.\n";
        return;
    }
    
    size_t val = translator[pos];
    excerptList.push_back(val);
    
    cout << "log entry " << pos << " appended\n";
    
}

void LogManager::delete_entry_EL(const size_t &pos){
    if (pos >= excerptList.size()){
        cerr << pos << " is not a valid position in excerpt list.\n";
        return;
    }
    
    auto it = excerptList.begin();
    it += (signed) pos; //-1?
    excerptList.erase(it);
    
    cout << "Deleted excerpt list entry " << pos << '\n';
    
}

void LogManager::move_begin_EL(const size_t &pos){
    if (pos >= excerptList.size()){
        cerr << pos << " is not a valid position in excerpt list.\n";
        return;
    }
    
    auto it = excerptList.begin();
    
    it += (signed) pos; //-1?
    if (it != excerptList.end()){
        size_t val = *it;
        excerptList.erase(it);
        excerptList.push_front(val);
    }
    
    cout << "Moved excerpt list entry " << pos << '\n';
    
}

void LogManager::move_end_EL(const size_t &pos){
    if (pos >= excerptList.size()){
        cerr << pos << " is not a valid position in excerpt list.\n";
        return;
    }
    
    auto it = excerptList.begin();
    it += (signed) pos; //-1?
    
    if (it != excerptList.end()){
        size_t val = *it;
        excerptList.erase(it);
        excerptList.push_back(val);
    }
    
    cout << "Moved excerpt list entry " << pos << '\n';
    
}

void LogManager::clear_EL(){
    cout << "excerpt list cleared\n";
    
    if (excerptList.empty()){
        cout << "(previously empty)\n";
    }
    
    else{
        const LogEntry &entry1 = masterLog[excerptList.front()];
        const LogEntry &entry2 = masterLog[excerptList.back()];
        
        cout << "previous contents:\n"
             << 0 << "|"
             << entry1.ID << "|"
             << entry1.ts << "|"
             << entry1.cat << "|"
             << entry1.msg << '\n';
        cout << "...\n";
        cout <<  excerptList.size() - 1 << "|"
             << entry2.ID << "|"
             << entry2.ts << "|"
             << entry2.cat << "|"
             << entry2.msg << '\n';
        //print_EL();
    }

    excerptList.clear();
    
}

void LogManager::sort_EL(){
    
    cout << "excerpt list sorted\n";
    
    if (excerptList.empty()){
        cout << "(previously empty)\n";
        return;
    }
    
    const LogEntry &entry1 = masterLog[excerptList.front()];
    const LogEntry &entry2 = masterLog[excerptList.back()];
    
    cout << "previous ordering:\n"
         << 0 << "|"
         << entry1.ID << "|"
         << entry1.ts << "|"
         << entry1.cat << "|"
         << entry1.msg << '\n'
         << "...\n"
         <<  excerptList.size() - 1 << "|"
         << entry2.ID << "|"
         << entry2.ts << "|"
         << entry2.cat << "|"
         << entry2.msg << '\n';
    
    sort(excerptList.begin(), excerptList.end());
    
    const LogEntry &entry3 = masterLog[excerptList.front()];
    const LogEntry &entry4 = masterLog[excerptList.back()];
    
    cout << "new ordering:\n"
         << 0 << "|"
         << entry3.ID << "|"
         << entry3.ts << "|"
         << entry3.cat << "|"
         << entry3.msg << '\n';
    cout << "...\n";
    cout <<  excerptList.size() - 1 << "|"
         << entry4.ID << "|"
         << entry4.ts << "|"
         << entry4.cat << "|"
         << entry4.msg << '\n';
    
}

unsigned long long LogManager::ts_convert(const string &ts_str){
    string ts = ts_str;

    auto it = ts.begin();
    while (it != ts.end()){
        if (*it == ':'){
            ts.erase(it--);
        }
        it++;
    }
    
    return stoull(ts);
    
}


int main(int argc, char ** argv){
    
    string filename = argv[1];
    
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    
    bool kwSearch = false;
    bool catSearch = false;
    
    LogManager logman;
    logman.store_masterLog(filename);
    //logman.store_hash();
    
    string junk;
    char cmd;
    
    do {
        cout << "% ";
        cin >> cmd;
        
        switch(cmd){
            case 'q': { break; }
                
            case '#': { getline(cin, junk); break;}
                
            case 'a': {
                size_t pos;
                cin >> pos;
                logman.append_pos_EL(pos);
                break;
            }
                
            case 't': {
                string tss;
                cin >> tss;
                
                size_t idx = tss.find('|');
                string _ts1 = tss.substr(0, idx);
                string _ts2 = tss.substr(idx + 1);
                
                if (_ts1.length() != 14 || _ts2.length() != 14){
                    cerr << "timestamp format incorrect\n";
                    break;
                }
                logman.timestamps_search(_ts1, _ts2);
                break;
                
            }
                
            case 'm': {
                string _ts;
                cin >> _ts;
                if (_ts.length() != 14){
                    cerr << "timestamp format incorrect\n";
                    break;
                }
                logman.matching_search(_ts);
                break;
            }
                
            case 'c': {
                if (!catSearch){
                    logman.store_cat();
                    catSearch = true;
                }
                
                string cat;
                getline(cin, cat);
                cat = cat.substr(1);
                logman.category_search(cat);
                break;
            }
                
            case 'k': {
                if (!kwSearch){
                    logman.store_kw();
                    kwSearch = true;
                }
                
                string kws;
                getline(cin, kws);
                kws = kws.substr(1);
                logman.keyword_search(kws);
                break;
            }
                
            case 'p': { logman.print_EL(); break;}
                
            case 'g': { logman.print_history(); break;}
                
            case 'r': { logman.append_history_EL(); break;}
                
            case 'd': {
                size_t pos;
                cin >> pos;
                logman.delete_entry_EL(pos);
                break;
            }
                
            case 'b': {
                size_t pos;
                cin >> pos;
                logman.move_begin_EL(pos);
                break;
            }
                
            case 'e': {
                size_t pos;
                cin >> pos;
                logman.move_end_EL(pos);
                break;
            }
                
            case 'l': { logman.clear_EL(); break;}
                
            case 's': { logman.sort_EL(); break;}
                
            default : {
                cerr << cmd << " is not a valid command\n";
                break;
            }
                
        }//switch statements
        
    } while (cmd != 'q');
    
}

