#include "efile.h"
#include "../../utils/functions.h"
EFILE::EFILE(){
  nsolutions=0;
  hasentered=false;
  firsttime=true;
  updatematrix=true;

  proximitymatrix= NULL;
  proximitymatrixindex = NULL;

}

EFILE::~EFILE()
{
    if(proximitymatrix) {
        for (int i = 0; i != maxbests; i++) {
          delete [] proximitymatrix[i];
        }
        delete [] proximitymatrix;
    }
    if(proximitymatrixindex) {
        for (int i = 0; i != maxbests * nclusters+1; i++) {
            delete [] proximitymatrixindex[i];
        }
        delete [] proximitymatrixindex;
    }
}


void EFILE::init(int _ndimensions, int _nobjectives, int _nclusters, int _maxsolutions, int _maxbests, int _nconstr){

    nobjectives=_nobjectives;
    ndimensions=_ndimensions;
    nconstr = _nconstr;
    FEpsilon.clear();

    tlb.resize(_nobjectives, 0.0);
    tub.resize(_nobjectives, 0.0);

    xtlb.resize(_ndimensions,0.0);
    xtub.resize(_ndimensions,0.0);

    nclusters=_nclusters;

    maxsolutions=_maxsolutions;

    maxbests = _maxbests;
    FEpsilon.clear();
    FEpsilon.resize(_nobjectives);

    for(int _i(0);_i<nobjectives;_i++)
      {
        tlb[_i]=MEVO::DOUBLE_MAX;
        tub[_i]=MEVO::DOUBLE_MIN;
        FEpsilon[_i] = algorithm->getEpsilon();
      }


    for(int _i(0);_i<ndimensions;_i++)
      {
        xtlb[_i]=MEVO::DOUBLE_MAX;
        xtub[_i]=MEVO::DOUBLE_MIN;
      }

  allnsolutions = 0;
  updatematrix=true;
  hasclusteredfirsttime=false;


  if(proximitymatrix) {
      for (int i = 0; i != maxbests; i++) {
        delete [] proximitymatrix[i];
      }
      delete [] proximitymatrix;
  }
  if(proximitymatrixindex) {
      for (int i = 0; i != maxbests * nclusters+1; i++) {
          delete [] proximitymatrixindex[i];
      }
      delete [] proximitymatrixindex;
  }

  proximitymatrix = new double*[maxbests];
  for (int i = 0; i != maxbests; i++) {
      proximitymatrix[i] = new double [maxbests];
  }

  proximitymatrixindex = new int* [maxbests*nclusters+1];
  for (int i = 0; i != maxbests * nclusters+1; i++) {
      proximitymatrixindex[i] = new int[maxbests * nclusters+1];
  }

  nsolutions=0;

  nsolutionsbyswarm.clear();
  nsolutionsbyswarm.resize(nclusters);
  for(int _i(0);_i<nclusters;_i++){
    nsolutionsbyswarm[_i]=0;
  }

  solutions.clear();
  allsolutions.clear();

  hasentered=false;
  firsttime=true;
}

void EFILE::update(){
    allsolutions.clear();
    for (int i=0; i<nsolutions;i++) {
        allsolutions.push_back(solutions[i]);
    }
    allnsolutions=nsolutions;

    if(firsttime){
      normalizeDistances1();
      firsttime=false;
    }
    else normalizeDistances2();
    hasentered=true;
}


void EFILE::printSolutions(){

}

int  EFILE::domine1(MOPSOParticle &_a,MOPSOParticle &_b){

    return _a.checkDominance(&_b);

}


int EFILE::domine(MOPSOParticle &_a,MOPSOParticle &_b){
  //assert (_a.fx.size() == nobjectives);
  int _tmp;
      if(!hasentered){
      _tmp=domine1(_a,_b);
      }
    else _tmp=domine2(_a,_b);

    return _tmp;
}




