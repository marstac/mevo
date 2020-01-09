#ifndef CINITIALIZER_H
#define CINITIALIZER_H

#include "../interfaces/ICInitializer.h"
#include "../interfaces/ICEvolOptimizer.h"
#include "../interfaces/ICProblem.h"
#include "../utils/CWithRNGen.h"

namespace MEVO{

//! Base class for initializer
/** Provides functions for implementation of different initializer types.
  */
class CInitializer : virtual public ICInitializer, virtual public CWithRNGen {
public:

    /** Constructor */
    CInitializer();

    /** Destructor */
    virtual ~CInitializer();

    /** function for initializing an initalizer object
        sets real and integer variables number and boundary values.
        @param _opt pointer to a evolutionary algorithm.
    */
    virtual void init(ICEvolOptimizer* _opt);

    /** function for execution of an initializer
            this function calls initialize function.
            @param _params the parameters necessary to perform initialization
    */
    virtual void execute(void* _params);

    /** pure virtual function for initialization of population
        this function is implemented in inherited classes.
        @param _population reference to a population
      */
    virtual void initialize(ICPopulation* _population)=0;

    /** pure virtual function for copying a computation object
            this function is implemented in inherited classes.
            @return an exact copy of an computation object.
    */
    virtual ICComputation* copy()=0;

protected:

    /** function copies parameters from selected initializer object
            @param _i initializer object
        */
    virtual void copyParams(CInitializer* _i);

    /** number of real coded variables to be initialized*/
    unsigned int FRealVarSize;

    /** number of integer coded variables to be initialized*/
    unsigned int FIntVarSize;

    /** high boundary values for integer coded parameters*/
    std::deque<int> FHILimits;

    /** low boundary values for integer coded parameters*/
    std::deque<int> FLILimits;

    /** high boundary values for real coded parameters*/
    std::deque<double> FHRLimits;

     /** low boundary values for real coded parameters*/
    std::deque<double> FLRLimits;

};
}

#endif // CINITIALIZER_H
