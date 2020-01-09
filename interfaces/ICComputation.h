#ifndef ICCALCULATION_H
#define ICCALCULATION_H

#include <deque>
#include <utility>

namespace MEVO {

class ICEvolOptimizer;

//! Interface class for computation task
/** Provides functions for implementation of various computations
    ex. mutation, crossover, evaluation of populations.
  */
class ICComputation {
public:

    /** pure virtual function which returns computation type
        this function is implemented in inherited classes.
        @return unique name of a computation type
    */
    virtual const char* getDescripton()=0;

    /** Default destructor
    */
    virtual ~ICComputation(){}

    /** pure virtual function for initializing a computation object
        this function is implemented in inherited classes.
        @param _opt pointer to a evolutionary algorithm.
      */
    virtual void init(ICEvolOptimizer* _opt)=0;

    /** pure virtual function for execution of a computation
        this function is implemented in inherited classes.
        @param _params the parameters necessary to perform computation
                        ex. population to mutate
      */
    virtual void execute(void* _params)=0;

    /** pure virtual function for copying a computation object
        this function is implemented in inherited classes.
        @return an exact copy of an computation object.
      */
    virtual ICComputation* copy()=0;
};
}
#endif // ICCALCULATION_H
