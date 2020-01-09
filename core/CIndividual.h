#ifndef CINDIVIDUAL_H
#define CINDIVIDUAL_H

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
#include <cstddef>

#include "../interfaces/ICIndividual.h"

namespace MEVO {

//! Base class forindividual
/** Provides functions and fields describing a single individual
  */
class CIndividual : public ICIndividual
{
public:

  /** Constructor
    @param _numRealVar number of real coded variables
    @param _numIntVar number of integer coded variables
    @param _numObj number of objective finctions
    @param _numConstr number of constraint functions
  */
  CIndividual(int _numRealVar, int _numIntVar, int _numObj, int _numConstr);

  /** Copy constructor
    @param a reference to an individual

  */
  CIndividual(const CIndividual&);


  /** function returns an excact copy of an individual */
  virtual ICIndividual* copy();

  /** destructor */
  virtual ~CIndividual();

  /** implementation of == operator
    returns true oinly if both all integer and real coded variables are the same for selected individual
    @param a reference to an individual
    */
  bool operator==(const ICIndividual&);

  /** real variables vector getter
    @return pointer to real variables vector
    */
  double* getRealVar();

  /** integer variables vector getter
    @return pointer to integer variables vector
    */
  int* getIntVar();

  /** objective functions values vector getter
    @return pointer to objective functions values vector
    */
  double* getObj();

  /** constraints functions values vector getter
    @return pointer to constraints functions values vector
    */
  double* getConstr();

  /** constraints violation (sum of all constraints values>0) getter
    @return constraints violation value
    */
  double getConstrViolation();

  /** constraints violation (sum of all constraints values>0) setter
    @param _constrViolation constraints violation value
    */
  void setConstrViolation(double _constrViolation);

  /** crowding distance (in objective functions space) setter
    @param _c crowding distance value
    */
  void setCrowdingDistance(double _c);

  /** crowding distance (in objective functions space) getter
    @return crowding distance value
    */
  double getCrowdingDistance() const;

  /** function returns number of real coded variables for individual
        @return number of real coded variables
  */
  int getNumRealVar();

  /** function returns number of integer coded variables for individual
        @return number of integer coded variables
  */
  int getNumIntVar();

  /** function returns number of objective functions
        @return number of objective functions
  */
  int getNumObj();

  /** function returns number of constraint functions
        @return number of constraint functions
  */
  int getNumConstr();

  /** virtual function to prepare individual state for evaluation.
  */
  virtual void prepare(){}

  /** virtual function to update individual state after evaluation.
  */
  virtual void update(){}

  /** Function for usual non-domination checking
   * It will return the following values
   * 1 if this dominates _ind
   * -1 if _ind dominates this
   * 0 if both this and _ind are non-dominated
   @param _ind pointer to an individual
  */
  virtual int checkDominance(ICIndividual* _ind, std::deque<double>* _eps=NULL);

  /** copies all parameters from selected individual
    @param _ind reference to individual object
    */
  virtual void copyParams(const ICIndividual* _ind);

  /** Epsilon dominance coefficient setter*/
  void setEps(std::deque<double> _eps);

protected:

  int checkConstrDominance(ICIndividual* _ind);

  int checkNDominance(ICIndividual* _ind);

  int checkEpsDominance(ICIndividual* _ind, int _sd, std::deque<double>* _eps);

  /** vector of real coded variables */
  double* FRealVar;

  /** vector of integer coded variables */
  int* FInegerVar;

  /** vector of objective functions values */
  double* FObj;

  /** vector of constraints functions values */
  double* FConstr;

  /** crowding distance in objective functions space */
  double FCrowdingDistance;

  /** number of real coded variables*/
  int FNumRealVar;

  /** number of integer coded variables*/
  int FNumIntVar;

   /** number of objective functions */
  int FNumObj;

  /** number of constraint functions */
  int FNumConstr;

  /** value of constraints violation (sum of all constraints values>0) */
  double FConstrViolation;

  /** Epsilon dominance coefficient */
  std::deque<double> FEps;

};
}
#endif // CINDIVIDUAL_H
