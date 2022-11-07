//  Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0
//  Project4.cpp
//  P4
//
//  Created by Jeongin Bae on 6/11/22.
//

#include <getopt.h>
#include <string>
#include <iostream>
#include <math.h>
#include <limits>
#include <algorithm>
#include <iomanip>
#include "Project4.hpp"
#include "xcode_redirect.hpp"

using namespace std;

char get_opt(int argc, char **argv){
    
    opterr = false;
    int choice;
    int option_index = 0;
    option long_options[] = {
        { "help", no_argument,       nullptr, 'h' },
        { "mode", required_argument, nullptr, 'm' },
        { nullptr, 0,                 nullptr, '\0'}
    };
    
    char mode = 'x';
    
    while ((choice = getopt_long(argc, argv, "hm:", long_options, &option_index)) != -1){
        switch (choice){
            case 'h':{
                cout << "h|-m <MODE>\n";
                exit(0);
            }
                
            case 'm':{
                if (optarg){
                    string temp = optarg;
                    
                    if (temp == "MST"){
                        mode = 'm';
                    }
                    else if (temp == "FASTTSP"){
                        mode = 'f';
                    }
                    else if (temp == "OPTTSP"){
                        mode = 'o';
                    }
                    //invalid mode specified
                    else{
                        cerr << "Invalid mode\n";
                        exit(1);
                    }
                }
                else {
                    cerr << "No mode specified\n";
                    exit(1);
                }
                break;
            }

            default: {
                cerr << "Unknown command line option\n";
                exit(1);
            }
        }
    }
    return mode;
}

void MST::store_input(){
    cin >> numDrones;
    drones.resize(numDrones);
    
    int x, y;
    int nN = 0; int nM = 0; int nB = 0;
    
    size_t i = 0;
    while (cin >> x){
        cin >> y;
        Campus cps = normal;
        nN++;
        
        if (x < 0){
            if (y < 0){
                cps = medical;
                nN--;
                nM++;
            }
            else if (y == 0){
                cps = border;
                nN--;
                nB++;
            }
        }
        else if (x == 0 && y <= 0){
            cps = border;
            nN--;
            nB++;
        }
        
        Drone temp = {x, y, cps};
        drones[i++] = temp;
    }
    
    if (nN > 0 && nM > 0 && nB == 0){
        cerr << "Cannot construct MST\n";
        exit(1);
    }
    
    primsTable.resize(numDrones);
    
}

void MST::mst(){
    size_t n = 0;
    //setting root distance to 0
    primsTable[0].d = 0;
    
    while (n++ < numDrones){
        //find index of minimum non-true Prim's value
        size_t d = unsigned (min_element(primsTable.begin(), primsTable.end(), ComparePrims()) - primsTable.begin());
        
        //change truth value of minimum index
        primsTable[d].k = true;
        
        for (size_t i = 0; i < numDrones; ++i){
            Prims &tempDrone = primsTable[i];
            
            //if false
            if (!tempDrone.k){
                double dist = euclidian_distance(d, i);
                
                if (tempDrone.d > dist){
                    tempDrone.d = dist;
                    tempDrone.p = d;
                }
            }
        }
    }
}

double MST::euclidian_distance(const size_t &n1,
                               const size_t &n2){
    
    const Drone &d1 = drones[n1];
    const Drone &d2 = drones[n2];
    
    if ((d1.cps == normal && d2.cps == medical) ||
        (d1.cps == medical && d2.cps == normal) ){
            
        return numeric_limits<double>::infinity();
    }
    
    return (sqrt( (double)pow(d2.x - d1.x, 2) +
                  (double)pow(d2.y - d1.y, 2) ));
    
}



double MST::total_distance(){
    //summing the distances
    double total = 0;
    for(const Prims &p : primsTable){
        if (p.d != numeric_limits<double>::infinity()){
            total += p.d;
        }
    }
    return total;
}

