
#include <eo>
#include <moeo>
//#include <core/moeoVector.h>
#include <mo>

#include <utils/eoRealBounds.h>

// DEFINES
// Output debug data
#define VERBOSE false

// STL
#include <vector>
#include <iomanip>

// VRP related code
/** definition of evaluation:
  * class moeoVRPEvalFunc MUST derive from eoEvalFunc<moeoVRP>
  * and should test for validity before doing any computation
  * see tutorial/Templates/evalFunc.tmpl
  */
#include "problems/VRPTW/moeoVRPEvalFunc.h"

/** definition of representation:
  * class moeoVRP MUST derive from EO<FitT> for some fitness
  */
#include "problems/VRPTW/moeoVRP.h"

/** definitions of operators: write as many classes as types of operators
  * and include them here. In this simple example,
  * one crossover (2->2) and one mutation (1->1) operators are used
  */
#include "problems/VRPTW/moeoVRPQuadCrossover.h"
#include "problems/VRPTW/moeoVRPMutation.h"
#include "problems/VRPTW/moeoVRPInit.h"
#include "problems/VRPTW/moeoVRPIterSwap.h"

// Do makes
#include "problems/VRPTW/do_makes.h"

#include "do_makes/make_neighborhood.h"

#include "do/make_checkpoint.h"


// CODEA -//////////////////////

// Algorithms
#include "core/moMOLS.h"

// Agent moeoNSGAII
#include "agents/defaultCommunicationPhase.h"
#include "agents/moeoJFOPhaseAlgorithm.h"

#include "moeoCoverageMetric.h"

#include "dummyPhase.h"


#include "libs/conversions.h"

// Seed
uint32_t seedValue;

// Coefficients
double c1, c2, c3 ,c4;

template <class EOT>
void runJFO(eoPop<EOT>& pop, moeoEvalFunc<EOT>& _eval,
            eoParser& _parser, eoState& _state, unsigned long _maxGenerations,
            std::vector < tObjectiveVector > & set)
{
    // Initialization of a given seed
    rng.reseed (seedValue);

    // Operators
    eoGenOp<moeoVRP>& _op = do_make_op(_parser, _state, "CODISEO");

    // LocalSearch -
    moNeighborhoodExplorer<EOT>* explorer = new moeoVRPIterSwap<EOT>;
    moMOLS<EOT, moeoVRPObjectiveVector> _ls(explorer, _eval);

    // Comparator
    moeoParetoObjectiveVectorComparator<tObjectiveVector> _comparator;

    // Neighborhood -
    do_make_star_topology(pop);

    // Reading coefficients for the PSO
    //std::fstream file("coefficients.dat");
    //double data = 0.0;
    std::vector<double> coeff;
    //if (file)
    //{
    //   while (file >> data)
    //      coeff.push_back(data);
    //}

    coeff.push_back(c1);
    coeff.push_back(c2);
    coeff.push_back(c3);
    coeff.push_back(c4);

    // Phases
    for (size_t i = 0; i < pop.size(); i++)
    {
        pop[i].setId(i);
        pop[i].addPhase(new moeoJFOPhaseAlgorithm<moeoVRP>(_eval, _op, _ls, _comparator));

        if (coeff.size() != 0)
            static_cast<moeoJFOPhaseAlgorithm<moeoVRP>*>(pop[i].getPhase(0))->setCoefficients(coeff);

        pop[i].addPhase(new defaultCommunicationPhase<moeoVRP>);
    }

    // Main loop
    // Run phases
    // Assuming all agents have the same number of phases
    unsigned maxPhases = pop[0].numberOfPhases();
    for (size_t generation = 0; generation < _maxGenerations; generation++)
    {
        //std::cout << "Gen: " << generation << std::endl;
        for (size_t phaseIndex = 0; phaseIndex < maxPhases; phaseIndex++)
            for (size_t i = 0; i < pop.size(); i++)
                pop[i].startPhase(phaseIndex, pop);
    }

    // Output: HyperVolumen + Non-dominated set
    moeoHyperVolumeMetric < tObjectiveVector > metric(false, 1.1);
    //std::vector < tObjectiveVector > set;
    for (size_t i = 0; i < moeoJFOPhaseAlgorithm<moeoVRP>::getArchive().size(); i++)
        set.push_back(moeoJFOPhaseAlgorithm<moeoVRP>::getArchive()[i].objectiveVector());

    // If we don't have any solution within the set something weird happened
    assert(set.size() > 0);


    //std::cout << "Final Archive\n";
    //moeoJFOPhaseAlgorithm<moeoVRP>::getArchive().sortedPrintOn(std::cout);
}

