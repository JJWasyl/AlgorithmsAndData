#pragma once
#include <iostream>
#include <utility>
#include <limits>
#include "graf.hpp"

using namespace std;

template <typename V, typename E>
std::pair<double, vector<int>> dijkstra(Graph<V,E>& graph, int src, int dest){
    
    double MAX = std::numeric_limits<double>::max();
    //double MAX = 9999999999;
    int maxpath = graph.nrOfVertices();
    vector<int> unvisited; 
    vector<int> path;
    vector<double> distance;
    unvisited.reserve(maxpath);
    distance.reserve(maxpath);
    path.reserve(maxpath);
    
    for(int k = 0; k < maxpath; k++){
        unvisited.push_back(k);
        distance.push_back(999999999);
        path.push_back(-1);
    }
    distance[src] = 0.0;
    
    /*
     * Main loop
     */
    while(!unvisited.empty()){
        double min_distance = MAX;
        int temp;
        int min_index = -1;
        
        for(int i = 0; i < unvisited.size(); i++){
            if(distance[unvisited[i]] < min_distance){
                min_distance = distance[unvisited[i]];
                min_index = i;
            }
        }
        temp = min_index;
        min_index = unvisited[temp];
        unvisited.erase(unvisited.begin() + temp);
        
        for(int i = 0; i < unvisited.size(); i++){
            
            if(graph.edgeExists(min_index, unvisited[i])){
                
                min_distance = distance[min_index] + graph.edgeLabel(min_index, unvisited[i]);
                
                if(min_distance < distance[unvisited[i]]){
                    distance[unvisited[i]] = min_distance;
                    path[unvisited[i]] = min_index;
                }
            }
        }
    }
    vector<int> final_path;
    int temp = dest;
    while(path[temp] != -1){
        final_path.insert(final_path.begin(), temp);
        temp = path[temp];
    }
    final_path.insert(final_path.begin(), temp);
    
    return (std::pair<double, vector<int>> (distance[dest], final_path));
}
