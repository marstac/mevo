#ifndef CMCHVINDICATOR_H
#define CMCHVINDICATOR_H

#include "CHyperVolumeIndicator.h"
#include "../utils/CWithRNGen.h"

#include "../utils/functions.h"

namespace MEVO {
//! class for indication of a population in terms of hypervolume
/** class for indication of a population in terms of hypervolume. Dummy monte carlo sampling method.
  */
class CMCSHVIndicator : public CHyperVolumeIndicator, CWithRNGen
{
public:
    /** Constructor */
    CMCSHVIndicator();

    /** Destructor */
    ~CMCSHVIndicator();

    /** Function returns name of an indicator
      @return algorithm name
     */
    virtual const char* getName();

    /** Function returns no. of samples in Monte Carlo sampling
        @return no. of samples used in MC sampling
    */
    unsigned int getFSamples() const;

    /** Function sets no. of samples in Monte Carlo sampling
        @param value no. of samples used in MC sampling
    */
    void setFSamples(unsigned int value);

protected:
    double calculateHypervolume(std::deque<std::deque<double> > &_front);

    unsigned int FSamples;
    std::deque<double> FMins;
    double FMaxVol;
};
}
#endif // CSIMPLEXHVINDICATOR_H
