//  Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68768FC9
//
//  Created by Jeongin Bae on 5/5/22.

#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include <chrono>
#include "xcode_redirect.hpp"

//#include "Letterman.h"

/*TODO: test files
 */

using namespace std;

struct DictEntry{
    string word;
    bool discovered;
    size_t pred;
};

class Letterman{
private:
    bool stackMode = false;
    bool wordMode = true;
    bool change = false;
    bool swap = false;
    bool length = false;
    
    string start_word;
    string end_word;
    
    vector<DictEntry> dict;
    deque<size_t> sc;
    int num_disc = 0;
    
    vector<string> finalOutput;
    
    void printHelp(char *argv[]);
    int final_idx = -1;
    
public:
    void getOpt(int argc, char *argv[]);
    
    void readDict();
    void readDictSimple();
    void readDictComplex();
    
    void search();
    void investigate(string &curr, const size_t &curr_idx, const size_t &i);

    void output(/*int final_idx*/);
    
};

void Letterman::printHelp(char *argv[]) {
    cout << "Usage: " << argv[0]
    << "[-s|-q]|-c|-p|-l|[-o W|M]|-b <start word>|-e <end word>|-h\n";
} // printHelp()

void Letterman::getOpt(int argc, char * argv[]) {
    int soq = 0;
    int cpl = 0;

    opterr = false; // Let us handle all error output for command line options
    int choice;
    int option_index = 0;
    option long_options[] = {
        { "stack",  no_argument,       nullptr, 's' },
        { "queue",  no_argument,       nullptr, 'q' },
        { "change", no_argument,       nullptr, 'c' },
        { "swap",   no_argument,       nullptr, 'p' },
        { "length", no_argument,       nullptr, 'l' },
        { "output", required_argument, nullptr, 'o' },
        { "begin",  required_argument, nullptr, 'b' },
        { "end",    required_argument, nullptr, 'e' },
        { "help",   no_argument,       nullptr, 'h' },
        { nullptr,  0,                 nullptr, '\0' }
    };

    // Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "sqcplo:b:e:h",
                                 long_options, &option_index)) != -1){
        switch (choice) {
        case 'h':
            printHelp(argv);
            return;
    
        case 's':
            stackMode = true;
            soq++;
            break;
    
        case 'q':
            soq++;
            break;
        
        case 'c':
            change = true;
            cpl++;
            break;
        
        case 'p':
            swap = true;
            cpl++;
            break;
                
        case 'l':
            length = true;
            cpl++;
            break;
            
        case 'b':
            if (optarg){
                start_word = optarg;
            }
            else{
                cerr << "Beginning word not specified\n";
                exit(1);
            }
            break;
                
        case 'e':
            if (optarg){
                end_word = optarg;
            }
            else{
                cerr << "End word not specified\n";
                exit(1);
            }
            break;
                
        case 'o':{
            string temp = optarg;
            if (temp != "W" && temp != "M"){
                cerr << "Invalid output mode specified\n";
                exit(1);
            }//if
            else if (temp == "M"){
                wordMode = false;
            }
            break;
        }
        
        default:
            cerr << "Unknown command line option\n";
            exit(1);
            
        } // switch
    } // while

    if (soq == 0) {
        cerr << "Must specify one of stack or queue\n";
        exit(1);
    } // if
    else if (soq >= 2){
        cerr << "Conflicting or duplicate stack and queue specified\n";
        exit(1);
    }
    
    if (cpl == 0){
        cerr << "Must specify at least one modification mode (change length swap)\n";
        exit(1);
    }
    
    if (start_word.length() != end_word.length() && !length){
        cerr << "The first and last words must have the same length when length mode is off\n";
        exit(1);
    }
    
} // getOpt()

void Letterman::readDict(){
    string type;
    cin >> type;
    
    if (type == "S"){
        readDictSimple();
    }
    else{
        readDictComplex();
    }
    
}

void Letterman::readDictSimple(){
    size_t size;
    cin >> size;
    
    dict.resize(size);
    
    for (size_t i = 0; i < size; ++i){
        string word;
        cin >> word;
        while (word.substr(0, 2) == "//"){
            getline(cin, word);
            cin >> word;
        }
        //dict.push_back({word, false, 0});
        dict[i] = {word, false, 0};
    }
}

