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

#include "Supervisor.h"

Define_Module(Supervisor);

void Supervisor::initialize()
{
    boxFeatureCnt = 0;
    maxCoord = new Coord(0,0,0);
    minCoord = new Coord(0,0,0);
    std::string path = par("mobilityFilePath").stdstringValue();
    file.open("output.csv", std::fstream::in);
    if(file.is_open()) {
        parseCoordinates();
        file.close();
        std::map<int,BoxFeature>::iterator it;
        it = featureMap.find(1);
        if(it != featureMap.end()){
            BoxFeature feature = it->second;
            minCoord->x = feature.getCoord().x;
            minCoord->y = feature.getCoord().y;
            minCoord->z = feature.getCoord().z;
        }

        it = featureMap.find(boxFeatureCnt);
        if(it != featureMap.end()){
            BoxFeature feature = it->second;
            maxCoord->x = feature.getCoord().x;
            maxCoord->y = feature.getCoord().y;
            maxCoord->z = feature.getCoord().z;
        }

    }
    else
        throw cRuntimeError("Supervisor can not open CSV file \n");


}

Coord Supervisor::getCoord(int index) {

    Enter_Method_Silent(); // for direct function call
    int max = featureList.size();
    int i = intuniform(0, max-1);
    try{
        BoxFeature feature = featureList.at(i);
        Coord coord = feature.getCoord();
        featureList.erase(featureList.begin()+(i)); // erase the i'th element
        return coord;
    }
    catch (const std::out_of_range& oor){
        throw cRuntimeError("Supervisor: out of range of FeatureList");
    }
}

Coord Supervisor::getCenterCoord() {
    double x = (maxCoord->x + minCoord->x)/2;
    double y = (maxCoord->y + minCoord->y)/2;
    double z = (maxCoord->z + minCoord->z)/2;
    Coord center = Coord(x,y,z);
    return center;
}

Coord Supervisor::getMaxCoord()
{
    return *maxCoord;
}

//int attach(int id) {
//    attachCnt++;
//    if(attachCnt > boxFeatureCnt)
//        throw cRuntimeError("Supervisor Error: no more features stored to attach another box");
//    else{
//
//    }
//
//}

void Supervisor::parseCoordinates() {
    double x, y ,z;
    BoxFeature* boxFeature;
    std::string line;
    const char* delim = ";\"";
    const char* token;
    std::getline(file, line); // ignore first line
    std::getline(file, line); // get first line with information
    featureList.clear();
    while(!file.eof()) {
        if(file.good()){
            cStringTokenizer tokenizer(line.c_str());
            tokenizer.setDelimiter(delim);
            if(tokenizer.hasMoreTokens()) {
                token = tokenizer.nextToken();
                // ignore first entry: id
            }
            if(tokenizer.hasMoreTokens()) {
                token = tokenizer.nextToken();
                // ignore second entry: description
            }
            if(tokenizer.hasMoreTokens()) {
                token = tokenizer.nextToken();
                x = (atof(token)); // get x coord mm
            }
            if(tokenizer.hasMoreTokens()) {
                token = tokenizer.nextToken();
                y = (atof(token)); // get y coord mm
            }
            if(tokenizer.hasMoreTokens()) {
                token = tokenizer.nextToken();
                z = (atof(token)); // get z coord mm
            }
            else
                throw cRuntimeError("Supervisor Error: unexpected CSV line format");

            boxFeatureCnt++;
            boxFeature = new BoxFeature(x,y,z);
            featureMap.insert( std::pair<int,BoxFeature>(boxFeatureCnt,*boxFeature) );
            featureList.push_back(*boxFeature);

            // TEST
            it = featureMap.find(1);
            if(it != featureMap.end()){
                BoxFeature feature = it->second;
                Coord coord = feature.getCoord();
            }
            // '''''''''''''''

            std::getline(file, line); // get next line
        }
        else
            throw cRuntimeError("Supervisor can not read line from CSV file \n");
    }
}

void Supervisor::handleMessage(cMessage *msg)
{
    // never happens
}

