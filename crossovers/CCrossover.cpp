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


#include "CCrossover.h"

namespace MEVO {

CCrossover::CCrossover() : CWithRNGen()
{
    FProb = 0.6;
    FRealVarSize=0;
    FIntVarSize=0;
}
void CCrossover::execute(void* _params){
    SCrossoverParams* p = (SCrossoverParams*)_params;
    crossover((p->FParentPopulation), (p->FOffspringPopulation));
}
CCrossover::~CCrossover(){

}

void CCrossover::init(ICEvolOptimizer* _opt) {
    FRealVarSize = _opt->getProblem()->getRealVarSize();
    FIntVarSize = _opt->getProblem()->getIntegerVarSize();
}

void CCrossover::copyParams(CCrossover *_c) {
    this->FIntVarSize =_c->FIntVarSize;
    this->FRealVarSize = _c->FRealVarSize;
    this->FProb = _c->FProb;
}

void CCrossover::setProbability(double _p) {
    FProb = _p;
}

double  CCrossover::getProbability() {
    return FProb;
}
}
