#ifndef CSPREADINDICATOR_H
#define CSPREADINDICATOR_H

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

#include  <math.h>
#include "CIndicator.h"
#include "../interfaces/ICIndividual.h"
#include "../utils/functions.h"

namespace MEVO {
class CSpreadIndicator
{
public:
     /** Constructor */
    CSpreadIndicator();

    /** Destructor */
    ~CSpreadIndicator();

    /** Function returns name of an indicator
      @return algorithm name
     */
    virtual const char* getName();

    /** calculates a value of an indicator for a population. function is implemented in derived classes
        @param _population a pointer to a population of individuals
        @return a vaule of an indicator
    */
    virtual double assessPopulation(ICPopulation* _population);

    /** function compares a population with the reference front. function is implemented in derived classes
        @param _population a pointer to a population of individuals
        @param _rf a reference (true) pareto front
        @return a vaule of an indicator
    */
    virtual double comparePopWithFront(ICPopulation* _population, std::deque<std::deque<double> > _rf);

    /** performes a comparison of the two populations. function is implemented in derived classes
        @param _population a pointer to a population of individuals
        @param _rf a reference pareto front
        @return a vaule of an indicator
    */
    virtual double comparePopulations(ICPopulation* _population1, ICPopulation* _population2);

    /** performes assesement of the pareto fronts. function is implemented in derived classes
        @param _front a deque to a solution (each solution is a vector of objective function values
        @return a vaule of an indicator
    */
    virtual double assessFront(std::deque<std::deque<double> > _front);

    /** performes a comparison of the two fronts. function is implemented in derived classes
        @param _front1 first pareto front
        @param _front2 second pareto front
        @return a vaule of an indicator
    */
    virtual double compareFronts(std::deque<std::deque<double> > _front1, std::deque<std::deque<double> > _front2);
protected:
    void quickSortPopulation(std::deque<ICIndividual* > &_population, int _fn, int _l, int _h);
    int partition(std::deque<ICIndividual* > &_population, int _fn, int _l, int _h);
};
}
#endif // CSPREADINDICATOR_H
