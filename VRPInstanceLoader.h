#ifndef VRPINSTANCELOADER_H
#define VRPINSTANCELOADER_H

/* **********************************************************************************
   ********************************************************************************** */

// VRP Related things
#include "problems/VRPTW/moeoVRPUtils.h"


// Parsing
#include "eoParser.h"

// Checks for help demand, and writes the status file
// and make_help; in libutils

void make_help (eoParser& _parser);

/* **********************************************************************************
   ********************************************************************************** */

// Now use all of the above, + representation dependent things
void loadInstance (int argc, char* argv [], eoParser& parser)
{
    // Checking parameters
    std::string dataset = "";
    if (argc > 1)
        dataset = std::string(argv[1]);

    if ((dataset == "") ||
            (dataset == "solomon" && argc != 7)  ||
            (dataset == "movrptw" && argc != 8))
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
        std::cout << "Example "<< argv[0] << " movrptw moeoNSGAII solomonData.dat distanceMatrixData.dat timeMatrixData.dat 20 1000 0" << std::endl;
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
    unsigned populationSize = (dataset == "solomon")? unsigned(atoi(argv[4])) : unsigned(atoi(argv[5]));
    eoValueParam<unsigned> popSize(populationSize, "popSize", "Size of the population");
    parser.processParam(popSize, "Algo params");

    // Parameter to set the max number of generations
    unsigned maxGenerations = (dataset == "solomon")? unsigned(atoi(argv[5])) : unsigned(atoi(argv[6]));
    eoValueParam<unsigned> maxGenParam(maxGenerations, "maxGen", "Maximum number of generations");
    parser.processParam(maxGenParam, "Algo params");

    // Parameter to set the seed to generate random numbers
    uint32_t seedValue = (dataset == "solomon")? uint32_t(atoi(argv[6])) : uint32_t(atoi(argv[7]));
    eoValueParam<uint32_t> seed(seedValue, "seed", "Seed to generate the stochastic behabiour");
    parser.processParam(seed, "Algo params");

    // Initialization of a given seed
    rng.reseed (seed.value());

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
}


#endif // VRPINSTANCELOADER_H