bool EFILE::add(MOPSOParticle &_MOPSOParticle, int _whichcluster){
  //assert (_MOPSOParticle.fx.size() == nobjectives);
  bool _flag=true;

  std::vector<bool> erase;
  std::vector<int> _delMOPSOParticles;

  addAll(_MOPSOParticle);

  for(int _i(0);_i<nsolutions;_i++){

    int _tmp=domine(solutions[_i],_MOPSOParticle);

    if(_tmp==1||solutions[_i]==_MOPSOParticle) {
      _flag=false;
    }
    if(_tmp==-1){
      _delMOPSOParticles.push_back(_i);
      erase.push_back(true);
    }
    else erase.push_back(false);
  }

//  if(_delMOPSOParticles.size()>0)
//    updateClusters(_delMOPSOParticles);

   std::vector<bool>::iterator _iterator=erase.begin();
   std::vector<MOPSOParticle>::iterator _iterator2=solutions.begin();
    for(_iterator=erase.begin(); _iterator != erase.end();++_iterator) {
     if(*_iterator==true){
       solutions.erase(_iterator2);
       nsolutions--;
     }
     else
       _iterator2++;
   }
  if(_flag==true){
      //if(_whichcluster!=-1){add2Cluster(_whichcluster);}
      solutions.push_back(_MOPSOParticle);
      nsolutions++;
      if(nsolutions>1.3*maxsolutions){
        update();
      //add(_MOPSOParticle);
      }
  }

  if(nsolutions<0.9*maxsolutions){
    hasentered=false;
    firsttime=true;
  }
   // update();

  return _flag;

}


void EFILE::add2Cluster(int _whichcluster){

  if(hasclusteredfirsttime==false)return;

  proximitymatrixindex[_whichcluster][nsolutionsbyswarm[_whichcluster]]=nsolutions;

  proximitymatrixindex[_whichcluster][nsolutionsbyswarm[_whichcluster]+1]=-1;

  nsolutionsbyswarm[_whichcluster]++;
}

int EFILE::deleteFromCluster(int _whichsolution){
  int _i;
  int _j;
  int _k;
  for(_j=0;_j<nclusters;_j++)
    for(_i=0;_i<nsolutionsbyswarm[_j];_i++)
      if(proximitymatrixindex[_j][_i]==_whichsolution){
  for(_k=_i;_k<nsolutionsbyswarm[_j];_k++){
    proximitymatrixindex[_j][_k]=proximitymatrixindex[_j][_k+1];
  }
  proximitymatrixindex[_j][_k]=-1;
  nsolutionsbyswarm[_j]--;
  return 1;
      }
  return -1;
}

int EFILE::substractFromTo(int _from, int _to, int _quantity){
  int _i;
  int _j;
  for(_j=0;_j<nclusters;_j++){
    for(_i=0;_i<nsolutionsbyswarm[_j];_i++){
      if(proximitymatrixindex[_j][_i]>_from&&proximitymatrixindex[_j][_i]<_to){
  proximitymatrixindex[_j][_i]-=_quantity;
      }
    }
  }
  return -1;

}
int EFILE::updateClusters(std::vector<int> &_whichMOPSOParticles){
  unsigned int _counter(0);
  if(hasclusteredfirsttime==false)return -1;
    _whichMOPSOParticles.push_back(nsolutions);
    while(_whichMOPSOParticles.size()>_counter+1){
      int _from(_whichMOPSOParticles[_counter]);
      int _to(_whichMOPSOParticles[_counter+1]);
       deleteFromCluster(_from);
       substractFromTo(_from,_to,_counter+1);
      _counter++;
    }

  return 1;
}


bool EFILE::addAll(MOPSOParticle &_MOPSOParticle){
    //assert (_MOPSOParticle.fx.size() == nobjectives);
  bool _flag=true;
  std::vector<bool> erase;
  //  vector<EPSs>::iterator _iterator;

  for(int _i(0);_i<allnsolutions;_i++){
    int _tmp;
    if (allsolutions[_i]==_MOPSOParticle) {
        _flag=false;
    }
    _tmp=domine(allsolutions[_i],_MOPSOParticle);
    if(_tmp==1/*||_tmp==0*/) _flag=false;

    if(_tmp==-1){
      erase.push_back(true);
    }
    else erase.push_back(false);
  }

   std::vector<bool>::iterator _iterator=erase.begin();
   std::vector<MOPSOParticle>::iterator _iterator2=allsolutions.begin();
   for(_iterator=erase.begin(); _iterator != erase.end();++_iterator) {


     if(*_iterator==true){
       allsolutions.erase(_iterator2);
       allnsolutions--;
     }
     else
       _iterator2++;
   }

  if(_flag==true){
    if(allnsolutions>maxsolutions*3) return _flag;
      allsolutions.push_back(_MOPSOParticle);
      allnsolutions++;
  }
  return _flag;

}


