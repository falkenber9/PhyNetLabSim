//
// Copyright (C) 2017 Jens Drenhaus     (PhyNetLabSim adaptations)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef BOXFEATURE_H_
#define BOXFEATURE_H_

#include <Coord.h>


class BoxFeature {
private:
    Coord* coord;

public:
    BoxFeature(double x, double y, double z);
    BoxFeature(const BoxFeature& other);
    BoxFeature& operator=(BoxFeature other);
    virtual ~BoxFeature();

    void setCoord(double x, double y, double z);
    Coord getCoord();
};

#endif /* BOXFEATURE_H_ */
