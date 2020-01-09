#ifndef CGDINDICATOR_H
#define CGDINDICATOR_H

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

#include <math.h>

#include "CIndicator.h"
#include "../utils/functions.h"

namespace MEVO {

//! class for indication of a population in terms of generalized distance
/** class for indication of a population in terms of generalized distance. caluclates generalized distance between two populations or fronts..
  */
class CGDIndicator : public CIndicator
{
public:
    /** Constructor */
    CGDIndicator();

    /** Destructor */
    ~CGDIndicator();

    /** Function returns name of an indicator
      @return algorithm name
     */
    virtual const char* getName();

    /** for this indicator always returns 0
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

    /** for this indicator always returns 0
        @param _front a deque to a solution (each solution is a vector of objective function values
        @return a vaule of an indicator
    */
    virtual double assessFront(std::deque<std::deque<double> > _front);

    /** calculates a distance between two fronts. Will be calculated for every point of first front
        (a closest pont from second point will be found).
        @param _front1 first pareto front
        @param _front2 second pareto front
        @return a vaule of an indicator
    */
    virtual double compareFronts(std::deque<std::deque<double> > _front1, std::deque<std::deque<double> > _front2);
};
}

#endif // CGDINDICATOR_H
