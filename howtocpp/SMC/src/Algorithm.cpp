

/** This in under the asumption that we have the Petri Net*/

#include "../../include/PetriEngine/SuccessorGenerator.h"

using namespace PetriEngine;

int main(){

    int runs = 10000;
    int depths = 10000;
    int c_depth = 0;
    int count = 0;

    PetriEngine::SuccessorGenerator SG(PN);

    for (i = 0; i < runs; i++;){
        if(function(PN.mark(),PN.trans())){
            count++;
        }
    }
    return count/runs;
}

bool function(&marking mark, &transition trans){
    int m, n = 0;
    while (SG.next(mark, trans)){
        m = trans.weight();
        n = n + m;
        if (m/n =< random(1)){
            SG.fire(trans);
            if (property == yes){
                return true;
            }
        }
    }
    return false;
}
