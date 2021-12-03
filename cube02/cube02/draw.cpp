//
//  draw.cpp
//  cube02
//
//  Created by jiong lin on 11/23/21.
//

#include "draw.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <math.h>
#include <iostream>
#include <thread>
#include <stdlib.h>     /* srand, rand */
#include "mass.hpp"

#define WIDTH 1280
#define HEIGHT 840
#define DEPTH 1280
#define length 100
#define PI 3.14
using namespace std;

extern int robotNum;
float rotationX = 30.0;
float rotationY = 0.0;
double edgelength = 20;
double h0 = -200;
double k0 = 1000;
double k1 = 5000;
double k2 = 10000;
double b0 = 0;
double b1 = 3;
double b2 = 6;
double c0 = 0;
double c1 = 0.5 * PI;
double c2 = PI;
double kc = 5000;
double gravity = -9.81 * 200;
double T = .0;
int gen = 0;
double dt = 0.0001;
double ma = 0.1;
double fr = 0.8;
double damping = 0.9999;
double ground = 280;
double fw = 6; //frequency

double centerpoint[3] = {640, 480, -640};
double startpoint[3] = {640, 480 + h0, -640};

glm::mat4 projection = glm::mat4(1.0f);;

double roundNum(double num)
{
    return floor(num * 10000 + 0.5) / 10000;
}

int absNum(int a, int b)
{
    if (a > b){
        return a - b;
    }
    else
    {
        return b - a;
    }
}

void renewMass(vector <vector<double>>& mymass, vector <vector<double>>& myspring)
{
    for (int i = 0; i < mymass.size(); i++)
    {
        mymass[i][6] = .0;
        mymass[i][7] = gravity;
        mymass[i][8] = .0;
    }
    
    // hit ground
    for (int i = 0; i < mymass.size(); i++)
    {
        if (mymass[i][1] < 280)
        {
            mymass[i][7] += kc * (280 - mymass[i][1]) * 10;
        }
        if (mymass[i][1] < 280)
        {
            for (int j = 0; j < 2; j++)
            {
                if (mymass[i][3 + 2*j] < 0)
                {
                    mymass[i][6 + 2*j] += fr * kc * (281 - mymass[i][1]) * 10;
                }
                else
                {
                    mymass[i][6 + 2*j] -= fr * kc * (281 - mymass[i][1]) * 10;
                }
            }
        }
    }
    
    // renew spring
    for (int i = 0; i < myspring.size(); i++)
    {
        int idx01 = int(myspring[i][1]);
        int idx02 = int(myspring[i][2]);
        double x1 = mymass[idx01][0];
        double x2 = mymass[idx02][0];
        double y1 = mymass[idx01][1];
        double y2 = mymass[idx02][1];
        double z1 = mymass[idx01][2];
        double z2 = mymass[idx02][2];
        myspring[i][3] = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
        mymass[idx01][6] += myspring[i][0] * (myspring[i][4] - myspring[i][3]) * 10 * (x1 - x2) / myspring[i][3]; // renew ax
        mymass[idx02][6] += myspring[i][0] * (myspring[i][4] - myspring[i][3]) * 10 * (x2 - x1) / myspring[i][3];
        mymass[idx01][7] += myspring[i][0] * (myspring[i][4] - myspring[i][3]) * 10 * (y1 - y2) / myspring[i][3]; // renew ay
        mymass[idx02][7] += myspring[i][0] * (myspring[i][4] - myspring[i][3]) * 10 * (y2 - y1) / myspring[i][3];
        mymass[idx01][8] += myspring[i][0] * (myspring[i][4] - myspring[i][3]) * 10 * (z1 - z2) / myspring[i][3]; // renew az
        mymass[idx02][8] += myspring[i][0] * (myspring[i][4] - myspring[i][3]) * 10 * (z2 - z1) / myspring[i][3];
    }
    
//    cout<<"01:  "<<mass[29][7]<<endl;
    
    for (int i = 0; i < mymass.size(); i++)
    {
        mymass[i][3] += mymass[i][6] * dt; // renew vx
        mymass[i][4] += mymass[i][7] * dt; // renew vy
        mymass[i][5] += mymass[i][8] * dt; // renew vz
        
        mymass[i][0] += mymass[i][3] * dt; // renew x
        mymass[i][1] += mymass[i][4] * dt; // renew y
        mymass[i][2] += mymass[i][5] * dt; // renew z
        
    }
    // damping
    if (gen > 0)
    {
        for (int i = 0; i < mymass.size(); i++)
            {
                mymass[i][3] *= damping;
                mymass[i][4] *= damping;
                mymass[i][5] *= damping;
            }
    }
}

