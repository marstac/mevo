#ifndef ICCOMPUTER_H
#define ICCOMPUTER_H
#include "ICComputation.h"

namespace MEVO {

class ICEvolOptimizer;
//! Interface class for execution of computations
/** Provides functions for executin computation tasks
    ex. mutation, crossover, evaluation of populations.
  */
class ICComputer {
public:

    /** Description of selected computer
    @return brief description of chosen computer
    */
    virtual const char *getName()=0;

    /** pure virtual function for execution of a computation
        this function is implemented in inherited classes.
        @param _params the parameters necessary to perform computation
                        ex. population to mutate
      */
    virtual void compute(void* _params)=0;

    /** Default destructor
    */
    virtual ~ICComputer(){}

    /** pure virtual function for to assign a computation task
        this function is implemented in inherited classes.
        @param _c a pointer to a computation object.
      */
    virtual void setComputationTask(ICComputation* _c)=0;

    /** pure virtual function which returns assigned computation task
        this function is implemented in inherited classes.
        @return  pointer to a computation object.
      */
    virtual ICComputation* getComputationTask()=0;

    /** pure virtual function for copying a computer object
        this function is implemented in inherited classes.
        @return an exact copy of an computer object.
      */
    virtual ICComputer* copy()=0;

    /** pure virtual function for initializing a computer object
        this function is implemented in inherited classes.
        @param _opt pointer to a evolutionary algorithm.
      */
    virtual void init(ICEvolOptimizer* _opt)=0;
};
}
#endif // ICCOMPUTER_H