void MST::output_mst(){
    
    cout << total_distance() << '\n';
    
    for (size_t i = 1; i < numDrones; ++i){
        cout << min(i, primsTable[i].p) << " " << max(i, primsTable[i].p) << '\n';
    }
}

//TSP storing input
void TSP::store_input(){
    cin >> numDrones;
    drones.resize(numDrones);
    
    int x, y;
    size_t i = 0;
    
    while (cin >> x){
        cin >> y;
        Drone temp = {x, y};
        drones[i++] = temp;
    }
    
    path.resize(numDrones);
    
}

//FASTTSP functions
void TSP::nearest_insertion(){
    size_t numVisited = 0;
    
    vector<bool> used(numDrones, false);
    used[0] = true;
    
    path[numVisited++] = 0;
    
    //find nearest neighbor
    double minDist = numeric_limits<double>::infinity();
    size_t nearIdx = 0;
    for (size_t i = 1; i < numDrones; ++i){
        double currDist = euclidian_distance(0, i);
        if (currDist < minDist){
            minDist = currDist;
            nearIdx = i;
        }
    }
    
    used[nearIdx] = true;
    
    path[numVisited++] = nearIdx;
    
    size_t uIdx;
    
    while (numVisited < numDrones){
        //finding unused drone
        uIdx = (unsigned)rand() % numDrones;
        
        while (used[uIdx]){
            uIdx = (unsigned)rand() % numDrones;
        }
        
        //finding a place to insert above unused index
        size_t toInsert = find_insertion(uIdx, numVisited);
        path.insert(path.begin() + (signed)toInsert, uIdx);
        used[uIdx] = true;
        numVisited++;
        
    }
    path.resize(numDrones);

}

size_t TSP::find_insertion(const size_t &u, const size_t &numVisited){
    double min = numeric_limits<double>::infinity();
    double currMin = numeric_limits<double>::infinity();
    size_t toInsert = 0;
    
    for (size_t i = 0; i < numVisited - 1; ++i){
        //triangular distance
        currMin = euclidian_distance(path[i], u) +
                  euclidian_distance(u, path[i+1]) -
                  euclidian_distance(path[i], path[i+1]);
        
        //update as needed
        if (currMin < min) {
            min = currMin;
            toInsert = i + 1;
        }
    }
    return toInsert;
}

void TSP::two_opt(){
    for (size_t i = 0; i < path.size() - 1; ++i){
        for (size_t j = i + 2; j < path.size() - 1; ++j){
            bool shorter = two_change(path[i], path[i+1],
                                      path[j], path[j+1]);
            if (shorter){
                reverse(path.begin() + signed(i + 1), path.begin() + signed(j + 1));
            }
            
        }
    }
}

bool TSP::two_change(const size_t &n1, const size_t &n2,
                     const size_t &n3, const size_t &n4){
    
    double currDist = euclidian_distance(n1, n2) + euclidian_distance(n3, n4);
    double newDist = euclidian_distance(n1, n3) + euclidian_distance(n2, n4);
    
    if (newDist < currDist){
        return true;
    }
    
    return false;
    
}

//OPTTSP functions
void TSP::initialize_upper(){
    fasttsp = true;
    store_input();
    nearest_insertion();
    upperBound = total_distance();
    two_opt();
    
    upperBound = total_distance();
    fasttsp = false;
    
    mst.helpingOpt = true;
    mst.get_numDrones() = numDrones;
    auto &mstDrones = mst.get_drones();
    
    mstDrones.resize(drones.size());
    
    for (size_t i = 0; i < drones.size(); ++i){
        auto &mstDrone = mstDrones[i];
        
        mstDrone.x = drones[i].x;
        mstDrone.y = drones[i].y;
        mstDrone.cps = normal;
        
    }
    
}

