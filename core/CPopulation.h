#ifndef CPOPULTAION_H
#define CPOPULTAION_H

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


#include "../interfaces/ICPopulation.h"
#include <iomanip>      // std::setprecision

namespace MEVO{
//! Class imlements a population
/** Class implements a population of individuals and related functions
  */
class CPopulation : public ICPopulation
{
public:
    /** Constructor */
    CPopulation();

    /** Destructor */
    virtual ~CPopulation();

    /** adding all individduals from the selected population */
    CPopulation& operator+=(CPopulation&);

    /** returns a copy of the population. copies all individuals*/
    virtual ICPopulation* copy();

    /** copy constructor */
    CPopulation(const CPopulation &);

    /** function deletes all individuals in population */
   virtual void deleteIndividuals();

    /** function clears all individuals in population */
   virtual void clearIndividuals();

    /** Function adds pointer an individual to a population. Adds a pointer only!
        @param _ind pointer to an indicidual
    */
   virtual void addIndividual(ICIndividual* _i);

    /** Function returns pointer an individual in a population.
        @param _i index of an individual
        @return _ind pointer to an indicidual
    */
   virtual ICIndividual* getIndividual(unsigned int _i) const;

    /** Function returns a population size
        @return population size
    */
   virtual unsigned int getSize();

    /** Function returns a population objectiive function vales as a matrix
        @return population objective function vales  matrix
    */
    virtual std::deque<std::deque<double> > objMatrix();

    /** Function returns a population variables vales as a matrix
        @return population population variables vales matrix
    */
   virtual std::deque<std::deque<double> > varMatrix();

    /** Function calculates eps_i values (eps_i =eps*(fmax_i-fmin_i)
        @param _eps parameter for epsilon domination calcuations
        @return deque of epsi values
    */
   virtual std::deque<double> calcEpsi(double _eps);

    /** Function calculates maximal values of objective functions*/
   virtual std::deque<double> calcMinimalValues();

    /** Function calculates minimal values of objective functions*/
   virtual std::deque<double> calcMaximalValues();

    /** Function returns a population of nondominated individuals
        @param _eps parameter for epsilon domination calcuations
        @return a population of nondominated solutions
    */
   virtual ICPopulation* getNondominatedPopulation(double _eps=0);

    /** Function returns a population of feasible individuals
        @return a population of feasible solutions
    */
    virtual ICPopulation* getFeasiblePopulation();

    /** Function normalizes population
        @param _max deque of maximal values
        @param _min deque of minimal values
    */
   virtual void normalizePopulation(std::deque<double>* _max=NULL, std::deque<double>* _min=NULL);

    /** Function inverts a normalization process
        @param _max deque of maximal values
        @param _min deque of minimal values
    */
   virtual void denormalizePopulation(std::deque<double> _max, std::deque<double> _min);

    /** Function invert normalized population. First function calls normilized population and then inverts it.
        @param _max deque of maximal values
        @param _min deque of minimal values
    */
   virtual void invertPopulation(std::deque<double>* _max=NULL, std::deque<double>* _min=NULL);

    /** saves population to a selected file
        @param _fname name of the selected file
    */
   virtual void save(std::string _fname);

    /** loads population from a selected file
        @param _fname name of the selected file
        @return true if a population was loaded sucessfuly false otherwise
    */
   virtual bool load(std::string _fname);

protected:
    /** deque of pointers to individuals in population */
    std::deque<ICIndividual*> FIndividuals;
};
}
#endif // CPOPULTAION_H
