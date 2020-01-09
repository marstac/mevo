
#include "CComputer.h"

#include "../core/CEvolOptimizer.h"

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

namespace MEVO{
CComputer::CComputer()
{
    FTask = NULL;
}

CComputer::~CComputer() {

}

const char* CComputer::getName() {
    return "Single thread computer";
}

void CComputer::setComputationTask(ICComputation* _c) {
   FTask = _c;
}
ICComputation* CComputer::getComputationTask() {
    return FTask;
}

void CComputer::compute(void* _params) {
    if(FTask) {
        FTask->execute(_params);
    }
}
ICComputer* CComputer::copy() {
    CComputer* comp = new CComputer();
    return comp;
}
void CComputer::init(ICEvolOptimizer* _opt) {
    while (_opt->getPopSize()%4) {
        _opt->setPopSize(_opt->getPopSize()+1);
    }
}
}
