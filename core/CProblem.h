#ifndef CPROBLEM_H
#define CPROBLEM_H

#include "../interfaces/ICProblem.h"

#include <cstdlib>
#include <deque>
#include <vector>
#include <limits>
#include <assert.h>
#include <string>

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


namespace MEVO {

class CParams;

class CProblem : public ICProblem
{

public:
    /** Constructor */
    CProblem();

    /** Destructor */
    ~CProblem();

    /** Virtual copy function. It should return the exact copy of the ICProblem object. It has to be defined!
    */
    virtual ICProblem* copy();

    /** Description of selected algorithm
    @return brief description of an optimization problem
     */
    virtual const char *getDescription();

    /** This Function adds an objective function used in optimization
      @param _f pointer to the objective function object
      */
    virtual void addObjectiveFnc(ICFunction* _f);

    /** This Function gives the number of stored objective functions
      @return number of objective functions
      */
    virtual unsigned int getObjectiveFncCnt();

    /** Function returns a pointer to ith objective function
      @param _i index of an objective function
      @return pointer to ith objective function
      */
    virtual ICFunction* getObjectiveFnc(unsigned int _i);

    /** This Function adds an constraint function used in optimization
    @param _f pointer to the constraint function object
    */
    virtual void addConstraintFnc(ICFunction* _f);

    /** This Function gives the number of stored constraint functions
    @return number of constraint functions
    */
    virtual unsigned int getConstraintFncCnt();


    /** Function returns a pointer to ith constraint function
      @param _i index of a constraint function
      @return pointer to ith constraint function
      */
    virtual ICFunction* getConstraintFnc(unsigned int _i);

    /** This Function sets a number of decision variables vector
    @param _v a number of decision variables vector

    */
    virtual void setRealVarSize(unsigned int _v);

    /** This Function returns a size of a decision vector variables
    @return a number of decision variables vector
    */
    virtual unsigned int getRealVarSize();

    /** This Function sets a number of binary-coded decision variables vector
    @param _v a number of decision variables vector
    */
    virtual void setIntegerVarSize(unsigned int _v);

    /** This Function returns a size of a binary-coded decision vector variables
    @return a number of decision variables vector
    */
    virtual unsigned int getIntegerVarSize();

    /**  Sets upper limits of the i-th real-coded variable
    @param _v limit value
    @param _i variable index
    */
    virtual void setRealHighLimit(double _v, unsigned int _i);

    /** Returns upper limits of the i-th real-coded variable
    @param _i variable index
    @return limit value
    */
    virtual double getRealHighLimit(unsigned int _i);

    /**  Sets lower limits of the i-th real-coded variable
    @param _v limit value
    @param _i variable index
    */
    virtual void setRealLowLimit(double _v, unsigned int _i);

    /** Returns lower limits of the i-th real-coded variable
    @param _i variable index
    @return limit value
    */
    virtual double getRealLowLimit(unsigned int _i);

    /**  Sets upper limits of the i-th binary-coded variable
    @param _v limit value
    @param _i variable index
    */
    virtual void setIntegerHighLimit(int _v, unsigned int _i);

    /** Returns upper limits of the i-th binary-coded variable
    @param _i variable index
    @return limit value
    */
    virtual double getIntegerHighLimit(unsigned int _i);

    /**  Sets lower limits of the i-th binary-coded variable
    @param _v limit value
    @param _i variable index
    */
    virtual void setIntegerLowLimit(int _v, unsigned int _i);

    /** Returns lower limits of the i-th binary-coded variable
    @param _i variable index
    @return limit value
    */
    virtual double getIntegerLowLimit(unsigned int _i);

    virtual void update(double _v);

    virtual CParams* getVariables() const;

    virtual CParams* getParameters() const;

    virtual void setVariables(CParams *_p);

    virtual void setParameters(CParams *_p);

    virtual double getObjectiveFncValue(unsigned int _i);

    virtual double getConstraintFncValue(unsigned int _i);

protected:

    /** function copies parameters from selected crossover object
        @param _c crossover object
    */
    virtual void copyParams(ICProblem* _p);

