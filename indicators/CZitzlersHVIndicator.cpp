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


#include "CZitzlersHVIndicator.h"

namespace MEVO {
CZitzlersHVIndicator::CZitzlersHVIndicator() : CHyperVolumeIndicator()
{
}
CZitzlersHVIndicator::~CZitzlersHVIndicator(){

}

const char *CZitzlersHVIndicator::getName()
{
    return "Hypervolume indicator (Zitzler's code)";
}

double CZitzlersHVIndicator::calculateHypervolume(std::deque< std::deque<double> > &_front) {

    invertFront(_front);

    move(_front);

    return calculateHypervolume(_front, _front.size(), _front[0].size());
}
void CZitzlersHVIndicator::move(std::deque< std::deque<double> > &_front) {
    for (int i =0; i<_front.size();i++) {
        for (int j=0;j<_front[i].size();j++){
            _front[i][j] = FRefPoint[j]+_front[i][j];
            if (_front[i][j]<0) {_front[i][j]=0;}
        }
    }
}

double CZitzlersHVIndicator::calculateHypervolume(std::deque<std::deque<double> > &_front, int _pts, int _obj) {
    int n= _pts;
    double volume=0, distance=0;

    while (n > 0) {

      int noNondominatedPoints;
      double tempVolume, tempDistance;

      noNondominatedPoints = filterNondominatedSet(_front, n, _obj - 1);
      tempVolume = 0;
      if (_obj < 3) {
        if (noNondominatedPoints < 1) {
            return 0;
        }
        tempVolume = _front[0][0];
      } else {
        tempVolume = calculateHypervolume(_front,
                                          noNondominatedPoints,
                                          _obj - 1);
      }

      tempDistance = surfaceUnchangedTo(_front, n, _obj - 1);
      volume += tempVolume * (tempDistance - distance);
      distance = tempDistance;
      n = reduceNondominatedSet(_front, n, _obj - 1, distance);
    }

    return volume;
}

bool CZitzlersHVIndicator::dominates(std::deque<double> _s1, std::deque<double> _s2, int _objNo) {
    bool better = false;
    bool worse = false;

    for (int i = 0; i < _objNo; i++) {
        if (worse) {
            break;
        }

        if (_s1[i] > _s2[i]) {
            better = true;
        } else if (_s1[i] < _s2[i]) {
            worse= true;
        }
    }

    return !worse && better;
}

void CZitzlersHVIndicator::swap(std::deque< std::deque<double> >& _front, int _i, int _j) {
    std::deque<double> temp= _front[_i];
    _front[_i] = _front[_j];
    _front[_j] = temp;
}

int CZitzlersHVIndicator::reduceNondominatedSet(std::deque<std::deque<double> > &_front, int _noPoints, int _obj, double _threshold) {
    int n;
    int i;

    n = _noPoints;
    for (i = 0; i < n; i++) {
      if (_front[i][_obj] <= _threshold) {
        n--;

        swap(_front,i,n);
      }
    }

    return n;
}

int CZitzlersHVIndicator::filterNondominatedSet(std::deque<std::deque<double> > &_front, int _noPoints, int _objNo) {
    int i, j;
    int n;

    n = _noPoints;
    i = 0;

    while (i < n) {
      j = i + 1;
      while (j < n) {
        if (dominates(_front[i], _front[j], _objNo)) {
          /* remove point 'j' */
          n--;
          swap(_front, j, n);
        } else if (dominates(_front[j], _front[i], _objNo)) {
          /* remove point 'i'; ensure that the point copied to index 'i'
             is considered in the next outer loop (thus, decrement i) */
          n--;
          swap(_front, i, n);
          i--;
          break;
        } else {
          j++;
        }
      } // while
      i++;
    } // while

    return n;
}

double CZitzlersHVIndicator::surfaceUnchangedTo(std::deque< std::deque<double> > &_front, int _noPoints, int _obj) {
    int i;
    double minValue, value;

    if (_noPoints < 1) {

    }

    minValue = _front[0][_obj];
    for (i = 1; i < _noPoints; i++) {
      value = _front[i][_obj];
      if (value < minValue) {
        minValue = value;
      }
    }

    return minValue;
}

}


