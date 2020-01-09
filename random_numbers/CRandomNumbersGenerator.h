#ifndef CRANDOMNUMBERSGENERATOR_H
#define CRANDOMNUMBERSGENERATOR_H

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

#include "../interfaces/ICRandomNumbersGenerator.h"

#include "mtrand.h"
namespace MEVO {
//! class for random numbers generator
/** Provides functions for random numbers generators. Uses MTRand generator developed by Makoto Matsumoto and Takuji Nishimura
  */
class CRandomNumbersGenerator : public ICRandomNumbersGenerator
{
public:
    /** Constructor */
    CRandomNumbersGenerator();

    /** Destructor */
    virtual ~CRandomNumbersGenerator();

    /** function for copying a random numbers generator object
        @return an exact copy of an random numbers generator object.
    */
    virtual ICRandomNumbersGenerator* copy() const;

    /** function for setting a seed value
           @param _seed value of a seed
    */
    void setSeed(unsigned long _seed);

    /** function for generating a random real value
            @param _l low value of selected bound for random value
            @param _h high value of selected bound for random value
    */
    virtual double randomDouble(double _l, double _h);

    /** function for generating a random integer value
        @param _l low value of selected bound for random value
        @param _h high value of selected bound for random value
     */
    virtual int  randomInteger(int _l, int _h);

    /** pure virtual function which returns Random numbers generator type
        @return unique name of a random numbers generator
    */
    virtual const char* getDescripton();
protected:

    /** MTRand object. Modified by Marcin Stachura */
    MTRand_open FRandD;
    MTRand_int32 FRandI;
};
}
#endif // CRANDOMNUMBERSGENERATOR_H
