// Project UID db1f506d06d84ab787baf250c265e24e
#include <iostream>
#include "csvstream.h"
#include <string>
#include <map>
#include <utility>
#include <cmath>
#include <algorithm>
#include <set>

using namespace std;

class Classifier{
  private:
    string train_file;
    string test_file;
    bool debug;

    int num_total_posts = 0;

    set<string> total_unique_word_set;

    map<string, int> num_posts_C;
    map<string, int> num_posts_w;
    map< string, map<string, int> > num_posts_wC;

    map<string, double> log_probs;
    pair<string, double> prediction;

  public:
    Classifier(string train_file_in, string test_file_in, bool debug_in)
    :train_file(train_file_in), test_file(test_file_in), debug(debug_in){};

    //training
    void store_tag_and_content();
    void check_counts_C(const string& input);
    void check_counts_w(const set<string>& word_set);
    void check_counts_wC(const string& label,const set<string>& word_set);

    //predicting
    void predict_posts();
    void log_probability(const string &correct_label, const string &content_datum);
    double log_prior(const string& label);
    double log_likelihood(const string& label, const string& words);
    void find_predicted();
    
    //printing
    void print_train_data();
    void print_trained_data();
    void print_test_data(const string & correct_label, const string &content_datum);

};

class LogProbLess {
  public:
  bool operator()(const pair<string,double> &lhs, const pair<string,double> &rhs)const{
    return lhs.second < rhs.second;
  }
};
// MODIFIES:  maps of training posts by word, label, and both
//            num_posts_w, num_posts_C, num_posts_wC
// EFFECTS:   stores the number of posts into respective maps
//            based on each unique label, unique word, 
//            and unique word per label
// NOTES:     also calls print_train_data to print() training data 
//            before storage process and print_trained_data() to print 
//            training data after storage process
// NOTES:     calls helper functions check_counts_C(), 
//                                   check_counts_w(),
//                                   check_counts_wC()
void Classifier::store_tag_and_content(){
  // Open file
  csvstream csv_train_file(train_file);

  //print training data
  if (debug){print_train_data();}

  // A row is a map<string, string>, key = column name, value = datum
  map<string, string> row;
  // Read file
  while (csv_train_file >> row) {
    //for tag (label)
    const string &tag_datum = row["tag"];
    check_counts_C(tag_datum);

    //for content (pre-bagged words)
    const string &content_datum = row["content"];

    istringstream source(content_datum);
    set<string> unique_word_set;
    string unique_word;

    while (source >> unique_word) {
      unique_word_set.insert(unique_word);
      total_unique_word_set.insert(unique_word);
    }
      
    check_counts_w(unique_word_set);
    check_counts_wC(tag_datum, unique_word_set);

    num_total_posts++;
  }
  cout << "trained on " << num_total_posts << " examples" << endl;
  if (!debug){ cout << endl; }
  if (debug){ print_trained_data(); }
}

// MODIFIES:  map of training posts by label num_posts_C
// EFFECTS:   stores the number of posts into num_posts_C 
//            based on each unique label
void Classifier::check_counts_C(const string& input){
  auto it = num_posts_C.find(input);
  if (it == num_posts_C.end()){
      num_posts_C.insert({input, 1});
  }
  else{
    (*it).second++;
  }
}

// MODIFIES:  map of training posts by word num_posts_w
// EFFECTS:   stores the number of posts into num_posts_Cw
//            based on each unique word
void Classifier::check_counts_w(const set<string> & word_set){
  //puts unique words into a set;
  for (const string &word : word_set){
    auto it = num_posts_w.find(word);

    if (it == num_posts_w.end()){
        num_posts_w.insert({word, 1});
    }
    else{
      (*it).second++;
    }
  }
}

// MODIFIES:  map of training posts by word per label num_posts_wC
// EFFECTS:   stores the number of posts into num_posts_wC 
//            based on each unique label per label
void Classifier::check_counts_wC(const string& label, const set<string> & word_set){
  for (const string &word : word_set){
    auto it = num_posts_wC.find(label);
      
    if (it == num_posts_wC.end()){
        num_posts_wC.insert({ label, {{word, 1}} });
    }
    else{
      auto it2 = ((*it).second).find(word);
        
      if (it2 == ((*it).second).end()){
          ((*it).second).insert({word, 1});
      }
      else{
          num_posts_wC[label][word]++;
      }
    }
  }
}

// EFFECTS:   predicts testing posts based on trained posts
//            by using a logarithmic probability algorithm
//            and prints out the performance of the prediction
// NOTE:      calls helper function log_probability()
void Classifier::predict_posts(){
  int num_correct = 0;
  int num_total = 0;
  csvstream csv_test_file(test_file, ',', false);
  map<string, string> row;
  // Read file
  cout << "test data:" << endl;
  while (csv_test_file >> row) {
    const string &correct_label = row["tag"];
    const string &content_datum = row["content"];
      
    log_probability(correct_label, content_datum);
    if (prediction.first == correct_label){
      num_correct++;
    }
    num_total++;
    cout << endl;
  }

  cout << "performance: " << num_correct << " / " << num_total
       << " posts predicted correctly" << endl;
}

