//
//  main.cpp
//  cube02
//
//  Created by jiong lin on 11/23/21.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <thread>
#include <time.h>
#include <stdlib.h>     /* srand, rand */
#include <string>

#include "mass.hpp"
#include "draw.hpp"
using namespace std;
int robotNum = 12;
int halfNum = 6;
int spNum = 792;
int spNum02 = 264;

void startrobots(vector<vector<double>>& places)
{
    
}

void startsprings(vector<vector<double>>& springfun)
{
    // get random functions
    for (int j = 0; j < robotNum; j++)
    {
        for (int i = 0; i < springfun[0].size(); i = i + 3)
        {
            int r1 = rand() % 3;
            int r2 = rand() % 3;
            int r3 = rand() % 3;
            springfun[j][i] = r1;
            springfun[j][i + 1] = r2;
            springfun[j][i + 2] = r3;
        }
    }
}

void mutation(vector<vector<double>>& smallgroup, vector<vector<double>>& fungroup)
{
    for (int i = 0; i < halfNum; i++)
    {
        fungroup[i] = smallgroup[i];
    }
    
    for (int i = 0; i < halfNum; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            int r4 = rand() % spNum;
            int r5 = rand() % 3;
            smallgroup[i][r4] = r5;
        }
        fungroup[i + halfNum] = smallgroup[i];
    }
}

void crossover(vector<vector<double>>& smallgroup, vector<vector<double>>& fungroup)
{
    double temp = 0;
    for (int i = 0; i < halfNum; i++)
    {
        fungroup[i] = smallgroup[i];
    }
    
    for (int i = 0; i < halfNum; i = i + 2)
    {
        int cr1 = rand() % (spNum-30);
        for (int j = 0; j < 30; j++)
        {
            temp = smallgroup[i][j + cr1];
            smallgroup[i][j + cr1] = smallgroup[i + 1][j + cr1];
            smallgroup[i + 1][j + cr1] = temp;
        }
    }
    
    
    for (int i = 0; i < halfNum; i++)
    {
        int mychance = rand() % 10;
        if (mychance < 10)
        {
            for (int j = 0; j < 5; j++)
            {
                int r4 = rand() % spNum;
                int r5 = rand() % 3;
                smallgroup[i][r4] = r5;
            }
        }
        fungroup[i + halfNum] = smallgroup[i];
    }
}

void selection(vector<vector<double>>& fungroup, vector<vector<double>>& smallgroup, vector<double>& fitness)
{
    vector<double> copy(robotNum, 0);
    vector<double> idx(robotNum, 0);
    copy = fitness;
    sort(fitness.begin(), fitness.end());
    for (int i = 0; i < robotNum; i++)
    {
        for (int j = 0; j < robotNum; j++)
        {
            if (fitness[i] == copy[j])
            {
                idx[i] = j;
            }
        }
    }
    
    for (int i = 0; i < halfNum; i++)
    {
        smallgroup[i] = fungroup[idx[robotNum - 1 - i]];
    }
}

//def getfitness(vector<double> fitness, )
//{
//
//}

