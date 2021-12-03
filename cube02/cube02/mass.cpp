//
//  mass.cpp
//  cube02
//
//  Created by jiong lin on 11/23/21.
//

#include "mass.hpp"
//#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

extern double edgelength;
extern double startpoint[];
extern double k0;
    
// initialize and get two parameters
mass::mass(vector<vector<int>> place)
{
    cubeplace = place;
    cubeNum = place.size();
}

// check repeated ones and add a new ones
void mass::addplace(vector<int> temp)
{
    bool add = true;
    for (int i = 0; i < massplace.size(); i++)
    {
        if (temp == massplace[i])
        {
            add = false;
        }
    }
    if (add)
    {
        massplace.push_back(temp);
    }
}
// similar
void mass::addSprings(vector<double> temp)
{
    bool add = true;
    for (int i = 0; i < springMatrix.size(); i++)
    {
        if (temp == springMatrix[i])
        {
            add = false;
        }
    }
    if (add)
    {
        springMatrix.push_back(temp);
    }
}

// add points index into massplace
void mass::getmass()
{
    vector<int> temp;
    for (int i = 0; i < cubeNum; i++){
        temp = cubeplace[i];
        addplace(temp);
        
        temp = cubeplace[i];
        temp[0] += 1;
        addplace(temp);
        
        temp = cubeplace[i];
        temp[1] += 1;
        addplace(temp);
        
        temp = cubeplace[i];
        temp[0] += 1;
        temp[1] += 1;
        addplace(temp);
        
        temp = cubeplace[i];
        temp[2] += 1;
        addplace(temp);
        
        temp = cubeplace[i];
        temp[0] += 1;
        temp[2] += 1;
        addplace(temp);
        
        temp = cubeplace[i];
        temp[1] += 1;
        temp[2] += 1;
        addplace(temp);
        
        temp = cubeplace[i];
        temp[0] += 1;
        temp[1] += 1;
        temp[2] += 1;
        addplace(temp);
    }
    for (int i = 0; i < massplace.size(); i++)
    {
        vector<double> temp2;
        temp2 = {0,0,0,0,0,0,0,0,0};
        // position
        temp2[0] = massplace[i][0] * edgelength + startpoint[0];
        temp2[1] = massplace[i][1] * edgelength + startpoint[1];
        temp2[2] = massplace[i][2] * edgelength + startpoint[2];
        // v
        temp2[3] = 0;
        temp2[4] = 0;
        temp2[5] = 0;
        // a
        temp2[6] = 0;
        temp2[7] = 0;
        temp2[8] = 0;
        
        massMatrix.push_back(temp2);
    }
}

void mass::restartmass()
{
    for (int i = 0; i < massMatrix.size(); i++)
    {
        massMatrix[i][3] = 0;
        massMatrix[i][4] = 0;
        massMatrix[i][5] = 0;
        massMatrix[i][6] = 0;
        massMatrix[i][7] = 0;
        massMatrix[i][8] = 0;
    }
}

void mass::giveIndex(int a1, int b1, int c1, int a2, int b2, int c2, vector<double> temp, double slen)
{
    for (int i = 0; i < massplace.size(); i++)
    {
        if (a1 == massplace[i][0] && b1 == massplace[i][1] && c1 == massplace[i][2])
        {
            temp[1] = i;
        }
        else if (a2 == massplace[i][0] && b2 == massplace[i][1] && c2 == massplace[i][2])
        {
            temp[2] = i;
        }
    }
    temp[3] = slen;
    temp[4] = slen;
    temp[5] = slen;
    addSprings(temp);
}

void mass::getsprings()
{
    vector<double> temp3;
    temp3 = {k0,0,0,0,0,0}; // 0: k, 1: index1, 2: index2, 3: length, 4: rest length, 5: rest length (const)
    for (int i = 0; i < cubeNum; i++)
    {
        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2],
                  cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2], temp3, edgelength);
        giveIndex(cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2],
                  cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2] + 1, temp3, edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2] + 1,
                  cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2] + 1, temp3, edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2],
                  cubeplace[i][0], cubeplace[i][1], cubeplace[i][2] + 1, temp3, edgelength);
        
        giveIndex(cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2],
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2], temp3, edgelength);
        giveIndex(cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2],
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2] + 1, temp3, edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2] + 1,
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2] + 1, temp3, edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2],
                  cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2] + 1, temp3, edgelength);
        
        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2],
                  cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2], temp3, edgelength);
        giveIndex(cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2],
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2], temp3, edgelength);
        giveIndex(cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2] + 1,
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2] + 1, temp3, edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2] + 1,
                  cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2] + 1, temp3, edgelength);
        // cross lines
        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2],
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2], temp3, sqrt(2) * edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2],
                  cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2], temp3, sqrt(2) * edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2] + 1,
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2] + 1, temp3, sqrt(2) * edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2] + 1,
                  cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2] + 1, temp3, sqrt(2) * edgelength);

        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2],
                  cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2] + 1, temp3, sqrt(2) * edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2] + 1,
                  cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2], temp3, sqrt(2) * edgelength);
        giveIndex(cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2],
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2] + 1, temp3, sqrt(2) * edgelength);
        giveIndex(cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2] + 1,
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2], temp3, sqrt(2) * edgelength);

        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2],
                  cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2] + 1, temp3, sqrt(2) * edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2] + 1,
                  cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2], temp3, sqrt(2) * edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2],
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2] + 1, temp3, sqrt(2) * edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2] + 1,
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2], temp3, sqrt(2) * edgelength);
        // longest lines
        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2],
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2] + 1, temp3, sqrt(3) * edgelength);
        giveIndex(cubeplace[i][0], cubeplace[i][1], cubeplace[i][2] + 1,
                  cubeplace[i][0] + 1, cubeplace[i][1] + 1, cubeplace[i][2], temp3, sqrt(3) * edgelength);

        giveIndex(cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2],
                  cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2] + 1, temp3, sqrt(3) * edgelength);
        giveIndex(cubeplace[i][0] + 1, cubeplace[i][1], cubeplace[i][2] + 1,
                  cubeplace[i][0], cubeplace[i][1] + 1, cubeplace[i][2], temp3, sqrt(3) * edgelength);
    }
//    cout << "size:" <<springMatrix.size() << endl;
}

