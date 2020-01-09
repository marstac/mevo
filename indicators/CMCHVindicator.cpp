#include "CMCHVindicator.h"
#include "iostream"
#include <math.h>


namespace MEVO {


CMCSHVIndicator::CMCSHVIndicator() : CHyperVolumeIndicator(), CWithRNGen() {
    FSamples=100000;
}


CMCSHVIndicator::~CMCSHVIndicator(){}

const char *CMCSHVIndicator::getName()
{
    return "Hypervolume indicator (MC sampling)";
}

unsigned int CMCSHVIndicator::getFSamples() const
{
    return FSamples;
}

void CMCSHVIndicator::setFSamples(unsigned int value)
{
    FSamples = value;
}


double CMCSHVIndicator::calculateHypervolume(std::deque<std::deque<double> > &_front) {

    FMins = getFrontMins(_front);

    double volume=1;
    for(int i=0; i<FMins.size();i++) {
        volume*=(FRefPoint[i] - FMins[i]);
    }
    unsigned int N=0;
    std::deque<double> x(_front[0].size(), 0);
    for(int k=0; k< FSamples; k++) {
        for (int j=0; j<_front[0].size(); j++) {
            x[j] = iRandGen->randomDouble(FMins[j],FRefPoint[j]);
        }

        for (int i=0; i<_front.size(); i++) {
            bool is=true;
            for (int j=0; j<_front[0].size();j++) {
                if (x[j]<=_front[i][j])  {
                    is = false;break;
                }
            }if(is){N++;break;}

        }

    }
    return (double)N/(FSamples)*volume;

}


}