void TSP::genPerms(size_t permLength) {
    if (permLength == path.size()) {
        // Do something with the path
        runningCost += euclidian_distance(path.back(), path.front());
        if (runningCost < upperBound){
            upperBound = runningCost;
            bestPath = path;
        }
        runningCost -= euclidian_distance(path.back(), path.front());
        return;
    }// if
    
    if (!promising(permLength)){
        return;
    }
    
    for (size_t i = permLength; i < path.size(); ++i) {
        swap(path[permLength], path[i]);
        runningCost += euclidian_distance(path[permLength - 1], path[permLength]);
        genPerms(permLength + 1);
        runningCost -= euclidian_distance(path[permLength - 1], path[permLength]);
        swap(path[permLength], path[i]);
    } // for
    
} // genPerms()

bool TSP::promising(size_t permLength){
    //resetting primsTable
    mst.get_primsTable().clear();
    mst.get_primsTable().resize(path.size()-permLength);
    
    auto &mstDrones = mst.get_drones();
    mstDrones.clear();
    mstDrones.resize(path.size() - permLength);
    
    for (size_t i = permLength; i < path.size(); ++i){
        auto &mstDrone = mstDrones[i - permLength];
        mstDrone.x = drones[path[i]].x;
        mstDrone.y = drones[path[i]].y;
        mstDrone.cps = normal;
    }
    
    mst.mst();
    
    //connect first node and permLength - 1 node to mst created
    pair<double, double> tempPair = min_dist(permLength);
    
    double lowerBound = mst.total_distance();
    lowerBound += runningCost;
    lowerBound += tempPair.first;
    lowerBound += tempPair.second;
    
    if (lowerBound < upperBound){
        return true;
    }
    return false;
    
}

//TSP output functions
double TSP::total_distance(){
    if (fasttsp){
        bestPath = path;
    }
    
    double sum = 0;
    for (size_t i = 0; i < bestPath.size() - 1; ++i){
        size_t curr = bestPath[i];
        size_t next = bestPath[i + 1];
        sum += euclidian_distance(curr, next);
    }
    
    sum += euclidian_distance(bestPath.back(), bestPath.front());
    
    return sum;
}

void TSP::output(){
    if (fasttsp){
        cout << total_distance() << '\n';
    }
    else{
        cout << upperBound << '\n';
    }
    for (const size_t &i : bestPath){
        cout << i << ' ';
    }
    
}

double TSP::euclidian_distance(const size_t &n1,
                            const size_t &n2){
    
    const Drone &d1 = drones[n1];
    const Drone &d2 = drones[n2];
    
    return (sqrt( (double)pow(d2.x - d1.x, 2) +
                  (double)pow(d2.y - d1.y, 2) ));
    
}

pair<double, double> TSP::min_dist(size_t permLength){
    pair<double, double> minPair;
    size_t i = permLength;
    
    double min1 = numeric_limits<double>::infinity();
    double min2 = numeric_limits<double>::infinity();
    
    for (; i < path.size(); ++i){
        double tempMin1 = euclidian_distance(0, path[i]);
        double tempMin2 = euclidian_distance(path[permLength - 1], path[i]);
        
        if (tempMin1 < min1){
            min1 = tempMin1;
            minPair.first = tempMin1;
        }
        
        if (tempMin2 < min2){
            min2 = tempMin2;
            minPair.second = tempMin2;
        }
    }
    return minPair;
}

int main(int argc, char ** argv){
    cout << std::setprecision(2);
    cout << std::fixed;
    
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    
    char mode = get_opt(argc, argv);
    
    if (mode == 'm'){
        MST mst;
        mst.store_input();
        mst.mst();
        mst.output_mst();
    }
    
    else if (mode == 'f'){
        TSP tsp;
        tsp.fasttsp = true;
        tsp.store_input();
        tsp.nearest_insertion();
        tsp.output();
    }
    
    else if (mode == 'o'){
        TSP tsp;
         tsp.initialize_upper();
        tsp.genPerms(1);
        tsp.output();
    }
    
}