// ch[io]p --> chip , chop
// st!ar -> star, tsar
// stop& --> stop, pots
// le?t --> let, leet
void Letterman::readDictComplex(){ //TODO: finish
    size_t size;
    cin >> size;
    
    for (size_t i = 0; i < size; ++i){
        string word;
        cin >> word;
        while (word.substr(0, 2) == "//"){
            getline(cin, word);
            cin >> word;
        }
        
        size_t opn = word.find('[');
        size_t exc = word.find('!');
        size_t qst = word.find('?');
        size_t it = 0;
        
        string before;
        string each;
        string after;

        if (opn != string::npos){
            size_t cls = word.find(']');
            while (it < opn){
                before += word[it++];
            }
            it = opn+1;
            while (it < cls){
                each+=word[it++];
            }
            it = cls+1;
            while (it < word.length()){
                after+=word[it++];
            }
            /*string before = word.substr(0, opn);
            string each = word.substr(opn + 1, cls - opn - 1);
            string after = word.substr(cls + 1);*/
            
            for (size_t i = 0; i < each.length(); ++i){
                
                word = before + each[i] + after;
                dict.push_back({word, false, 0});
            }
        }
        
        else if (exc != string::npos){
            while (it < exc-2){
                before += word[it++];
            }
            it = exc-2;
            while (it < exc){
                each += word[it++];
            }
            it = exc+1;
            while (it < word.length()){
                after += word[it++];
            }
            /*string before = word.substr(0, exc - 2);
            string each = word.substr(exc - 2, 2);
            string after = word.substr(exc + 1);*/
            
            word = before + each + after;
            dict.push_back({word, false, 0});
            
            word = before + each[1] + each[0] + after;
            dict.push_back({word, false, 0});
        }
        
        else if (word.find('&') != string::npos){
            word = word.substr(0, word.length() - 1);
            dict.push_back({word, false, 0});
            
            string reversed;
            for (int i = static_cast<int>(word.length()) - 1; i >= 0; --i){
                reversed += word[(unsigned)i];
            }
            dict.push_back({reversed, false, 0});
        }
        
        else if (qst != string::npos){
            while (it < qst){
                before += word[it++];
            }
            it = qst+1;
            while (it < word.length()){
                after += word[it++];
            }
            //string before = word.substr(0, qst);
            //string after = word.substr(qst + 1);
            char to_repeat = before[before.length() - 1];
            
            word = before + after;
            dict.push_back({word, false, 0});
            
            word = before + to_repeat + after;
            dict.push_back({word, false, 0});
        }
        else{
            dict.push_back({word, false, 0});
        }
    }//for loop
    
}

void Letterman::search(){
    string curr;
    size_t curr_idx;
    bool begin_in_dict = false;
    bool end_in_dict = false;
    size_t dict_size = dict.size();
    if (dict_size == 1){
        final_idx = 0;
    }

    for (size_t i = 0; i < dict_size; ++i){
        DictEntry& temp_entry = dict[i];
        
        if (temp_entry.word == start_word){
            
            begin_in_dict = true;
            curr_idx = i;
            temp_entry.pred = string::npos;
            temp_entry.discovered = true;
            
        }
        if (temp_entry.word == end_word){
            end_in_dict = true;
        }
    }
    if (!begin_in_dict){
        cerr << "Beginning word does not exist in the dictionary\n";
        exit(1);
    }
    if (!end_in_dict){
        cerr << "Ending word does not exist in the dictionary\n";
        exit(1);
    }

    sc.push_back(curr_idx);
    
    while (!sc.empty()){
        if (stackMode){
            curr_idx = sc.back();
            curr = dict[curr_idx].word;
            sc.pop_back();
        }
        else{
            curr_idx = sc.front();
            curr = dict[curr_idx].word;
            sc.pop_front();
        }
        ++num_disc;
        
        for (size_t i = 0; i < dict_size; ++i){
            /*final_idx =*/ investigate(curr, curr_idx, i);
            if (final_idx != -1){
                return;
            }
        }//for loop
        
    }//while

}

