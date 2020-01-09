#ifndef CPSOALGORITHM_H
#define CPSOALGORITHM_H

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

namespace MEVO {

//! Basic Particle Swarm Optimization class
/** Basic class implementing multiobjective Particle Swarm Optimization algorithm
*/
class CPSOAlgorithm : virtual public CEvolOptimizer
{
public:
    /** Constructor */
    CPSOAlgorithm();

    /** Destructor*/
    ~CPSOAlgorithm();

    /**  starts optimization*/
    virtual void run()=0;

    /** Function check algorithm configuration
   @return true if an algorithm is properly configured
       false if it is not
    */
    virtual bool checkConf();

    /** Function returns name of an algorithm
      @return algorithm name
     */
    virtual const char* getName() {return "Particle Swarm Optimization algorithm";}

    /** Function sets particle social acceleration
    @param _sacc value of social acceleration
    */
    void setSocialAcc(double _sacc);

    /** Function sets particle cognitive acceleration
    @param _cacc value of cognitive acceleration
    */
    void setCognitiveAcc(double _cacc);

    /** Function gets particle social acceleration
    @return value of social acceleration
    */
    double getSocialAcc();

    /** Function gets particle cognitive acceleration
    @return _cacc value of cognitive acceleration
    */
    double getCognitiveAcc();

    /** Function gets particle ineria
    @return  value of inertia
    */
    double getInertia();

    /** Function sets particle ineria
    @param _p value of inertia
    */
    void setInertia(double _i);
protected:

    /** inertia value */
    double W;

    /** social acceleration value */
    double C1;

    /** cognitive acceleration*/
    double C2;
};
}
#endif // CPSOALGORITHM_H
