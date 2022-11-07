// Project UID c1f28c309e55405daf00c565d57ff9ad

#include "List.h"
#include "unit_test_framework.h"

using namespace std;

// Add your test cases here

TEST(test_list_default_ctor) {
    List<int> empty_list;
    ASSERT_TRUE(empty_list.empty());
}

TEST(test_list_front_back_copyctor){
    List<int> list1;

    list1.push_front(2);
    int x = list1.front();
    ASSERT_TRUE(x == 2);
    ASSERT_TRUE(list1.size() == 1);

    list1.push_front(5);
    x = list1.front();
    ASSERT_TRUE(x == 5);

    list1.push_back(10);
    int y = list1.front();
    x = list1.back();
    ASSERT_TRUE(y == 5);
    ASSERT_TRUE(x == 10);
    ASSERT_TRUE(list1.size() == 3);

    List<int> list2(list1);
    x = list2.front();
    y = list2.back();
    ASSERT_TRUE(x == 5);
    ASSERT_TRUE(y == 10);

    list2.push_front(100);
    x = list2.front();
    ASSERT_TRUE(x == 100);
    ASSERT_TRUE(list2.size() == 4);

}

TEST(test_list_pop){
    List<int> list1;

    list1.push_front(3);
    list1.push_front(2);
    list1.push_front(1);
    ASSERT_TRUE(list1.front() == 1);

    list1.pop_front();
    ASSERT_TRUE(list1.front() == 2);
    ASSERT_TRUE(list1.back() == 3);

    list1.pop_front();
    ASSERT_TRUE(list1.front() == list1.back());

    list1.pop_front();
    ASSERT_TRUE(list1.empty());

    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);
    ASSERT_TRUE(list1.back() == 3);

    list1.pop_back();
    ASSERT_TRUE(list1.back() == 2);

    list1.pop_back();
    ASSERT_TRUE(list1.back() == list1.back());

    list1.pop_back();
    ASSERT_TRUE(list1.empty());

}

TEST(test_list_clear){
    List<int> list1;

    list1.push_front(0);
    ASSERT_TRUE(list1.size() == 1);
    list1.clear();
    ASSERT_TRUE(list1.empty());

    list1.push_front(0);
    list1.push_front(0);
    ASSERT_TRUE(list1.size() == 2);
    list1.clear();
    ASSERT_TRUE(list1.empty());

}

TEST(test_list_copy_oprt){
    List<int> list1;

    List<int> list2 = list1;

    list2.push_back(2);
    list2.push_front(1);
    ASSERT_TRUE(list2.back() == 2);
    ASSERT_TRUE(list2.front() == 1);
    ASSERT_TRUE(list1.empty());

    list1.push_back(100);
    list1.push_back(200);
    list1.push_back(300);
    ASSERT_TRUE(list2.back() == 2);
    ASSERT_TRUE(list2.front() == 1);

    ASSERT_TRUE(list1.back() == 300);

    list2 = list1;
    ASSERT_TRUE(list2.size() == list1.size());
    ASSERT_TRUE(list2.front() == list1.front());
    ASSERT_TRUE(list2.back() == list1.back());


}

TEST(test_list_copy_oprt2){
    List<char> list1;
    List<char> list2;
    list2.push_back('a');
    list2.push_back('b');

    list1 = list2;

    ASSERT_TRUE(list1.front() == 'a');
    ASSERT_TRUE(list1.back() == 'b');
}

TEST(test_list_itr_ctor){
    List<double> list1;
    list1.push_back(0);
    List<double>::Iterator it(list1.begin());
    List<double>::Iterator it2(list1.begin());
    ASSERT_TRUE(it == it2);

}

TEST(test_list_itr_deref_oprt){
    List<double> list1;
    list1.push_back(0);
    List<double>::Iterator it_first = list1.begin();

    ASSERT_TRUE(*it_first == 0);

    list1.push_back(0.1);
    
    it_first = list1.begin();
    List<double>::Iterator it_last = ++it_first;
    
    ASSERT_TRUE(*it_first == 0.1);
    ASSERT_TRUE(*it_last == 0.1);

}

