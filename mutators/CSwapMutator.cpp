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

#include "CSwapMutator.h"
namespace MEVO {

CSwapMutator::CSwapMutator() : CMutator()
{

}

CSwapMutator::~CSwapMutator(){

}

ICComputation* CSwapMutator::copy() {
    CSwapMutator* mut  = new CSwapMutator();
    mut->copyParams(this);
    return mut;
}

const char* CSwapMutator::getDescripton(){
    std::stringstream desc;
    desc << "Swap mutator" << "\tProbability:\t"<<FProb;
    return desc.str().c_str();
}

void CSwapMutator::mutatePopulation(ICPopulation *_population) {
    if(FProb<=0) {return;}
    double nMut = FProb*(FIntVarSize + FRealVarSize);
    if (nMut<1.0) {
        nMut=0;
        for (int i=0; i< _population->getSize(); i++) {
            mutateIndividual(_population->getIndividual(i), FProb);
        }
    } else {
        for (int i=0; i< _population->getSize(); i++) {
            nmutateIndividual(_population->getIndividual(i), nMut);
        }
    }
}

void CSwapMutator::mutateIndividual(ICIndividual* _ind, double _p) {


    int length = _ind->getNumIntVar()-1;
    for (int i=0; i<=length;i++) {
        if (iRandGen->randomDouble(0,1) <= _p){
            int x = iRandGen->randomInteger(0, length);
            int y = iRandGen->randomInteger(0, length);
            swap(_ind, x, y);
        }
    }
}

void CSwapMutator::nmutateIndividual(ICIndividual *_ind, int _n) {
    int length = FIntVarSize + FRealVarSize-1;
    for (int i=0; i<_n;i++) {
        int x = iRandGen->randomInteger(0, length);
        int y = iRandGen->randomInteger(0, length);
        swap(_ind, x, y);
    }
}

void CSwapMutator::swap(ICIndividual* _ind, int _x, int _y) {
    int tmp = _ind->getIntVar()[_x];
    _ind->getIntVar()[_x] = _ind->getIntVar()[_y];
    _ind->getIntVar()[_y] = tmp;
}
void CSwapMutator::init(ICEvolOptimizer* _opt){
    return;
}

}
