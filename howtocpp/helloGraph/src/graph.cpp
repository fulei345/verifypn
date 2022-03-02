#include<vector>
#include<algorithm>
#include<string>
#include "graph.h"

Graph::Graph(int x){
    size = x;

    for (int i = 0; i < size; i++){
        Edge edge(rand() % 20);
        W.push_back(edge);
    }
}

Edge Graph::popfromW(){
    auto it = std::max_element(W.begin(), W.end(), 
                [](auto a, auto b){
                    return false; // a->targets.size() < b->targets.size();
                });
    auto edge = *it;
    W.erase(it);
    return edge;
}

Edge Graph::myPopfromW(){
    auto it = W.begin();
    auto edge = *it;
    W.erase(it);
    return edge;
}

int Graph::getSize(){
    return W.size();
}

std::string Graph::getString(){
    std::string str = "";
    for (Edge e: W){
        str.append(std::to_string(e.getWeight()));
        str.append(", ");
    }
    return str;
}