int main(int argc, const char * argv[])
{
    vector<double> springfuns;
    vector<double> runfuns;
    vector<vector<double>> fungroup;
    vector<vector<double>> smallgroup;
    vector<mass> massgroup;
    vector<double> fitness(robotNum, 0);
    unsigned long int springnum;

    srand ((unsigned)time(NULL));

//    vector<vector<int>> myplace01 = {{0,0,0},{0,1,0},{1,1,0},{2,1,0},{2,0,0},{0,1,1},{0,1,2},{0,0,2},{2,1,1},{2,1,2},{2,0,2},{1,1,2}};
//    vector<vector<int>> myplace01 = {{0,0,0},{0,1,0},{1,1,0},{2,1,0},{2,0,0},{0,1,1},{0,1,2},{0,0,2},{2,1,1},{2,1,2},{2,0,2},{1,1,2},
//        {1,0,0},{1,0,2}
//    };
    vector<vector<int>> myplace01 = {{0,0,0},{0,1,0},{1,1,0},{2,1,0},{2,0,0},{0,1,1},{0,1,2},{0,0,2}};
    vector<vector<int>> myplace02 = {{0,0,0},{0,1,0},{0,2,0},{0,3,0},{0,4,0},{0,5,0},{0,6,0}};
    vector<vector<int>> myplace03 = {{0,0,0},{1,0,0},{2,0,0},{1,0,1},{1,0,-1}};
    vector<vector<int>> myplace04 = {{0,0,0},{0,1,0},{1,1,0},{2,1,0},{2,0,0},{0,1,1},{0,1,2},{0,0,2},{2,1,1},{2,1,2},{2,0,2},{1,1,2}};
    vector<vector<int>> myplace05 = {{0,0,0},{0,0,1},{1,0,1},{2,0,1},{2,0,0},{3,0,0},{4,0,0},{4,0,1}};

//    for (int i = 0; i < robotNum; i++)
//    {
//        mass masstemp(myplace01);
//        masstemp.getmass();
//        masstemp.getsprings();
//        massgroup.push_back(masstemp);
//    }
    // r1
    mass masstemp1(myplace01);
    masstemp1.getmass();
    masstemp1.getsprings();
    massgroup.push_back(masstemp1);
    // r2
    mass masstemp2(myplace02);
    masstemp2.getmass();
    masstemp2.getsprings();
    massgroup.push_back(masstemp2);

    mass masstemp3(myplace03);
    masstemp3.getmass();
    masstemp3.getsprings();
    massgroup.push_back(masstemp3);

    mass masstemp4(myplace04);
    masstemp4.getmass();
    masstemp4.getsprings();
    massgroup.push_back(masstemp4);

    mass masstemp5(myplace05);
    masstemp5.getmass();
    masstemp5.getsprings();
    massgroup.push_back(masstemp5);
    
    cout<< massgroup.size()<<endl;
    
    springnum = massgroup[0].springMatrix.size();
    
//    evo loop
    
    std::ifstream outFile01;
    outFile01.open("function02.txt");

    if (!outFile01)
    {
        cout << "fail to open";
        return -1;
    }

    // read file
    while (outFile01)
    {
      string s;
      if (!getline( outFile01, s )) break;

      istringstream ss( s );
      vector <double> record;

      while (ss)
      {
        string s;
        if (!getline( ss, s, ',' )) break;
        record.push_back( stod(s) );
      }

      fungroup.push_back( record );
    }
    outFile01.close();

//    startsprings(fungroup);
    cout << fungroup[0].size();

    for (int i = 0; i < fungroup.size(); i = i + 2)
    {
        vector<double> temp(0, fungroup[0].size());
        smallgroup.push_back(temp);
    }

    jump(massgroup, fungroup, fitness);
    for (int i = 0; i < 12; i++)
    {
        cout << fitness[i] << ", ";
    }

//    selection(fungroup, smallgroup, fitness);
//    crossover(smallgroup, fungroup);

    // write fitness file
//    ofstream fitFile;
//    fitFile.open("fitness02.txt", fstream::app);
////    fitFile.open("fitness02.txt");
//    for (int i = 0; i < fitness.size() - 1; i++)
//    {
//        fitFile << fitness[i] << ",";
//    }
//    fitFile << fitness[11] << endl;
//
//    // write function file
//    ofstream outFile02;
//    outFile02.open("function02.txt");
//    for (int i = 0; i < 12; i++)
//    {
//        for (int j = 0; j < fungroup[0].size(); j++)
//        {
//            outFile02 << fungroup[i][j] << ",";
//        }
//        outFile02 << endl;
//    }

    for (int i = 0; i < springnum; i++)
    {
        springfuns.push_back(0); //k
        springfuns.push_back(0); //b
        springfuns.push_back(0); //c
    }

    for (int j = 0; j < robotNum; j = j + 2)
    {
        fungroup.push_back(springfuns);
        fungroup.push_back(springfuns);
        smallgroup.push_back(springfuns);
    }
    startsprings(fungroup);

//    for (int n = 0; n < 1; n++)
//    {
//        // reset robot
//        for (int i = 0; i < robotNum; i++)
//        {
//            mass masstemp(myplace01);
//            masstemp.getmass();
//            masstemp.getsprings();
//            massgroup[i] = masstemp;
//        }
//
//        vector<vector<double>> testcopy01;
//        testcopy01 = fungroup;
//        jump(massgroup, fungroup, fitness);
//        for(int i = 0; i < 12; i++)
//        {
//            cout<<fitness[i]<<";";
//        }
//        cout<<endl;
////        selection(fungroup, smallgroup, fitness);
////        mutation(smallgroup, fungroup);
//    }
//
//
//
    //evo end
    
    return 0;
}

