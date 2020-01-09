#ifndef ICINDIVIDUAL_H
#define ICINDIVIDUAL_H

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

#include <deque>
#include <stddef.h>

namespace MEVO {

//! Base class forindividual
/** Provides functions and fields describing a single individual
  */
class ICIndividual
{
public:

  /** destructor */
  virtual ~ICIndividual(){};

  /** function returns an excact copy of an individual */
  virtual ICIndividual* copy()=0;

  /** implementation of == operator
      returns true oinly if both all integer and real coded variables are the same for selected individual
      @param a reference to an individual
  */
  virtual bool operator==(const ICIndividual&)=0;

  /** real variables vector getter
    @return pointer to real variables vector
    */
  virtual double* getRealVar()=0;

  /** integer variables vector getter
    @return pointer to integer variables vector
    */
  virtual int* getIntVar()=0;

  /** objective functions values vector getter
    @return pointer to objective functions values vector
    */
  virtual double* getObj()=0;

  /** constraints functions values vector getter
    @return pointer to constraints functions values vector
    */
  virtual double* getConstr()=0;

  /** constraints violation (sum of all constraints values>0) getter
    @return constraints violation value
    */
  virtual double getConstrViolation()=0;

  /** constraints violation (sum of all constraints values>0) setter
    @param _constrViolation constraints violation value
    */
  virtual void setConstrViolation(double _constrViolation)=0;

  /** crowding distance (in objective functions space) setter
    @param _c crowding distance value
    */
  virtual void setCrowdingDistance(double _c)=0;

  /** crowding distance (in objective functions space) getter
    @return crowding distance value
    */
  virtual double getCrowdingDistance() const=0;

  /** function returns number of real coded variables for individual
        @return number of real coded variables
  */
  virtual int getNumRealVar()=0;

  /** function returns number of integer coded variables for individual
        @return number of integer coded variables
  */
  virtual int getNumIntVar()=0;

  /** function returns number of objective functions
        @return number of objective functions
  */
  virtual int getNumObj()=0;

  /** function returns number of constraint functions
        @return number of constraint functions
  */
  virtual int getNumConstr()=0;

  /** virtual function to prepare individual state for evaluation.
  */
  virtual void prepare()=0;

  /** virtual function to update individual state after evaluation.
  */
  virtual void update()=0;

  /** Function for usual non-domination checking
   * It will return the following values
   * 1 if this dominates _ind
   * -1 if _ind dominates this
   * 0 if both this and _ind are non-dominated
   @param _ind pointer to an individual
  */
  virtual int checkDominance(ICIndividual* _ind, std::deque<double>* _eps=NULL)=0;

  /** copies all parameters from selected individual
      @param _ind reference to individual object
   */
  virtual void copyParams(const ICIndividual* _ind)=0;
};
}

#endif // ICINDIVIDUAL_H
