#ifndef OPTIONS_H
#define OPTIONS_H

#include <ctype.h>
#include <stddef.h>
#include <limits>
#include <set>
#include <sstream>
#include <vector>
#include <iostream>

#include "CTL/Algorithm/AlgorithmTypes.h"
#include "LTL/AlgorithmTypes.h"

enum class Strategy {
    BFS,
    DFS,
    HEUR,
    RDFS,
    OverApprox,
    DEFAULT
};

enum class TemporalLogic {
    CTL, LTL
};

//TODO can be moved to LTL/AlgorithmTypes.h?
enum class TraceLevel {
    None,
    Transitions,
    Full
};

enum class APCompression {
    Choose,
    None,
    Full
};

enum class LTLPartialOrder {
    None,
    Visible,
    Automaton,
    Liebke
};

enum class BuchiOptimization {
    Low = 1,
    Medium = 2,
    High = 3
};

enum class LTLHeuristic {
    Distance,
    Automaton,
    FireCount,
};
struct options_t {
//    bool outputtrace = false;
    bool smc = false;
    int smcruns = 1000;
    int smcdepth = 1000;
    int smcit = 0;
    std::set<size_t> heuristicnumbers;

    int kbound = 0;
    const char* modelfile = nullptr;
    const char* queryfile = nullptr;
    int enablereduction = 1; // 0 ... disabled,  1 ... aggresive (default), 2 ... k-boundedness preserving, 3 ... selection
    std::vector<uint32_t> reductions{8,2,3,4,5,7,9,6,0,1};
    int reductionTimeout = 60;
    bool stubbornreduction = true;
    bool statespaceexploration = false;
    bool printstatistics = true;
    std::set<size_t> querynumbers;
    Strategy strategy = Strategy::DEFAULT;
    int queryReductionTimeout = 30, intervalTimeout = 10, partitionTimeout = 5, lpsolveTimeout = 10;
    TraceLevel trace = TraceLevel::None;
    bool use_query_reductions = true;
    uint32_t siphontrapTimeout = 0;
    uint32_t siphonDepth = 0;
    uint32_t cores = 1;
    bool doVerification = true;

    TemporalLogic logic = TemporalLogic::CTL;
    bool noreach = false;
    //CTL Specific options
    bool usedctl = false;
    CTL::CTLAlgorithmType ctlalgorithm = CTL::CZero;
    bool tar = false;
    uint32_t binary_query_io = 0;

    // LTL Specific options
    bool usedltl = false;
    LTL::Algorithm ltlalgorithm = LTL::Algorithm::Tarjan;
    bool ltluseweak = true;
    std::string buchi_out_file;
    LTL::BuchiOutType buchi_out_type = LTL::BuchiOutType::Dot;
    APCompression ltl_compress_aps = APCompression::None;
    LTLPartialOrder ltl_por = LTLPartialOrder::Automaton;
    BuchiOptimization buchiOptimization = BuchiOptimization::Low;
    LTLHeuristic ltlHeuristic = LTLHeuristic::Automaton;

    bool replay_trace = false;
    std::string replay_file;


    std::string query_out_file;
    std::string model_out_file;
    std::string unfolded_out_file;
    std::string unfold_query_out_file;
    bool keep_solved = false;


    //CPN Specific options
    bool cpnOverApprox = false;
    bool computeCFP = true;
    bool computePartition = true;
    bool symmetricVariables = true;
    bool isCPN = false;
    uint32_t seed_offset = 0;
    int max_intervals = 250; //0 disabled
    int max_intervals_reduced = 5;

    std::string strategy_output;

    size_t seed() { return ++seed_offset; }
    void print(std::ostream& out = std::cout);
    bool parse(int argc, const char** argv);
};
#endif
