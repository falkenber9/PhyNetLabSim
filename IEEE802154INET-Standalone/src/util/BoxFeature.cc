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

#include <BoxFeature.h>

BoxFeature::BoxFeature(double x, double y, double z) {
    coord = new Coord(x, y, z);
}

BoxFeature::BoxFeature(const BoxFeature& other) {
    coord = new Coord(other.coord->x, other.coord->y, other.coord->z);
}

BoxFeature& BoxFeature::operator=(BoxFeature other){
    coord->x = other.coord->x;
    coord->y = other.coord->y;
    coord->z = other.coord->z;
}

void BoxFeature::setCoord(double x, double y, double z){
    coord->x = x;
    coord->y = y;
    coord->z = z;
}

Coord BoxFeature::getCoord() {
    return *coord;
}

BoxFeature::~BoxFeature() {
    delete coord;
}

