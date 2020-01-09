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


#include "CFileNotifier.h"

namespace MEVO {

CFileNotifier::CFileNotifier()
{
    FFileName="";
    clear=false;
}
CFileNotifier::~CFileNotifier() {
    if(FFile.is_open()) {
        FFile.close();
    }
}

void CFileNotifier::onBeforeStart(const char* _name, ICEvolOptimizer *_opt) {
    if (FFileName==""){
        FFileName=_opt->getName();
        FFileName+="_";
        FFileName+=_opt->getProblem()->getDescription();
        FFileName+="_";
        FFileName+=currentDateTime();
        FFileName+=".txt";
        clear=true;
    }
    FFile.open(FFileName.c_str());
    FFile << initialReport(_opt);
    startTimer();
}

void CFileNotifier::onAfterStop(const char*_msg, ICPopulation* _fpop) {
    double time = stopTimer();
    FFile <<'\n'<<_msg<<" done.\n"<<"Computation time: "<<time;
    FFile.close();
    std::string name = "result_";
    name+=FFileName;
    _fpop->save(name);
    if(clear) {FFileName="";}
}

void CFileNotifier::onError(const char* _error) {
    FFile <<'\n'<<_error<<'\n';
}

void CFileNotifier::onGenerationDone(unsigned int _g, ICPopulation* _pop) {
    if(_g%100==0) {
        FFile<<"\nGeneration no. "<<_g << " done";
    }
}

std::string CFileNotifier::getFileName() const
{
    return FFileName;
}

void CFileNotifier::setFileName(const std::string _name)
{
    FFileName = _name;
    clear=false;
}

const std::string CFileNotifier::currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    sprintf(buf, "%i_%i_%i_%i_%i_%i", tstruct.tm_year-100, tstruct.tm_mon, tstruct.tm_mday, tstruct.tm_hour, tstruct.tm_min, tstruct.tm_sec);
    return buf;
}

}