void givefun(vector <vector<double>>& myspring, vector<double> myfunction)
{
    unsigned long int myspsize = myspring.size();
    double fun_k = 0;
    double fun_b = 0;
    double fun_c = 0;
    
    for (int i = 0; i < myspsize; i++)
    {
        // give k
        if (myfunction[3 * i] < 0.5)
        {
            fun_k = k0;
        }
        else if (myfunction[3 * i] < 1.5)
        {
            fun_k = k1;
        }
        else
        {
            fun_k = k2;
        }
        // give b
        if (myfunction[3 * i + 1] < 0.5)
        {
            fun_b = b0;
        }
        else if (myfunction[3 * i + 1] < 1.5)
        {
            fun_b = b1;
        }
        else
        {
            fun_b = b2;
        }
        // give c
        if (myfunction[3 * i + 2] < 0.5)
        {
            fun_c = c0;
        }
        else if (myfunction[3 * i + 2] < 1.5)
        {
            fun_c = c1;
        }
        else
        {
            fun_c = c2;
        }
        
        myspring[i][0] = fun_k;
        myspring[i][4] = myspring[i][5] + fun_b * sin(fw * T + fun_c);
    }
}

void perspective(float *vertex, int num_p)
{
    bool change = true;
    if (change)
    {
        for (int i = 0; i < num_p; i++)
        {
            float x = vertex[3 * i];
            float y = vertex[3 * i + 1];
            float z = vertex[3 * i + 2];
            x = (x - startpoint[0]) * ((z + 1280 * 3) / (1280 * 3)) + startpoint[0];
            y = (y - startpoint[1]) * ((z + 1280 * 3) / (1280 * 3)) + startpoint[1];
            vertex[3 * i] = x;
            vertex[3 * i + 1] = y;
        }
    }
}