bool EFILE::reAdd(){
  bool _flag=true;
  std::vector<bool> erase;
  std::vector<int> _delMOPSOParticles;
  erase.resize(nsolutions);

  //  vector<EPSs>::iterator _iterator;
  for(int _i(0);_i<nsolutions;_i++){
    erase[_i]=false;
    for(int _j(0);_j<nsolutions;_j++){
      int _tmp;
      if(_i==_j) continue;
      _tmp=domine2(solutions[_i],solutions[_j]);

      if(_tmp==1/*||_tmp==0*/) _flag=false;

      if(_tmp==-1){
      _delMOPSOParticles.push_back(_i);
  erase[_i]=true;
      }
    }
  }
//    if(_delMOPSOParticles.size()>0)
//   updateClusters(_delMOPSOParticles);

   std::vector<bool>::iterator _iterator=erase.begin();
   std::vector<MOPSOParticle>::iterator _iterator2=solutions.begin();
   for(_iterator=erase.begin(); _iterator != erase.end();++_iterator) {
     if(*_iterator==true){
       solutions.erase(_iterator2);
       nsolutions--;
     }
     else
       _iterator2++;
   }

  return _flag;

}



int EFILE::falseReAdd(){
  int _howmany(0);
  std::vector<bool> erase;
  erase.resize(nsolutions);

  //  vector<EPSs>::iterator _iterator;
  for(int _i(0);_i<nsolutions;_i++){
    erase[_i]=false;
    for(int _j(0);_j<nsolutions;_j++){
      int _tmp;
      if(_i==_j)continue;
      _tmp=domine2(solutions[_i],solutions[_j]);
      if(_tmp==-1)
  erase[_i]=true;
    }
  }
   std::vector<bool>::iterator _iterator=erase.begin();
   std::vector<MOPSOParticle>::iterator _iterator2=solutions.begin();
   for(_iterator=erase.begin(); _iterator != erase.end();++_iterator) {
     if(*_iterator==true){
       _howmany++;
     }
     else
       _iterator2++;
   }
  return _howmany;
}


int EFILE::domine2(MOPSOParticle &_a,MOPSOParticle &_b){
    if (_a==_b) {
        return 1;
    }
    if (_a.getConstrViolation() > _b.getConstrViolation()) {
      return -1;
  } else if (_a.getConstrViolation() < _b.getConstrViolation()) {
      return 1;
  }

  std::vector<double> _box(nobjectives),_box2(nobjectives),_box3(nobjectives);
  bool _flag=true;
  //calculate the box of both MOPSOParticles
  for (int _i = 0; _i < nobjectives; _i++){
   if( FEpsilon[_i]!=0) {
      _box[_i] = (int) floor ((fabs(tlb[_i]-_a.getObj()[_i]) /FEpsilon[_i]));
      _box2[_i] = (int)floor ((fabs(tlb[_i]-_b.getObj()[_i]) / FEpsilon[_i]));
   } else {
       _box[_i]=0;
       _box2[_i]=0;
   }

    _box3[_i] = (int)_box[_i]*FEpsilon[_i];
    //if they are in the same box
    if(_box[_i]!=_box2[_i])_flag=false;
  }
  if(_flag==true){//check for dominance
    int anterior = 0, mejor;

    for(int _i=0;_i<nobjectives;_i++){
      if(_a.getObj()[_i] <_b.getObj()[_i])	mejor = 1;
      else if(_b.getObj()[_i]<_a.getObj()[_i])mejor = -1;
      else mejor = 0;
      if(mejor!=anterior&&anterior!=0&&mejor!=0){
          if(euclideanDistance(_a.getObj(),&_box3[0], nobjectives)<euclideanDistance(_b.getObj(),&_box3[0], nobjectives))
    return 1;
  else return -1;	}
      if(mejor!=0) anterior = mejor;
    }
    //      if(anterior==1) return true;
    //else return false;
    return(anterior);

  }



//  int res;
//  if (algorithm->getEpsilon()!=0) {
//   res =  _a.checkDominance(&_b, &FEpsilon);
//  } else {
//   res =    _a.checkDominance(&_b);
//  }
//  return res;
  int anterior = 0, mejor;
    for(int _i=0;_i<nobjectives;_i++){
      if(_box[_i] <_box2[_i])	mejor = 1;
      else if(_box2[_i]<_box[_i])mejor = -1;
      else mejor = 0;
      if(mejor!=anterior&&anterior!=0&&mejor!=0)return 0;
      if(mejor!=0) anterior = mejor;
    }
    return(anterior);


}

