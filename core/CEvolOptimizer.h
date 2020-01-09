//---------------------------------------------------------------------------

#ifndef CEvolOptimizerH
#define CEvolOptimizerH

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

#include "../interfaces/ICProblem.h"
#include "../interfaces/ICEvolOptimizer.h"
#include "../interfaces/ICRandomNumbersGenerator.h"

#include "../interfaces/ICComputer.h"
#include "../interfaces/ICEvaluator.h"
#include "../interfaces/ICInitializer.h"
#include "../interfaces/ICStop.h"

#include "../utils/CWithNotifier.h"
#include "../utils/CWithRNGen.h"

#include "../utils/functions.h"

namespace MEVO {

//---------------------------------------------------------------------------
class CEvolOptimizer : virtual public ICEvolOptimizer, virtual public CWithNotifiers, virtual public CWithRNGen {
public:

   CEvolOptimizer();

  /** default destructor
  */
  virtual ~CEvolOptimizer();

   /** Function initating fields of this, basic class
  */
  virtual void initVariables();

  /** Sets a number of the individuals in the population
   @param _v number of the individuals in the population
  */
  virtual void setPopSize(unsigned int _v);

  /**
   Returns number of the individuals in the population
   @return number of the individuals in the population
  */
  virtual unsigned int getPopSize();

  /** Returns no. of current generation for which the EA will let the population evolve
  @return number of generations
  */
  virtual unsigned int getCurrentGeneration();

  /** Function starts optimization*/
   virtual void run()=0;

   /** Function check algorithm configuration
  @return true if an algorithm is properly configured
      false if it is not
   */
   virtual bool checkConf();

   /** Function terminates calculations before assumed calculations */
   virtual void terminate();

   /** Function returns termination status
    @return flag value
    */
   virtual bool isTerminated();

   /** Function returns name of an algorithm
     @return algorithm name
    */
   virtual const char* getName() {return "Evolutionary algorithm";}

    /** Function sets last population evaluated
    @param _p last population evaluated by an algorithm
    */
   virtual void setLastPop(ICPopulation *_p);

   /** Function returns last evaluated population
     @return last population of individuals
     */
   virtual ICPopulation* getLastPop();

   /** sets computer object
    @param _c pointer to a computer object
    */
   virtual void setComputer(ICComputer* _c);

   /** gets computer object
    @return pointer to a computer object
    */
   virtual ICComputer* getComputer();

   /** sets evaluator object
    @param _e pointer to an evaluator object
    */
   virtual void setEvaluator(ICEvaluator* _e);

   /** function evaluates a population
     calls evaluator object
    @param _population reference to a population of individuals
    */
   virtual void evaluatePopulation(ICPopulation *_population);

   /** Method for mutation
     @param _population a population that will be mutated

   */
   void mutatePopulation(ICPopulation *_population);

   /** Methods sets a single initializer for both integer and real coded variables. The genotype will be treated as one vector!
     This methods deletes separate initializers for real and integer coded variables!
     @param _i pointer to an initializer object
     */
   virtual void setInitializer(ICInitializer* _i);

   /** A coeficcient for epsilon domination calculations setter
    @param _eps epsilon value
    */
   virtual void setEpsilon(double _eps);

   /** A coeficcient for epsilon domination calculations getter
    @return _eps epsilon value
    */
   virtual double getEpsilon();

   /** Methods sets a separate initializers for integer and real coded variables. The genotype will be treated as two vectors (Real coded and integer coded)!
     This methods deletes  a single initializer for real and integer coded variables!
     This method works only if both arguments do not point to NULL!
     @param pointer to an initializer object
     */
   virtual void setSeparateInitializers(ICInitializer* _ri, ICInitializer* _ii);

   /** Methods sets a single mutator for both integer and real coded variables. The genotype will be treated as one vector!
     This methods deletes separate mutators for real and integer coded variables!
     @param _c pointer to a mutator object
     */
   void setMutator(ICMutator* _m);

