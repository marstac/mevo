#ifndef CPOLYNOMIALMUTATOR_H
#define CPOLYNOMIALMUTATOR_H

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

namespace MEVO {

//! class for polynomial mutator
/** Provides functions for implementation of polynomial mutation type.
  */
class CPolynomialMutator : virtual public CMutator {
public:
    /** Constructor */
    CPolynomialMutator();

    /** Destructor*/
    ~CPolynomialMutator();

    /** function which returns mutation type
        @return unique name of a mutation type
    */
    virtual const char* getDescripton();

    /** function for initializing a mutator object
        sets boundary values for real coded parameters.
        @param _opt pointer to a evolutionary algorithm.
    */
    virtual void init(ICEvolOptimizer* _opt);

    /** function for polynomial mutation of a population
        @param _population reference to a population
    */
    virtual void mutatePopulation(ICPopulation* _population);

    /** function for copying a mutator object
        @return an exact copy of an computation object.
    */
    virtual ICComputation* copy();

    /** function sets distribution index
         @param _di distribution index value
    */
    void setDistributionIndex(double _di);

    /** function returns distribution index
        @return  distribution index value
    */
    double getDistributionIndex();

protected:

    /** function mutates a genes of one individual
          @param _ind pointer to individual object
          @param _p mutation probability
    */
    void mutateIndividual(ICIndividual* _ind, double _p);

    /** function copies parameters from selected mutator object
        @param _c mutator object
    */
    virtual void copyParams(CMutator *_m);

    /** Distribution index */
    double FEtaMut;

    /** high boundary values for real coded variables*/
    std::deque<double> FHLimits;

    /** low boundary values for real coded variables*/
    std::deque<double> FLLimits;
};

}
#endif // CPOLYNOMIALMUTATOR_H
