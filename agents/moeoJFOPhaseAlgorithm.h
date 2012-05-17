#ifndef moeoJFOPhaseAlgorithm_H
#define moeoJFOPhaseAlgorithm_H

// STL
#include <memory>

// CODEA
#include "core/phase.h"
#include "core/eoSingleOp.h"
#include "core/moMOLS.h"

// Paradiseo
#include <algo/moeoEA.h>
#include <comparator/moeoStrictObjectiveVectorComparator.h>
#include <eoEvalFunc.h>
#include <eoGenOp.h>
#include <eoPop.h>

template <class EOT, class tObjectiveVector>
class moMOLS;

template <class EOT>
class moeoJFOPhaseAlgorithm : public phase<EOT>
{

private:

    // Coefficients or weights to chose where to flight
    std::vector<double> coefficients;

    // Best personal position
    EOT* bestPersonalPosition;

    // Best personal position
    static EOT* bestGlobalPosition;

    // Best neighbouring particle
    boost::shared_ptr<EOT> bestNeighbor;

    // This could be a parameter and not a member as it is not part of the JFO itself
    eoGenOp<EOT>& operators;

    // Evaluation function
    eoEvalFunc<EOT>& eval;

    // Comparator
    // Pareto-based comparator
    moeoParetoObjectiveVectorComparator<tObjectiveVector> comparator;

    // Local Search operator
    moMOLS<EOT, tObjectiveVector>& localSearch;

    // Unbounded Archive to store non-dominated solutions
    static moeoUnboundedArchive<EOT> archive;

    // Fitness Assigments
    moeoDominanceDepthFitnessAssignment <EOT> fitnessAssignment;

    // Diversity Assigments
    moeoCrowdingDiversityAssignment <EOT> diversityAssigment;


protected:

    void initialize(EOT* _particle, eoPop<EOT>& _pop)
    {
        if (_particle->invalidObjectiveVector())
            eval(*_particle);
        
        // Initialisation - assuming _agent is initialised
        if (bestPersonalPosition == NULL)
            bestPersonalPosition = new EOT(*_particle);
        bestPersonalPosition->invalidateFitness();

        
        // This will only happen once
        if (bestGlobalPosition == NULL)
        {
            bestGlobalPosition = new EOT(*_particle);
            bestGlobalPosition->invalidateFitness();
            for (size_t i = 0; i < _pop.size(); i++)
            {                
                // is 'first' is strictly dominated by 'second'? compartor(first, second)
                if ( comparator(bestGlobalPosition->objectiveVector(), _pop[i].objectiveVector() ))
                {
                    // We update the content, do not touch the pointer
                    *bestGlobalPosition = _pop[i];
                }
            }
            
        }
    }

    
public:


    // Default ctro. If not params are passed, all coefficients will be set to 0.25
    moeoJFOPhaseAlgorithm(eoEvalFunc<EOT>& _eval, eoGenOp<EOT>& _operators, moMOLS<EOT, tObjectiveVector>& _localSearch, moeoParetoObjectiveVectorComparator<tObjectiveVector>& _comparator) :
        eval(_eval), operators(_operators), localSearch(_localSearch), comparator(_comparator)

    {
        // inertial
        coefficients.push_back(0.40);

        // best personal
        coefficients.push_back(0.15);

        // best neighbor
        coefficients.push_back(0.15);

        // best global
        coefficients.push_back(0.30);

        bestGlobalPosition = NULL;
        bestPersonalPosition = NULL;
    }

    moeoJFOPhaseAlgorithm(double _c1, double _c2, double _c3, double _c4, eoEvalFunc<EOT>& _eval, eoGenOp<EOT>& _operators)
        : eval(_eval), operators(_operators)
    {
        if (_c1 + _c2 + _c3 + _c4 != 1)
            throw std::runtime_error("moeoJFOPhaseAlgorithm.h - coefficients do not sum up 1.");
        
        coefficients.push_back(_c1);
        coefficients.push_back(_c2);
        coefficients.push_back(_c3);
        coefficients.push_back(_c4);

        bestPersonalPosition = NULL;
        bestGlobalPosition = NULL;
    }

    moeoJFOPhaseAlgorithm(const moeoJFOPhaseAlgorithm<EOT>& _phase)
        : localSearch(_phase.localSearch),
          operators(_phase.operators),
          eval(_phase.eval),
          comparator(_phase.comparator)
    {
        coefficients = _phase.coefficients;

        if (_phase.bestPersonalPosition != NULL)
            bestPersonalPosition = new EOT(*(_phase.bestPersonalPosition));
        else
            bestPersonalPosition = NULL;

    }

    ~moeoJFOPhaseAlgorithm()
    {
        if (bestPersonalPosition != NULL)
            delete(bestPersonalPosition);
    }

    void setCoefficients(double _c1, double _c2, double _c3, double _c4)
    {
        assert(_c1 + _c2 + _c3 + _c4 == 1);
        if (coefficients.size() != 4)
            coefficients.resize(4);

        coefficients[0] = _c1;
        coefficients[1] = _c2;
        coefficients[2] = _c3;
        coefficients[3] = _c4;
    }

    void setCoefficients(const std::vector<double>& _coefficients)
    {
        coefficients = _coefficients;

        double sum = 0.0;
        for (size_t i = 0; i < coefficients.size(); i++)
            sum += coefficients[i];

        if (sum != 1)
            for (size_t i = 0; i < coefficients.size(); i++)
                coefficients[i] /= sum;

    }

    moeoJFOPhaseAlgorithm<EOT>* clone() const
    { return new moeoJFOPhaseAlgorithm(*this); }

