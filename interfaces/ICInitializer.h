#ifndef ICINITIALIZER_H
#define ICINITIALIZER_H
#include <deque>

#include "ICComputation.h"

#include "ICPopulation.h"
namespace MEVO{

//! Interface class for initializer
/** Provides functions for implementation of different initialization types.
  */
class ICInitializer : virtual public ICComputation {
public:

    /** Default destructor
    */
    virtual ~ICInitializer(){};

    /** pure virtual function for initialization of population
        this function is implemented in inherited classes.
        @param _population reference to a population
      */
    virtual void initialize(ICPopulation* _population)=0;
};
}
#endif // ICINITIALIZER_H
