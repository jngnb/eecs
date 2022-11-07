//  Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0
//  Project4.hpp
//  P4
//
//  Created by Jeongin Bae on 6/11/22.
//

#ifndef Project4_hpp
#define Project4_hpp

#include <stdio.h>
#include <vector>
#include <limits>

using namespace std;

enum Campus{
    normal,
    medical,
    border
};

//Part A
class MST{
private:
    
    struct Drone{
        int x;
        int y;
        Campus cps;
    };
    
    struct Prims{
        bool k = false;
        double d = numeric_limits<double>::infinity();
        size_t p;
    };
    
    struct ComparePrims{
        bool operator()(const Prims &lhs, const Prims &rhs){
            if (!lhs.k){ //lhs = false
                if (!rhs.k){
                    return lhs.d < rhs.d;
                }
                else{ //rhs = true;
                    return true;
                }
            }
            return false;
        }
    };
    
    size_t numDrones;
    vector<Drone> drones;
    
    vector<Prims> primsTable;
    
public:
    bool helpingOpt = false;
    
    vector<Prims> & get_primsTable(){
        return primsTable;
    }
    
    vector<Drone> & get_drones(){
        return drones;
    }
    
    size_t & get_numDrones(){
        return numDrones;
    }
    
    void store_input();
    
    void mst();
    pair<double, double> border_distance(const size_t &nml, const size_t &med, size_t &bdr);
    double total_distance();
    void output_mst();
    
    double euclidian_distance(const size_t &n1, const size_t &n2);
    
};

//Part B & C
class TSP{
private:
    struct Drone{
        int x;
        int y;
    };
    
    size_t numDrones;
    vector<Drone> drones;
    vector<size_t> path;
    
    double upperBound = numeric_limits<double>::infinity();
    double runningCost = 0;
    
    vector<size_t> bestPath;
    
    MST mst;

public:
    uint64_t n = 0;

    bool fasttsp = false;
    
    void store_input();
    
    //FASTTSP
    void nearest_insertion();
    size_t find_insertion(const size_t &u, const size_t &numVisited);
    void two_opt();
    bool two_change(const size_t &n1, const size_t &n2,
                    const size_t &n3, const size_t &n4);
    
    //OPTTSP
    void initialize_upper();
    void genPerms(size_t permLength);
    bool promising(size_t permLength);
    pair<double, double> min_dist(size_t permLength);
    
    //output
    double total_distance();
    void output();
    
    //helper
    double euclidian_distance(const size_t &n1, const size_t &n2);

    
};

char get_opt(int argc, char** argv);

#endif /* Project4_hpp */