template <class EOT>
void runNSGAII(eoPop<EOT>& _pop, eoCheckPoint<EOT>& _checkpoint,
               eoParser& _parser, eoState& _state, moeoEvalFunc<EOT>& _eval,
               std::vector < tObjectiveVector >& set)
{
    // Initializatijon of a given seed
    rng.reseed (seedValue);

    // Operators
    eoGenOp<EOT>& _op = do_make_op(_parser, _state);

    // definition of the archive
    moeoUnboundedArchive <EOT> archive;
    moeoArchiveUpdater <EOT> updater(archive, _pop);
    _checkpoint.add(updater);

    // NSGA-II
    moeoEA < EOT >* algo = new moeoNSGAII <EOT> (_checkpoint, _eval, _op);

    // run the algo
    (*algo)(_pop);

    // ---
    // Output: HyperVolumen + Non-dominated set
    moeoHyperVolumeMetric < tObjectiveVector > metric(false, 1.1);
    //std::vector < tObjectiveVector > set;
    for (size_t i = 0; i < archive.size(); i++)
        set.push_back(archive[i].objectiveVector());

    // If we don't have any solution within the set something weird happened
    assert(set.size() > 0);


    //std::cout << "Final Archive\n";
    //archive.sortedPrintOn(std::cout);
}



