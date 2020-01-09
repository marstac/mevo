#ifndef CPARTICLE_H
#define CPARTICLE_H

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

#include "../utils/CWithRNGen.h"
#include "CIndividual.h"

namespace MEVO {

class CPSOAlgorithm;

//! Base class for particle
/** Provides functions and fields describing a single particle of a swarm
  */
class CParticle : public CIndividual, public CWithRNGen
{
public:

    /** Constructor
        @param _numRealVar number of real coded variables
        @param _numIntVar number of integer coded variables
        @param _numObj number of objective finctions
        @param _numConstr number of constraint functions
        @param  _w inertia value
        @param _c1 social acceleration value
        @param _c2 cognitive acceleration
        @param _parent pointer to a PSO algorithm in which particle is used
      */
    CParticle(int _numVar, int _numIntVar, int _numObj, int _numConstr, double _w, double _c1, double _c2, CPSOAlgorithm* _parent);

    /** Copy constructor
        @param a reference to an particle

      */
    CParticle(const CParticle&);

    /** function returns an excact copy of an individual */
    virtual ICIndividual* copy();

    /** destructor */
    virtual ~CParticle();

    /** velocity vector getter
        @return pointer to velocity vector
    */
    double* getVelocity();

    /** best real variables vector getter
        @return pointer to best real variables vector
    */
    double* getBestRealVar();

    /** best integer variables vector getter
        @return pointer to best integer variables vector
    */
    int* getBestIntVar();

    /** best objective functions values vector getter
        @return pointer to best objective functions values vector
    */
    double* getBestObj();

    /** function updates velocity and calculates a new position for a particle */
    void updatePosition();

    /** virtual function to prepare individual state for evaluation.
    */
    virtual void prepare();

    /** function to update a particle state after evaluation.
      */
    virtual void update();

    /** function sets a best neighbour for a particle
        @param _l pointer to a leader
    */
    virtual void setLeader(CParticle* _l);

    /** function gets a best neighbour for a particle
        @return pointer to a leader
    */
    CParticle* getLeader();

    /** function informs if particle is at its best position
        @return flag value (true if particle is at its best position
    */
    virtual bool isBest();

    /** Function copies parameters from selected particle
        @param reference to a particle
    */
    virtual void copyParams(const CParticle*);

    double getW() const;
    void setW(double value);

    double getC1() const;
    void setC1(double value);

    double getC2() const;
    void setC2(double value);

protected:

    /** function copies current position and objective functions values to best vectors
      */
    virtual void updateBest();

    /** velocity vector*/
    double* FVelocity;

    /** maximal velocity vector*/
    double* FMaxVelocity;

    /** minimal velocity vector*/
    double* FMinVelocity;

    /** best real coded variables vector */
    double* FBestRealVar;

    /** best integer coded variables vector */
    int* FBestIntVar;

    /** best objective functions variables vector */
    double* FBestObj;

    /** best objective functions variables vector */
    double FBestConstrVal;

    /** Pointer to a best neighbour */
    CParticle* FLeader;

    /** inertia value */
    double W;

    /** social acceleration value */
    double C1;

    /** cognitive acceleration*/
    double C2;

    /** flag which informs if particle is at its best position */
    bool FBest;

    /** Pointer to a PSO algorithm in which particle is used */
    CPSOAlgorithm* FParent;

};
};
#endif // CPARTICLE_H
