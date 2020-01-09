#ifndef CFLIPMUTATOR_H
#define CFLIPMUTATOR_H

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
namespace MEVO {

//! class for flip mutator
/** Provides functions for implementation flip mutation type.
  */
class CFlipMutator : virtual public CMutator
{
public:

    /** Constructor */
    CFlipMutator();

    /** Destructor*/
    ~CFlipMutator();

    /** function which returns mutation type
        @return unique name of a mutation type
    */
    virtual const char* getDescripton();

    /** function for initializing a mutator object
        sets boundary values for real and integer coded parameters.
        @param _opt pointer to a evolutionary algorithm.
    */
    virtual void init(ICEvolOptimizer* _opt);

    /** function for flip mutation of a population
        @param _population reference to a population
    */
    virtual void mutatePopulation(ICPopulation* _population);

    /** function for copying a mutator object
         @return an exact copy of an computation object.
    */
    virtual ICComputation* copy();
protected:

    /** function copies parameters from selected mutator object
        @param _c mutator object
    */
    virtual void copyParams(CMutator* _m);

    /** function mutates a genes of one individual
      @param _ind pointer to individual object
      @param _p mutation probability
      */
    void mutateIndividual(ICIndividual* _ind, double _p);

    /** function changes a selected gene with a random number between boundary values
      @param _ind pointer to individual object
      @param _x gene number
      */
    void flip(ICIndividual* _ind, int _x);

    /** high boundary values for integer coded parameters*/
    std::deque<int> FHILimits;

    /** low boundary values for integer coded parameters*/
    std::deque<int> FLILimits;

    /** high boundary values for real coded parameters*/
    std::deque<double> FHRLimits;

     /** low boundary values for real coded parameters*/
    std::deque<double> FLRLimits;
};

}

#endif // CFLIPMUTATOR_H