int main(int argc, char *argv[])
{

    eoParser parser(argc, argv);
    eoRealBounds* eoR;

    // Load instance and parameters
    // Checking parameters
    std::string dataset = "";
    if (argc > 1)
        dataset = std::string(argv[1]);

    if ((dataset == "") ||
            (dataset == "solomon" && argc != 10)  ||
            (dataset == "movrptw" && argc != 12))
    {
        std::cout << "Error!: Insuffient Parameters: " << argc << std::endl;
        std::cout << "You must provide:" << std::endl;
        std::cout << " - dataset: movrptw, solomon" << std::endl;
        std::cout << " - file containig the (solomon-like) data of the problem" << std::endl;
        std::cout << " - file containig the distance matrices of the problem" << std::endl;
        std::cout << " - file containig the time matrices of the problem" << std::endl;
        std::cout << " - size of population" << std::endl;
        std::cout << " - number of generations" << std::endl;
        std::cout << " - seed" << std::endl;
        std::cout << " - cofficients: c1 c2 c3 c4" << std::endl;
        std::cout << "Example "<< argv[0] << " movrptw solomonData.dat distanceMatrixData.dat timeMatrixData.dat 20 1000 0" << std::endl;
        std::cout << std::endl;
        std::cout << "Your entered: ";
        for (size_t i = 0; i < argc; i++)
            std::cout << argv[i] << " ";
        std::cout << std::endl;
        exit(1);
    }

    // ////////////////////// //
    // User parameter reading //
    // ////////////////////// //

    // Parameter for loading a problem instance
    eoValueParam<std::string> instanceParam (std::string(argv[2]), "instance", "Instance to be loaded");
    parser.processParam (instanceParam, "Problem params");
    std::string instance = instanceParam.value ();

    // Parameter for loading the distance matrix
    std::string distanceMarixFile = (dataset == "solomon")? "" : std::string(argv[3]);
    eoValueParam<std::string> instanceDistanceMatrixParam(distanceMarixFile, "distanceMatrixFile", "File that contains the distance matrix");
    parser.processParam(instanceDistanceMatrixParam, "Problem params");

    // Parameter for loading the time matrix
    std::string timeMatrixFile = (dataset == "solomon")? "" : std::string(argv[4]);
    eoValueParam<std::string> instanceTimeMatrixParam(timeMatrixFile, "timeMatrixFile", "File that contains the time matrix");
    parser.processParam(instanceTimeMatrixParam, "Problem params");

    // Parameter that establishes the max size of the fleet
    // Not used in this implementation. The size of the fleet is not fixed
    // eoValueParam<unsigned> fleet(atoi(argv[4]), "sizeOfFleet", "Number of vehicle to be used when creating the route-plan");
    // parser.processParam(fleet, "Problem params");
    // unsigned sizeOfFleet = fleet.value();

    // Parameter to set the size of the population
    unsigned populationSize = (dataset == "solomon")? unsigned(atoi(argv[3])) : unsigned(atoi(argv[5]));
    eoValueParam<unsigned> popSize(populationSize, "popSize", "Size of the population");
    parser.processParam(popSize, "Algo params");

    // Parameter to set the max number of generations
    unsigned maxGenerations = (dataset == "solomon")? unsigned(atoi(argv[4])) : unsigned(atoi(argv[6]));
    eoValueParam<unsigned long> maxGenParam(maxGenerations, "maxGen", "Maximum number of generations");
    parser.processParam(maxGenParam, "Algo psizeOfArchiveJFOarams");

    // Parameter to set the seed to generate random numbers
    seedValue = (dataset == "solomon")? uint32_t(atoi(argv[5])) : uint32_t(atoi(argv[7]));
    eoValueParam<uint32_t> seed(seedValue, "seed", "Seed to generate the stochastic behabiour");
    parser.processParam(seed, "Algo params");

    // Coefficients
    c1 = (dataset == "solomon")? atod(argv[6]) : atod(argv[8]);
    c2 = (dataset == "solomon")? atod(argv[7]) : atod(argv[9]);
    c3 = (dataset == "solomon")? atod(argv[8]) : atod(argv[10]);
    c4 = (dataset == "solomon")? atod(argv[9]) : atod(argv[11]);




    // Load an instance of the problem
    moeoVRPUtils::load(instance.c_str ());
    if (dataset == "solomon")
    {
        moeoVRPUtils::computeDistances();
        moeoVRPUtils::setTimeMatrixAsDistanceMatrix();
    }
    else
    {
        moeoVRPUtils::loadDistanceMatrix(distanceMarixFile.c_str());
        moeoVRPUtils::loadTimeMatrix(timeMatrixFile.c_str());
    }

    // --- --- ---

    // Keep things allocated
    eoState state;

    // Chromosome initialisation
    moeoVRPInit init;

    // Initialize the population
    eoPop<moeoVRP>& popJFO = do_make_pop (parser, state, init);
    popJFO.reserve(populationSize + unsigned(populationSize * 50 /100));

    eoPop<moeoVRP> popNSGAII;
    popNSGAII = popJFO;

    // Stopping criteria
    eoContinue<moeoVRP>& genCont = do_make_continue (parser, state);

    // Evaluation
    moeoVRPEvalFunc eval;

    // Checkpoint
    eoCheckPoint<moeoVRP>& checkpoint = do_make_checkpoint (parser, state, genCont);


    // Initial evaluation
    apply<moeoVRP>(eval, popNSGAII);
    apply<moeoVRP>(eval, popJFO);


    // run algorithms
    std::vector < tObjectiveVector > archiveJFO;
    std::vector < tObjectiveVector > archiveNSGAII;

    //std::cout << "Running JFO..." << std::endl;

    clock_t begin = clock();
    moeoVRPUtils::evaluations = 0;                                          
    moeoVRPUtils::numberOfOperations = 0;
 
    runJFO<moeoVRP>(popJFO, eval, parser, state, maxGenerations, archiveJFO);
    long evalJFO = moeoVRPUtils::evaluations;
    long operationsJFO = moeoVRPUtils::numberOfOperations;
    clock_t end = clock();
    double timerJFO = ((end - begin) * 1000) / CLOCKS_PER_SEC;
  
    //std::cout << "Running NSGAII..." << std::endl;
    begin = clock();
    moeoVRPUtils::evaluations = 0;                                           
    moeoVRPUtils::numberOfOperations = 0;
    runNSGAII<moeoVRP>(popNSGAII, checkpoint, parser, state, eval, archiveNSGAII);
    long evalNSGAII = moeoVRPUtils::evaluations;
    long operationsNSGAII = moeoVRPUtils::numberOfOperations;
    end = clock();
    double timerNSGAII = ((end - begin) * 1000) / CLOCKS_PER_SEC;
    //std::cout << std::endl;

    // Hypervolumen
    moeoHyperVolumeMetric < tObjectiveVector > metric(false, 1.1);
    double hypervolumenJFO = metric(archiveJFO);
    double hypervolumenNSGAII = metric(archiveNSGAII);

    // Number of non-dominated vectors found
    double ONDVJFO = archiveJFO.size();
    double ONDVNSGAII = archiveNSGAII.size();

    // Contribution
    moeoContributionMetric<tObjectiveVector> contributionMetric;
    double contributionJFO = contributionMetric(archiveJFO, archiveNSGAII);
    double contributionNSGAII =  contributionMetric(archiveNSGAII, archiveJFO);

    // Coverage
    moeoCoverageMetric<tObjectiveVector> coverageMetric;
    double coverageJFO = coverageMetric(archiveJFO, archiveNSGAII);
    double coverageNSGAII = coverageMetric(archiveNSGAII, archiveJFO);

    unsigned width = 15;

    // header of the outputfile
    for (size_t i = 0; i < argc; i++)
        std::cout << argv[i] << " ";
    std::cout << std::endl;
    std::cout << "Metric       " <<  std::setw(width) << "MOPSO" <<  std::setw(width) << "NSGA-II" << std::endl;
    std::cout << "HyperVolumen " <<  std::setw(width) << hypervolumenJFO <<  std::setw(width) << hypervolumenNSGAII << std::endl;
    std::cout << "Overall-NDV  " <<  std::setw(width) << ONDVJFO <<  std::setw(width) << ONDVNSGAII << std::endl;
    std::cout << "Contribution " <<  std::setw(width) << contributionJFO <<  std::setw(width) << contributionNSGAII << std::endl;
    std::cout << "Coverage     " <<  std::setw(width) << coverageJFO <<  std::setw(width) << coverageNSGAII << std::endl;
    std::cout << "Elapsed Time " <<  std::setw(width) << timerJFO <<  std::setw(width) << timerNSGAII << std::endl;
    std::cout << "Evaluations  " <<  std::setw(width) << evalJFO <<  std::setw(width) << evalNSGAII << std::endl;
    std::cout << "Operations   " <<  std::setw(width) << operationsJFO <<  std::setw(width) << operationsNSGAII << std::endl;
    return 0 ;

}

