#ifndef ICCROSSOVER_H
#define ICCROSSOVER_H
#include <deque>

#include "ICComputation.h"

#include "ICPopulation.h"
namespace MEVO{

class CGeneticAlgorithm;


//! Interface class for crossover
/** Provides functions for implementation of different crossover types.
  */
class ICCrossover : virtual public ICComputation {
public:

    /** Default destructor
    */
    virtual ~ICCrossover(){}

    /** pure virtual function for crossover parent population
        this function is implemented in inherited classes.
        @param _parentPopulation reference to a parent population
        @param _offspringPopulation reference to a offspring population
      */
    virtual void crossover(ICPopulation* _parentPopulation, ICPopulation* _offspringPopulation)=0;

    /** function sets crossover probability
        @param probability value
    */
    virtual void setProbability(double _p)=0;

    /** function returns crossover probability
        @return _p probability value
    */
    virtual double  getProbability()=0;
};
}
#endif // ICCROSSOVER_H
