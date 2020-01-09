#include <iostream>
#include <cstdlib>
#include <cmath>

#include "Individual.h"
#include "AMGA2.h"

using namespace std;

AMGA2Individual::AMGA2Individual(int _numVar, int _numIntVar, int _numObj, int _numConstr) : CIndividual(_numVar, _numIntVar, _numObj, _numConstr){

  evalCounter = 0;
  rank = 0;

  normalObj = new double[_numObj];

 for (register int i = 0; i < _numObj; i++) {
    normalObj[i] = 0.0;
  }
  if (_numConstr > 0) {
      normalConstr = new double[_numConstr];
    for (register int i = 0; i < _numConstr; i++) {
      normalConstr[i] = 0.0;
    }
  }
}

AMGA2Individual::AMGA2Individual(const AMGA2Individual &rhs) : CIndividual (rhs){
  normalObj = new double[FNumObj];
  if (FNumConstr > 0) {
    normalConstr = new double[FNumConstr];
  }
  AMGA2Individual::copyParams(&rhs);
}

const AMGA2Individual& AMGA2Individual::operator=(const AMGA2Individual& rhs) {
  if (this == &rhs) {
    return (*this);
  }
  CIndividual::copyParams(&rhs);
  copyParams(&rhs);
  return (*this);
}

MEVO::ICIndividual *AMGA2Individual::copy()
{
    AMGA2Individual* ind = new AMGA2Individual(*this);
    return ind;
}

AMGA2Individual::~AMGA2Individual()  {
  if (FNumConstr > 0) {
     delete[](normalConstr);
  }
  delete [] normalObj;
}

void AMGA2Individual::copyParams(const ICIndividual *rhs) {
    const AMGA2Individual* ind = dynamic_cast<const AMGA2Individual*>(rhs);
    if(!ind){return;}

    for (register int i = 0; i < FNumObj; i++) {
     normalObj[i] = ind->normalObj[i];
  }
  if (FNumConstr > 0) {
    for (register int i = 0; i < FNumConstr; i++) {
      normalConstr[i] = ind->normalConstr[i];
    }
  }

  evalCounter = ind->evalCounter;
  rank = ind->rank;
  return;
}

void AMGA2Individual::setRank(int _rank) {
  rank = _rank;
}

int AMGA2Individual::getRank() {
  return rank;
}

int AMGA2Individual::getEvalCounter() {
  return evalCounter;
}

void AMGA2Individual::setEvalCounter(int _evalCounter) {
  evalCounter = _evalCounter;
}