void drawGround(double ground)
{
    double gridWidth = 50;
    float colors0[] = {
        0.6, 0.6, 0.6,
        0.6, 0.6, 0.6,
        0.6, 0.6, 0.6,
        0.6, 0.6, 0.6
    };
    float colors1[] = {
        0, 0.2f, 0.4f,
        0, 0.2f, 0.4f,
        0, 0.2f, 0.4f,
        0, 0.2f, 0.4f
    };
    float colors2[] = {
        0.8, 0.2f, 0.2f,
        0.8, 0.2f, 0.2f,
        0.8, 0.2f, 0.2f,
    };

    float tempGrid1[] = {
        .0,.0,.0,
        .0,.0,.0,
        .0,.0,.0,
        .0,.0,.0
    };
    
    for (int j = 0; j < 9; j++)
    {
        for (int i = 0; i < 9; i++)
        {
            tempGrid1[0] = 190 + 2 * i * gridWidth; tempGrid1[1] = ground; tempGrid1[2] = -190 - 2 * j * gridWidth;
            tempGrid1[3] = 190 + 2 * i * gridWidth; tempGrid1[4] = ground; tempGrid1[5] = -190 - (2 * j + 1) * gridWidth;
            tempGrid1[6] = 190 + (2 * i + 1) * gridWidth; tempGrid1[7] = ground; tempGrid1[8] = -190 - (2 * j + 1) * gridWidth;
            tempGrid1[9] = 190 + (2 * i + 1) * gridWidth; tempGrid1[10] = ground; tempGrid1[11] = -190 - 2 * j * gridWidth;
            perspective(tempGrid1, 4);
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, tempGrid1);
            glColorPointer(3, GL_FLOAT, 0, colors1);
            glDrawArrays(GL_QUADS, 0, 8);

            
            tempGrid1[0] = 190 + (2 * i + 2) * gridWidth; tempGrid1[1] = ground; tempGrid1[2] = -190 - 2 * j * gridWidth;
            tempGrid1[3] = 190 + (2 * i + 2) * gridWidth; tempGrid1[4] = ground; tempGrid1[5] = -190 - (2 * j + 1) * gridWidth;
            tempGrid1[6] = 190 + (2 * i + 1) * gridWidth; tempGrid1[7] = ground; tempGrid1[8] = -190 - (2 * j + 1) * gridWidth;
            tempGrid1[9] = 190 + (2 * i + 1) * gridWidth; tempGrid1[10] = ground; tempGrid1[11] = -190 - 2 * j * gridWidth;
            perspective(tempGrid1, 4);
     
            glVertexPointer(3, GL_FLOAT, 0, tempGrid1);
            glColorPointer(3, GL_FLOAT, 0, colors0);
            glDrawArrays(GL_QUADS, 0, 8);
            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
        for (int i = 0; i < 9; i++)
        {
            tempGrid1[0] = 190 + 2 * i * gridWidth; tempGrid1[1] = ground; tempGrid1[2] = -190 - (2 * j + 1) * gridWidth;
            tempGrid1[3] = 190 + 2 * i * gridWidth; tempGrid1[4] = ground; tempGrid1[5] = -190 - (2 * j + 2) * gridWidth;
            tempGrid1[6] = 190 + (2 * i + 1) * gridWidth; tempGrid1[7] = ground; tempGrid1[8] = -190 - (2 * j + 2) * gridWidth;
            tempGrid1[9] = 190 + (2 * i + 1) * gridWidth; tempGrid1[10] = ground; tempGrid1[11] = -190 - (2 * j + 1) * gridWidth;
            perspective(tempGrid1, 4);
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, tempGrid1);
            glColorPointer(3, GL_FLOAT, 0, colors0);
            glDrawArrays(GL_QUADS, 0, 8);

            
            tempGrid1[0] = 190 + (2 * i + 2) * gridWidth; tempGrid1[1] = ground; tempGrid1[2] = -190 - (2 * j + 1) * gridWidth;
            tempGrid1[3] = 190 + (2 * i + 2) * gridWidth; tempGrid1[4] = ground; tempGrid1[5] = -190 - (2 * j + 2) * gridWidth;
            tempGrid1[6] = 190 + (2 * i + 1) * gridWidth; tempGrid1[7] = ground; tempGrid1[8] = -190 - (2 * j + 2) * gridWidth;
            tempGrid1[9] = 190 + (2 * i + 1) * gridWidth; tempGrid1[10] = ground; tempGrid1[11] = -190 - (2 * j + 1) * gridWidth;
            perspective(tempGrid1, 4);
     
            glVertexPointer(3, GL_FLOAT, 0, tempGrid1);
            glColorPointer(3, GL_FLOAT, 0, colors1);
            glDrawArrays(GL_QUADS, 0, 8);
            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
    }
    

    
    
    float tempGrid[] = { 0,0,0,0,0,0 };
    for (int i = 0; i < 19; i = i + 18)
    {
        tempGrid[0] = i * gridWidth + 190;
        tempGrid[1] = ground;
        tempGrid[2] = -190;
        tempGrid[3] = i * gridWidth + 190;
        tempGrid[4] = ground;
        tempGrid[5] = -1090;
        perspective(tempGrid, 2);
        glEnableClientState(GL_VERTEX_ARRAY);
//        glLineStipple(1, 0x00FF);
        glLineWidth(2);
        glVertexPointer(3, GL_FLOAT, 0, tempGrid);
        glDrawArrays(GL_LINES, 0, 2);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    for (int i = 0; i < 19; i = i + 18)
    {
        tempGrid[0] = 190;
        tempGrid[1] = ground;
        tempGrid[2] = -i * gridWidth - 190;
        tempGrid[3] = 1090;
        tempGrid[4] = ground;
        tempGrid[5] = -i * gridWidth - 190;
        perspective(tempGrid, 2);
        glEnableClientState(GL_VERTEX_ARRAY);
        glLineWidth(2);
        glVertexPointer(3, GL_FLOAT, 0, tempGrid);
        glDrawArrays(GL_LINES, 0, 2);
        glDisableClientState(GL_VERTEX_ARRAY);

    }
    float flag[] = {
        640,280,-640,
        640,280+100,-640
    };
    perspective(flag, 2);
    glEnable(GL_POINT_SMOOTH);
    glEnableClientState(GL_VERTEX_ARRAY);
    glLineWidth(4);
    glVertexPointer(3, GL_FLOAT, 0, flag);
    glDrawArrays(GL_LINES, 0, 2);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_POINT_SMOOTH);
    
    float flag01[] = {
        640,280+100,-640,
        640,280+60,-640,
        640 + 40,280+80,-640,
    };
    perspective(flag01, 3);
    glEnable(GL_POINT_SMOOTH);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, flag01);
    glColorPointer(3, GL_FLOAT, 0, colors2);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_POINT_SMOOTH);
}

