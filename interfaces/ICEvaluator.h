#ifndef ICEVALUATOR_H
#define ICEVALUATOR_H

#include <deque>

#include "ICPopulation.h"
#include "ICProblem.h"
#include "ICComputation.h"

namespace MEVO {

//! Interface class for evaluation of a population
/** Provides functions for implementation of evaluation task.
  */
class ICEvaluator : virtual public ICComputation {
public:

    /** default destructor
      */
    virtual ~ICEvaluator(){}

    /** Pure virtual function for evaluation of a population
        this function is implemented in inherited classes.
        @param _population reference to a population
      */
    virtual void evaluatePopulation(ICPopulation* _population)=0;
};
}
#endif // ICEVALUATOR_H