    /** Deque of pointers to the constraints functions*/
    std::deque<ICFunction*> FConstraintFncs;

    /** Deque of pointers to the objective functions*/
    std::deque<ICFunction*> FObjectiveFncs;

    /** Real coded decision variables size*/
    unsigned int FRealVarSize;

    /** Bianry coded decision variables size*/
    unsigned int FIntegerVarSize;

    /** Upper limits of the real-coded variables*/
    std::vector<double> FRealHighLimits;
    /** Lower limits of the real-coded variables*/
    std::vector<double> FRealLowLimits;

    /** Upper limits of the binary-coded variables*/
    std::vector<int> FIntegerHighLimits;
    /** Lower limits of the binary-coded variables*/
    std::vector<int> FIntegerLowLimits;

    /** values of calculated objectve functions */
    std::vector<double> FObjValues;

    /** values of calculated constraint functions */
    std::vector<double> FConstrValues;


    CParams* FVariables;

    CParams* FParameters;

    unsigned int FCurrent;
};


enum EnPropertyType {wptInt,
                     wptReal,
                     wptRealMatrix,
                     wptRealVect,
                     wptIntMatrix,
                     wptIntVect
                     };


class CParam {
public:
    CParam(std::string _name, int _val);
    CParam(std::string _name, double _val);

    CParam(std::string _name, int _r, double _val);
    CParam(std::string _name, int _r, int _c, double _val);

    CParam(std::string _name, int _r, int _val);
    CParam(std::string _name, int _r, int _c, int _val);

    CParam(const CParam& src) {
        FType = src.FType;
        FName = src.FName;
        FValue = src.FValue;
    }

   virtual ~CParam();

   virtual int getInt() const;
   virtual void setInt(int _val);

   virtual std::deque<int> getIntVector();
   virtual void setInt(std::deque<int> _v);

   virtual std::deque<std::deque<int> > getIntMatrix();
   virtual void setInt(std::deque<std::deque<int> > _m);

   virtual double getReal() const;
   virtual void setReal(double _val);

   virtual std::deque<double> getRealVector();
   virtual void setReal(std::deque<double> _v);

   virtual std::deque<std::deque<double> > getRealMatrix();
   virtual void setReal(std::deque<std::deque<double> > _m);

   virtual CParam* copy() const;

   virtual std::string getName() const;

   virtual EnPropertyType getType() const;

protected:
    EnPropertyType FType;
    struct SVal {
        std::deque<std::deque<int> >iIntMatrix;
        std::deque<std::deque<double> >iRealMatrix;
    } FValue;
    std::string FName;
};

class EParamException
{
public:
    enum EnExType {epeParamNotExists, epeBadType};
    EParamException(EnExType _type) {Type = _type; }

    EnExType Type;
};


class CParams {
public:
    CParams();
    ~CParams();

    virtual CParams* copy();

    void clear();


    void add(std::string _name, int _val);
    void add(std::string _name, double _val);

    void add(std::string _name, int _r, double _val);
    void add(std::string _name, int _r, int _c, double _val);

    void add(std::string _name, int _r, int _val);
    void add(std::string _name, int _r, int _c, int _val);

    CParam* getParam(std::string _name) const;

    virtual int getInt(std::string _name) const;
    virtual void setInt(std::string _name, int _val);

    virtual std::deque<int> getIntVector(std::string _name);
    virtual void setInt(std::string _name, std::deque<int> _v);

    virtual std::deque<std::deque<int> > getIntMatrix(std::string _name);
    virtual void setInt(std::string _name, std::deque<std::deque<int> > _m);

    virtual double getReal(std::string _name) const;
    virtual void setReal(std::string _name, double _val);

    virtual std::deque<double> getRealVector(std::string _name);
    virtual void setReal(std::string _name, std::deque<double> _v);

    virtual std::deque<std::deque<double> > getRealMatrix(std::string _name);
    virtual void setReal(std::string _name, std::deque<std::deque<double> > _m);

protected:
    std::deque<CParam*> FParams;
};

}
#endif // CPROBLEM_H