    void getBestNeighboringParticle(EOT* _particle)
    {
        bestNeighbor.reset();
        
        // Let's just count the messages we had when we started (safe-parallel mode).
        size_t numberOfMessages = _particle->getInbox().size();

        // Is there any message at all?
        if (numberOfMessages == 0)
            return;
        
        boost::shared_ptr<EOT> content;
        tMessage message;

        eoPop<EOT> auxPop;
        
        for (size_t i = 0; i < numberOfMessages; i++) 
        {
            if (_particle->getPointerToInbox()->try_pop(message))
            {
                content = boost::any_cast< boost::shared_ptr<EOT> > (message[1].getContent());
                auxPop.push_back(*content);
            }
            //if (bestNeighbor == NULL || comparator( bestNeighbor->objectiveVector(), content->objectiveVector()) )
            //    bestNeighbor = content;
        }

        //fitnessAssignment(auxPop);
        //bestNeighbor = boost::shared_ptr<EOT>(new EOT(auxPop.best_element()));

        fitnessAssignment(auxPop);
        unsigned bestIndividual = 0;
        double bestValue = 0.0;

        for (size_t i = 0; i < auxPop.size(); i++)
            if (bestValue < auxPop[i].fitness())
            {
                bestValue = auxPop[i].fitness();
                bestIndividual = i;
            }

        bestNeighbor = boost::shared_ptr<EOT>(new EOT(auxPop[bestIndividual]));

    }
    
    // Operator controlling just one individual
    void operator() (EOT* _agent, eoPop<EOT>& _pop)
    {
        //std::cout << "I am agent " << _agent->getId() << " [of " << _pop.size() << "]" << std::endl;
        //std::cout << "_c1: " << coefficients[0] << " _c2: " << coefficients[1] << " _c3: " << coefficients[2] << " _c4: " << coefficients[3] << std::endl;

        // Initialization if required (bestPersonalPosition and bestGlobalPosition)
        initialize(_agent, _pop);

        // First we randomly select the type of move according to the coefficients
        unsigned move = rng.roulette_wheel<double>(coefficients, 1);

        // Next position of the particle + (auxixiar particle for the second offspring).
        EOT* nextParticle = new EOT(*_agent);
        EOT* auxParticle = NULL;

        
        // Flight (operation)
        eoSingleOp<EOT>& flight = static_cast<eoSingleOp<EOT>&>(operators);
        switch(move)
        {

        // Inertia (mutation)
        case 0:
            flight.apply(*nextParticle);
            break;

            // Towards the best personal position (b_i)
        case 1:
            if (bestPersonalPosition != NULL && *bestPersonalPosition != *nextParticle)
            {
                auxParticle = new EOT(*bestPersonalPosition);
                flight.apply(*nextParticle, *auxParticle);
            }
            else
                flight.apply(*nextParticle);
            break;

            // Towards the best neighboring particle's position (g_i)
        case 2:
            getBestNeighboringParticle(_agent);

            if (bestNeighbor != NULL && *bestNeighbor != *nextParticle)
            {
                auxParticle = new EOT(*bestNeighbor);
                flight.apply(*nextParticle, *auxParticle);
            }
            else
                flight.apply(*nextParticle);
            break;

            // Towards the best position of the swarm so far (g)
        case 3:
            if (bestGlobalPosition != NULL && *bestGlobalPosition != *nextParticle)
            {
                auxParticle = new EOT(*bestGlobalPosition);
                flight.apply(*nextParticle, *auxParticle);
            }
            else
                flight.apply(*nextParticle);
            break;

            // This should not happen
        default:
            std::cout << "Move: " << move << "???" << std::endl;
            throw std::runtime_error("moeoJFOPhaseAlgorithm.h - move out of bounds");
        }


        // Evaluate
        eval(*nextParticle);
        archive(*nextParticle);


        //if (localSearch(*nextParticle))
        //   archive(*nextParticle);

        // We push the new particle's position into the pop to use assigment operators.
        _pop.push_back(*nextParticle);

        fitnessAssignment(_pop);
        diversityAssigment(_pop);

        if (_pop[_agent->getId()].fitness() < _pop[_pop.size() - 1].fitness())
        //if (_pop[_agent->getId()].diversity() < _pop[_pop.size() - 1].diversity())
        {
            _agent = nextParticle;

            // In case of improvement, we update the content and NOT the pointer
            if ( bestPersonalPosition->invalidFitness() || bestPersonalPosition->fitness(), _pop[_pop.size() - 1].fitness() )
                *bestPersonalPosition = _pop[_pop.size() - 1];

            if ( bestGlobalPosition->invalidFitness() || bestGlobalPosition->fitness(), _pop[_pop.size() - 1].fitness() )
                *bestGlobalPosition = *bestPersonalPosition;
        }

        // Clear aux data
        if (nextParticle != NULL)
            delete(nextParticle);
        if (auxParticle != NULL)
            delete(auxParticle);

        // Remove the nextParticle of the population
        _pop.pop_back();
    }

    // Operator controlling not one but the whole population
    void operator() (eoPop<EOT>& pop)
    { }

    static moeoUnboundedArchive<EOT> getArchive()
    {
        return archive;
    }
};

// Initialisation of the static member (bestGlobalPosition is shared accross all particles)
template <class EOT>
EOT* moeoJFOPhaseAlgorithm<EOT>::bestGlobalPosition;

template <class EOT>
moeoUnboundedArchive<EOT> moeoJFOPhaseAlgorithm<EOT>::archive;





#endif // moeoJFOPhaseAlgorithm_H