TEST(test_list_itr_plus_oprt){
    List<int> list1;
    list1.push_back(0);
    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);
    List<int>::Iterator it = list1.begin();
    ASSERT_TRUE(*it == 0);
    List<int>::Iterator it2 = ++it;
    ++it;
    ASSERT_TRUE(*it2 == 1);
    ASSERT_TRUE(*it == 2);
    --it;
    --it;
    ++(++(++it));
    ASSERT_TRUE(*it == 3);
    ++it;
    it2 = list1.end();
    ASSERT_TRUE(it == it2);
}

TEST(test_list_itr_equals_oprt){
    List<double> list1;
    list1.push_back(0);
    List<double>::Iterator it = list1.begin();
    List<double>::Iterator it2 = list1.begin();


    ASSERT_TRUE(it == it2);

    list1.push_back(0.1);
    ++it2; 

    ASSERT_TRUE(*it != *it2);
    ASSERT_TRUE(it != it2);

}

TEST(test_list_erase){
    List<int> list1;
    list1.push_back(0);
    List<int>::Iterator it = list1.begin();
    list1.erase(it);
    ASSERT_TRUE(list1.empty());

    list1.push_back(0);
    list1.push_back(1);
    list1.push_back(2);
    it = list1.begin();
    list1.erase(it);
    ASSERT_TRUE(list1.size() == 2);
    ASSERT_TRUE(list1.front() == 1);
    
    it = list1.begin();
    list1.erase(it);
    ASSERT_TRUE(list1.size() == 1);
    ASSERT_TRUE(list1.front() == 2);
    ASSERT_TRUE(list1.back() == 2);
    
    it = list1.begin();
    list1.erase(it);
    ASSERT_TRUE(list1.empty());

    list1.push_back(0);
    list1.push_back(1);
    list1.push_back(2);
    
    it = list1.begin();
    ++(++it);
    list1.erase(it);
    ASSERT_TRUE(list1.size() == 2);
    ASSERT_TRUE(list1.front() == 0);
    ASSERT_TRUE(list1.back() == 1);
    
    it = list1.begin();
    list1.erase(it);
    ASSERT_TRUE(list1.front() == 1);
    ASSERT_TRUE(list1.front() == list1.back());
    
    it = list1.begin();
    list1.erase(it);
    ASSERT_TRUE(list1.empty());

}

TEST(list_test_erase_middle){
    List<int> list1;

    list1.push_back(0);
    list1.push_back(1);
    list1.push_back(2);
    
    List<int>::Iterator it = list1.begin();
    ++it;
    list1.erase(it);

    ASSERT_TRUE(list1.front() == 0);
    ASSERT_TRUE(list1.back() == 2);
    ASSERT_TRUE(list1.size() == 2);

    
}

TEST(test_list_insert){
    List<int> list1;
    List<int>::Iterator it = list1.begin();
    list1.insert(it, 2);//VALGRIND PROBLEM
    ASSERT_TRUE(list1.front() == 2);

    it = list1.begin();
    list1.insert(it, 0);
    ASSERT_TRUE(list1.front() == 0);
    ASSERT_TRUE(list1.back() == 2);

    list1.insert(it, 1);
    ASSERT_TRUE(list1.front() == 0);
    ASSERT_TRUE(list1.back() == 2);

    it = list1.begin();
    ++it;
    ASSERT_TRUE(*it == 1);

}

TEST(test_list_insert2){
    List<int> list1;
    list1.push_back(4);

    List<int>::Iterator it = list1.begin();
    list1.insert(it, 1);
    list1.insert(it, 2);
    list1.insert(it, 3);
    ASSERT_TRUE(list1.front() == 1);
    ASSERT_TRUE(list1.back() == 4);
    ASSERT_TRUE(list1.size() == 4);

    List<int>::Iterator it2 = list1.end();
    list1.insert(it2, 5);
    ASSERT_TRUE(list1.back() == 5);
    ASSERT_TRUE(list1.size() == 5);

    it = list1.begin();
    list1.insert(it, 0);
    ASSERT_TRUE(list1.front() == 0);
    ASSERT_TRUE(list1.size() == 6);

    it = list1.begin(); list1.erase(it);
    it = list1.begin(); list1.erase(it);
    it = list1.begin(); list1.erase(it);
    it = list1.begin(); list1.erase(it);
    it = list1.begin(); list1.erase(it);
    it = list1.begin(); list1.erase(it);
    ASSERT_TRUE(list1.empty());
}
TEST_MAIN()
