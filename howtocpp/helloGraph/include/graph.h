#include "edge.h"

class Graph{
private:
    int size;
public:
    Graph(int x);
    std::vector<Edge> W;
    Edge popfromW();
    Edge myPopfromW();
    int getSize();
    std::string getString();
};