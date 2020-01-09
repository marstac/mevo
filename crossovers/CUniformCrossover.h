#ifndef CUNIFORMCROSSOVER_H
#define CUNIFORMCROSSOVER_H

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

#include "../crossovers/CCrossover.h"
#include "../core/CGeneticAlgorithm.h"

namespace MEVO {

//! Class for uniform crossover
/** Provides functions for one point crossover.
  */
class CUniformCrossover : virtual public CCrossover
{
public:

    /**Constructor*/
    CUniformCrossover();

    /**Destructor*/
    virtual ~CUniformCrossover();

    /** function which returns crossover type
            @return unique name of a crossover type
    */
    const char* getDescripton();

    /** function for initializing a crossover object
        sets real variables number.
        @param _opt pointer to a evolutionary algorithm.
    */
    virtual void init(ICEvolOptimizer *_opt);

    /** function for one point crossover of parent population
        @param _parentPopulation reference to a parent population
        @param _offspringPopulation reference to a offspring population
    */
    virtual void crossover(ICPopulation* _parentPopulation, ICPopulation* _offspringPopulation);

    /** function for copying a computation object
            @return an exact copy of an computation object.
    */
    virtual ICComputation* copy();

protected:

    /** function copies parameters from selected crossover object
        @param _c crossover object
    */
    virtual void copyParams(CCrossover* _c);

    /** function performs a DE crossover of primary and auxiliary parents
        @param _parent1 pointer to first parent
        @param _parent2 pointer to second parent
        @param _child1 pointer to first offspring
        @param _child2 pointer to second offspring
    */
    void crossTwoIndividuals(ICIndividual* _parent1, ICIndividual* _parent2, ICIndividual* _child1, ICIndividual* _child2);
};
}

#endif // CUNIFORMCROSSOVER_H
