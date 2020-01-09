#ifndef PARTICLE_H
#define PARTICLE_H

#include "../../core/CParticle.h"

#include <vector>
class EPSs{
public:
    ~EPSs(){eps.clear();}
    std::vector<double> eps;
};

class MOPSOParticle : public MEVO::CParticle{
public:

  MOPSOParticle(int _numVar, int _numIntVar, int _numObj, int _numConstr, double _w, double _c1, double _c2, MEVO::CPSOAlgorithm* _parent);

  MOPSOParticle(const MOPSOParticle&);

  //const MOPSOParticle& operator=(MOPSOParticle&);

  /** function returns an excact copy of an individual */
  virtual ICIndividual* copy();

  virtual ~MOPSOParticle();

  virtual void prepare();

  std::vector<double>  nx;
  std::vector<EPSs> epss;
  int subswarm;

   void copyParams(const ICIndividual *_ind);
protected:

};
#endif // PARTICLE_H
