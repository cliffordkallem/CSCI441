#include "glwidget.h"
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QtGui>
#include <QTextStream>
#include <QColor>
#include <cmath>
using namespace std;
#define PI (3.14159265)

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), outline(false), drawMode(0) {
    num_pts = 0;
    inputColor =   false;
    drawMode = GL_POINTS;
    num_mode = 0;
    size_dif = 4.0;
    inColor = glm::vec3(1.0,1.0,1.0);
}

GLWidget::~GLWidget() {
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    glUseProgram(program);
    glBindVertexArray(vao);

    switch(event->key()) {
    case Qt::Key_C:
        cout << "Cleared all the points. Make sure to "
                "update this once you modify how the points "
                "are stored." << endl;
        pts.resize(0);
        pts_data.resize(0);
        colors.resize(0);

        num_pts = 0;
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
        break;
    case Qt::Key_W:
        outline = !outline;
        if(outline) {
            cout << "Displaying outlines." << endl;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            cout << "Displaying filled polygons." << endl;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        break;
    case Qt::Key_F:
        for(int i = size_dif;i<sizeWid.x-size_dif && i <image->width();i+=1.5*size_dif)
        {
            for(int j = size_dif;j<sizeWid.y-size_dif && j <image->height();j+=1.5*size_dif)
            {
                if(drawMode == 1){
                    Triangle(i,j);
                }
                else if(drawMode == 0)
                {
                    Square(i,j);
                }
                else if(drawMode ==2){
                    Circle(i,j);
                }
                else if(drawMode ==3){
                    semiCircle(i,j);
                }
                else if(drawMode ==4){
                    Hexagon(i,j);
                }
                else if(drawMode ==5){
                    Star(i,j);
                }
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, pts.size()*sizeof(glm::vec3), &pts[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(glm::vec3), &colors[0], GL_DYNAMIC_DRAW);
        update();
        break;
    case Qt::Key_R:
        for(int i = 0; i <pts.size();i++){
            if(pts_data[i].point != -1){
                if(pts_data[i].type==0){
                    pts_data[i].nothing=rand()%(int)(200+size_dif);
                    resizeSquareT(i);
                }
                else if(pts_data[i].type==1)
                {
                    pts_data[i].nothing=rand()%(int)(200+size_dif);
                    resizeTriangleT(i);
                }
                else if(pts_data[i].type==2)
                {
                    pts_data[i].nothing=rand()%(int)(200+size_dif);
                    resizeCircleT(i);
                }
                else if(pts_data[i].type == 3)
                {
                    pts_data[i].nothing=rand()%(int)(200+size_dif);
                    resizesemiCircleT(i);
                }
                else if(pts_data[i].type==4)
                {
                    pts_data[i].nothing=rand()%(int)(200+size_dif);
                    resizeHexagonT(i);
                }
                else if(pts_data[i].type == 5)
                {

                    if(pts_data[i].point <=3)
                    {
                        pts_data[i].nothing=rand()%(int)(200+size_dif);
                        resizeTriangleT(i);
                    }
                    else{
                        pts_data[i].nothing=rand()%(int)(200+size_dif);
                        resizeTriangleT2(i);
                    }

                }
            }

        }
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, pts.size()*sizeof(glm::vec3), &pts[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(glm::vec3), &colors[0], GL_DYNAMIC_DRAW);
        update();
        break;
    case Qt::Key_E:
        for(int i = 0; i <pts.size();i++){
            if(pts_data[i].size >1 && pts_data[i].point != -1){
                if(pts_data[i].type==0){
                    pts_data[i].nothing=rand()%(int)(200-size_dif);
                    resizeSquareT(i);
                }
                else if(pts_data[i].type==1)
                {
                    pts_data[i].nothing=rand()%(int)(200-size_dif);
                    resizeTriangleT(i);
                }
                else if(pts_data[i].type==2)
                {
                    pts_data[i].nothing=rand()%(int)(200-size_dif);
                    resizeCircleT(i);
                }
                else if(pts_data[i].type == 3)
                {
                    pts_data[i].nothing=rand()%(int)(200-size_dif);
                    resizesemiCircleT(i);
                }
                else if(pts_data[i].type==4)
                {
                    pts_data[i].nothing=rand()%(int)(200-size_dif);
                    resizeHexagonT(i);
                }
                else if(pts_data[i].type == 5)
                {

                    if(pts_data[i].point <=3)
                    {
                        pts_data[i].nothing=rand()%(int)(200-size_dif);
                        resizeTriangleT(i);
                    }
                    else{
                        pts_data[i].nothing=rand()%(int)(200-size_dif);
                        resizeTriangleT2(i);
                    }

                }
            }

        }
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, pts.size()*sizeof(glm::vec3), &pts[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(glm::vec3), &colors[0], GL_DYNAMIC_DRAW);
        update();
        break;
    case Qt::Key_S:
        for(int i = 0; i <pts.size();i++){
            if(pts_data[i].point != -1){
                if(pts_data[i].type==0){
                    pts_data[i].size++;
                    resizeSquare(i);
                }
                else if(pts_data[i].type==1)
                {
                    pts_data[i].size++;
                    resizeTriangle(i);
                }
                else if(pts_data[i].type==2)
                {
                    pts_data[i].size++;
                    resizeCircle(i);
                }
                else if(pts_data[i].type == 3)
                {
                    pts_data[i].size++;
                    resizesemiCircle(i);
                }
                else if(pts_data[i].type==4)
                {
                    pts_data[i].size++;
                    resizeHexagon(i);
                }
                else if(pts_data[i].type == 5)
                {

                    if(pts_data[i].point <=3)
                    {
                        pts_data[i].size++;
                        resizeTriangle(i);
                    }
                    else{
                        pts_data[i].size++;
                        resizeTriangle2(i);
                    }

                }
            }

        }
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, pts.size()*sizeof(glm::vec3), &pts[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(glm::vec3), &colors[0], GL_DYNAMIC_DRAW);
        update();
        break;
    case Qt::Key_A:
        for(int i = 0; i <pts.size();i++){
            if(pts_data[i].size >1 && pts_data[i].point != -1){
                if(pts_data[i].type==0){
                    pts_data[i].size--;
                    resizeSquare(i);
                }
                else if(pts_data[i].type==1)
                {
                    pts_data[i].size--;
                    resizeTriangle(i);
                }
                else if(pts_data[i].type==2)
                {
                    pts_data[i].size--;
                    resizeCircle(i);
                }
                else if(pts_data[i].type == 3)
                {
                    pts_data[i].size--;
                    resizesemiCircle(i);
                }
                else if(pts_data[i].type==4)
                {
                    pts_data[i].size--;
                    resizeHexagon(i);
                }
                else if(pts_data[i].type == 5)
                {
                    if(pts_data[i].point <=3){
                        pts_data[i].size--;
                        resizeTriangle(i);
                    }
                    else{
                        pts_data[i].size--;
                        resizeTriangle2(i);
                    }
                }
            }

        }
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, pts.size()*sizeof(glm::vec3), &pts[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(glm::vec3), &colors[0], GL_DYNAMIC_DRAW);
        update();
        break;
    case Qt::Key_X:
        size_dif++;
        cout<<"New size "<< size_dif<<endl;
        break;
    case Qt::Key_Z:
        if(size_dif != 0)
        {
            size_dif--;
        }
        else{

        }
        cout<<"New size "<< size_dif<<endl;
        break;
    case Qt::Key_I:
        inputColor = true;
        cout<<"Input color r,g,b"<<endl;
        char stuff;
        float red,blue,green;
        cin>>red;
        cin>>stuff;
        cin>>green;
        cin>>stuff;
        cin>>blue;
        inColor = glm::vec3(red,green,blue);
        break;
    case Qt::Key_V:
        cout<< "Using last color inputed "<<inColor.x<<", "<<inColor.y<<", "<<inColor.z<<endl;
        inputColor = true;
        break;
    case Qt::Key_Space:
        num_mode ++;
        if(num_mode > 5)
            num_mode = 0;
        switch(num_mode){
        case 0:
            cout << "Squares" << endl;
            drawMode = 0;
            break;
        case 1:
            cout << "Triangle"  << endl;
            drawMode = 1;
            break;
        case 2:
            cout << "Circle"  << endl;
            drawMode = 2;
            break;
        case 3:
            cout << "semiCircle"  << endl;
            drawMode = 3;
            break;
        case 4:
            cout << "Hexagon"  << endl;
            drawMode = 4;
            break;
        case 5:
            cout << "Star"  << endl;
            drawMode = 5;
            break;
            break;
        }
    }

    update();
}
void GLWidget::mousePressEvent(QMouseEvent *event) {
    glUseProgram(program);
    glBindVertexArray(vao);
    if(event->x() <= image->width() && event ->y()<= image->height()){
        if(drawMode == 1){
            Triangle(event->x(),event->y());
        }
        else if(drawMode == 0)
        {
            Square(event->x(),event->y());
        }
        else if(drawMode ==2){
            Circle(event->x(),event->y());
        }
        else if(drawMode ==3){
            semiCircle(event->x(),event->y());
        }
        else if(drawMode ==4){
            Hexagon(event->x(),event->y());
        }
        else if(drawMode ==5){
            Star(event->x(),event->y());
        }
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, pts.size()*sizeof(glm::vec3), &pts[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(glm::vec3), &colors[0], GL_DYNAMIC_DRAW);
        update();
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    glUseProgram(program);
    glBindVertexArray(vao);
    if(event->x() <= image->width() && event ->y() <= image->height()){
        if(drawMode == 1){
            Triangle(event->x(),event->y());
        }
        else if(drawMode == 0)
        {
            Square(event->x(),event->y());
        }
        else if(drawMode ==2){
            Circle(event->x(),event->y());
        }
        else if(drawMode ==3){
            semiCircle(event->x(),event->y());
        }
        else if(drawMode ==4){
            Hexagon(event->x(),event->y());
        }
        else if(drawMode ==5){
            Star(event->x(),event->y());
        }
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, pts.size()*sizeof(glm::vec3), &pts[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(glm::vec3), &colors[0], GL_DYNAMIC_DRAW);
        update();
    }
}
void GLWidget::Triangle(int x, int y)
{
    glUseProgram(program);
    glBindVertexArray(vao);
    QRgb colorTemp= image->pixel(x,y);
    int red = qRed(colorTemp);
    int blue = qBlue(colorTemp);
    int green = qGreen(colorTemp);
    glm::vec3 tempColor;
    if(inputColor){
        tempColor = inColor;
        inputColor= false;
    }
    else{
        tempColor = glm::vec3((float)red/255,(float)green/255,(float)blue /255);
    }

    for(int i =0; i < 3;i++)
    {
        //first pixel
        glm::vec3 temp = glm::vec3(x,y, 1);
        pts.push_back(temp);
        points temp4;
        temp4.center.x = temp.x;
        temp4.center.y = temp.y;
        temp4.point=-1;
        temp4.size = size_dif;
        temp4.type=1;
        pts_data.push_back(temp4);
        colors.push_back(tempColor);
        //second pixel
        glm::vec3 temp3 = glm::vec3(x+size_dif*cos((float)(PI / 6)+((float)PI/6)*4*i),y+size_dif*sin((float)(PI / 6)+((float)PI/6)*4*i), 1);
        pts.push_back(temp3);
        points temp5;
        temp5.center.x= temp.x;
        temp5.center.y= temp.y;
        temp5.point=i;
        temp5.size = size_dif;
        temp5.type=1;
        pts_data.push_back(temp5);
        colors.push_back(tempColor);
        //third pixel
        glm::vec3  temp2 = glm::vec3(x+size_dif*cos((float)PI/6+((float)PI/6)*4*(i+1)),y+size_dif*sin((float)PI/6+((float)PI/6)*4*(i+1)),1);
        pts.push_back(temp2);
        points temp6;
        temp6.center.x= temp.x;
        temp6.center.y= temp.y;
        temp6.size = size_dif;
        temp6.point=i+1;
        temp6.type=1;
        pts_data.push_back(temp6);
        colors.push_back(tempColor);
    }
}
void GLWidget::Star(int x, int y)
{
    glUseProgram(program);
    glBindVertexArray(vao);
    QRgb colorTemp= image->pixel(x,y);
    int red = qRed(colorTemp);
    int blue = qBlue(colorTemp);
    int green = qGreen(colorTemp);
    glm::vec3 tempColor;
    if(inputColor){
        tempColor = inColor;
        inputColor= false;
    }
    else{
        tempColor = glm::vec3((float)red/255,(float)green/255,(float)blue /255);
    }
    for(int i =0; i < 3;i++)
    {
        //first pixel
        glm::vec3 temp = glm::vec3(x,y, 1);
        pts.push_back(temp);
        points temp4;
        temp4.center.x = temp.x;
        temp4.center.y = temp.y;
        temp4.point=-1;
        temp4.size = size_dif;
        temp4.type=5;
        pts_data.push_back(temp4);
        colors.push_back(tempColor);
        //second pixel
        glm::vec3 temp3 = glm::vec3(x+size_dif*cos((float)(PI / 6)+((float)PI/6)*4*i),y+size_dif*sin((float)(PI / 6)+((float)PI/6)*4*i), 1);
        pts.push_back(temp3);
        points temp5;
        temp5.center.x= temp.x;
        temp5.center.y= temp.y;
        temp5.point=i;
        temp5.size = size_dif;
        temp5.type=5;
        pts_data.push_back(temp5);
        colors.push_back(tempColor);
        //third pixel
        glm::vec3  temp2 = glm::vec3(x+size_dif*cos((float)PI/6+((float)PI/6)*4*(i+1)),y+size_dif*sin((float)PI/6+((float)PI/6)*4*(i+1)),1);
        pts.push_back(temp2);
        points temp6;
        temp6.center.x= temp.x;
        temp6.center.y= temp.y;
        temp6.size = size_dif;
        temp6.point=i+1;
        temp6.type=5;
        pts_data.push_back(temp6);
        colors.push_back(tempColor);
    }
    for(int i =0; i < 3;i++)
    {
        //first pixel
        glm::vec3 temp = glm::vec3(x,y, 1);
        pts.push_back(temp);
        points temp4;
        temp4.center.x = temp.x;
        temp4.center.y = temp.y;
        temp4.point=-1;
        temp4.size = size_dif;
        temp4.type=1;
        pts_data.push_back(temp4);
        colors.push_back(tempColor);
        //second point
        glm::vec3 temp3 = glm::vec3(x+size_dif*cos((float)(PI / 2)+((float)PI/6)*4*i),y+size_dif*sin((float)(PI / 2)+((float)PI/6)*4*i), 1);
        pts.push_back(temp3);
        points temp5;
        temp5.center.x= temp.x;
        temp5.center.y= temp.y;
        temp5.point=i+4;
        temp5.size = size_dif;
        temp5.type=5;
        pts_data.push_back(temp5);
        colors.push_back(tempColor);
        //third pixel
        glm::vec3  temp2 = glm::vec3(x+size_dif*cos((float)PI/2+((float)PI/6)*4*(i+1)),y+size_dif*sin((float)PI/2+((float)PI/6)*4*(i+1)),1);
        pts.push_back(temp2);
        points temp6;
        temp6.center.x= temp.x;
        temp6.center.y= temp.y;
        temp6.size = size_dif;
        temp6.point=i+5;
        temp6.type=5;
        pts_data.push_back(temp6);
        colors.push_back(tempColor);
    }
}
void GLWidget::Hexagon(int x, int y)
{
    glUseProgram(program);
    glBindVertexArray(vao);
    QRgb colorTemp= image->pixel(x,y);
    int red = qRed(colorTemp);
    int blue = qBlue(colorTemp);
    int green = qGreen(colorTemp);
    glm::vec3 tempColor;
    if(inputColor){
        tempColor = inColor;
        inputColor= false;
    }
    else{
        tempColor = glm::vec3((float)red/255,(float)green/255,(float)blue /255);
    }
    for(int i =0; i < 6;i++)
    {
        //first pixel
        glm::vec3 temp = glm::vec3(x,y, 1);
        pts.push_back(temp);
        points temp4;
        temp4.center.x= temp.x;
        temp4.center.y= temp.y;
        temp4.point=-1;
        temp4.size = size_dif;
        temp4.type=4;
        pts_data.push_back(temp4);
        colors.push_back(tempColor);
        //second pixel
        glm::vec3 temp3 = glm::vec3(x+size_dif*cos(((float)PI/3)*i),y+size_dif*sin(((float)PI/3)*i), 1);
        pts.push_back(temp3);
        points temp5;
        temp5.center.x= temp.x;
        temp5.center.y= temp.y;
        temp5.point=i;
        temp5.size = size_dif;
        temp5.type=4;
        pts_data.push_back(temp5);
        colors.push_back(tempColor);
        //third pixel
        glm::vec3  temp2 = glm::vec3(x+size_dif*cos(((float)PI/3)*(i+1)),y+size_dif*sin(((float)PI/3)*(i+1)),1);
        pts.push_back(temp2);
        points temp6;
        temp6.center.x= temp.x;
        temp6.center.y= temp.y;
        temp6.point=i+1;
        temp6.size = size_dif;
        temp6.type=4;
        pts_data.push_back(temp6);
        colors.push_back(tempColor);
    }
}
void GLWidget::semiCircle(int x, int y)
{
    glUseProgram(program);
    glBindVertexArray(vao);
    QRgb colorTemp= image->pixel(x,y);
    int red = qRed(colorTemp);
    int blue = qBlue(colorTemp);
    int green = qGreen(colorTemp);
    glm::vec3 tempColor;
    if(inputColor){
        tempColor = inColor;
        inputColor= false;
    }
    else{
        tempColor = glm::vec3((float)red/255,(float)green/255,(float)blue /255);
    }
    for(int i =0; i < 12;i++)
    {
        //firt pixel
        glm::vec3 temp = glm::vec3(x,y, 1);
        pts.push_back(temp);
        colors.push_back(tempColor);
        points temp4;
        temp4.center.x= temp.x;
        temp4.center.y= temp.y;
        temp4.point=-1;
        temp4.size = size_dif;
        temp4.type=3;
        pts_data.push_back(temp4);
        //second pixel
        glm::vec3 temp3 = glm::vec3(x+size_dif*cos(((float)PI/12)*i),y+size_dif*sin(((float)PI/12)*i), 1);
        pts.push_back(temp3);
        colors.push_back(tempColor);
        points temp5;
        temp5.center.x= temp.x;
        temp5.center.y= temp.y;
        temp5.size = size_dif;
        temp5.point=i;
        temp5.type=3;
        pts_data.push_back(temp5);
        //third pixel
        glm::vec3  temp2 = glm::vec3(x+size_dif*cos(((float)PI/12)*(i+1)),y+size_dif*sin(((float)PI/12)*(i+1)),1);
        pts.push_back(temp2);
        points temp6;
        temp6.center.x= temp.x;
        temp6.center.y= temp.y;
        temp6.point=i+1;
        temp6.size = size_dif;
        temp6.type=3;
        pts_data.push_back(temp6);
        colors.push_back(tempColor);
    }
}
void GLWidget::Circle(int x, int y)
{
    glUseProgram(program);
    glBindVertexArray(vao);
    QRgb colorTemp= image->pixel(x,y);
    int red = qRed(colorTemp);
    int blue = qBlue(colorTemp);
    int green = qGreen(colorTemp);
    glm::vec3 tempColor;
    if(inputColor){
        tempColor = inColor;
        inputColor= false;
    }
    else{
        tempColor = glm::vec3((float)red/255,(float)green/255,(float)blue /255);
    }
    for(int i =0; i < 24;i++)
    {
        //first pixel
        glm::vec3 temp = glm::vec3(x,y, 1);
        pts.push_back(temp);
        colors.push_back(tempColor);
        points temp4;
        temp4.center.x= temp.x;
        temp4.center.y= temp.y;
        temp4.size = size_dif;
        temp4.point=-1;
        temp4.type=2;
        pts_data.push_back(temp4);
        //second pixel
        glm::vec3 temp3 = glm::vec3(x+size_dif*cos(((float)PI/12)*i),y+size_dif*sin(((float)PI/12)*i), 1);
        pts.push_back(temp3);
        colors.push_back(tempColor);
        points temp5;
        temp5.center.x= temp.x;
        temp5.center.y= temp.y;
        temp5.size = size_dif;
        temp5.point=i;
        temp5.type=2;
        pts_data.push_back(temp5);
        //third pixel
        glm::vec3  temp2 = glm::vec3(x+size_dif*cos(((float)PI/12)*(i+1)),y+size_dif*sin(((float)PI/12)*(i+1)),1);
        pts.push_back(temp2);
        points temp6;
        temp6.center.x= temp.x;
        temp6.center.y= temp.y;
        temp6.point=i+1;
        temp6.size = size_dif;
        temp6.type=2;
        pts_data.push_back(temp6);
        colors.push_back(tempColor);
    }
}

void GLWidget::Square(int x, int y){
    glUseProgram(program);
    glBindVertexArray(vao);
    QRgb colorTemp= image->pixel(x,y);
    int red = qRed(colorTemp);
    int blue = qBlue(colorTemp);
    int green = qGreen(colorTemp);
    glm::vec3 tempColor;
    if(inputColor){
        tempColor = inColor;
        inputColor= false;
    }
    else{
        tempColor = glm::vec3((float)red/255,(float)green/255,(float)blue /255);
    }
    for(int i =0; i < 4;i++)
    {
        //first pixel
        glm::vec3 temp = glm::vec3(x,y, 1);
        pts.push_back(temp);
        colors.push_back(tempColor);
        points temp4;
        temp4.center.x= temp.x;
        temp4.center.y= temp.y;
        temp4.size = size_dif;
        temp4.point=-1;
        temp4.type=0;
        pts_data.push_back(temp4);
        //second pixel
        glm::vec3 temp3 = glm::vec3(x+size_dif*cos((float)(PI / 4)+((float)PI/2)*i),y+size_dif*sin((float)(PI / 4)+((float)PI/2)*i), 1);
        pts.push_back(temp3);
        colors.push_back(tempColor);
        points temp5;
        temp5.center.x= temp.x;
        temp5.center.y= temp.y;
        temp5.size = size_dif;
        temp5.point=i;
        temp5.type=0;
        pts_data.push_back(temp5);
        //third pixel
        glm::vec3  temp2 = glm::vec3(x+size_dif*cos((float)PI/4+((float)PI/2)*(i+1)),y+size_dif*sin((float)PI/4+((float)PI/2)*(i+1)),1);
        pts.push_back(temp2);
        points temp6;
        temp6.center.x= temp.x;
        temp6.center.y= temp.y;
        temp6.size = size_dif;
        temp6.point=i+1;
        temp6.type=0;
        pts_data.push_back(temp6);
        colors.push_back(tempColor);
    }
}

void GLWidget::resizeTriangle(int x){
    glUseProgram(program);
    glBindVertexArray(vao);
    glm::vec3 temp = glm::vec3(pts_data[x].center.x+pts_data[x].size*cos((float)(PI / 6)+((float)PI/6)*4*pts_data[x].point),pts_data[x].center.y+pts_data[x].size*sin((float)(PI / 6)+((float)PI/6)*4*pts_data[x].point), 1);
    pts[x].x=temp.x;
    pts[x].y=temp.y;
}
void GLWidget::resizeTriangle2(int x){
    glUseProgram(program);
    glBindVertexArray(vao);
    glm::vec3 temp = glm::vec3(pts_data[x].center.x+pts_data[x].size*cos((float)(PI / 2)+((float)PI/6)*4*(pts_data[x].point-3)),pts_data[x].center.y+pts_data[x].size*sin((float)(PI / 2)+((float)PI/6)*4*(pts_data[x].point-3)), 1);
    pts[x].x=temp.x;
    pts[x].y=temp.y;
}
void GLWidget::resizeSquare(int x){
    glUseProgram(program);
    glBindVertexArray(vao);
    glm::vec3 temp3 = glm::vec3(pts_data[x].center.x+pts_data[x].size*cos((float)(PI / 4)+((float)PI/2)*pts_data[x].point),pts_data[x].center.y+pts_data[x].size*sin((float)(PI / 4)+((float)PI/2)*pts_data[x].point), 1);
    pts[x].x=temp3.x;
    pts[x].y=temp3.y;
}

void GLWidget::resizeHexagon(int x){
    glUseProgram(program);
    glBindVertexArray(vao);
    glm::vec3 temp3 = glm::vec3(pts_data[x].center.x+(pts_data[x].size)*cos(((float)PI/3)*pts_data[x].point),pts_data[x].center.y+pts_data[x].size*sin(((float)PI/3)*pts_data[x].point), 1);
    pts[x].x=temp3.x;
    pts[x].y=temp3.y;
}

void GLWidget::resizesemiCircle(int x){
    glUseProgram(program);
    glBindVertexArray(vao);
    glm::vec3 temp3 = glm::vec3(pts_data[x].center.x+(pts_data[x].size)*cos(((float)PI/12)*pts_data[x].point),pts_data[x].center.y+pts_data[x].size*sin(((float)PI/12)*pts_data[x].point), 1);
    pts[x].x=temp3.x;
    pts[x].y=temp3.y;
}

void GLWidget::resizeCircle(int x){
    glUseProgram(program);
    glBindVertexArray(vao);
    glm::vec3 temp3 = glm::vec3(pts_data[x].center.x+(pts_data[x].size)*cos(((float)PI/12)*(pts_data[x].point)),pts_data[x].center.y+(pts_data[x].size)*sin(((float)PI/12)*(pts_data[x].point)), 1);
    pts[x].x=temp3.x;
    pts[x].y=temp3.y;
}
void GLWidget::resizeTriangleT(int x){
    glUseProgram(program);
    glBindVertexArray(vao);
    glm::vec3 temp = glm::vec3(pts_data[x].center.x+pts_data[x].nothing*cos((float)(PI / 6)+((float)PI/6)*4*pts_data[x].point),pts_data[x].center.y+pts_data[x].nothing*sin((float)(PI / 6)+((float)PI/6)*4*pts_data[x].point), 1);
    pts[x].x=temp.x;
    pts[x].y=temp.y;
}
void GLWidget::resizeTriangleT2(int x){
    glUseProgram(program);
    glBindVertexArray(vao);
    glm::vec3 temp = glm::vec3(pts_data[x].center.x+pts_data[x].nothing*cos((float)(PI / 2)+((float)PI/6)*4*(pts_data[x].point-3)),pts_data[x].center.y+pts_data[x].nothing*sin((float)(PI / 2)+((float)PI/6)*4*(pts_data[x].point-3)), 1);
    pts[x].x=temp.x;
    pts[x].y=temp.y;
}
void GLWidget::resizeSquareT(int x){
    glUseProgram(program);
    glBindVertexArray(vao);
    glm::vec3 temp3 = glm::vec3(pts_data[x].center.x+pts_data[x].nothing*cos((float)(PI / 4)+((float)PI/2)*pts_data[x].point),pts_data[x].center.y+pts_data[x].nothing*sin((float)(PI / 4)+((float)PI/2)*pts_data[x].point), 1);
    pts[x].x=temp3.x;
    pts[x].y=temp3.y;
}

void GLWidget::resizeHexagonT(int x){
    glUseProgram(program);
    glBindVertexArray(vao);
    glm::vec3 temp3 = glm::vec3(pts_data[x].center.x+(pts_data[x].nothing)*cos(((float)PI/3)*pts_data[x].point),pts_data[x].center.y+pts_data[x].nothing*sin(((float)PI/3)*pts_data[x].point), 1);
    pts[x].x=temp3.x;
    pts[x].y=temp3.y;
}

void GLWidget::resizesemiCircleT(int x){
    glUseProgram(program);
    glBindVertexArray(vao);
    glm::vec3 temp3 = glm::vec3(pts_data[x].center.x+(pts_data[x].nothing)*cos(((float)PI/12)*pts_data[x].point),pts_data[x].center.y+pts_data[x].nothing*sin(((float)PI/12)*pts_data[x].point), 1);
    pts[x].x=temp3.x;
    pts[x].y=temp3.y;
}

void GLWidget::resizeCircleT(int x){
    glUseProgram(program);
    glBindVertexArray(vao);
    glm::vec3 temp3 = glm::vec3(pts_data[x].center.x+(pts_data[x].nothing)*cos(((float)PI/12)*(pts_data[x].point)),pts_data[x].center.y+(pts_data[x].nothing)*sin(((float)PI/12)*(pts_data[x].point)), 1);
    pts[x].x=temp3.x;
    pts[x].y=temp3.y;
}
void GLWidget::initializeGL() {

    initializeOpenGLFunctions();
    if(input == false){
        std::string input;
        cout<<"Input file path for image or enter the word skip to use default"<<endl;
        cin>> input;
        QString in = QString::fromStdString(input);
        image = new QImage();
        if(input.compare("skip")==0)
        {
            if(image->load("sunset.jpg")){
                cout<<"here"<<endl;}
            else{
                cout<<"you fail, cliff"<<endl;
            }
        }
        else if(image->load(in))
        {

        }
        else
        {
            cout<<"Could not read image file. you fail, cliff"<<endl;
        }
    }else{
        QString inTemp = QString::fromStdString(in);
        image = new QImage();
        if(image->load(inTemp)){

        }
        else{
            cout<<"Could not read image file. you fail, cliff"<<endl;
        }
    }
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    // Create a new Vertex Array Object on the GPU which
    // saves the attribute layout of our vertices.

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a buffer on the GPU for position data
    glGenBuffers(1, &positionBuffer);
    glGenBuffers(1, &colorBuffer);
    //glGenBuffers(1, &windowBuffer);
    // Upload the position data to the GPU, storing
    // it in the buffer we just allocated.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
    //    glBindBuffer(GL_ARRAY_BUFFER, windowBuffer);
    //    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
    // Load our vertex and fragment shaders into a program object
    // on the GPU
    // Create a buffer on the GPU for position data
    //    GLuint projectionBuffer;
    //    glGenBuffers(1, &projectionBuffer);

    // Upload the position data to the GPU, storing
    // it in the buffer we just allocated.
    //    glBindBuffer(GL_ARRAY_BUFFER, projectionBuffer);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(projection), projection, GL_STATIC_DRAW);

    program = loadShaders();

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information
    // is stored in our vertex array object.
    //cout<<"here"<<endl;
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);



    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    GLint colorIndex = glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(colorIndex);
    glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //    glBindBuffer(GL_ARRAY_BUFFER, windowBuffer);
    //    GLint windowIndex = glGetAttribLocation(program, "window");
    //    glEnableVertexAttribArray(windowIndex);
    //    glVertexAttribPointer(windowIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //  glGetActiveUniform(projectionBuffer, positionIndex, sizeof(projection), );
    //   glBindBuffer(GL_ARRAY_BUFFER, projectionBuffer);
    //setIdentityMatrix(projection,4);
    //currentIndex = glGetUniformLocation(program, "current");
    projectionIndex = glGetUniformLocation(program, "projection");
    // cout<<projectionIndex<<endl;

    // glVertexAttribPointer(projectionIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void GLWidget::resizeGL(int w, int h) {
    glUseProgram(program);
    glBindVertexArray(vao);
    sizeWid=vec2((float)w,(float)h);
    glViewport(0,0,w,h);
    projection = glm::ortho((float)0.0,(float)w,(float)h,(float)0.0);
    glUniformMatrix4fv(projectionIndex, 1, false, &projection[0][0]);
}
void GLWidget::paintGL() {
    glUseProgram(program);
    glBindVertexArray(vao);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, pts.size());
}

// Copied from LoadShaders.cpp in the the oglpg-8th-edition.zip
// file provided by the OpenGL Programming Guide, 8th edition.
const GLchar* GLWidget::readShader(const char* filename) {
#ifdef WIN32
    FILE* infile;
    fopen_s( &infile, filename, "rb" );
#else
    FILE* infile = fopen( filename, "rb" );
#endif // WIN32

    if ( !infile ) {
#ifdef _DEBUG
        std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
        return NULL;
    }

    fseek( infile, 0, SEEK_END );
    int len = ftell( infile );
    fseek( infile, 0, SEEK_SET );

    GLchar* source = new GLchar[len+1];

    fread( source, 1, len, infile );
    fclose( infile );

    source[len] = 0;

    return const_cast<const GLchar*>(source);
}

GLuint GLWidget::loadShaders() {
    GLuint program = glCreateProgram();

    // read vertex shader from Qt resource file
    QFile vertFile(":/vert.glsl");
    vertFile.open(QFile::ReadOnly | QFile::Text);
    QString vertString;
    QTextStream vertStream(&vertFile);
    vertString.append(vertStream.readAll());
    string vertSTLString = vertString.toStdString();

    cout << "Vertex Shader:" << endl;
    cout << vertSTLString << endl;

    const GLchar* vertSource = vertSTLString.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSource, NULL);
    glCompileShader(vertShader);
    {
        GLint compiled;
        glGetShaderiv( vertShader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            GLsizei len;
            glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( vertShader, len, &len, log );
            std::cout << "Shader compilation failed: " << log << std::endl;
            delete [] log;
        }
    }
    glAttachShader(program, vertShader);

    // read fragment shader from Qt resource file
    QFile fragFile(":/frag.glsl");
    fragFile.open(QFile::ReadOnly | QFile::Text);
    QString fragString;
    QTextStream fragStream(&fragFile);
    fragString.append(fragStream.readAll());
    string fragSTLString = fragString.toStdString();

    cout << "Fragment Shader:" << endl;
    cout << fragSTLString << endl;

    const GLchar* fragSource = fragSTLString.c_str();

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);
    {
        GLint compiled;
        glGetShaderiv( fragShader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            GLsizei len;
            glGetShaderiv( fragShader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( fragShader, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
        }
    }
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glUseProgram(program);

    return program;
}
