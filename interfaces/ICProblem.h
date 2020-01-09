#ifndef ICPROBLEM_H
#define ICPROBLEM_H

//! Interface class for description of a multiobjective optimization problem
/** Provides functions for implementation of description of a multiobjective optimization problem.
  */

#include "ICFunction.h"

namespace MEVO {
class CParams;

class ICProblem {

public:
    /** default destructor
    */
    virtual ~ICProblem(){};

    /** Virtual copy function. It should return the exact copy of the ICProblem object. It has to be defined!
    */
    virtual ICProblem* copy()=0;

    /** Description of selected algorithm
    @return brief description of an optimization problem
     */
    virtual const char *getDescription()=0;

    /** This Function adds an objective function used in optimization
      @param _f pointer to the objective function object
      */
    virtual void addObjectiveFnc(ICFunction* _f)=0;

    /** This Function gives the number of stored objective functions
      @return number of objective functions
      */
    virtual unsigned int getObjectiveFncCnt()=0;

    /** Function returns a pointer to ith objective function
      @param _i index of an objective function
      @return pointer to ith objective function
      */
    virtual ICFunction* getObjectiveFnc(unsigned int _i)=0;

    /** This Function adds an constraint function used in optimization
    @param _f pointer to the constraint function object
    */
    virtual void addConstraintFnc(ICFunction* _f)=0;

    /** This Function gives the number of stored constraint functions
    @return number of constraint functions
    */
    virtual unsigned int getConstraintFncCnt()=0;

    /** Function returns a pointer to ith constraint function
      @param _i index of a constraint function
      @return pointer to ith constraint function
      */
    virtual ICFunction* getConstraintFnc(unsigned int _i)=0;

    /** This Function sets a number of decision variables vector
    @param _v a number of decision variables vector

    */
    virtual void setRealVarSize(unsigned int _v)=0;

    /** This Function returns a size of a decision vector variables
    @return a number of decision variables vector
    */
    virtual unsigned int getRealVarSize()=0;

    /** This Function sets a number of binary-coded decision variables vector
    @param _v a number of decision variables vector
    */
    virtual void setIntegerVarSize(unsigned int _v)=0;

    /** This Function returns a size of a binary-coded decision vector variables
    @return a number of decision variables vector
    */
    virtual unsigned int getIntegerVarSize()=0;

    /**  Sets upper limits of the i-th real-coded variable
    @param _v limit value
    @param _i variable index
    */
    virtual void setRealHighLimit(double _v, unsigned int _i)=0;

    /** Returns upper limits of the i-th real-coded variable
    @param _i variable index
    @return limit value
    */
    virtual double getRealHighLimit(unsigned int _i)=0;

    /**  Sets lower limits of the i-th real-coded variable
    @param _v limit value
    @param _i variable index
    */
    virtual void setRealLowLimit(double _v, unsigned int _i)=0;

    /** Returns lower limits of the i-th real-coded variable
    @param _i variable index
    @return limit value
    */
    virtual double getRealLowLimit(unsigned int _i)=0;

    /**  Sets upper limits of the i-th binary-coded variable
    @param _v limit value
    @param _i variable index
    */
    virtual void setIntegerHighLimit(int _v, unsigned int _i)=0;

    /** Returns upper limits of the i-th binary-coded variable
    @param _i variable index
    @return limit value
    */
    virtual double getIntegerHighLimit(unsigned int _i)=0;

    /**  Sets lower limits of the i-th binary-coded variable
    @param _v limit value
    @param _i variable index
    */
    virtual void setIntegerLowLimit(int _v, unsigned int _i)=0;

    /** Returns lower limits of the i-th binary-coded variable
    @param _i variable index
    @return limit value
    */
    virtual double getIntegerLowLimit(unsigned int _i)=0;

    virtual void update(double _v)=0;

    virtual CParams* getVariables() const=0;

    virtual CParams* getParameters() const=0;


    virtual void setVariables(CParams *_p)=0;

    virtual void setParameters(CParams *_p)=0;

    virtual double getObjectiveFncValue(unsigned int _i)=0;
    virtual double getConstraintFncValue(unsigned int _i)=0;
};
}



#endif // ICPROBLEM_H