void EFILE::normalizeDistances2(){

  for(int _j(0);_j<nsolutions;_j++){
    for(int _i(0);_i<nobjectives;_i++)
      {
  if(solutions[_j].getObj()[_i] < tlb[_i]) tlb[_i]=solutions[_j].getObj()[_i];
  if(solutions[_j].getObj()[_i] > tub[_i]) tub[_i]=solutions[_j].getObj()[_i];

      }
  }
  int _ncurrentsolutions=nsolutions;
  int _a1(0);
  int _a2(0);
  while(_ncurrentsolutions>1.1*maxsolutions||_ncurrentsolutions<0.9*maxsolutions){
      _ncurrentsolutions=nsolutions-falseReAdd();

      bool check=false;
    if(_ncurrentsolutions<0.9*maxsolutions){
      for(int _i(0);_i<nobjectives;_i++){
    FEpsilon[_i]/=1.2;
      if (FEpsilon[_i]<0.000001) {check=true;}
      }
      if(check){break;}
    if(_a1==2&&_a2==1) break;
    _a2=_a1;
    _a1=1;
    }
      else if(_ncurrentsolutions>1.1*maxsolutions){
        for(int _i(0);_i<nobjectives;_i++){
      FEpsilon[_i]*=1.2;
        }
      if(_a1==1&&_a2==2) break;
      _a2=_a1;
      _a1=2;
      }

    }
    reAdd();
}



void EFILE::normalizeDistances1(){

  for(int _j(0);_j<nsolutions;_j++){
    for(int _i(0);_i<nobjectives;_i++)
      {
  if(solutions[_j].getObj()[_i] < tlb[_i]) tlb[_i]=solutions[_j].getObj()[_i];
  if(solutions[_j].getObj()[_i] > tub[_i]) tub[_i]=solutions[_j].getObj()[_i];

      }
  }

  for(int _i(0);_i<nobjectives;_i++){
    FEpsilon[_i]=(double)(tub[_i]-tlb[_i])/nsolutions;
    if(FEpsilon[_i] < algorithm->getEpsilon()) {FEpsilon[_i]=algorithm->getEpsilon();};
  }

  int _a1(0),_a2(0);
  int _ncurrentsolutions=nsolutions;

  while(_ncurrentsolutions>1.1*maxsolutions||_ncurrentsolutions<0.9*maxsolutions){
      _ncurrentsolutions=nsolutions-falseReAdd();
        bool check=false;
      if(_ncurrentsolutions<0.9*maxsolutions){
        for(int _i(0);_i<nobjectives;_i++){
      FEpsilon[_i]/=1.2;
        if (FEpsilon[_i]<0.000001) {check=true;}
        }
        if(check){break;}
      if(_a1==2&&_a2==1) break;
      _a2=_a1;
      _a1=1;
      }
      else if(_ncurrentsolutions>1.1*maxsolutions){
        for(int _i(0);_i<nobjectives;_i++){
      FEpsilon[_i]*=1.2;
        }
      if(_a1==1&&_a2==2) break;
      _a2=_a1;
      _a1=2;
      }

    }
    reAdd();
}






void EFILE::deleteExcedent(){

  for(int _j(0);_j<nsolutions;_j++){
    for(int _i(0);_i<nobjectives;_i++)
      {
  if(solutions[_j].getObj()[_i] < tlb[_i]) tlb[_i]=solutions[_j].getObj()[_i];
  if(solutions[_j].getObj()[_i] > tub[_i]) tub[_i]=solutions[_j].getObj()[_i];

      }
  }

  std::vector<double> distances;
  std::vector<int> _indexes;
  std::vector<double> distances2;
  std::vector<int> _indexes2;
  while(nsolutions>maxsolutions){

    _indexes.resize(nsolutions);

    distances.resize(nsolutions);
    _indexes2.resize(nsolutions);

    distances2.resize(nsolutions);

    for(int _j(0);_j<nsolutions;_j++){
      distances[_j]=MEVO::DOUBLE_MAX;
      for(int _k(0);_k<nsolutions;_k++){
  if(_j==_k)continue;
  double _sum(0);
  for(int _i(0);_i<nobjectives;_i++)
    {
      _sum+=pow(solutions[_j].getObj()[_i]-solutions[_k].getObj()[_i],2.0);
    }
  double _tot=sqrt(_sum);

  if(_tot<distances[_j]){
    distances2[_j]=distances[_j];
    distances[_j]=_tot;
    _indexes2[_j]=_indexes[_j];
    _indexes[_j]=_k;
  }
      }
    }

  double min;
  int _index;
  min=MEVO::DOUBLE_MAX;
  for(int _j(0);_j<nsolutions;_j++){
    if(distances[_j]+distances2[_j]<min){
      min=distances[_j]+distances2[_j];
      _index=_j;
    }
  }

   std::vector<MOPSOParticle>::iterator _iterator2=solutions.begin();
   _iterator2+=_index;
   solutions.erase(_iterator2);
   nsolutions--;
  }
}


