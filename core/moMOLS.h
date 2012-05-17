#ifndef MOMOVNS_H
#define MOMOVNS_H

//#include <moAlgo.h>
#include <mo>

#include "../agents/moeoJFOPhaseAlgorithm.h"
//! Variable Neighbors Search (VNS)
/*!
  Class which describes the algorithm for a Variable Neighbors Search.
*/

template < class EOT, class tObjectiveVector>
class moMOLS : public moAlgo < EOT>
{

public:

    //! Generic constructor
    /*!
    Generic constructor using a moExpl

    \param _explorer Vector of Neighborhoods.
    \param _full_evaluation The evaluation function.
    */
    moMOLS(moNeighborhoodExplorer< EOT > * _explorer, eoEvalFunc < EOT> & _evaluation):
        explorer(_explorer),
        eval(_evaluation),
        acceptanceCriterion("bestImprovement") { }


    void setAcceptanceStrategy(const std::string& _criterion)
    {
        acceptanceCriterion = _criterion;
    }

    bool firstImprovement() const
    {
        return (acceptanceCriterion == "firstImprovemnt");
    }

    bool bestImprovement() const
    {
        return (acceptanceCriterion == "bestImprovemnt");
    }


    bool randomImprovement() const
    {
        return (acceptanceCriterion == "randomImprovemnt");
    }

    //! Function which launches the VNS
    /*!
    The LS has to improve a current solution.

    \param _solution a current solution to improve (we assume this is a multi-objective solution)
    \return true if there was an improvement, false otherwise
  */
    bool operator()(EOT & _solution)
    {
        bool improvement = true;
        bool improvementHappened = false;

        if (_solution.invalidObjectiveVector())
            eval(_solution);

        EOT newSolution = _solution;
        EOT bestSolution = _solution;
        EOT solutionPrime = _solution;

        explorer->initParam(newSolution);

        while (improvement)
        {
            improvement = false;
            newSolution = bestSolution;

            while ( explorer->move(newSolution) )
            {
                eval(newSolution);
                if ( comparator(bestSolution.objectiveVector(), newSolution.objectiveVector()))
                {
                   bestSolution = newSolution;
                   moeoJFOPhaseAlgorithm<EOT>::getArchive()(newSolution);

                   improvement = true;
                   improvementHappened = true;
                }

                if (improvement && firstImprovement())
                {
                    explorer->initParam(newSolution);
                    break;
                }

                newSolution = solutionPrime;

            }
        }

        if (improvementHappened)
        {
            _solution = bestSolution;
            return true;
        }

        return false;
    }

private:

    //!Neighborhood
    moNeighborhoodExplorer<EOT>* explorer;

    //!The full evaluation function
    eoEvalFunc<EOT> & eval;

    //!Compator based on strict pareto dominance
    moeoWeakObjectiveVectorComparator<tObjectiveVector> comparator;

    //!Acceptance strategy
    std::string acceptanceCriterion;

};

#endif // MOMOVNS_H
