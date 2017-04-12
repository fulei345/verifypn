#include "STSolver.h"
#include "../lpsolve/lp_lib.h"
#include <assert.h>

namespace PetriEngine {     
    #define VarPlace(p,i) (((_net._nplaces * (i)) + (p)) + 1)
    #define VarPostT(t,i) ((_nPlaceVariables + (_net._ntransitions * (i)) + (t)) + 1)
    
    void STSolver::MakeConstraint(std::vector<STVariable> constraint, int constr_type, REAL rh){
        int count = constraint.size();
        REAL* row = new REAL[count];
        int* col = new int[count];
        for(int c = 0; c < count; c++){
            row[c]=constraint[c].value;
            col[c]=constraint[c].colno;
        }
        add_constraintex(_lp, count, row, col, constr_type, rh);
        delete[] row;
        delete[] col;
    }
    
    STSolver::STSolver(Reachability::ResultPrinter& printer, const PetriNet& net, PQL::Condition * query) : printer(printer), _query(query), _net(net){
        // Approximating depth to 10 for fewer but faster answers. 
        // Otherwise, this should at most be the number of places.
        _siphonDepth = 10; 
        _m0 = _net._initialMarking;
        _nPlaceVariables = (_net._nplaces * (_siphonDepth + 2));
        uint32_t nPostTransVariables = (_net._ntransitions * (_siphonDepth + 1));
        _nCol = _nPlaceVariables + nPostTransVariables;
        
        constructPrePost(); // TODO: Refactor this out...
    }

    STSolver::~STSolver() {
        if(_lp != NULL) delete_lp(_lp);
    }

    /* 
     * (SUM(p in P) p^0) GE 1
     * 
     * for all (t in T)    
     *     for all (p in post(t))
     *         (-p^0 + SUM(p' in pre(t)) p'^0) LE 0
     */ 
    void STSolver::CreateSiphonConstraints(){       
        // (SUM(p in P) p^0) >= 1
        std::vector<STVariable> variables;
        for(uint32_t p=0;p<_net._nplaces;p++){ 
            variables.push_back(STVariable(VarPlace(p,0), 1)); // p^0
        }  
        MakeConstraint(variables, GE, 1);
        
        // for all (t in T) for all (p in post(t)) (-p^0 + SUM(p' in pre(t)) p'^0) GE 0
        for(uint32_t t=0; t<_net._ntransitions; t++){
            uint32_t finv = _net._transitions[t].outputs;
            uint32_t linv = _net._transitions[t+1].inputs; 
            for (; finv < linv; finv++) { // for all p in post(t)
                std::vector<STVariable> variables2;
                variables2.push_back(STVariable(VarPlace(_net._invariants[finv].place,0), -1)); // -p^0
                
                uint32_t finv2 = _net._transitions[t].inputs;
                uint32_t linv2 = _net._transitions[t].outputs;
                for(; finv2 < linv2; finv2++){ // SUM(p' in pre(t))
                    variables2.push_back(STVariable(VarPlace(_net._invariants[finv2].place,0), 1)); // p'^0
                }
                MakeConstraint(variables2, GE, 0);
            }
        }
    }
    
    /* 
     * for all (p in P)
     *     -p^i+1 + p^i + SUM(t in post(p) post_t^i LE card(post(p))
     * 
     *     -p^i+1 + p^i GE 0
     *
     *     for all (t in post(p)) 
     *         -p^i+1 + post_t^i GE 0
     */
    void STSolver::CreateStepConstraints(uint32_t i){  
        // for all (p in P) -p^i+1 + p^i + SUM(t in post(p) post_t^i LE card(post(p))         
        for(uint32_t p=0; p<_net._nplaces; p++){
            std::vector<STVariable> variables1;
            variables1.push_back(STVariable(VarPlace(p,(i+1)), -1)); // -p^i+1
            variables1.push_back(STVariable(VarPlace(p,i), 1)); // p^i
            
            // for all (p in P) for all (t in post(p)) -p^i+1 + post_t^i GE 0
            for (uint32_t t = _places.get()[p].post; t < _places.get()[p + 1].pre; t++){ // for all t in post(p)
                variables1.push_back(STVariable(VarPostT(_transitions.get()[t],i), 1)); // post_t^i
                std::vector<STVariable> variables3;
                variables3.push_back(STVariable(VarPlace(p,(i+1)), -1)); // -p^i+1
                variables3.push_back(STVariable(VarPostT(_transitions.get()[t],i), 1)); // post_t^i
                MakeConstraint(variables3, GE, 0);
            }
            MakeConstraint(variables1, LE, (_places.get()[p + 1].pre - _places.get()[p].post));        
            // for all (p in P) -p^i+1 + p^i GE 0
            std::vector<STVariable> variables2;
            variables2.push_back(STVariable(VarPlace(p,(i+1)), -1)); // -p^i+1
            variables2.push_back(STVariable(VarPlace(p,i), 1)); // p^i
            MakeConstraint(variables2, GE, 0);
        }
    }
    