//adds appropriate words to search container for each "current" word
void Letterman::investigate(string &curr, const size_t &curr_idx, const size_t &idx){
    
    DictEntry &temp_entry = dict[idx];
    
    string &temp = temp_entry.word;
    bool disc = temp_entry.discovered;
    
    size_t curr_length = curr.length();
    size_t temp_length = temp.length();
    
    bool add = false;
    
    if (!disc){
        if ((change || swap) &&
            curr_length == temp_length){
            size_t count = 0;
            size_t changed = 0;
            for (size_t i = 0; i < curr_length; ++i){
                if (curr[i] != temp[i]){
                    count++;
                    changed = i;
                }
            }
            if (count == 1 && change){
                add = true;
            }
            else if (count == 2 && swap){
                if (curr[changed] == temp[changed - 1] &&
                    temp[changed] == curr[changed - 1]){
                    add = true;
                }
            }
        }
        
        else if (length &&
                 (curr_length == temp_length + 1 ||
                  temp_length == curr_length + 1)){
            
            string longer = curr;
            string shorter = temp;
            
            if (curr_length < temp_length){
                longer = temp;
                shorter = curr;
            }
                
            size_t count = 0;
            size_t inc = 0;
            size_t shorter_length = shorter.length();
            
            while (longer[inc] == shorter[inc]){
                ++inc;
            }
            for (size_t x = inc; x < shorter_length; ++x){
                if (longer[x + 1] == shorter[x]){
                    ++count;
                }
            }
            if (count == shorter_length - inc){
                add = true;
            }
           
        }// morph mode if statements
        
        // returning final index if added to search container
        // and it's the end word
        if (add){
            sc.push_back(idx);
            temp_entry.pred = curr_idx;
            temp_entry.discovered = true;
        }
        
        if (add && temp == end_word){
            final_idx = (signed)idx;
        }
    }//if not alr discovered
    //return -1;
}

void Letterman::output(/*int final_idx*/){
    if (final_idx == -1){
        cout << "No solution, "
             << (num_disc + static_cast<int>(sc.size()))
             << " words discovered.\n";
        return;
    }
    
    else {
        finalOutput.push_back(dict[final_idx].word);
        size_t idx = dict[final_idx].pred;
        while (idx != string::npos){
            DictEntry &temp_entry = dict[idx];
            finalOutput.push_back(temp_entry.word);
            idx = temp_entry.pred;
        }
    }
     
    cout << "Words in morph: " << finalOutput.size() << '\n';
    
    if (wordMode){
        for (int i = static_cast<int>(finalOutput.size())-1; i >= 0; --i){
            cout << finalOutput[i] << '\n';
        }
    }
    
    else{
        cout << finalOutput[finalOutput.size() - 1] << '\n';
        for (int i = static_cast<int>(finalOutput.size()) - 2; i >= 0; --i){
            
            string w = finalOutput[i];
            string w2 = finalOutput[i + 1];
            
            size_t w_length = w.length();
            size_t w2_length = w2.length();
            
            if (w_length == w2_length){
                size_t count = 0;
                size_t rm = 0;
                for (size_t i = 0; i < w_length; ++i){
                    if (w[i] != w2[i]){
                        rm = i;
                        count++;
                    }
                }
                if (count == 1){
                    cout << "c," << rm << "," << w[rm] << '\n';
                }
                else{
                    cout << "s," << rm - 1 << '\n'; //to account for "first difference"
                }
            }
            else if(w_length > w2_length){
                size_t rm = w.find_last_not_of(w2);//step --> stepp //step-->steep
                if (rm == string::npos){
                    rm = 0;
                    while (w[rm] == w2[rm]){
                        ++rm;
                    }
                }
                cout << "i," << rm << "," << w[rm] << '\n';
            }
            else if (w_length < w2_length){
                size_t rm = w2.find_last_not_of(w);//steep --> step //stepp --> step
                if (rm == string::npos){
                    rm = 0;
                    while (w[rm] == w2[rm]){
                        ++rm;
                    }
                }
                cout << "d," << rm << '\n';

            }
                
        }
    }
}


int main(int argc, char *argv[]){
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    
    Letterman letterman;
    letterman.getOpt(argc, argv);
    letterman.readDict();
    //auto start = std::chrono::high_resolution_clock::now();
    letterman.search();
    /*auto stop =  std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<chrono::microseconds>(stop - start);
    
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
    */
    letterman.output();
    
    return 0;
    
}
