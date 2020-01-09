#ifndef CMUTATOR_H
#define CMUTATOR_H

#include <sstream>

#include "../interfaces/ICMutator.h"
#include "../interfaces/ICEvolOptimizer.h"
#include "../interfaces/ICProblem.h"
#include "../utils/CWithRNGen.h"

namespace MEVO {

//! Base class for mutator
/** Provides functions for implementation of different mutation types.
  */
class CMutator : virtual public ICMutator, virtual public CWithRNGen {
public:

    /** Constructor */
    CMutator();

    /** Destructor*/
    virtual ~CMutator();

    /** function for initializing a crossover object
            sets real and integer variables number.
            @param _opt pointer to a evolutionary algorithm.
        */
    virtual void init(ICEvolOptimizer* _opt);

    /** function for execution of a crossover
                this function calls mutatePopulation function.
                @param _params the parameters necessary to perform crossover
        */
    virtual void execute(void* _params);

    /** pure virtual function for mutation of a population
                this function is implemented in inherited classes.
                @param _population reference to a population
        */
    virtual void mutatePopulation(ICPopulation* _population)=0;


    /** pure virtual function for copying a mutator object
                this function is implemented in inherited classes.
                @return an exact copy of an computation object.
        */
    virtual ICComputation* copy()=0;

    /** function sets mutation probability
            @param probability value
        */
    void setProbability(double _p);

    /** function returns mutation probability
            @return _p probability value
        */
    double  getProbability();

protected:
    /** function copies parameters from selected mutator object
            @param _c mutator object
        */
    virtual void copyParams(CMutator* _m);

    /** number of real coded variables to be mutated*/
    unsigned int FRealVarSize;

    /** number of integer coded variables to be mutated*/
    unsigned int FIntVarSize;

    /** mutation probability*/
    double FProb;

};


}
#endif // CMUTATOR_H