    /*
     * post_t <=> OR(p in post(t))
     * ---
     * for all (t in T)
     *     -post_t^i + SUM(p in post(t)) p^i GE 0
     *      
     *     for all (p in post(t))
     *         -post_t + p^i LE 0
     */
    void STSolver::CreatePostVarDefinitions(uint32_t i){ 
        for(uint32_t t=0; t<_net._ntransitions; t++){
            // -post_t^i + SUM(p in post(t)) p^i GE 0
            std::vector<STVariable> variables;
            variables.push_back(STVariable(VarPostT(t,i), -1)); // -post_t^i
            
            uint32_t finv = _net._transitions[t].outputs;
            uint32_t linv = _net._transitions[t+1].inputs;
            for (; finv < linv; finv++) { // for all p in post(t)
                variables.push_back(STVariable(VarPlace(_net._invariants[finv].place,i), 1)); // p^i
            }
            MakeConstraint(variables, GE, 0);

            // for all (p in post(t)) -post_t + p^i LE 0
            finv = _net._transitions[t].outputs;
            linv = _net._transitions[t+1].inputs;
            for (; finv < linv; finv++) {  // for all p in post(t)
                std::vector<STVariable> variables;
                variables.push_back(STVariable(VarPostT(t,i), -1)); // -post_t^i
                variables.push_back(STVariable(VarPlace(_net._invariants[finv].place,i), 1)); // p^i
                MakeConstraint(variables, LE, 0);
            }   
        }
    }
  
    int STSolver::CreateFormula(){
        _ret = 0;
        
        for(auto &i : _net._invariants){
            if(i.tokens > 1){
                _ret = -6; // the net has weighted arcs
                break;
            }
        }        
        if(_ret == 0){
            _lp = make_lp(0, _nCol);
            if(_lp == NULL) { std::cout<<"lp_solve: Could not construct new model ..."<<std::endl; _ret=2; }
        }
        if(_ret == 0){           
            
#ifdef DEBUG
        // Set variable names (not strictly necessary)
        for(uint32_t c=0; c<_nCol; c++){ 
            set_col_name(_lp, c+1, const_cast<char *> (VarName(c).c_str())); 
            std::cout<<"name of col "<<c+1<<" is "<<VarName(c).c_str()<<std::endl;
        }       
#endif                  
            for (size_t i = 1; i <= _nCol; i++){ set_binary(_lp, i, TRUE); }
            // Create constraints
            set_add_rowmode(_lp, TRUE);  
            CreateSiphonConstraints();          // ST formula step 1
            for(uint32_t i=0; i<=_siphonDepth; i++){ 
                CreateStepConstraints(i);       // ST formula maximal trap
                CreatePostVarDefinitions(i);
            }

            set_add_rowmode(_lp, FALSE);      
            
            // Bounds
            // Ensures the no-trap constraints
            for(uint32_t p = 0; p < _net._nplaces; p++){
                if(_m0[p] > 0){
                    set_bounds(_lp,VarPlace(p,_siphonDepth+1), 0, 0);
                }
            }    
            
            // Create objective
            std::vector<REAL> row = std::vector<REAL>(_nCol + 1);
            memset(row.data(), 0, sizeof (REAL) * _nCol + 1);
            for (size_t p = 0; p < _nCol; p++)
                row[1 + p] = 1;

            // Set objective
            set_obj_fn(_lp, row.data());

            // Minimize the objective
            set_minim(_lp);
        }
        
        return _ret;
    }
    
    int STSolver::Solve(int timeout){
        CreateFormula();
        
        if(_ret == 0){
            set_break_at_first(_lp, TRUE);
            set_presolve(_lp, PRESOLVE_ROWS | PRESOLVE_COLS | PRESOLVE_LINDEP, get_presolveloops(_lp));
            set_pivoting(_lp, PRICER_DEVEX|PRICE_LOOPALTERNATE|PRICE_AUTOPARTIAL); 
            
    #ifdef DEBUG
            write_LP(_lp, stdout);
    #endif       
            set_verbose(_lp, IMPORTANT);
            set_timeout(_lp, timeout);
            _ret = solve(_lp);
            
    #ifdef DEBUG
            PrintStatus();
    #endif     
        }
        
        return _ret;
    }
    
    Reachability::ResultPrinter::Result STSolver::PrintResult(){
        if(_ret == INFEASIBLE){
            return printer.printResult(0, _query, Reachability::ResultPrinter::NotSatisfied);
        } else {
            return Reachability::ResultPrinter::Unknown;
        }
    }
    
