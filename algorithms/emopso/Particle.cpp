#include "Particle.h"

#include "emopso.h"

MOPSOParticle::MOPSOParticle(int _numVar, int _numIntVar, int _numObj, int _numConstr, double _w, double _c1, double _c2, MEVO::CPSOAlgorithm* _parent): CParticle(_numVar, _numIntVar, _numObj, _numConstr, _w, _c1, _c2, _parent) {
    nx.resize( _numVar+_numIntVar);
}
MOPSOParticle::MOPSOParticle(const MOPSOParticle& _p) :CParticle(_p) {
    nx.resize(_p.nx.size());
    epss.resize(_p.epss.size());
    MOPSOParticle::copyParams(&_p);
}

//const MOPSOParticle& MOPSOParticle::operator=(MOPSOParticle& _p) {
//  if (this == &_p) {
//    return (*this);
//  }
//  CIndividual::copyParams(&_p);
//  CParticle::copyParams(&_p);
//  copyParams(&_p);
//  return (*this);
//}

MEVO::ICIndividual *MOPSOParticle::copy()
{
    MOPSOParticle* ind = new MOPSOParticle(*this);
    return ind;
}

MOPSOParticle::~MOPSOParticle() {
    nx.clear();
    epss.clear();
}

void MOPSOParticle::prepare()
{
    EMOPSO* e = dynamic_cast<EMOPSO*>(FParent);
    double r=iRandGen->randomDouble(0,1);
    std::deque<double> c1p=*(e->getC1P());

    double c1pl=0;

    if (r<c1p[0]){
        c1pl=0;
    } else {

    for(int i=1; i<c1p.size();i++) {
        if (r>=c1p[i-1]&&r<=c1p[i]) {
            c1pl=i;
            break;
        }
    }
    }
    r=iRandGen->randomDouble(0,1);
    std::deque<double> c2p=*(e->getC2P());

    double c2pl=0;

    if (r<c2p[0]){
        c2pl=0;
    } else {

    for(int i=1; i<c2p.size();i++) {
        if (r>=c2p[i-1]&&r<=c2p[i]) {
            c2pl=i;
            break;
        }
    }
    }
    r=iRandGen->randomDouble(0,1);
    std::deque<double> wp=*(e->getWP());

    double wpl=0;

    if (r<wp[0]){
        wpl=0;
    } else {

    for(int i=1; i<wp.size();i++) {
        if (r>=wp[i-1]&&r<=wp[i]) {
            wpl=i;
            break;
        }
    }

    }
    C1 = (3.0-1)*c1pl/10.0+1;
    C2 = (3.0-1)*c2pl/10.0+1;
    W =  (0.4)*wpl/10.0+0;

    CParticle::prepare();
}

void MOPSOParticle::copyParams(const ICIndividual *_ind) {
    const MOPSOParticle* ind = dynamic_cast<const MOPSOParticle*>(_ind);
    if(!ind) return;

    subswarm = ind->subswarm;
    for (int i=0; i<nx.size();i++){
        nx[i] = ind->nx[i];
    }
    for (int i=0; i<epss.size();i++){
        epss[i] = ind->epss[i];
    }

}
