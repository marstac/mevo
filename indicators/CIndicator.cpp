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


#include "CIndicator.h"
namespace MEVO {
CIndicator::CIndicator()
{

}

CIndicator::~CIndicator()
{

}

std::deque<double> CIndicator::getFrontMins(std::deque<std::deque<double> > &_rf) {
    std::deque<double> mins;
    mins.resize(_rf[0].size(),std::numeric_limits<double>::max());
    for (int i =0; i<_rf.size();i++) {
        for (int j=0;j<_rf[i].size();j++){
            if (_rf[i][j] < mins[j]) {
                mins[j] = _rf[i][j];
            }
        }
    }
    return mins;
}

std::deque<double> CIndicator::getFrontMaxs(std::deque<std::deque<double> > &_rf) {
    std::deque<double> maxs;
    maxs.resize(_rf[0].size(),std::numeric_limits<double>::min());
    for (int i =0; i<_rf.size();i++) {
        for (int j=0;j<_rf[i].size();j++){
            if (_rf[i][j] > maxs[j]) {
                maxs[j] = _rf[i][j];
            }
        }
    }
    return maxs;
}
std::deque<std::deque<double> > CIndicator::normalizedFront(std::deque<std::deque<double> > &_rf) {
    std::deque<std::deque<double> > nf;
    std::deque<double> min = getFrontMins(_rf);
    std::deque<double> max = getFrontMaxs(_rf);
    for (int i =0; i<_rf.size();i++) {
        std::deque<double> nfi;
        for (int j=0;j<_rf[i].size();j++){
            nfi.push_back((_rf[i][j]-min[j])/(max[j]-min[j]));
        }
        nf.push_back(nfi);
    }
    return nf;
}

void CIndicator::invertFront(std::deque<std::deque<double> > &_rf){

    for (int i =0; i<_rf.size();i++) {
        for (int j=0;j<_rf[i].size();j++){
            _rf[i][j] = -1*_rf[i][j];
        }
    }
}
}
