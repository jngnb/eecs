// Project UID c1f28c309e55405daf00c565d57ff9ad


#include <iostream>
#include <list>

using namespace std;

class OHQueue {
public:
  void read_json();
  void write_get_api_json();
  void write_response();

  void add_queue();

private:
  struct Student {
    string uniqname;
    string location;
    int position;
  };
  std::list<Student> queue;
};

int main() {
  cout << "hello from main!\n";
}

void OHQueue::read_json(){
    string method;
    string endpoint;
    string protocol;
    string ph;
    int content_length;

    cin >> method >> endpoint >> protocol;
    for (int i = 0; i < 6; ++i){
      cin >> ph;
    }
    cin >> content_length;
    if (content_length > 0){
      json request;
      int count = 0;
      while(count < content_length){
        cin >> request;
      }
    }
}

void OHQueue::write_get_api_json(){
  json response = {
    {"queue_head_url", "http://localhost/queue/head/"},
    {"queue_list_url", "http://localhost/queue/"},
    {"queue_tail_url", "http://localhost/queue/tail/"}
  };
  string resp = response.dump(4) + "\n";  // dump with indentation using 4 spaces
  size_t content_length = resp.length();

  cout << resp;
}

void OHQueue::write_response(){

}