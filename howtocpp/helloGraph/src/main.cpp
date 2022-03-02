#include<iostream>
#include<string>
#include<vector>

#include "graph.h"

using namespace std;

// compilation:
// g++ -I ../include *.cpp -o main
//

int main(){
    Graph graph(10);

    int size_b = graph.getSize();
    cout << "graph before: \n" << graph.getString() << endl;

    graph.popfromW();
    
    int size_a = graph.getSize();
    cout << "graph after: \n" << graph.getString() << endl;

    graph.myPopfromW();
    
    int size_a2 = graph.getSize();
    cout << "graph after2: \n" << graph.getString() << endl;

    cout << "size before: " << size_b << ", size after: " << size_a << endl;
    cout << "size after2: " << size_a2 << endl;
}
