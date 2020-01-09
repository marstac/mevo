#ifndef CSWAPMUTATOR_H
#define CSWAPMUTATOR_H

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

#include "CMutator.h"
#include "../core/CGeneticAlgorithm.h"
namespace MEVO{



//! class for swap mutator
/** Provides functions for implementation swap mutation type.
  */
class CSwapMutator : virtual public CMutator
{
public:
    /** Constructor */
    CSwapMutator();

    /** Destructor */
    ~CSwapMutator();

    /** function which returns mutation type
        @return unique name of a mutation type
    */
    virtual const char* getDescripton();

    /** function for initializing a mutator object
            sets boundary values for real and integer coded parameters.
            @param _opt pointer to a evolutionary algorithm.
    */
    void init(ICEvolOptimizer* _opt);

    /** function for swapo mutation of a population
        @param _population reference to a population
    */
    virtual void mutatePopulation(ICPopulation* _population);

    /** function for copying a mutator object
        @return an exact copy of an computation object.
    */
    virtual ICComputation* copy();
protected:

    /** function mutates a genes of one individual. Randomly swaps elements in a gene array.
          @param _ind pointer to individual object
          @param _p mutation probability
    */
    void mutateIndividual(ICIndividual* _ind, double _p);

    /** function mutates n genes of one individual
          @param _ind pointer to individual object
          @param _n mutations number
    */
    void nmutateIndividual(ICIndividual* _ind, int _n);

    /** Swaps elements in a variables array
        @param _ind pointer to individual
        @param _x location of a first variable
        @param _x location of a second variable
    */
    void swap(ICIndividual* _ind, int _x, int _y);
};


}
#endif // CSWAPMUTATOR_H
