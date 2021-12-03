//
//  mass.hpp
//  cube02
//
//  Created by jiong lin on 11/23/21.
//

#ifndef mass_hpp
#define mass_hpp

#include <stdio.h>
#include <vector>

using namespace std;

class mass
{
public:
    vector <vector<int>> cubeplace;
    vector <vector<int>> massplace;
    vector <vector<double>> massMatrix;
    vector <vector<double>> springMatrix;
    unsigned long int cubeNum;
    double edge;
    
    // initialize and get two parameters
    mass(vector<vector<int>> place);
    // check repeated ones and add a new ones
    void addplace(vector<int> temp);
    // similar
    void addSprings(vector<double> temp);
    // add points index into massplace
    void getmass();
    
    void restartmass();
    
    void giveIndex(int a1, int b1, int c1, int a2, int b2, int c2, vector<double> temp, double slen);
    
    void getsprings();
};

#endif /* mass_hpp */


