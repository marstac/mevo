//---------------------------------------------------------------------------

#ifndef NSGA2H
#define NSGA2H
//---------------------------------------------------------------------------

#include <stdio.h>

#include <stdio.h>
#include <math.h>

#include "../../core/CGeneticAlgorithm.h"
#include "../../core/CIndividual.h"
#include "../../core/CPopulation.h"
/** Nondominated Sorting Genetic Algorithm II namespace: has to be used with this algorithm*/
  class NSGA2Globpop
  {
    public:
      NSGA2Globpop();
      NSGA2Globpop(const NSGA2Globpop& src);
      ~NSGA2Globpop();
      int maxrank,   /*Max rank of the global population*/
      **rankar, /*record of array of NSGA2Individual numbers at
				  a particular rank */
      *rankno;           /*record of no. of individuals at a particular rank*/

      int **genes,
      *rank,            /*rank of different individuals*/
      *flag;            /*Setting the flag */

      double **fitness, /*Fitness function values for the different
			 individuals*/
      *cub_len,              /*Dummyfitness*/
      **xreal,       /*value of the decoded variables for different individuals */

      *error,               /*Error Values of the individuals*/
      **constr;
      int **xbin;   /* binray-coded variables */
      void init();
  };

  class NSGA2Individual : public MEVO::CIndividual     /*NSGA2Individual properties*/
  {
    public:
      NSGA2Individual();
      NSGA2Individual(const NSGA2Individual& src);
      virtual ~NSGA2Individual();
      virtual ICIndividual* copy();
      int rank,          /*Rank of the NSGA2Individual*/
      flag;              /*Flag for ranking*/
  };        /*Structure defining NSGA2Individual*/

  class population
  {
    public:
      population();
      ~population();
      population(const population& src);
      int maxrank;            /*Maximum rank present in the population*/
      float *rankrat;  /*Rank Ratio*/
      int *rankno;     /*Individual at different ranks*/
      NSGA2Individual *ind, /*Different Individuals*/
      *ind_ptr;
      void init();
  };
/** Nondominated Sorting Genetic Algorithm II */
  class NSGA2 : virtual public MEVO::CGeneticAlgorithm {
  public:

    void run();
    const char* getName() { return "NSGA2";}
    public:
      /** Nondominated Sorting Genetic Algorithm II constructor*/
	  NSGA2();
      ~NSGA2();
      void initVariables();
      bool checkConf();
    private:
      NSGA2Globpop globalpop,*global_pop_ptr;
      int left,Lastrank;
      float **fpara1;
      int nvar,nchrom,          /*No of variables*/
      ncons,         /*No of Constraints*/
      *vlen,  /*Array to store no of bits for each variable*/
      nmut,          /* No of Mutations */
      ncross,        /*No of crossovers*/
      ans;

      float **lim_b, **lim_r;/*Limits of variable in array*/
      float delta_fit,     /* variables required forfitness for fitness sharing */
      min_fit,
      front_ratio;
      int nfunc,         /*No of functions*/
      sharespace;    /*Sharing space (either parameter or fitness)*/
      double *coef; /*Variable used for decoding*/

      int popsize,  /*Population Size*/
      chrom;             /*Chromosome size*/

      population oldpop,
      newpop, matepop,
      *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;



      int maxpop;
      int maxchrom;
      int maxvar;
      int maxfun;
      int maxcons;

      void func(population *pop_ptr);
      void input();
      void keepalive(population *pop1_ptr,population *pop2_ptr,population *pop3_ptr,int gen);

      void grankc(int gen);

      void gsort(int rnk,int sel);
      void gshare(int rnk);
      void sort(int rnk);
     void rankcon(population *pop_ptr);

      void advance_random();

      void report(int t,population *pop1_ptr, population *pop2_ptr,FILE *rep_ptr,FILE *gen_ptr, FILE *lastit);
      /*void roulette(population *old_pop_ptr,population *mate_pop_ptr);*/
      void nselect(population *old_pop_ptr,population *pop2_ptr);
      void unicross(population *new_pop_ptr,population *mate_pop_ptr);

      MEVO::CPopulation FPop;

  };
  extern NSGA2* gNSGA2;
#endif