    void STSolver::PrintStatus(){
        if(_ret == OPTIMAL) {
            std::cout<<"lp_solve: An optimal solution was obtained."<<std::endl;
        } else if(_ret == PRESOLVED){
            std::cout<<"lp_solve: The model could be solved by presolve."<<std::endl;
        } else if(_ret == INFEASIBLE){
            std::cout<<"lp_solve: The model is infeasible."<<std::endl;
        } else if(_ret == NOMEMORY){
            std::cout<<"lp_solve: Out of memory."<<std::endl;
        } else if(_ret == UNBOUNDED){
            std::cout<<"lp_solve: The model is unbounded."<<std::endl;
        } else if(_ret == DEGENERATE){
            std::cout<<"lp_solve: The model is degenerative."<<std::endl;
        } else if(_ret == NUMFAILURE){
            std::cout<<"lp_solve: Numerical failure encountered."<<std::endl;
        } else if(_ret == USERABORT){
            std::cout<<"lp_solve: The abort routine returned TRUE."<<std::endl;
        } else if(_ret == TIMEOUT){
            std::cout<<"lp_solve: A timeout occurred."<<std::endl;
        } else if(_ret == NUMFAILURE){
            std::cout<<"lp_solve: Accuracy error encountered."<<std::endl;
        } else if(_ret == -6) {
            std::cout<<"lp_solve: The net has weighed arcs."<<std::endl;
        }
    }
    
    std::string STSolver::VarName(uint32_t index){
        std::string name = "";
        // place/transition index ^ siphon depth step
        if(index < _nPlaceVariables){ // place
            name = _net.placeNames()[(index % (_net._nplaces))] + "^" + std::to_string((uint32_t)floor(index / (_net._nplaces))); 
        } else{ // transition
            index=index-_nPlaceVariables;
            name = _net.transitionNames()[(index % (_net._ntransitions))]+ "^" + std::to_string(((uint32_t)floor(index / (_net._ntransitions))));
        }
        return name;
    }
    
    
    // TODO: Refactor this out... Copy paste from ReducingSuccessorGenerator.cpp
    // Also, we dont need the preset here.
    void STSolver::constructPrePost() {
        std::vector<std::pair<std::vector<uint32_t>, std::vector < uint32_t>>> tmp_places(_net._nplaces);
        for (uint32_t t = 0; t < _net._ntransitions; t++) {
            const TransPtr& ptr = _net._transitions[t];
            uint32_t finv = ptr.inputs;
            uint32_t linv = ptr.outputs;
            for (; finv < linv; finv++) { // Post set of places
                tmp_places[_net._invariants[finv].place].second.push_back(t);
            }

            finv = linv;
            linv = _net._transitions[t + 1].inputs;
            for (; finv < linv; finv++) { // Pre set of places
                tmp_places[_net._invariants[finv].place].first.push_back(t);
            }
        }

        // flatten
        size_t ntrans = 0;
        for (auto p : tmp_places) {
            ntrans += p.first.size() + p.second.size();
        }
        _transitions.reset(new uint32_t[ntrans]);

        _places.reset(new place_t[_net._nplaces + 1]);
        uint32_t offset = 0;
        uint32_t p = 0;
        for (; p < _net._nplaces; ++p) {
            std::vector<uint32_t>& pre = tmp_places[p].first;
            std::vector<uint32_t>& post = tmp_places[p].second;

            // keep things nice for caches
            std::sort(pre.begin(), pre.end());
            std::sort(post.begin(), post.end());

            _places.get()[p].pre = offset;
            offset += pre.size();
            _places.get()[p].post = offset;
            offset += post.size();
            for (size_t tn = 0; tn < pre.size(); ++tn) {
                _transitions.get()[tn + _places.get()[p].pre] = pre[tn];
            }

            for (size_t tn = 0; tn < post.size(); ++tn) {
                _transitions.get()[tn + _places.get()[p].post] = post[tn];
            }

        }
        assert(offset == ntrans);
        _places.get()[p].pre = offset;
        _places.get()[p].post = offset;
    }
    
    // for debugging purpose
    void STSolver::SanityCheckVariables(){
        // Testing place variables
        for(uint32_t i = 0; i<=_siphonDepth+1; i++){
            for(uint32_t p = 0; p<_net._nplaces; p++){
                std::cout<<"adding place "<<get_origcol_name(_lp, VarPlace(p,i))<<std::endl;
                std::vector<STVariable> variables;
                variables.push_back(STVariable(VarPlace(p,i), 1));
                MakeConstraint(variables, GE, 1);
            }
            for(uint32_t p = 0; p<_net._nplaces; p++){
                std::cout<<"adding place negative "<<get_origcol_name(_lp, VarPlace(p,i))<<std::endl;
                std::vector<STVariable> variables;
                variables.push_back(STVariable(VarPlace(p,i), -1));
                MakeConstraint(variables, GE, 1);
            }
        }
        
        // Testing transition variables
        for(uint32_t i = 0; i<=_siphonDepth; i++){
            for(uint32_t t = 0; t<_net._ntransitions; t++){
                std::cout<<"adding transition "<<get_origcol_name(_lp, VarPostT(t,i))<<std::endl;
                std::vector<STVariable> variables;
                variables.push_back(STVariable(VarPostT(t,i), 1));
                MakeConstraint(variables, GE, 1);
            }
            for(uint32_t t = 0; t<_net._ntransitions; t++){
                std::cout<<"adding transition negative "<<get_origcol_name(_lp, VarPostT(t,i))<<std::endl;
                std::vector<STVariable> variables;
                variables.push_back(STVariable(VarPostT(t,i), -1));
                MakeConstraint(variables, GE, 1);
            }
        }
    }
}