// MODIFIES:  map of logarithm probabilities of a post per label log_probs
// EFFECTS:   calculates log probability scores of a testing post
//            with each stored label then stores the scores into a map
//            to find the maximum score then prints the testing data
// NOTE:      calls helper functions log_prior(),
//                                   log_likelihood(),
//                                   find_predicted(),
//                                   print_test_data()
void Classifier::log_probability(const string &correct_label,const string &content_datum){

  for (const auto &predicting_tag : num_posts_C){
    double score = 0;
    const string &label = predicting_tag.first;

    score += log_prior(label);
    score += log_likelihood(label, content_datum);
    log_probs[label] = score;
  }

  find_predicted();
  print_test_data(correct_label, content_datum);
}

// EFFECTS: returns the log prior score of each stored label
//          that is passed in as argument
double Classifier::log_prior(const string& label){
    double n = (*num_posts_C.find(label)).second;
  return (log(n/num_total_posts));
}

// EFFECTS: returns the log likelihood score of the post's content
//          that is passed in as argument alongside a stored label
double Classifier::log_likelihood(const string& label, const string& words){
  double sum_likelihood = 0;
  
  istringstream source(words);
  set<string> testing_word_set;
  string unique_word;

  while (source >> unique_word) {
    testing_word_set.insert(unique_word);
  }

  auto it_C_in_wC = num_posts_wC.find(label);
  auto it_C = num_posts_C.find(label);

  for (const auto& word : testing_word_set){
      auto it_w_given_C = ((*it_C_in_wC).second).find(word);
      auto it_w = num_posts_w.find(word);
      //when word is nowhere in the training set
      if (it_w == num_posts_w.end()/*it_w_given_C == ((*it_C_in_wC).second).end()*/){
        sum_likelihood += log((double)1/num_total_posts);
      }
      //when word is not in the given label but in the total training set
      else if ((it_w_given_C == ((*it_C_in_wC).second).end()) &&
               (it_w != num_posts_w.end())){
        sum_likelihood +=
          log((double)((*it_w).second)/num_total_posts);
      }
      //when word is in the given label
      else{
        sum_likelihood +=
          log(((double)(*it_w_given_C).second)/((*it_C).second));
      }
  }
  return sum_likelihood;
}

// MODIFIES:  private member variable prediction that stores
//            the predicted label and the log probability
// EFFECTS:   finds the maxium element in the map log_probs
//            by comparing only the log probability scores then sets
//            the variable 'prediction' to the predicted values
void Classifier::find_predicted(){
  auto predicted = *max_element(log_probs.begin(), log_probs.end(), LogProbLess());
  
  string predicted_label = predicted.first;
  double predicted_log_prob = predicted.second;

  prediction = {predicted_label, predicted_log_prob};

}

// EFFECTS: prints the training data from a csv file
void Classifier::print_train_data(){
  csvstream csv(train_file);
  cout << "training data:" << endl;
  map<string, string> row;
  while (csv >> row) {
    cout << "  label = " << row["tag"]
      << ", content = " << row["content"] << endl;
  }
}

// EFFECTS: prints the trained data after storage process by iterating 
//          through private maps formed from store_tag_and_content()
void Classifier::print_trained_data(){
    cout << "vocabulary size = " << total_unique_word_set.size()
       << endl << endl; 
  cout << "classes:" << endl;
  for (const auto &label_class : num_posts_C){
    const string label = label_class.first;
    const int count = label_class.second;
    cout << "  " << label << ", " << count << " examples, log-prior = " 
         << log_prior(label) << endl;
  }
  cout << "classifier parameters:" << endl;
  auto it = num_posts_wC.begin();
  auto it2 = ((*it).second).begin();
  while(it!=num_posts_wC.end() &&
        it2!=((*num_posts_wC.end()).second).end()){
    
    const string label = (*it).first;
    const string word = (*it2).first;
    const int count = (*it2).second;

    cout << "  " << label << ":" << word 
         << ", count = " << count
         << ", log-likelihood = " << log_likelihood(label, word)
         << endl;
    //it++;
    it2++;
    if (it2 == ((*it).second).end()){
        it++;
        it2 = ((*it).second).begin();
    }
  }
  cout << endl;

}

// EFFECTS: prints the test data
void Classifier::print_test_data(const string & correct_label, 
                                 const string &content_datum){
  cout << "  correct = " << correct_label
       << ", predicted = " << prediction.first
       << ", log-probability score = " << prediction.second << endl
       << "  content = " << content_datum << endl;
}


int main(int argc, char ** argv) {
  string s = "--debug";
  if (argc == 4) { s = argv[3]; }
  //Error message if arguments are not correct
  if (argc < 3 || argc > 4 || s != "--debug"){
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
    return -1;
  }
  cout.precision(3);
  string train_file = argv[1];
  string test_file = argv[2];

  ifstream if_train_file(train_file);
  ifstream if_test_file(test_file);

  //Error message if files cannot be opened
  if (!if_train_file.is_open()){
    cout << "Error opening file: " << train_file << endl; 
    return -1; }
  if (!if_test_file.is_open()){ 
    cout << "Error opening file: " << test_file << endl; 
    return -1; }
  
  bool debug = false;
  if (argc == 4){
    debug = true;
  }

  Classifier piazza(train_file, test_file, debug);
  piazza.store_tag_and_content();
  piazza.predict_posts();

}