   /** Methods sets a separate mutators for integer and real coded variables. The genotype will be treated as two vectors (Real coded and integer coded)!
     This methods deletes a single mutator for real and integer coded variables!
     This method works only if both arguments do not point to NULL!
     @param pointer to a mutator object
     */
   void setSeparateMutators(ICMutator* _rm, ICMutator* _im);


   /** Method returns a pointer to an initializer object for both integer and real coded variables
       @return pointer to mutator object
   */
   virtual ICInitializer* getInitializer();

   /** Method returns a pointer to an initializer object for integer coded variables
       @return pointer to mutator object
   */
   virtual ICInitializer* getIntInitializer();

   /** Method returns a pointer to an initializer object for real coded variables
       @return pointer to mutator object
   */
   virtual ICInitializer* getRealInitializer();

   /** Function sets initial population
    @param _pop pointer to an initial population
    */
   virtual void setInitialPopulation(ICPopulation* _pop);

   /** Function gets initial population
    @return pointer to an initial population
    */
   virtual ICPopulation* getInitialPopulation();

   /** function creates a random initial population
        calls initalizer object
       @param _population reference to a population of individuals
    */
   virtual void initializePopulation(ICPopulation *_population);

   /** Method returns a pointer to a mutator object for both integer and real coded variables
       @return pointer to mutator object
   */
   virtual ICMutator* getMutator();

   /** Method returns a pointer to a mutator object for integer coded variables
       @return pointer to mutator object
   */
   virtual ICMutator* getIntMutator();

   /** Method returns a pointer to a mutator object for real coded variables
       @return pointer to mutator object
   */
   virtual ICMutator* getRealMutator();


   /** Function sets stop condition object
    @param _s pointer to a stop condition object
    */
   virtual void setStop(ICStop* _s);

   /** Function returns stop condition object
    @return pointer to a stop condition object
    */
   virtual ICStop* getStop();

   /** This function provides calculation of stop condition
   @param _population reference to a last population generated by an algorithm
   @return boolean value of stop criterion (TRUE if the algorithm is to be finished)
   */
   virtual bool checkStop(ICPopulation *_population);

   /** Function returns an optimization problem
    @return pointer to an optimization problem
    */
   virtual ICProblem* getProblem() const;

   /** Function sets an optimization problem
    @param pointer to an optimization problem
    */
   virtual void setProblem(ICProblem *_p);

protected:

   /** Pointer to an optimization problem */
   ICProblem* FProblem;

   /** Functions copy individuals from initial population to initialized population */
   void copyInitialPop(ICPopulation* _population);

   /** Number of the individuals in the population*/
   unsigned int FPopSize;

  /** No. of generations for which the GA will let the population evolve
    Default value is 100
    Too large value will take very long time and very small value will
    not let the GA reach the global Pareto front to the problem*/
   unsigned int FGenerations;

   /** This variable stores current generation number
     */
   unsigned int FCurrentGeneration;

   /** Variable used to stop calculations before assumed generations*/
   bool FTerminated;

   /** A coeficcient for epsilon domination calculations */
   double FEps;


   /** Initial Population */
   ICPopulation* FInitialPop;

   /** Last population evaluated */
   ICPopulation* FLastPop;

   /** Population evaluator object */
   ICEvaluator* iEvaluator;

   /** Computer object*/
   ICComputer* iComputer;

   /** Dedicated computer for mutation */
   ICComputer* mComputer;

   /** Pointer to an initializer object for both integer and real coded variables*/
   ICInitializer* iInitializer;

   /** Pointer to an initializer  object for integer coded variables*/
   ICInitializer* iIntInitializer;

   /** Pointer to an initializer  object for real coded variables*/
   ICInitializer* iRealInitializer;

   /** Pointer to a mutator object for both integer and real coded variables*/
   ICMutator* iMutator;

   /** Pointer to a mutator object for integer coded variables*/
   ICMutator* iIntMutator;

   /** Pointer to a mutator object for real coded variables*/
   ICMutator* iRealMutator;


   /** Pointer to a stop criterion object */
   ICStop* iStop;
 };
}
#endif
