#ifndef CGeneticAlgorithm_H
#define CGeneticAlgorithm_H

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


#include "CEvolOptimizer.h"

#include "../interfaces/ICMutator.h"
#include "../interfaces/ICCrossover.h"

#include "../crossovers/CSBXCrossover.h"
#include "../crossovers/COnePointCrossover.h"
#include "../crossovers/CDECrossover.h"


namespace MEVO {
//! Basic genetic algorithm class
/** Basic virtual class implementing multiobjective genetic algoritm
    Provides methods and fields for crossover and mutation
*/
class CGeneticAlgorithm : virtual public CEvolOptimizer
{
public:
    CGeneticAlgorithm();
    virtual ~CGeneticAlgorithm();

    /** Method initating fields of this, basic class
   */
    virtual void initVariables();

    /** Method starts optimization*/
    virtual void run()=0;

    /** Method check algorithm configuration
   @return true if an algorithm is properly configured
       false if it is not
    */
    virtual bool checkConf();

    /** Method returns name of an algorithm
      @return algorithm name
     */
    virtual const char* getName() {return "Genetic algorithm";}


    /** Method for crossover both population have to point at existing individuals objects!
      @param _parentPopulation a population of parents
      @param _offspringPopulation a result population of the offspring generated

    */
    void crossoverPopulation(ICPopulation *_parentPopulation, ICPopulation *_offspringPopulation);


    /** Methods sets a single crossover for both integer and real coded variables. The genotype will be treated as one vector!
      This methods deletes separate crossovers for real and integer coded variables!
      @param _c pointer to a mutator object
      */
    void setCrossover(ICCrossover* _c);

    /** Methods sets a separate crossovers for integer and real coded variables. The genotype will be treated as two vectors (Real coded and integer coded)!
      This methods deletes  a single crossover for real and integer coded variables!
      This method works only if both arguments do not point to NULL!
      @param pointer to a mutator object
      */
    void setSeparateCrossovers(ICCrossover* _rc, ICCrossover* _ic);

    /** Method returns a pointer to a crossover object for both integer and real coded variables
        @return pointer to crossover object
    */
    ICCrossover* getCrossover();

    /** Method returns a pointer to a crossover object for integer coded variables
        @return pointer to crossover object
    */
    ICCrossover* getIntCrossover();

    /** Method returns a pointer to a crossover object for real coded variables
        @return pointer to crossover object
    */
    ICCrossover* getRealCrossover();

private:

    /** Dedicated computer for Crossover*/
    ICComputer* cComputer;

    /** Pointer to a crossover object for both integer and real coded variables*/
    ICCrossover* iCrossover;

    /** Pointer to a crossover object for integer coded variables*/
    ICCrossover* iIntCrossover;

    /** Pointer to a crossover object for real coded variables*/
    ICCrossover* iRealCrossover;
};
}
#endif // CGeneticAlgorithm_H
