#ifndef CUNIFORMINITIALIZER_H
#define CUNIFORMINITIALIZER_H
#include "CInitializer.h"
namespace MEVO {
class CUniformInitializer : public CInitializer
{
public:

    /** Constructor */
    CUniformInitializer();

    /** Destructor */
    ~CUniformInitializer();

    /** function which returns initializer type
        @return unique name of a initializer type
    */
    virtual const char* getDescripton(){return "Uniform initializer";}

    /** function for uniform initialization of population
        creates a random population
        @param _population reference to a population
      */
    virtual void initialize(ICPopulation* _population);

    /** pure virtual function for copying a computation object
            this function is implemented in inherited classes.
            @return an exact copy of an computation object.
    */
    virtual ICComputation* copy();
};
}
#endif // CUNIFORMINITIALIZER_H
