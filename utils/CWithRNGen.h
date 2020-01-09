#ifndef CWITHRNGEN_H
#define CWITHRNGEN_H

#include "../interfaces/ICRandomNumbersGenerator.h"
namespace MEVO {

//! an auxilliary class for objects which have random numbers generator
/** Class contains a random number generator object
  */
class CWithRNGen
{
public:
    /** Constructor */
    CWithRNGen();

    /** copy constructor*/
    CWithRNGen(const CWithRNGen& _rg);

    /** Destructor */
    virtual ~CWithRNGen();

    /** function sets a random number generator object
      @param _g pointer to a randoim number generator object
      */
    void setRandomNumbersGenerator(ICRandomNumbersGenerator* _g);

    /** function returns a random number generator object
      @return _g pointer to a randoim number generator object
      */
    ICRandomNumbersGenerator* getRandomNumbersGenerator();

protected:
    /** Random numbers generator object */
     ICRandomNumbersGenerator* iRandGen;
};
}
#endif // CWITHRNGEN_H
