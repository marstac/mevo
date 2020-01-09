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

#include "CConsoleNotifier.h"

namespace MEVO {

CConsoleNotifier::CConsoleNotifier()
{
    FFrequency=10;
}
CConsoleNotifier::~CConsoleNotifier() {

}

void CConsoleNotifier::onBeforeStart(const char* _name, ICEvolOptimizer *_opt) {
    std::cout << initialReport(_opt);
    startTimer();
}

void CConsoleNotifier::onAfterStop(const char*_msg, ICPopulation* _fpop) {
    double time = stopTimer();
    std::cout <<'\n'<<_msg<<" done.\n"<<"Computation time: "<<time; 
}

void CConsoleNotifier::onError(const char* _error) {
    std::cout <<'\n'<<_error<<'\n';
}

void CConsoleNotifier::onGenerationDone(unsigned int _g, ICPopulation* _pop) {
    if(_g%FFrequency==0) {
        std::cout<<"\nGeneration no. "<<_g << " done";
    }
}

unsigned int CConsoleNotifier::getFFrequency() const
{
    return FFrequency;
}

void CConsoleNotifier::setFFrequency(unsigned int value)
{
    FFrequency = value;
}

}
