#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <iostream>

#include "../../core/CIndividual.h"

class AMGA2Individual : public MEVO::CIndividual {
public:
    AMGA2Individual(int _numVar, int _numIntVar, int _numObj, int _numConstr);
    AMGA2Individual(const AMGA2Individual&);
    const AMGA2Individual & operator=(const AMGA2Individual&);

    virtual ICIndividual* copy();

    double* getNormalObj() {return normalObj;}

    void setRank(int _rank);
    int getRank();
    int getEvalCounter();
    void setEvalCounter(int _evalCounter);

    virtual ~AMGA2Individual();

    virtual void copyParams(const ICIndividual* _ind);

    double* normalObj;
    double* normalConstr;
    unsigned int evalCounter;
    unsigned int rank;
};

#endif
