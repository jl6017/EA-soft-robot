//
//  draw.hpp
//  cube02
//
//  Created by jiong lin on 11/23/21.
//

#ifndef draw_hpp
#define draw_hpp
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "mass.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

double roundNum(double num);

int absNum(int a, int b);

void renewMass(vector <vector<double>>& mymass, vector <vector<double>>& myspring);

void givefun(vector <vector<double>>& myspring, vector<double> myfunction);

void drawGround(double halfHeight);

void drawCube(vector <vector<double>> mass, vector <vector<double>> spring, float colors[]);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int jump(vector<mass>& mymass, vector<vector<double>>& springfun, vector<double>& fitness);

#endif /* draw_hpp */