void EFILE::finalSolutions(){
  for(int _i(0);_i<allnsolutions;_i++){
    add(allsolutions[_i]);
  }
  deleteExcedent();
}

double EFILE::euclideanDistance(double* _a, double* _b, int _s){
  double s(0);

  for(int i=0; i<_s;i++) {
      s+= pow(_a[i]-_b[i],2.0);
  }
  return sqrt(s);
}


int EFILE::nSolutions(){
  return nsolutions;
}

int EFILE::selectRandomSolution(int _whichcluster){
    return algorithm->getRandomNumbersGenerator()->randomInteger(0,nsolutions-1);
}

int EFILE::selectClusteredRandomSolution(int _whichcluster){
  if(nsolutions>nclusters){
    if(_whichcluster>nclusters) {
    _whichcluster=algorithm->getRandomNumbersGenerator()->randomInteger(0,nclusters-1);//ojo hay que verificar
  }
  if(nsolutionsbyswarm[_whichcluster]==0){
      if(nsolutions == 1){return 0;}
      return algorithm->getRandomNumbersGenerator()->randomInteger(0,nsolutions-1);
  }
  int _tmp=algorithm->getRandomNumbersGenerator()->randomInteger(0,nsolutionsbyswarm[_whichcluster]-1);

  return proximitymatrixindex[_whichcluster][_tmp];
  } else {
      if(nsolutions == 1){return 0;}
      return algorithm->getRandomNumbersGenerator()->randomInteger(0,nsolutions-1);
  }
}

int EFILE::nClusteredSolutions(int _whichcluster){
  return nsolutionsbyswarm[_whichcluster];
}
int EFILE::selectClusteredSolution(int _whichcluster,int _whichsolution){
  return proximitymatrixindex[_whichcluster][_whichsolution];
}

