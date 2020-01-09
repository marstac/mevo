#include "CProblem.h"
namespace MEVO{
CProblem::CProblem(){
    FRealVarSize=0;
    FIntegerVarSize=0;
    FCurrent=0;
    FParameters=new CParams();
    FVariables = new CParams();
}

CProblem::~CProblem()
{
    for (int i=0;i<FObjectiveFncs.size();i++) {
        delete FObjectiveFncs[i];
    }
    for (int i=0;i<FConstraintFncs.size();i++) {
        delete FConstraintFncs[i];
    }
    FObjectiveFncs.clear();
    FConstraintFncs.clear();

    FRealHighLimits.clear();
    FRealLowLimits.clear();
    FIntegerHighLimits.clear();
    FIntegerLowLimits.clear();
    FObjValues.clear();
    FConstrValues.clear();

    delete FParameters;
    delete FVariables;
}

ICProblem* CProblem::copy()
{
    CProblem* p = new CProblem();
    p->copyParams(this);
    return p;
}

const char *CProblem::getDescription()
{
    return "Multi-objective optimization problem";
}

void CProblem::addConstraintFnc(ICFunction* _f) {
    _f->setProblem(this);
    FConstraintFncs.push_back(_f);
}

unsigned int CProblem::getConstraintFncCnt() {
  return FConstraintFncs.size();
}


ICFunction* CProblem::getObjectiveFnc(unsigned int _i){
    if (_i < getObjectiveFncCnt()) {
        return FObjectiveFncs[_i];
    }
    return NULL;
}


ICFunction* CProblem::getConstraintFnc(unsigned int _i) {
    if (_i < getConstraintFncCnt()) {
        return FConstraintFncs[_i];
    }
    return NULL;
}

void CProblem::setRealVarSize(unsigned int _v) {

  FRealVarSize = _v;
  FRealHighLimits.resize(FRealVarSize, std::numeric_limits<double>::max());
  FRealLowLimits.resize(FRealVarSize, -1*std::numeric_limits<double>::max());
}

unsigned int CProblem::getRealVarSize() {
  return FRealVarSize;
}

void CProblem::setIntegerVarSize(unsigned int _v) {

  FIntegerVarSize = _v;
  FIntegerHighLimits.resize(FIntegerVarSize, std::numeric_limits<int>::max());
  FIntegerLowLimits.resize(FIntegerVarSize, -1*std::numeric_limits<int>::max());
}

unsigned int CProblem::getIntegerVarSize() {
  return FIntegerVarSize;
}

void CProblem::setRealHighLimit(double _v, unsigned int _i) {

  assert(_i < FRealHighLimits.size());
  FRealHighLimits[_i]=_v;
}

double CProblem::getRealHighLimit(unsigned int _i) {
  assert(_i < FRealHighLimits.size());
  return FRealHighLimits[_i];
}

void CProblem::setRealLowLimit(double _v, unsigned int _i) {

  assert(_i < FRealLowLimits.size());
  FRealLowLimits[_i]=_v;
}

double CProblem::getRealLowLimit(unsigned int _i){
  assert(_i < FRealLowLimits.size());
  return FRealLowLimits[_i];
}

void CProblem::setIntegerHighLimit(int _v, unsigned int _i) {

  assert(_i < FIntegerHighLimits.size());
  FIntegerHighLimits[_i]=_v;
}

double CProblem::getIntegerHighLimit(unsigned int _i) {
  assert(_i < FIntegerHighLimits.size());
  return FIntegerHighLimits[_i];
}

void CProblem::setIntegerLowLimit(int _v, unsigned int _i) {

  assert(_i < FIntegerLowLimits.size());
  FIntegerLowLimits[_i]=_v;
}

double CProblem::getIntegerLowLimit(unsigned int _i){
  assert(_i < FIntegerLowLimits.size());
  return FIntegerLowLimits[_i];
}

void CProblem::update(double _v)
{
    FCurrent++;
    if (FCurrent<=FObjectiveFncs.size()) {
        FObjValues.push_back(_v);
    } else if(FCurrent<=FObjectiveFncs.size()+FConstraintFncs.size()) {
        FConstrValues.push_back(_v);
    } else {
        FObjValues.clear();
        FConstrValues.clear();
        FCurrent=0;
        update(_v);
    }
}

CParams *CProblem::getVariables() const
{
    return FVariables;
}

CParams *CProblem::getParameters() const
{
    return FParameters;
}

void CProblem::copyParams(ICProblem *_p)
{
    if (this->getObjectiveFncCnt()!=_p->getObjectiveFncCnt()) {
        for (int i=0;i<FObjectiveFncs.size();i++) {
            delete FObjectiveFncs[i];
        }
        FObjectiveFncs.clear();
        for(int i=0; i<_p->getObjectiveFncCnt();i++) {
            addObjectiveFnc(_p->getObjectiveFnc(i)->copy());
        }
    }
    if (this->getConstraintFncCnt()!=_p->getConstraintFncCnt()){
        for (int i=0;i<FConstraintFncs.size();i++) {
             delete FConstraintFncs[i];
        }
        FConstraintFncs.clear();
        for(int i=0; i<_p->getConstraintFncCnt();i++) {
            addConstraintFnc(_p->getConstraintFnc(i)->copy());
        }
    }

    setRealVarSize(_p->getRealVarSize());
    for(int i=0; i<FRealVarSize; i++) {
        setRealLowLimit(_p->getRealLowLimit(i),i);
        setRealHighLimit(_p->getRealHighLimit(i),i);
    }

    setIntegerVarSize(_p->getIntegerVarSize());
    for(int i=0; i<FIntegerVarSize; i++) {
        setIntegerLowLimit(_p->getIntegerLowLimit(i),i);
        setIntegerHighLimit(_p->getIntegerHighLimit(i),i);
    }

    setParameters(_p->getParameters()->copy());
    setVariables(_p->getVariables()->copy());

}

void CProblem::setParameters(CParams *_p)
{
   if(FParameters){
    delete FParameters;
   }
   FParameters = _p;
}

double CProblem::getObjectiveFncValue(unsigned int _i)
{
    assert(_i <FCurrent);
    return FObjValues[_i];
}

double CProblem::getConstraintFncValue(unsigned int _i)
{
    assert(_i + FObjectiveFncs.size() <FCurrent);
    return FConstrValues[_i];
}

void CProblem::setVariables(CParams *_p)
{
   if (FVariables) {
    delete FVariables;
   }
    FVariables = _p;
}

void CProblem::addObjectiveFnc(ICFunction* _f) {
    _f->setProblem(this);
    FObjectiveFncs.push_back(_f);
}

unsigned int CProblem::getObjectiveFncCnt() {
    return FObjectiveFncs.size();
}
//Parameters

CParam::CParam(std::string _name, int _val) {
    FType=wptInt;
    FName = _name;
    std::deque<int> cols(1, _val);
    FValue.iIntMatrix.resize(1, cols);
    setInt(_val);
}

CParam::CParam(std::string _name, double _val) {
    FType=wptReal;
    FName = _name;
    std::deque<double> cols(1, _val);
    FValue.iRealMatrix.resize(1, cols);
    setReal(_val);
}

CParam::CParam(std::string _name, int _r, double _val) {
    FType = wptRealVect;
    FName=_name;
    std::deque<double> cols(1, _val);
    FValue.iRealMatrix.resize(_r, cols);
}

CParam::CParam(std::string _name, int _r, int _c, double _val) {
    FType = wptRealVect;
    FName=_name;
    std::deque<double> cols(_c, _val);
    FValue.iRealMatrix.resize(_r, cols);
}

CParam::CParam(std::string _name, int _r, int _val) {
    FType = wptIntVect;
    FName=_name;
    std::deque<int> cols(1, _val);
    FValue.iIntMatrix.resize(_r, cols);
}

CParam::CParam(std::string _name, int _r, int _c, int _val) {
    FType = wptIntMatrix;
    FName=_name;
    std::deque<int> cols(_c, _val);
    FValue.iIntMatrix.resize(_r, cols);
}

CParam::~CParam()
{
    FValue.iIntMatrix.clear();
    FValue.iRealMatrix.clear();
}

int CParam::getInt() const {
    if (FType != wptInt)
        throw new EParamException(EParamException::epeBadType);
    return FValue.iIntMatrix[0][0];
}

void CParam::setInt(int _val) {
    if (FType != wptInt)
        throw new EParamException(EParamException::epeBadType);

  FValue.iIntMatrix[0][0] = _val;
}

std::deque<int> CParam::getIntVector() {
    if (FType != wptIntVect)
        throw new EParamException(EParamException::epeBadType);
    return FValue.iIntMatrix[0];
}

void CParam::setInt(std::deque<int> _v)
{
    if (FType!=wptIntVect) {
        throw new EParamException(EParamException::epeBadType);
    }
    FValue.iIntMatrix.clear();
    FValue.iIntMatrix.push_back(_v);
}

std::deque<std::deque<int> > CParam::getIntMatrix() {
    if (FType!=wptIntMatrix) {
        throw new EParamException(EParamException::epeBadType);
    }
    return FValue.iIntMatrix;
}

void CParam::setInt(std::deque<std::deque<int> > _m) {
    if (FType!=wptIntMatrix) {
        throw new EParamException(EParamException::epeBadType);
    }
    FValue.iIntMatrix = _m;
}

std::deque<double> CParam::getRealVector() {
    if (FType != wptRealVect)
        throw new EParamException(EParamException::epeBadType);
    return FValue.iRealMatrix[0];
}

void CParam::setReal(std::deque<double> _v)
{
    if (FType!=wptRealVect) {
        throw new EParamException(EParamException::epeBadType);
    }
    FValue.iRealMatrix.clear();
    FValue.iRealMatrix.push_back(_v);
}

std::deque<std::deque<double> > CParam::getRealMatrix() {
    if (FType!=wptRealMatrix) {
        throw new EParamException(EParamException::epeBadType);
    }
    return FValue.iRealMatrix;
}

void CParam::setReal(std::deque<std::deque<double> > _m) {
    if (FType!=wptRealMatrix) {
        throw new EParamException(EParamException::epeBadType);
    }
    FValue.iRealMatrix = _m;
}

double CParam::getReal() const
{
  if (FType != wptReal)
    throw new EParamException(EParamException::epeBadType);
  return FValue.iRealMatrix[0][0];
}

void CParam::setReal(double _val)
{
    if (FType != wptReal)
        throw new EParamException(EParamException::epeBadType);
    FValue.iRealMatrix[0][0] = _val;
}


CParam* CParam::copy() const
{
    CParam* rval = new CParam(*this);
    return rval;
}

std::string CParam::getName() const
{
    return FName;
}

EnPropertyType CParam::getType() const
{
    return FType;
}

CParams::CParams()
{

}

CParams::~CParams()
{
    clear();
}

CParams *CParams::copy()
{
    CParams* p = new CParams();
    for(int i=0; i<FParams.size();i++) {
        p->FParams.push_back(FParams[i]->copy());
    }
    return p;
}

void CParams::clear()
{
    for(int i=0;i<FParams.size();i++){
        delete FParams[i];
    }
    FParams.clear();
}

void CParams::add(std::string _name, int _val)
{
    FParams.push_back(new CParam(_name, _val));
}

void CParams::add(std::string _name, double _val)
{
    FParams.push_back(new CParam(_name, _val));
}

void CParams::add(std::string _name, int _r, double _val)
{
    FParams.push_back(new CParam(_name, _r, _val));
}

void CParams::add(std::string _name, int _r, int _c, double _val)
{
    FParams.push_back(new CParam(_name, _r, _c, _val));
}

void CParams::add(std::string _name, int _r, int _val)
{
    FParams.push_back(new CParam(_name, _r, _val));
}

void CParams::add(std::string _name, int _r, int _c, int _val)
{
    FParams.push_back(new CParam(_name, _r, _c, _val));
}

CParam* CParams::getParam(std::string _name) const
{
    for(int i=0;i<FParams.size();i++){
       if(FParams[i]->getName() == _name) {
           return FParams[i];
       }
    }
    throw new EParamException(EParamException::epeParamNotExists);
}

int CParams::getInt(std::string _name) const
{
    CParam* p = getParam(_name);
    if(p->getType()==wptInt) {
        return p->getInt();
    }
    throw new EParamException(EParamException::epeBadType);
}

void CParams::setInt(std::string _name, int _val)
{
    CParam* p = getParam(_name);
    if(p->getType()==wptInt) {
        p->setInt(_val);
        return;
    }
    throw new EParamException(EParamException::epeBadType);
}

std::deque<int> CParams::getIntVector(std::string _name)
{
    CParam* p = getParam(_name);
    if(p->getType()==wptIntVect) {
        return p->getIntVector();
    }
    throw new EParamException(EParamException::epeBadType);
}

void CParams::setInt(std::string _name, std::deque<int> _v)
{
    CParam* p = getParam(_name);
    if(p->getType()==wptIntVect) {
        p->setInt(_v);
        return;
    }
    throw new EParamException(EParamException::epeBadType);
}

std::deque<std::deque<int> > CParams::getIntMatrix(std::string _name)
{
    CParam* p = getParam(_name);
    if(p->getType()==wptIntMatrix) {
        return p->getIntMatrix();
    }
    throw new EParamException(EParamException::epeBadType);
}

void CParams::setInt(std::string _name, std::deque<std::deque<int> > _m)
{
    CParam* p = getParam(_name);
    if(p->getType()==wptIntMatrix) {
        p->setInt(_m);
        return;
    }
    throw new EParamException(EParamException::epeBadType);
}

double CParams::getReal(std::string _name) const
{
    CParam* p = getParam(_name);
    if(p->getType()==wptReal) {
        return p->getReal();
    }
    throw new EParamException(EParamException::epeBadType);
}

void CParams::setReal(std::string _name, double _val)
{
    CParam* p = getParam(_name);
    if(p->getType()==wptReal) {
        p->setReal(_val);
        return;
    }
    throw new EParamException(EParamException::epeBadType);
}

std::deque<double> CParams::getRealVector(std::string _name)
{
    CParam* p = getParam(_name);
    if(p->getType()==wptRealVect) {
        return p->getRealVector();
    }
    throw new EParamException(EParamException::epeBadType);
}

void CParams::setReal(std::string _name, std::deque<double> _v)
{
    CParam* p = getParam(_name);
    if(p->getType()==wptRealVect) {
        p->setReal(_v);
        return;
    }
    throw new EParamException(EParamException::epeBadType);
}

std::deque<std::deque<double> > CParams::getRealMatrix(std::string _name)
{
    CParam* p = getParam(_name);
    if(p->getType()==wptRealMatrix) {
        return p->getRealMatrix();
    }
    throw new EParamException(EParamException::epeBadType);
}

void CParams::setReal(std::string _name, std::deque<std::deque<double> > _m)
{
    CParam* p = getParam(_name);
    if(p->getType()==wptRealMatrix) {
        p->setReal(_m);
        return;
    }
    throw new EParamException(EParamException::epeBadType);
}




}
