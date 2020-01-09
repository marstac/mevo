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


#include "CPSOalgorithm.h"
#include "../utils/messages.h"
#include "../mutators/CPertutbation.h"
namespace MEVO{
CPSOAlgorithm::CPSOAlgorithm() : CEvolOptimizer()
{
    setSocialAcc(2);
    setCognitiveAcc(2);
    setInertia(0.4);

    setMutator(new CPerturbation());

}

CPSOAlgorithm::~CPSOAlgorithm() {}

bool CPSOAlgorithm::checkConf(){
    if (CEvolOptimizer::checkConf()) {
        return true;
    }
    return false;
}
void CPSOAlgorithm::setSocialAcc(double _sacc){
    if (_sacc < 0) {
       fireOnErrorNotify(E_OPT_PARAM_SACCEL);
    } else {
        C1 = _sacc;
    }

}

void CPSOAlgorithm::setCognitiveAcc(double _cacc){
    if (_cacc < 0) {
       fireOnErrorNotify(E_OPT_PARAM_CACCEL);
    } else {
        C2 = _cacc;
    }
}

void CPSOAlgorithm::setInertia(double _i){
    if (_i < 0) {
       fireOnErrorNotify(E_OPT_PARAM_DECEL);
    } else {
        W = _i;
    }
}


double CPSOAlgorithm::getSocialAcc(){
    return C1;
}


double CPSOAlgorithm::getCognitiveAcc(){
    return C2;
}


double CPSOAlgorithm::getInertia(){
   return W;
}

}
