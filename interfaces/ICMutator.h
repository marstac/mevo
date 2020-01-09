#ifndef ICMUTATOR_H
#define ICMUTATOR_H
#include <deque>

#include "ICComputation.h"
#include "ICPopulation.h"

namespace MEVO {

//! Interface class for mutatorr
/** Provides functions for implementation of different mutation types.
  */
class ICMutator : virtual public ICComputation {
public:

    /** Default destructor */
    virtual ~ICMutator(){}

    /** pure virtual function for mutation of individuals in a population
        this function is implemented in inherited classes.
        @param _population reference to a population
      */
    virtual void mutatePopulation(ICPopulation* _population)=0;

    /** function sets mutation probability
            @param probability value
        */
    virtual void setProbability(double _p)=0;

    /** function returns mutation probability
            @return _p probability value
        */
    virtual double  getProbability()=0;
};

}
#endif // ICMUTATOR_H
