/*  Author:
  Marcin A. Stachura <m.stachura@mchtr.pw.edu.pl>

  Copyright (c) 2015 Marcin A. Stachura

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CIndividual.h"
namespace MEVO{
CIndividual::CIndividual(int _numRealVar, int _numIntVar, int _numObj, int _numConstr) {
  FConstrViolation = 0.0;
  FCrowdingDistance = 0.0;
  FNumRealVar = _numRealVar;
  FNumIntVar = _numIntVar;

  FNumObj = _numObj;
  FNumConstr = _numConstr;

  FRealVar = NULL;
  FInegerVar = NULL;
  FConstr = NULL;

  if (_numRealVar>0) {
    FRealVar = new double[_numRealVar];
  }

  if (_numIntVar>0) {
    FInegerVar = new int[_numIntVar];
  }

  FObj = new double[_numObj];

  for (register int i = 0; i < _numRealVar; i++) {
    FRealVar[i] = 0.0;
  }

  for (register int i = 0; i < _numIntVar; i++) {
    FInegerVar[i] = 0.0;
  }

  for (register int i = 0; i < _numObj; i++) {
    FObj[i] = 0.0;
  }
  if (_numConstr > 0) {
    FConstr = new double[_numConstr];
    for (register int i = 0; i < _numConstr; i++) {
      FConstr[i] = 0.0;
    }
  }
}

CIndividual::CIndividual(const CIndividual &rhs) {
  FNumRealVar = rhs.FNumRealVar;
  FNumObj = rhs.FNumObj;
  FNumConstr = rhs.FNumConstr;
  FNumIntVar = rhs.FNumIntVar;

  if (FNumRealVar>0) {
    FRealVar = new double[FNumRealVar];
  }

  if (FNumIntVar>0) {
    FInegerVar = new int[FNumIntVar];
  }

  FObj = new double[FNumObj];

  if (FNumConstr > 0) {
    FConstr = new double[FNumConstr];

  }
  CIndividual::copyParams(&rhs);
}

ICIndividual *CIndividual::copy()
{
    CIndividual* ind = new CIndividual(*this);
    return ind;
}

CIndividual::~CIndividual() {
  if (FNumRealVar) delete[] FRealVar;
  if (FNumIntVar) delete[] FInegerVar;
  delete [] FObj;
  if (FNumConstr > 0) {
    delete[](FConstr);
  }
}

bool CIndividual::operator==(const ICIndividual& _i) {
    ICIndividual* ind = (ICIndividual*)&_i;

//    if (FNumRealVar || FNumIntVar) {
//         for (int i=0; i<FNumRealVar; i++) {
//             if (this->getRealVar()[i]!=ind->getRealVar()[i]) {
//                return false;
//            }
//        }

//        for (int i=0; i<FNumIntVar; i++) {
//            if (this->getIntVar()[i]!=ind->getIntVar()[i]) {
//                return false;
//            }
//        }
//    }
    for (int i=0; i< FNumObj; i++) {
        if (this->getObj()[i] != ind->getObj()[i]) {
             return false;
        }

    }

    return true;
}


void CIndividual::copyParams(const ICIndividual *_ind) {
  const CIndividual* ind = dynamic_cast<const CIndividual*>(_ind);
  if(!ind) return;

  for (int i = 0; i < FNumRealVar; i++) {
    FRealVar[i] = ind->FRealVar[i];
  }

  for (int i = 0; i < FNumIntVar; i++) {
    FInegerVar[i] = ind->FInegerVar[i];
  }

  for (int i = 0; i < FNumObj; i++) {
    FObj[i] = ind->FObj[i];
    }
  if (FNumConstr > 0) {
    for (int i = 0; i < FNumConstr; i++) {
      FConstr[i] = ind->FConstr[i];
    }
  }
  FConstrViolation = ind->FConstrViolation;
  FCrowdingDistance = ind->FCrowdingDistance;
}

void CIndividual::setEps(const std::deque<double> _eps)
{
    FEps=_eps;
}

double CIndividual::getConstrViolation() {
  return FConstrViolation;
}

void CIndividual::setConstrViolation(double _constrViolation) {
  FConstrViolation = _constrViolation;
}


int CIndividual::checkConstrDominance(ICIndividual *_ind) {
    if (this->getConstrViolation()>0 && _ind->getConstrViolation()>0) {
        if (this->getConstrViolation() < _ind->getConstrViolation()) {
            return 1;
        } else {
            if (this->getConstrViolation() > _ind->getConstrViolation()) {
                return -1;
            }
            else {
                return 0;
            }
       }
    } else {
        if (this->getConstrViolation() > 0 && _ind->getConstrViolation() <= 0) {
            return -1;
        } else {
            if (this->getConstrViolation() <= 0 && _ind->getConstrViolation() >0) {
                return 1;
            }
        }
    }
    return 0;
}

int CIndividual::checkNDominance(ICIndividual *_ind) {
    int flag1=0;
    int flag2=0;

    for (int i=0; i<FNumObj; i++) {
        if (this->getObj()[i] < _ind->getObj()[i]) {
                flag1 = 1;

        } else {
            if (this->getObj()[i] > _ind->getObj()[i]) {
                flag2 = 1;
            }
        }
    }
    if (flag1==1 && flag2==0) {
        return 1;
    } else if (flag1==0 && flag2==1) {
        return -1;
    }
    return 0;
}
int CIndividual::checkEpsDominance(ICIndividual *_ind, int _sd, std::deque<double>* _eps) {
    int flag1 = 0;
    int flag2 = 0;

    if(_sd==1) {
        for (int i=0; i<FNumObj; i++) {
            if ((this->getObj()[i]+_eps->operator [](i)) < _ind->getObj()[i]){
                flag1 = 1;
            }
        }
        if (flag1 == 1){
            return 1;
        } else {
            return 0;
        }
    } else if (_sd==-1) {
        for (int i=0; i<FNumObj; i++) {
            if (this->getObj()[i] > (_ind->getObj()[i]+_eps->operator [](i))) {
                flag2 = 1;
            }
        }
        if (flag2 == 1){
            return -1;
        } else {
            return 0;
        }
    }
    return 0;
}

int CIndividual::checkDominance(ICIndividual *_ind, std::deque<double>* _eps) {

    int cv = checkConstrDominance(_ind);
    if (cv==0){
        int sd = checkNDominance(_ind);
        if(_eps) {
            if (_eps->size()!= FNumObj) {
                return sd;
            }
            return checkEpsDominance(_ind, sd, _eps);
        }
        return sd;
    } else {
        return cv;
    }
    return 0;
}



void CIndividual::setCrowdingDistance(double _c) {
    FCrowdingDistance = _c;
}


double CIndividual::getCrowdingDistance() const {
    return FCrowdingDistance;
}


double* CIndividual::getRealVar() {return FRealVar;}
int* CIndividual::getIntVar() {return FInegerVar;}

double* CIndividual::getObj() {return FObj;}
double* CIndividual::getConstr() {return FConstr;}
int  CIndividual::getNumRealVar(){return FNumRealVar;}
int  CIndividual::getNumIntVar(){return FNumIntVar;}

int  CIndividual::getNumObj(){return FNumObj;}
int  CIndividual::getNumConstr(){return FNumConstr;}

}