void drawCube(vector <vector<double>> mass, vector <vector<double>> spring, float colors[])
{
    float mycolors01[] = {
        0, 0.5, 0,
//        0, 0.5, 0,
//        0, 0.5, 0,
//        0, 0.5, 0
    };
    unsigned long int pointNum = mass.size();
        
    float tempv[6];
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glLineWidth(4);

    for (int i = 0; i < spring.size(); i++)
    {
        int idx01 = int(spring[i][1]);
        int idx02 = int(spring[i][2]);
        tempv[0] = mass[idx01][0];
        tempv[1] = mass[idx01][1];
        tempv[2] = mass[idx01][2];
        tempv[3] = mass[idx02][0];
        tempv[4] = mass[idx02][1];
        tempv[5] = mass[idx02][2];
        perspective(tempv, 2);
        glVertexPointer(3, GL_FLOAT, 0, tempv);
        glColorPointer(3, GL_FLOAT, 0, colors);
        glDrawArrays(GL_LINES, 0, 2);
    }
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    float vertexes[pointNum * 3];
    for (int i = 0; i < pointNum; i++)
    {
        vertexes[3 * i] = mass[i][0];
        vertexes[3 * i + 1] = mass[i][1];
        vertexes[3 * i + 2] = mass[i][2];
    }
    perspective(vertexes, int(pointNum));
    glEnable(GL_POINT_SMOOTH);
    glEnableClientState(GL_VERTEX_ARRAY);
//    glEnableClientState(GL_COLOR_ARRAY);
    glPointSize(4);
    glVertexPointer(3, GL_FLOAT, 0, vertexes);
    glColorPointer(3, GL_FLOAT, 0, mycolors01);
    glDrawArrays(GL_POINTS, 0, int(pointNum));
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_POINT_SMOOTH);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const GLfloat rotationSpeed = 10;
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            rotationX -= rotationSpeed;
            break;
        case GLFW_KEY_DOWN:
            rotationX += rotationSpeed;
            break;
        case GLFW_KEY_RIGHT:
            rotationY += rotationSpeed;
            break;
        case GLFW_KEY_LEFT:
            rotationY -= rotationSpeed;
            break;
        }
    }
}

int jump(vector<mass>& mymass, vector<vector<double>>& springfun, vector<double>& fitness)
{
    //
//    glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
//    glm::mat4 model;
//    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    glm::mat4 projection;
//    projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.1f, 100.0f);
    //draw prepare
    GLFWwindow* window;
    if (!glfwInit())
    {
        return -1;
    }
    window = glfwCreateWindow(WIDTH, HEIGHT, "cubes", NULL, NULL);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, 0, DEPTH);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
//    while (!glfwWindowShouldClose(window))
    gen = 0;
    while (true)
    {
        T = T + dt;
        gen = gen + 1;
        // renew mass matrix and spring matrix
        for (int i = 0; i < robotNum - 7; i++)
        {
            givefun(mymass[i].springMatrix, springfun[i]);
            renewMass(mymass[i].massMatrix, mymass[i].springMatrix);
        }

        if (gen % 60 == 0)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            glPushMatrix();
            glTranslatef(centerpoint[0], centerpoint[1], centerpoint[2]);
            glRotatef(rotationX, 1, 0, 0);
            glRotatef(rotationY, 0, 1, 0);
            glTranslatef(-centerpoint[0], -centerpoint[1], -centerpoint[2]);
            drawGround(ground);
            for (int i = 0; i < robotNum - 7; i++)
            {
                float colors[24] = {
                    0.2, 0.8, 0.2,
                    0.2, 0.8, 0.2,
                    0.2, 0.8, 0.2,
                    0.2, 0.8, 0.2
                };
                drawCube(mymass[i].massMatrix, mymass[i].springMatrix, colors);
            }
            glPopMatrix();
            glfwSwapBuffers(window);// swap buffers
            glfwPollEvents();
            
            if (gen > 600000)
            {
//                for (int i = 0; i < robotNum; i++){
//                    fitness[i] = mymass[i].massMatrix[0][0] - startpoint[0];
//                }
                break;
            }
        }
    }
    glfwTerminate();
    return 0;
}


