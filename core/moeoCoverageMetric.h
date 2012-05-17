#ifndef MOEOCOVERAGEMETRIC_H
#define MOEOCOVERAGEMETRIC_H

/*
* <moeoContributionMetric.h>
* Copyright (C) DOLPHIN Project-Team, INRIA Futurs, 2006-2007
* (C) OPAC Team, LIFL, 2002-2007
*
* Juan Castro
* Nottingham University - 2011
*
* ParadisEO WebSite : http://paradiseo.gforge.inria.fr
* Contact: paradiseo-help@lists.gforge.inria.fr
*
*/
//-----------------------------------------------------------------------------

#include <comparator/moeoParetoObjectiveVectorComparator.h>
#include <comparator/moeoWeakObjectiveVectorComparator.h>
#include <metric/moeoMetric.h>

/**
 * The Coverage Metric measures the extent to which one solution set B is covered by another solution set A.
 */
template < class ObjectiveVector >
class moeoCoverageMetric : public moeoVectorVsVectorBinaryMetric < ObjectiveVector, double >
  {
  public:

    /**
     * Returns the coverage of the Pareto set '_setB' relatively to the Pareto set '_setA'
     * @param _setA the first Pareto set
     * @param _setB the second Pareto set
     */
    double operator()(const std::vector < ObjectiveVector > & _setA, const std::vector < ObjectiveVector > & _setB)
    {
        long counter = 0;
        for (size_t i = 0; i < _setB.size(); i++)
            for (size_t j = 0; j < _setA.size(); j++)
                if (paretoWeakComparator(_setA[j], _setB[i]))
                {
                    counter++;
                    break;
                }

        return (double)counter/(double)_setB.size();
    }


  private:

    /** Functor to compare two objective vectors according to Pareto dominance relation */
    moeoParetoObjectiveVectorComparator < ObjectiveVector > paretoComparator;

    /** Functor to compare two objective vectors according to Pareto weak dominance relation */
    moeoWeakObjectiveVectorComparator < ObjectiveVector > paretoWeakComparator;

};

#endif // MOEOCOVERAGEMETRIC_H
