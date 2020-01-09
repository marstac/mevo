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

#include "CRandomNumbersGenerator.h"
#include "../utils/functions.h"
#include <cstdlib>
#include <time.h>
namespace MEVO {
CRandomNumbersGenerator::CRandomNumbersGenerator()
{

    FRandD.seed(rand());
    FRandI.seed(rand());
}

CRandomNumbersGenerator::~CRandomNumbersGenerator() {

}


const char* CRandomNumbersGenerator::getDescripton() {
    return "Random numners generator coded by by Takuji Nishimura and Makoto Matsumoto, see http://www.bedaux.net/mtrand/";
}


ICRandomNumbersGenerator* CRandomNumbersGenerator::copy() const{
    CRandomNumbersGenerator* gen = new CRandomNumbersGenerator();
    return gen;
}

void CRandomNumbersGenerator::setSeed(unsigned long _seed) {
    FRandD.seed(_seed);
    FRandI.seed(_seed);
}

double CRandomNumbersGenerator::randomDouble(double _l, double _h){
    return _l + FRandD()*(_h-_l);
}

int  CRandomNumbersGenerator::randomInteger(int _l, int _h){
    return _l + FRandI()%(_h-_l+1);
}

}
