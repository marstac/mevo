//---------------------------------------------------------------------------




#include "NSGA2.h"

//---------------------------------------------------------------------------


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "../../utils/functions.h"
#include "../../utils/messages.h"

#include "../../mutators/CPolynomialMutator.h"
#include "../../crossovers/CSBXCrossover.h"
  NSGA2* gNSGA2 = NULL;

  void NSGA2::initVariables() {

      if (fpara1){
          for (int i = 0; i < 2*maxpop; i++) {
            delete [] fpara1[i];
          }
          delete [] fpara1;
      }

      if(vlen) delete [] vlen;

      if (lim_b && lim_r) {
          for (int i = 0; i < maxvar; i++) {
            delete [] lim_b[i];
            delete [] lim_r[i];
          }
          delete [] lim_b;
          delete [] lim_r;
      }
      if (coef) delete [] coef;

    CGeneticAlgorithm::initVariables();
    maxpop = getPopSize();
    maxchrom = FProblem->getIntegerVarSize();
    maxvar = FProblem->getRealVarSize() + FProblem->getIntegerVarSize();
    maxfun = FProblem->getObjectiveFncCnt();
    maxcons = FProblem->getConstraintFncCnt();
    popsize = getPopSize();

    globalpop.init();
    oldpop.init();
    newpop.init();
    matepop.init();



    fpara1 = new float*[2*maxpop];
    for (int i = 0; i < 2*maxpop; i++) {
      fpara1[i] = new float[2];
    }
    vlen = new int[maxvar];
    lim_b = new float*[maxvar];
    lim_r = new float*[maxvar];
    for (int i = 0; i < maxvar; i++) {
      lim_b[i] = new float[2];
      lim_r[i] = new float[2];
    }
    coef = new double[maxvar];
    ans = 1;
  }

  NSGA2Globpop::NSGA2Globpop() {
      rankar = NULL;
      rankno = NULL;
      genes = NULL;
      rank = NULL;
      flag = NULL;
      fitness = NULL;
      cub_len = NULL;
      xreal = NULL;
      xbin = NULL;
      error = NULL;
      constr = NULL;
  }



  bool NSGA2::checkConf() {
      return true;
  }

  void NSGA2Globpop::init() {
      int  maxpop = gNSGA2->getPopSize();
      int  maxchrom = gNSGA2->getProblem()->getIntegerVarSize();
      int  maxvar = gNSGA2->getProblem()->getRealVarSize() + gNSGA2->getProblem()->getIntegerVarSize();
      int  maxfun = gNSGA2->getProblem()->getObjectiveFncCnt();
      int  maxcons = gNSGA2->getProblem()->getConstraintFncCnt();

      if(rankar)
          for (int i=0; i < 2*maxpop; i++) {
            delete [] rankar[i];
          }
      if(genes)
          for (int i=0; i < 2*maxpop; i++) {
            delete [] genes[i];;
          }
      if(fitness)
          for (int i=0; i < 2*maxpop; i++) {
            delete [] fitness[i];
          }
      if(xreal)
          for (int i=0; i < 2*maxpop; i++) {
            delete [] xreal[i];
          }
      if(xbin)
          for (int i=0; i < 2*maxpop; i++) {
            delete [] xbin[i];
          }
      if(constr)
          for (int i=0; i < 2*maxpop; i++) {
            delete [] constr[i];
          }

      if(rankar) delete [] rankar;
      if(rankno) delete [] rankno;
      if(genes) delete [] genes;
      if(rank) delete [] rank;
      if(flag) delete [] flag;

      if(cub_len) delete [] cub_len;
      if(xreal) delete [] xreal;
      if(xbin) delete [] xbin;
      if(error) delete [] error;
      if(constr) delete [] constr;


      rankar = new int*[2*maxpop];
      rankno = new int[2*maxpop];
      genes = new int*[2*maxpop];
      rank = new int[2*maxpop];
      flag = new int[2*maxpop];
      fitness = new double*[2*maxpop];
      cub_len = new double[2*maxpop];
      xreal = new double*[2*maxpop];
      xbin = new int*[2*maxpop];
      error = new double[2*maxpop];
      constr = new double*[2*maxpop];
      for (int i=0; i < 2*maxpop; i++) {
        rankar[i] = new int [2*maxpop];
        genes[i] = new int [maxchrom];
        fitness[i] = new double [maxfun];
        xreal[i] = new double[maxvar];
        xbin[i] = new int[maxvar];
        constr[i] = new double[maxcons];
      }
  }

  NSGA2Globpop::NSGA2Globpop(const NSGA2Globpop& src) {
    NSGA2Globpop();

    int  maxpop = gNSGA2->getPopSize();
    int  maxchrom = gNSGA2->getProblem()->getIntegerVarSize();
    int  maxvar = gNSGA2->getProblem()->getRealVarSize() + gNSGA2->getProblem()->getIntegerVarSize();
    int  maxfun = gNSGA2->getProblem()->getObjectiveFncCnt();
    int  maxcons = gNSGA2->getProblem()->getConstraintFncCnt();

    for (int i = 0; i < 2*maxpop; i++) {
      for (int j=0; j < 2*maxpop; j++) {
        rankar[i][j] = src.rankar[i][j];
      }
      for (int j=0; j < maxchrom; j++) {
        genes[i][j] = src.genes[i][j];
      }
      for (int j=0; j < maxfun; j++) {
        fitness[i][j] = src.fitness[i][j];
      }
      for (int j=0; j < maxvar; j++) {
        xreal[i][j] = src.xreal[i][j];
        xbin[i][j] = src.xbin[i][j];
      }
      for (int j=0; j < maxcons; j++) {
        constr[i][j] = src.constr[i][j];
      }
    }
  }
  NSGA2Globpop::~NSGA2Globpop() {
    int  maxpop = gNSGA2->getPopSize();


    for (int i=0; i < 2*maxpop; i++) {
      if(rankar) delete [] rankar[i];
      if(genes) delete [] genes[i];
      if(fitness) delete [] fitness[i];
      if(xreal) delete [] xreal[i];
      if(xbin) delete [] xbin[i];
      if(constr) delete [] constr[i];
    }

    if (rankar) delete [] rankar;
    if (rankno) delete [] rankno;
    if (genes) delete [] genes;
    if (rank) delete [] rank;
    if (flag) delete [] flag;

    if (cub_len) delete [] cub_len;
    if (xreal) delete [] xreal;
    if (xbin) delete [] xbin;
    if (error) delete [] error;
    if (constr) delete [] constr;
  }

  NSGA2Individual::NSGA2Individual() : CIndividual(gNSGA2->getProblem()->getRealVarSize(),
                                                   gNSGA2->getProblem()->getIntegerVarSize(),
                                                   gNSGA2->getProblem()->getObjectiveFncCnt(),
                                                   gNSGA2->getProblem()->getConstraintFncCnt()) {

  }
  NSGA2Individual::NSGA2Individual(const NSGA2Individual& src) : CIndividual(src) {

  }
  NSGA2Individual::~NSGA2Individual() {

  }

  MEVO::ICIndividual *NSGA2Individual::copy()
  {
      NSGA2Individual* ind = new NSGA2Individual(*this);
      return ind;
  }

  population::population() {
      rankrat = NULL;
      rankno = NULL;
      ind = NULL;

  }

  void population::init(){
    int  maxpop = gNSGA2->getPopSize();

    if(rankrat) delete [] rankrat;
    if(rankno) delete [] rankno;
    if(ind) delete [] ind;

    rankrat = new float[maxpop];
    rankno = new int[maxpop];
    ind = new NSGA2Individual[maxpop];
  }

  population::~population() {
    delete [] rankrat;
    delete [] rankno;
    delete [] ind;
  }
  population::population(const population& src) {
    population();
    int  maxpop = gNSGA2->getPopSize();
    for (int i = 0; i < maxpop; i++) {
      rankrat[i] = src.rankrat[i];
      rankno[i] = src.rankno[i];
      ind[i] = src.ind[i];
    }
  }
  NSGA2::NSGA2() : CGeneticAlgorithm() {
    gNSGA2 = this;
    setMutator(new MEVO::CPolynomialMutator);
    setCrossover(new MEVO::CSBXCrossover);

    fpara1 = NULL;

    vlen = NULL;
    lim_b = NULL;
    lim_r = NULL;
    coef =  NULL;
  }

  NSGA2::~NSGA2() {
    if (fpara1){
        for (int i = 0; i < 2*maxpop; i++) {
          delete [] fpara1[i];
        }
        delete [] fpara1;
    }

    if (lim_b && lim_r) {
        for (int i = 0; i < maxvar; i++) {
          delete [] lim_b[i];
          delete [] lim_r[i];
        }
        delete [] lim_b;
        delete [] lim_r;
    }
    if (vlen)delete [] vlen;
    if (coef) delete [] coef;
  }

  void NSGA2::func(population *pop_ptr)
  {
    pop_ptr->ind_ptr= &(pop_ptr->ind[0]);

    /*Initializing the max rank to zero*/
    pop_ptr->maxrank = 0;

    FPop.clearIndividuals();
    for(int i = 0;i < popsize;i++)
    {
      pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
      FPop.addIndividual(pop_ptr->ind_ptr);

    }
    evaluatePopulation(&FPop);
    /*---------------------------* RANKING *------------------------------*/

//    if(ncons == 0)
//      ranking(pop_ptr);
//    else
      rankcon(pop_ptr);

    return;
  }

  void NSGA2::input()
  {
    int i;

    float cc;

    maxpop = getPopSize();

    nvar=FProblem->getRealVarSize();
    nchrom=FProblem->getIntegerVarSize();


    if ((nvar > maxvar) || (nchrom > maxvar))
    {
      printf(" Increase No. of the variables in the program. Currently set %d.\n",maxvar);
      exit(1);
    }

    nfunc=FProblem->getObjectiveFncCnt();


    if(nfunc > maxfun)
    {
      printf("Increase the number of functions in the program. Curently set %d.\n",maxfun);
      exit(1);
    }


    ncons=FProblem->getConstraintFncCnt();


    if(ncons > maxcons)
    {
      printf("Increase the number of constraints in the program. Currently set %d.\n",maxcons);
      exit(1);
    }



    fflush(stdin);

    popsize=getPopSize();



    if(popsize > maxpop)
    {
      printf("Increase Population size. Currently set %d\n",maxpop);
      exit(1);
    }



    if (nvar > 0)
    {
      cc = 1.0/nvar;

      for(i = 0; i < nvar; i++)
      {

        lim_r[i][0]=FProblem->getRealLowLimit(i);
        lim_r[i][1]=FProblem->getRealHighLimit(i);

      }


    }

    chrom = 0;
    if (nchrom > 0)
    {

      /*Specify the Cross-over Probability for the Simple or Real Coded
  Cross-over suited Range is 0.9 to 0.99*/

	  for (i = 0; i < nchrom; i++)
      {

		vlen[i] = 1;//gNSGA2Optimizer->getBitsNumber(i);
        chrom += vlen[i];

        //printf("Give lower & the upper limits of the %d variable\n",i+1);
        //scanf("%f %f",&lim_b[i][0],&lim_b[i][1]);
        lim_b[i][0] = FProblem->getIntegerLowLimit(i);
        lim_b[i][1] = FProblem->getIntegerHighLimit(i);
      }
            cc = 1.0/chrom;


    }



    return;
  }
  void NSGA2::keepalive(population *pop1_ptr,population *pop2_ptr,population *pop3_ptr,int gen)
  {
    int i,j,jj,k,m,l,rec;

    int *front_pop = new int[maxpop];

    int st,pool,poolf,sel;

    int *gene1_ptr, *gene2_ptr;

    double *gene3_ptr,*gene4_ptr;
    int *xbin1_ptr,*xbin2_ptr;

    /*Forming the global mating pool*/
    FPop.clearIndividuals();

    for(i = 0;i < popsize;i++)
    {
      if(nchrom > 0)
      {
        /*Binary Coded GA genes are copied*/
        for(k = 0;k < chrom;k++)
        {
          globalpop.genes[i][k]=pop1_ptr->ind[i].getIntVar()[k];
          globalpop.genes[i+popsize][k] = pop2_ptr->ind[i].getIntVar()[k];
        }
        for (k=0; k < nchrom; k++)
        {
          globalpop.xbin[i][k] = pop1_ptr->ind[i].getIntVar()[k];
          globalpop.xbin[i+popsize][k] = pop2_ptr->ind[i].getIntVar()[k];
        }
      }
      if (nvar > 0)
      {
        /*For Real Coded GA x values are copied */
        for(k = 0;k < nvar;k++)
        {
          globalpop.xreal[i][k] = pop1_ptr->ind[i].getRealVar()[k];
          globalpop.xreal[i+popsize][k] = pop2_ptr->ind[i].getRealVar()[k];
        }
      }

      /*Fitness is copied to the global pool */
      for(l = 0;l < nfunc;l++)
      {
        globalpop.fitness[i][l] = pop1_ptr->ind[i].getObj()[l];
        globalpop.fitness[i+popsize][l] = pop2_ptr->ind[i].getObj()[l];
      }

      /*Initial;ising the dummyfitness to zero */
      globalpop.cub_len[i] = 0;
      globalpop.cub_len[i+popsize] = 0;
      globalpop.error[i] = pop1_ptr->ind[i].getConstrViolation();
      globalpop.error[i+popsize] = pop2_ptr->ind[i].getConstrViolation();
      for (jj=0; jj<ncons; jj++)
      {
        globalpop.constr[i][jj] = pop1_ptr->ind[i].getConstr()[jj];
        globalpop.constr[i+popsize][jj] = pop2_ptr->ind[i].getConstr()[jj];
      }
   }
    for(i = 0;i < popsize;i++)
    {
        FPop.addIndividual(&pop1_ptr->ind[i]);
    }

    for(i = 0;i < popsize;i++)
    {
        FPop.addIndividual(&pop2_ptr->ind[i]);
    }

    global_pop_ptr = &(globalpop);

    /*Finding the global ranks */
//    if(ncons == 0)
//      grank(gen);
//    else
      grankc(gen);

    m = globalpop.maxrank;

    /* Sharing the fitness to get the dummy fitness */
    for(i = 0;i < m;i++)
    {
      gshare(i+1);
    }

    poolf = popsize;
    pool = 0;


    /*Initializing the flags of population to zero */
    for(i = 0;i < 2*popsize;i++)
    {
      globalpop.flag[i] = 0;
    }
    // decide which all solutions belong to the pop3
    rec = 0;
    st = 0;
    for(i = 0 ;i < m ; i++)
    {
      /*    Elitism Applied Here     */
      st = pool;
      pool += globalpop.rankno[i];

      if(pool <= popsize)
      {
        for(k = 0;k < 2*popsize ;k++)
        {
          if(globalpop.rank[k] == i+1)
            globalpop.flag[k] = 1;
        }
        pop3_ptr->rankno[i] = globalpop.rankno[i];
      }
      else
      {
        sel = popsize - st;
        Lastrank = i+1;
        pop3_ptr->rankno[i] = sel;
        gsort(i+1,sel);
        break;
      }
    }

    k = 0;
    for(i = 0,k = 0;i < 2*popsize && k < popsize; i++)
    {
      if(nchrom > 0)
      {
        if(globalpop.flag[i] == 1)
        {
          gene1_ptr = &(globalpop.genes[i][0]);
          xbin1_ptr = &(globalpop.xbin[i][0]);
          pop3_ptr->ind_ptr = &(pop3_ptr->ind[k]);
          gene2_ptr = &(pop3_ptr->ind_ptr->getIntVar()[0]);
          xbin2_ptr = &(pop3_ptr->ind_ptr->getIntVar()[0]);

          for(j = 0 ; j < chrom; j++)
          {
            *gene2_ptr++ = *gene1_ptr++;
          }
          for (j=0; j < nchrom; j++)
            *xbin2_ptr++ = *xbin1_ptr++;
        }
      }
      if (nvar > 0)
      {
        if(globalpop.flag[i] == 1)
        {
          gene3_ptr = &(globalpop.xreal[i][0]);
          pop3_ptr->ind_ptr = &(pop3_ptr->ind[k]);
          gene4_ptr = &(pop3_ptr->ind_ptr->getRealVar()[0]);

          for(j = 0 ;j < nvar;j++)
          {
            *gene4_ptr++ = *gene3_ptr++;
          }
        }
      }
      if(globalpop.flag[i] == 1)
      {
        for(j = 0;j < nfunc;j++)
          pop3_ptr->ind[k].getObj()[j] = globalpop.fitness[i][j];
        pop3_ptr->ind[k].setCrowdingDistance(globalpop.cub_len[i]);
        if(ncons != 0)
          pop3_ptr->ind[k].setConstrViolation(globalpop.error[i]);
        for (jj=0; jj<ncons; jj++)
          pop3_ptr->ind[k].getConstr()[jj] = globalpop.constr[i][jj];
        pop3_ptr->ind[k].rank = globalpop.rank[i];
        k++;  // increment the pop3 counter
      }
    }

    pop3_ptr->maxrank = Lastrank;
    delete [] front_pop;
    return;
  }

  void NSGA2::grankc(int gen)
  {

      std::deque<double>* epss = NULL;
      std::deque<double> epsi;
      if (FEps !=0) {
          epsi = FPop.calcEpsi(FEps);
          epss=&epsi;
      }
    int i,j,k,rnk,val,nondom,popsize1,q;
    int *gflg =  new int[2*maxpop];
    double *ptr1,*ptr2;
    double *err_ptr1,*err_ptr2;

        /*----------------------------* RANKING *---------------------------------*/
    rnk = 0;
    nondom = 0;
    popsize1 = 2*popsize;
    min_fit = popsize1;
    delta_fit = 0.1 *popsize1;
    for(i=0;i<popsize1;i++)
    {
      gflg[i] = 2;
    }
    for(k = 0;k < popsize1;k++)
    {
      q =  0;
      for(j = 0;j < popsize1;j++)
      {
        if (gflg[j] != 1) break;
      }
      if(j == popsize1) break;
      rnk = rnk +1;
      for( j = 0 ;j < popsize1; j++)
      {
        if(gflg[j] == 0) gflg[j] = 2;
      }
      for(i = 0;i< popsize1 ; i++)
      {
        if(gflg[i] != 1 && gflg[i] != 0)
        {

          MEVO::ICIndividual* indi = FPop.getIndividual(i);
          for(j = 0;j < popsize1 ; j++)
          {
            if( i!= j)
            {
              if(gflg[j] != 1)
              {

                MEVO::ICIndividual* indj = FPop.getIndividual(j);
                val = indi->checkDominance(indj, epss);
                if( val == -1){
                    gflg[i] = 0;
                    /* NSGA2Individual 1 is dominated */
                    break;
                }
                if(val == 1){
                    gflg[j] = 0;
                    /* NSGA2Individual 2 is dominated */
                }
                if(val == 0){
                    nondom++;/* NSGA2Individual 1 & 2 are non dominated */
                    if(gflg[j] != 0) gflg[j] = 3;
                }

              }
            }
          }
          if( j == popsize1)
          {
            global_pop_ptr->rank[i] = rnk;
            gflg[i] = 1;
            global_pop_ptr->rankar[rnk-1][q] =  i;
            q++;
          }
        }
      }
      global_pop_ptr->rankno[rnk-1] = q;
    }
    global_pop_ptr->maxrank = rnk;

    delete [] gflg;
    return;
  }

    void NSGA2::gsort(int rnk,int sel)
  {
    int i,j,a,q;
    float temp,temp1;

    float**array = new float*[2*maxpop];
    for (int i=0; i < 2*maxpop; i++) {
      array[i] = new float[2];
    }

    q = globalpop.rankno[rnk-1];

    for(i = 0 ;i < q ;i++)
    {
      array[i][0] = globalpop.rankar[rnk-1][i];
      a = globalpop.rankar[rnk-1][i];
      array[i][1] = globalpop.cub_len[a];
    }
    for(i = 0;i < q ;i++)
    {
      for(j = i+1;j < q;j++)
      {
        if(array[i][1] < array[j][1])
        {
          temp = array[i][1];
          temp1 = array[i][0];
          array[i][1] = array[j][1];
          array[i][0] = array[j][0];

          array[j][1] = temp;
          array[j][0] = temp1;
        }
      }
    }

    for(i = 0;i < sel;i++)
    {
      a = array[i][0];
      globalpop.flag[a] = 1;
    }

    for (int i=0; i < 2*maxpop; i++) {
      delete [] array[i];
    }
    delete [] array;
    return;
  }

  void NSGA2::gshare(int rnk)
  {
    float max;

    float**length =  new float*[2*maxpop];
    for (int i=0; i < 2*maxpop; i++) {
      length[i] = new float[2];
    }

    int i,j,m1,a ;
    float min, Diff;  // Added 18.08.2003

    m1 = globalpop.rankno[rnk-1];

    for(j = 0;j < nfunc;j++)
    {
      for(i = 0;i < m1;i++)
      {
        fpara1[i][0] = 0;
        fpara1[i][1] = 0;
      }

      for(i = 0;i < m1;i++)
      {
        a = globalpop.rankar[rnk-1][i];
        fpara1[i][0] = (float)a ;
        fpara1[i][1] = globalpop.fitness[a][j];
      }

      sort(m1); /*Sort the arrays in ascending order of the fitness*/

      max = fpara1[m1-1][1];
      min = fpara1[0][1];  // Added 18.08.2003
      Diff = max-min;      // Added 18.08.2003 and 5 subsequent lines
      if (Diff < 0.0)
      {
        printf("Something wrong in keepaliven.h\n");
        exit(1);
      }
      for(i = 0;i < m1;i++)
      {
        if(i == 0 ||i == (m1-1))
        {
          length[i][0] = fpara1[i][0];
          length[i][1] = 100*max;
        }
        else
        {
		  length[i][0] = fpara1[i][0];
		  length[i][1] = fabs(fpara1[i+1][1]- fpara1[i-1][1]);
		  if (Diff!=0) length[i][1]/=Diff; // crowding distances are normalized 18.08.2003
        }
      }
      for(i = 0;i < m1;i++)
      {
        a = length[i][0];
        globalpop.cub_len[a] += length[i][1];
      }
    }
    for (int i=0; i < 2*maxpop; i++) {
      delete [] length[i];
    }
    delete [] length;
    return;
  }

  void NSGA2::sort(int m1)
  {
    float temp,temp1;
    int i1,k1;
    for(k1 = 0;k1 < m1-1;k1++)
    {
      for(i1 = k1+1;i1 < m1;i1++)
      {
        if(fpara1[k1][1] > fpara1[i1][1])
        {
          temp = fpara1[k1][1];
          temp1 = fpara1[k1][0];
          fpara1[k1][1] = fpara1[i1][1];
          fpara1[k1][0] = fpara1[i1][0];
          fpara1[i1][1] = temp;
          fpara1[i1][0] = temp1;
        }
      }
    }
    return;
  }


  void NSGA2::run()
  {

      if (!checkConf()) {
        fireOnErrorNotify(MEVO::E_OPT_TERM);
        return;
      }


      FTerminated = false;






    /*Some Local variables to this Problem (Counters And some other pointers*/

    int l,f,maxrank1;
    float tot;



    old_pop_ptr = &(oldpop);

    nmut = 0;
    ncross = 0;

    initVariables();
    fireOnBeforeStartNotify(MEVO::M_OPT_STARTED, this);
    /*Get the input from the file input.h*/
    input();



    MEVO::CPopulation ipop;
    for (int i=0; i<popsize;i++) {
        old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[i]);
        ipop.addIndividual(old_pop_ptr->ind_ptr);
    }
     initializePopulation(&ipop);


    old_pop_ptr = &(oldpop);


    old_pop_ptr = &(oldpop);
    new_pop_ptr = &(newpop);

    for(int j = 0;j < popsize;j++)
    {
      /*Initializing the Rank array having different individuals
  at a particular  rank to zero*/
      old_pop_ptr->rankno[j] = 0;
      new_pop_ptr->rankno[j] = 0;
    }

    old_pop_ptr = &(oldpop);

    func(old_pop_ptr);



    /********************************************************************/
    /*----------------------GENERATION STARTS HERE----------------------*/

    int i=0;
    MEVO::CPopulation spop;
    do {

      if (isTerminated()) {break;}
      //printf("Generation = %d\n",i+1);
      old_pop_ptr = &(oldpop);
      mate_pop_ptr = &(matepop);
           /*--------SELECT----------------*/
      nselect(old_pop_ptr ,mate_pop_ptr );

      new_pop_ptr = &(newpop);
      mate_pop_ptr = &(matepop);

      MEVO::CPopulation cppop;
      MEVO::CPopulation copop;
      for(int j = 0;j < popsize;j++)
      {
        new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
        copop.addIndividual(new_pop_ptr->ind_ptr);
        mate_pop_ptr->ind_ptr = &(mate_pop_ptr->ind[j]);
        cppop.addIndividual(mate_pop_ptr->ind_ptr);
      }
      crossoverPopulation(&cppop, &copop);


      /*------MUTATION-------------------*/
      new_pop_ptr = &(newpop);

      MEVO::CPopulation mpop;
      for(int j = 0;j < popsize;j++)
      {
        new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
        mpop.addIndividual(new_pop_ptr->ind_ptr);

      }
      mutatePopulation(&mpop);

      new_pop_ptr = &(newpop);


      /*----------FUNCTION EVALUATION-----------*/
      new_pop_ptr = &(newpop);
      func(new_pop_ptr );

      /*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
      old_pop_ptr = &(oldpop);
      new_pop_ptr = &(newpop);
      mate_pop_ptr = &(matepop);

      /*Elitism And Sharing Implemented*/
      keepalive(old_pop_ptr ,new_pop_ptr ,mate_pop_ptr,i+1);

      mate_pop_ptr = &(matepop);


      mate_pop_ptr = &(matepop);
      /*------------------REPORT PRINTING--------------------------------*/

      /*==================================================================*/

      /*----------------Rank Ratio Calculation------------------------*/
      new_pop_ptr = &(matepop);
      old_pop_ptr = &(oldpop);

      /*Finding the greater maxrank among the two populations*/

      if(old_pop_ptr->maxrank > new_pop_ptr->maxrank)
        maxrank1 = old_pop_ptr->maxrank;
      else
        maxrank1 = new_pop_ptr->maxrank;

      /*==================================================================*/

      /*=======Copying the new population to old population======*/

      old_pop_ptr = &(oldpop);
      new_pop_ptr = &(matepop);

      for(int j = 0;j < popsize;j++)
      {
        old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
        new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
        if(nchrom > 0)
        {
          /*For Binary GA copying of the chromosome*/

          for(l = 0;l < chrom;l++)
            old_pop_ptr->ind_ptr->getIntVar()[l]=new_pop_ptr->ind_ptr->getIntVar()[l];

          for(l = 0;l < nchrom;l++)
            old_pop_ptr->ind_ptr->getIntVar()[l] = new_pop_ptr->ind_ptr->getIntVar()[l];
        }
        if(nvar > 0)
        {
          /*For Real Coded GA copying of the chromosomes*/
          for(l = 0;l < nvar;l++)
            old_pop_ptr->ind_ptr->getRealVar()[l] = new_pop_ptr->ind_ptr->getRealVar()[l];
        }

        /*Copying the fitness vector */
        for(l = 0 ; l < nfunc ;l++)
          old_pop_ptr->ind_ptr->getObj()[l] = new_pop_ptr->ind_ptr->getObj()[l];

        /*Copying the dummy fitness*/
        old_pop_ptr->ind_ptr->setCrowdingDistance(new_pop_ptr->ind_ptr->getCrowdingDistance());

        /*Copying the rank of the individuals*/
        old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

        /*Copying the error and constraints of the NSGA2Individual*/

        old_pop_ptr->ind_ptr->setConstrViolation(new_pop_ptr->ind_ptr->getConstrViolation());
        for(l = 0;l < ncons;l++)
        {
          old_pop_ptr->ind_ptr->getConstr()[l] = new_pop_ptr->ind_ptr->getConstr()[l];
        }

        /*Copying the flag of the individuals*/
        old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
      }   // end of j

      maxrank1 = new_pop_ptr->maxrank ;

      /*Copying the array having the record of the NSGA2Individual
  at different ranks */
      spop.clearIndividuals();
      for(l = 0;l < popsize;l++)
      {
        old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
        spop.addIndividual(&(old_pop_ptr->ind[l]));
      }

      /*Copying the maxrank */
      old_pop_ptr->maxrank = new_pop_ptr->maxrank;

      /*Printing the fitness record for last generation in a file last*/
        i++;
      fireOnGenerationDoneNotify(i, &spop);


    } while (!checkStop(&spop));
    // end of i
    // for the last generation
    old_pop_ptr = &(matepop);
    MEVO::CPopulation resPop;
    for(f = 0;f < popsize ; f++) // for printing
    {

      old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

      if ((old_pop_ptr->ind_ptr->getConstrViolation() <= 0.0) && (old_pop_ptr->ind_ptr->rank == 1))  // for all feasible solutions and non-dominated solutions
      {




        resPop.addIndividual(old_pop_ptr->ind_ptr);
      }  // feasibility check

    } // end of f (printing)
    setLastPop(&resPop);
    if (resPop.getSize()==0) {
      fireOnAfterStopNotify(MEVO::M_OPT_NO_FSOL, FLastPop);
    }

    // for the last generation
    /*                   Generation Loop Ends                                */
    /************************************************************************/



    fireOnAfterStopNotify(MEVO::M_OPT_FINISHED, FLastPop);
  }

  void NSGA2::rankcon(population *pop_ptr)
  {
    int i,j,k,       /*counters*/
    rnk,           /*rank*/
    val,           /*value obtained after comparing two individuals*/
    nondom,        /*no of non dominated members*/
    maxrank1,      /*Max rank of the population*/
    q;

    float* rankarr=new float[maxpop]; /*Array storing the NSGA2Individual number at a rank*/
    //double *ptr1,*ptr2,*err_ptr1,*err_ptr2;

    /*------------------------------* RANKING *------------------------------*/

    std::deque<double>* epss = NULL;
    std::deque<double> epsi;
    if (FEps !=0) {
        epsi = FPop.calcEpsi(FEps);
        epss=&epsi;
    }


    /*Initializing the ranks to zero*/
    rnk = 0 ;

    nondom = 0 ;
    maxrank1 = 0;
    /*min_fit is initialize to start distributing the dummy fitness =
    popsize to the rank one individuals and keeping the record such
    that the minimum fitness of the better rank NSGA2Individual is always
    greater than max fitness of the relatively worse rank*/

    min_fit = popsize;


    /*Difference in the fitness of minimum dummy fitness of better rank
    and max fitness of the next ranked individuals*/

    delta_fit = 0.1 * popsize;

    /*Initializing all the flags to 2*/

    for( j = 0 ;j < popsize; j++)
    {
      pop_ptr->ind[j].flag = 2;
    }

    q = 0;

    for(k =  0;k < popsize;k++,q=0)
    {
      for(j = 0;j < popsize;j++)
      {
        if (pop_ptr->ind[j].flag != 1)break;
        /*Break if all the individuals are assigned a rank*/
      }
      if(j == popsize)break;

      rnk = rnk + 1;

      for( j = 0 ;j < popsize; j++)
      {
        if(pop_ptr->ind[j].flag == 0) pop_ptr->ind[j].flag = 2;
        /*Set the flag of dominated individuals to 2*/
      }

      for(i = 0;i < popsize ; i++)
      {
        /*Select an NSGA2Individual which rank to be assigned*/

        pop_ptr->ind_ptr = &(pop_ptr->ind[i]);

        if(pop_ptr->ind_ptr->flag != 1 && pop_ptr->ind_ptr->flag != 0)
        {
          //ptr1 = &(pop_ptr->ind_ptr->getObj()[0]);
          MEVO::CIndividual* indi = pop_ptr->ind_ptr;
//          double err = pop_ptr->ind_ptr->getConstrViolation();
//          err_ptr1 = &err;

          for(j = 0;j < popsize ; j++)
          {

            /*Select the other NSGA2Individual which has not got a rank*/
            if( i!= j)
            {
              if(pop_ptr->ind[j].flag != 1)
              {
                pop_ptr->ind_ptr = &(pop_ptr->ind[j]);
                //ptr2 = &(pop_ptr->ind_ptr->getObj()[0]);
                MEVO::CIndividual* indj = pop_ptr->ind_ptr;
//                double err2 = pop_ptr->ind_ptr->getConstrViolation();
//                err_ptr2 = &err2;
                val = indi->checkDominance(indj, epss);
                if( val == -1){
                    pop_ptr->ind[i].flag = 0;
                    /* NSGA2Individual 1 is dominated */
                    break;
                }

                if(val == 1){
                pop_ptr->ind[j].flag = 0;
                    /* NSGA2Individual 2 is dominated */
                }

                    if(val == 0){
                    nondom++;
                    /* NSGA2Individual 1 & 2 are
                             non dominated */
                    if(pop_ptr->ind[j].flag != 0)
                        pop_ptr->ind[j].flag = 3;
                    }
              }
            }
          }        /*loop over j ends*/
          if( j == popsize)
          {
            /*Assign the rank and set the flag*/
            pop_ptr->ind[i].rank = rnk;
            pop_ptr->ind[i].flag = 1;
            rankarr[q] = i;
            q++;
          }
        }       /*Loop over flag check ends*/
      }           /*Loop over i ends */
      pop_ptr->rankno[rnk-1] = q ;
    }
    maxrank1 = rnk;


    /*     Find Max Rank of the population    */
    for(i = 0;i < popsize;i++)
    {
      rnk = pop_ptr->ind[i].rank;

      if(rnk > maxrank1)maxrank1 = rnk;

    }

    pop_ptr->maxrank = maxrank1;
    delete [] rankarr;
    return;
  }

  void  NSGA2::report(int t,population *pop1_ptr,population *pop2_ptr,FILE *rep_ptr,FILE *gen_ptr, FILE *lastit )
  {
    int i,j,*rptr,*rptr1;

    double *ptr1,*fptr,*fptr1;
    int *ptr1_b, *ptr2_b;

    double *ptr2,*cons_ptr1,*cons_ptr2, *err2;

    fprintf(rep_ptr,"\n\n---------------------------------------------------\n");
    fprintf(rep_ptr,"Generation No.     ->%d\n",t+1);
    fprintf(rep_ptr,"------------------------------------------------------\n");
    if(ncons == 0)
      fprintf(rep_ptr," variables (real %d binary %d)  fitness (%d)  rank cublen || variables  fitness rank cublen\n",nvar,nchrom,nfunc);
    else
      fprintf(rep_ptr," variables (real %d binary %d)  fitness (%d) constraint (%d) penalty rank cublen || variables  fitness constraint penalty rank cublen\n",nvar,nchrom,nfunc,ncons);

    pop1_ptr->ind_ptr = &(pop1_ptr->ind[0]);

    pop2_ptr->ind_ptr = &(pop2_ptr->ind[0]); // Deb 31/10/01


    for(i = 0;i < popsize;i++)
    {
      fprintf(rep_ptr,"\n------------------------------------------------\n");

      ptr1_b = &(pop1_ptr->ind_ptr->getIntVar()[0]);
      ptr2_b = &(pop2_ptr->ind_ptr->getIntVar()[0]);

      ptr1 = &(pop1_ptr->ind_ptr->getRealVar()[0]);
      ptr2 = &(pop2_ptr->ind_ptr->getRealVar()[0]);  // Deb 31/10/01

      fptr = &(pop1_ptr->ind_ptr->getObj()[0]);
      fptr1 = &(pop2_ptr->ind_ptr->getObj()[0]);

      rptr = &(pop1_ptr->ind_ptr->rank);
      rptr1 = &(pop2_ptr->ind_ptr->rank);

      cons_ptr1 = &(pop1_ptr->ind_ptr->getConstr()[0]);
      cons_ptr2 = &(pop2_ptr->ind_ptr->getConstr()[0]);
      double eerr = pop2_ptr->ind_ptr->getConstrViolation();
      err2 = &eerr;

      for(j = 0;j < nvar;j++)
        fprintf(rep_ptr,"%f ",*ptr1++);

      for(j = 0;j < nchrom; j++)
        fprintf(rep_ptr,"%i ",*ptr1_b++);

      fptr =  &(pop1_ptr->ind_ptr->getObj()[0]);
      fptr1 = &(pop2_ptr->ind_ptr->getObj()[0]);

      for(j = 0;j < nfunc;j++)
        fprintf(rep_ptr,"  %.4f",*fptr++);

      if(ncons != 0)
      {
        for(j = 0;j < ncons;j++)
          fprintf(rep_ptr,"  %.2e",*cons_ptr1++);
        fprintf(rep_ptr," %.2e",pop1_ptr->ind_ptr->getConstrViolation());
      }

      fprintf(rep_ptr," %d ",*rptr);

      fprintf(rep_ptr,"%f ",pop1_ptr->ind_ptr->getCrowdingDistance());
      fprintf(rep_ptr,"|**|");

      for(j = 0;j < nvar;j++)
      {
        fprintf(rep_ptr," %f ",*ptr2);
        fprintf(gen_ptr," %f ",*ptr2++);
      }
      for(j = 0;j < nchrom; j++)
      {
        fprintf(rep_ptr,"%i ",*ptr2_b);
        fprintf(gen_ptr,"%i ",*ptr2_b++);
      }
      for(j = 0;j < nfunc;j++)
      {
        fprintf(rep_ptr,"  %f",*fptr1);
        fprintf(gen_ptr,"  %f",*fptr1++);
      }
      fprintf(rep_ptr," %d ",*rptr1);

      if(ncons != 0)
      {
        for(j = 0;j < ncons;j++)
        {
          fprintf(rep_ptr,"  %.2e",*cons_ptr2);
          fprintf(gen_ptr,"  %.2e",*cons_ptr2++);
        }
        fprintf(rep_ptr," %.2e",pop2_ptr->ind_ptr->getConstrViolation());
        fprintf(gen_ptr," %.2e",pop2_ptr->ind_ptr->getConstrViolation());
      }

      fprintf(rep_ptr," %f ",pop2_ptr->ind_ptr->getCrowdingDistance());

      pop1_ptr->ind_ptr = &(pop1_ptr->ind[i+1]);
      pop2_ptr->ind_ptr = &(pop2_ptr->ind[i+1]);

      fprintf(gen_ptr,"\n");
    }

    fprintf(rep_ptr,"\n--------------------------------------------------\n\n");
    fprintf(rep_ptr,"-------------------------------------------------------\n");
    fprintf(gen_ptr,"\n--------------------------------------------------\n\n");
    return;
  }

  void NSGA2::nselect(population *old_pop_ptr,population *pop2_ptr)
  {
    int *fit_ptr1,*fit_ptr2;

    double f1_ptr, f2_ptr;

    int *s1_ptr,*s2_ptr,*select_ptr;
    double *select_ptr_r, *s1_ptr_r, *s2_ptr_r;

    NSGA2Individual *j,*j1;

    int i,rnd,rnd1,k,n,j2,r,s;

    old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[0]);

    pop2_ptr->ind_ptr= &(pop2_ptr->ind[0]);

    j =  &(old_pop_ptr->ind[popsize-1]);

    old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[0]);
    j2 = 0;
    r = popsize;
    s = chrom;

    for(n = 0,k = 0;n < popsize;n++,k++)
    {
      pop2_ptr->ind_ptr = &(pop2_ptr->ind[k]);
      select_ptr = &(pop2_ptr->ind_ptr->getIntVar()[0]);
      select_ptr_r = &(pop2_ptr->ind_ptr->getRealVar()[0]);

      rnd = iRandGen->randomInteger(0, popsize);

      if(rnd == 0)
        rnd = popsize - k;

      if(rnd == popsize)
        rnd = (popsize-2)/2;

      /*Select first parent randomly*/
      j = &(old_pop_ptr->ind[rnd-1]);

      rnd1 = iRandGen->randomInteger(0, popsize);

      if (rnd1 == 0)
        rnd1 = popsize - n;

      if(rnd1 == popsize)
        rnd1 = (popsize - 4)/2;


      /*Select second parent randomly*/
      j1 = &(old_pop_ptr->ind[rnd1-1]);

      old_pop_ptr->ind_ptr = j;

      s1_ptr = &(old_pop_ptr->ind_ptr->getIntVar()[0]);
      s1_ptr_r = &(old_pop_ptr->ind_ptr->getRealVar()[0]);
      fit_ptr1 = &(old_pop_ptr->ind_ptr->rank);
      f1_ptr = (old_pop_ptr->ind_ptr->getCrowdingDistance());

      old_pop_ptr->ind_ptr = j1;
      s2_ptr = &(old_pop_ptr->ind_ptr->getIntVar()[0]);
      s2_ptr_r = &(old_pop_ptr->ind_ptr->getRealVar()[0]);
      fit_ptr2 = &(old_pop_ptr->ind_ptr->rank);
      f2_ptr = (old_pop_ptr->ind_ptr->getCrowdingDistance());
      /*--------------------------------------------------------------------------*/

      /*------------------SELECTION PROCEDURE------------------------------------*/

      /*Comparing the fitnesses*/

      if(fit_ptr1 > fit_ptr2)
      {
        for(i = 0;i < chrom;i++)
          *select_ptr++=*s2_ptr++;
        for(i = 0;i < nvar;i++)
          *select_ptr_r++=*s2_ptr_r++;
      }
      else
      {
        if(fit_ptr1 < fit_ptr2)
        {
          for(i = 0;i < chrom;i++)
            *select_ptr++=*s1_ptr++;
          for(i = 0;i < nvar;i++)
            *select_ptr_r++=*s1_ptr_r++;
        }
        else
        {
          if(f1_ptr < f2_ptr)
          {
            for(i = 0;i < chrom;i++)
              *select_ptr++=*s2_ptr++;
            for(i = 0;i < nvar;i++)
              *select_ptr_r++=*s2_ptr_r++;
          }
          else
          {
            for(i = 0;i < chrom;i++)
              *select_ptr++=*s1_ptr++;
            for(i = 0;i < nvar;i++)
              *select_ptr_r++=*s1_ptr_r++;
          }
        }
      }
    }
    return;
  }

