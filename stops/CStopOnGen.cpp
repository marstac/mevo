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

#include "CStopOnGen.h"
#include "../core/CEvolOptimizer.h"
namespace MEVO {
CStopOnGen::CStopOnGen()
{
    FCurrentGeneration=0;
    FMaxGenerations=500;
}
CStopOnGen::~CStopOnGen() {

}

const char* CStopOnGen::getDescription() {
    std::stringstream desc;
    desc << "Maxium generations stop" << "\tNumber of generations:\t"<<FMaxGenerations;
    return desc.str().c_str();
}

bool CStopOnGen::checkStop(ICPopulation* _population) {
    FCurrentGeneration++;
    if (FCurrentGeneration>=FMaxGenerations) {return true;}
    return false;

}

void CStopOnGen::setMaxGenerations(unsigned int _g) {
    FMaxGenerations = _g;
}


unsigned int CStopOnGen::getMaxGenerations() {
    return FMaxGenerations;
}

void CStopOnGen::init(ICEvolOptimizer* _opt){FCurrentGeneration=0;};
}
