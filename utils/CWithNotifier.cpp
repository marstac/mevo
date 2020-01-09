#include "CWithNotifier.h"
#include "messages.h"
namespace MEVO {
CWithNotifiers::CWithNotifiers()
{

}
CWithNotifiers::~CWithNotifiers()
{
    clearNotifiers();
}

void CWithNotifiers::addNotifier(ICNotifier* _notifier) {
    FNotifiers.push_back(_notifier);
}

void CWithNotifiers::clearNotifiers()
{
    for (int i=0; i<FNotifiers.size(); i++)
        delete FNotifiers[i];
    FNotifiers.clear();
}

ICNotifier* CWithNotifiers::getNotifier(unsigned int _i) {
  if (_i>=0 && _i < FNotifiers.size())
    return FNotifiers[_i];
  return 0;
}

void CWithNotifiers::fireOnBeforeStartNotify(const char* _name, ICEvolOptimizer* _opt) {
  for (int i=0; i<FNotifiers.size(); i++)
    FNotifiers[i]->onBeforeStart(_name, _opt);

}

void CWithNotifiers::fireOnAfterStopNotify(const char *_msg, ICPopulation *_fpop) {
    for (int i=0; i<FNotifiers.size(); i++)
      FNotifiers[i]->onAfterStop(_msg, _fpop);
}

void CWithNotifiers::fireOnErrorNotify(const char* _error) {
    for (int i=0; i<FNotifiers.size(); i++)
      FNotifiers[i]->onError(_error);
}

void CWithNotifiers::fireOnGenerationDoneNotify(unsigned int _g, ICPopulation *_pop) {
    for (int i=0; i<FNotifiers.size(); i++)
      FNotifiers[i]->onGenerationDone(_g, _pop);
}
};