void EFILE::normalizeDistances(){

      for(int _i(0);_i<ndimensions;_i++)
        {
          xtlb[_i]=MEVO::DOUBLE_MAX;
          xtub[_i]=MEVO::DOUBLE_MIN;
        }
      for(int _i(0);_i<nclusters;_i++){
        nsolutionsbyswarm[_i]=0;
      }

      for(int _j(0);_j<nsolutions;_j++){
        for( unsigned int _i(0);_i<algorithm->getProblem()->getRealVarSize();_i++)
          {
      if(solutions[_j].getRealVar()[_i] < xtlb[_i]) xtlb[_i]=solutions[_j].getRealVar()[_i];
      if(solutions[_j].getRealVar()[_i] > xtub[_i]) xtub[_i]=solutions[_j].getRealVar()[_i];

          }
        for(unsigned int _i(0);_i<algorithm->getProblem()->getIntegerVarSize();_i++)
          {
      if(solutions[_j].getIntVar()[_i] < xtlb[_i+algorithm->getProblem()->getRealVarSize()]) xtlb[_i+algorithm->getProblem()->getRealVarSize()]=solutions[_j].getIntVar()[_i];
      if(solutions[_j].getIntVar()[_i] > xtub[_i+algorithm->getProblem()->getRealVarSize()]) xtub[_i+algorithm->getProblem()->getRealVarSize()]=solutions[_j].getIntVar()[_i];

          }
      }



      for(int _j(0);_j<nsolutions;_j++){
        for(int _i(0);_i<ndimensions;_i++)
          {

      if ((unsigned int)_i < algorithm->getProblem()->getRealVarSize()){
        solutions[_j].nx[_i]=(double)(solutions[_j].getRealVar()[_i]-xtlb[_i]);
      } else {
        solutions[_j].nx[_i]=(double)(solutions[_j].getIntVar()[_i-algorithm->getProblem()->getRealVarSize()]-xtlb[_i]);
      }
      if(fabs(xtub[_i]- xtlb[_i])==0)solutions[_j].nx[_i]=0;
      else
        solutions[_j].nx[_i]/=(double)(xtub[_i]-xtlb[_i]);
          }
      }

    }




    void EFILE::hierarchicalClustering()
    {
      //  update();
        int _i,_j;
      if(nsolutions>nclusters){
        hasclusteredfirsttime=true;
        updatematrix=false;
        normalizeDistances();
        createProximityMatrix();
        while(pmsize>nclusters){
          findLowestValue(&_i,&_j);
          copy2Column(_i);
          copy2Column(_j);
          joinColumns(_i,_j);
          copy2Row(_i);
          deleteColumn(_j);
          deleteRow(_j);
          pmsize--;
        }
        for(_i=0;_i<nclusters;_i++){
          for(_j=0;proximitymatrixindex[_i][_j]!=-1;_j++);
          nsolutionsbyswarm[_i]=_j;
        }

      }
    }

    void EFILE::createProximityMatrix(){
      int _i(0),_j(0);
      for(int _k(0);_k<nsolutions-1;_k++,_i++){
        proximitymatrix[_i][_i]=0;
        _j=_i+1;
        for(int _l(_k+1);_l<nsolutions;_l++,_j++){
            proximitymatrix[_i][_j]=euclideanDistance(&(solutions[_k].nx[0]),&(solutions[_l].nx[0]), ndimensions);
        }
        pmsize=_i+1;
        for(int _i(0);_i<pmsize;_i++){
          proximitymatrixindex[_i][0]=_i;
          for(int _j(1);_j<pmsize;_j++){
      proximitymatrixindex[_i][_j]=-1;
          }
        }


      }
    }
    void EFILE::copy2Column(int _i){
      for(int _k(_i+1);_k<pmsize;_k++){
        proximitymatrix[_k][_i]=proximitymatrix[_i][_k];
      }
    }

    void EFILE::copy2Row(int _i){
      for(int _k(_i+1);_k<pmsize;_k++){
        proximitymatrix[_i][_k]=proximitymatrix[_k][_i];
      }
    }

    void EFILE::findLowestValue(int *_i,int *_j){
      double _maxvalue=MEVO::DOUBLE_MAX;
      int _tempi=-1,_tempj=-1;
      for(int _k(0);_k<pmsize;_k++){
        for(int _l=_k+1;_l<pmsize;_l++){
          if(proximitymatrix[_k][_l]<_maxvalue){
      _tempi=_k;
      _tempj=_l;
      _maxvalue=proximitymatrix[_k][_l];
          }
        }
      }
      if(_tempi>_tempj)
        {
          int _tmp=_tempi;
          _tempi=_tempj;
          _tempj=_tmp;
        }
      *_i=_tempi;
      *_j=_tempj;
    }

    void EFILE::deleteRow(int _j){
      for(int _k(_j);_k+1<pmsize;_k++){
        for(int _l(0);_l<pmsize;_l++){//ojo hay que optimizar
          proximitymatrix[_k][_l]=proximitymatrix[_k+1][_l];
        }
        int _m(0);
        for(;proximitymatrixindex[_k+1][_m]!=-1;_m++)
          proximitymatrixindex[_k][_m]=proximitymatrixindex[_k+1][_m];
        proximitymatrixindex[_k][_m]=-1;
      }
    }
    void EFILE::deleteColumn(int _i){
      for(int _k(_i);_k+1<pmsize;_k++){
        for(int _l(0);_l<pmsize;_l++){//ojo hay que optimizar
          proximitymatrix[_l][_k]=proximitymatrix[_l][_k+1];
        }
      }
    }
    void EFILE::joinColumns(int _i,int _j){
      for(int _k=0;_k < pmsize;_k++)
        {
          double _ii=proximitymatrix[_k][_i];
          double _jj=proximitymatrix[_k][_j];

          proximitymatrix[_k][_i]=(_ii<_jj)?_ii:_jj;//si se toma el ms cercano
        }
      int _l(0);
      for(;proximitymatrixindex[_i][_l]!=-1;_l++);
      int _k(0);
      for(;proximitymatrixindex[_j][_k]!=-1;_k++){
        proximitymatrixindex[_i][_l+_k]=proximitymatrixindex[_j][_k];
      }
      proximitymatrixindex[_i][_l+_k]=-1;
    }
