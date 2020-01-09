#ifndef CDECROSSOVER_H
#define CDECROSSOVER_H

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
#include "../core/CAlgWithArchieve.h"

namespace MEVO {

//! Class for Diferential evolution crossover
/** Provides functions for DE crossover.
  */
class CDECrossover : virtual public CCrossover
{
public:

    /** DE crossover subtype
    */
    enum enDEType{
        /** DE - all parents are selected from parent population*/
        DE,

        /** DE_1 - all parents are selected from archive population (if algorithm do not posses archive DE type is called)*/
        DE_1,

        /** DE_2 - primary parent is selected from parent population, auxilliary parents are selected from archive (if algorithm do not posses archive DE type is called)*/
        DE_2
    };

    /**Constructor*/
    CDECrossover();

    /**Destructor*/
    virtual ~CDECrossover();

    /** function which returns crossover type
        @return unique name of a crossover type
        */
    const char* getDescripton();

    /** function for initializing a crossover object
            sets real crossover rate and boundary values for real coded parameters.
            @param _opt pointer to a evolutionary algorithm.
        */
    virtual void init(ICEvolOptimizer* _opt);

    /** function for DE crossover of parent population
            @param _parentPopulation reference to a parent population
            @param _offspringPopulation reference to a offspring population
        */
    virtual void crossover(ICPopulation *_parentPopulation, ICPopulation *_offspringPopulation);

    /** function for copying a computation object
        @return an exact copy of an computation object.
    */
    virtual ICComputation* copy();

    /** function sets crossover rate
            @param _c probability value
    */
    void setCrossoverRate(double _c);

    /** function returns crossover rate
            @return probability value
        */
    double getCrossoverRate();

    /**  function sets DE substype
        @param _t selected type
    */
    void setType(enDEType _t);

    /**  function returns DE substype
        @return DE type
    */
    enDEType getType();

protected:

    /** function copies parameters from selected crossover object
            @param _c crossover object
        */
    virtual void copyParams(CCrossover* _c);

    /** DE - all parents are selected from parent population
        @param _parentPopulation reference to a parent population
        @param _offspringPopulation reference to a offspring population
    */
    void crossDE(ICPopulation *_parentPopulation, ICPopulation *_offspringPopulation);

     /** DE_1 - all parents are selected from archive population (if algorithm do not posses archive DE type is called)
        @param _offspringPopulation reference to a offspring population
    */
    void crossDE_1(ICPopulation *_offspringPopulation);

     /** DE_2 - primary parent is selected from parent population, auxilliary parents are selected from archive (if algorithm do not posses archive DE type is called)
        @param _parentPopulation reference to a parent population
        @param _offspringPopulation reference to a offspring population
    */
    void crossDE_2(ICPopulation* _parentPopulation, ICPopulation* _offspringPopulation);

    /** function performs a DE crossover of primary and auxiliary parents
            @param _r1 first auxiliary parent
            @param _r2 second auxiliary parent
            @param _r3 third auxiliary parent
            @param _parent primary parent
            @param _child child
        */
    void crossIndividuals(ICIndividual* _r1, ICIndividual* _r2, ICIndividual* _r3, ICIndividual* _parent, ICIndividual* _child);

    /** Crossover rate*/
    double FCR;

    /** high boundary values for real coded parameters*/
    std::deque<double> FHRLimits;

    /** low boundary values for real coded parameters*/
    std::deque<double> FLRLimits;

    /** Pointer to archive*/
    ICPopulation* FArchive;

    /** DE type */
    enDEType FType;
};
}
#endif // CDECROSSOVER_H
