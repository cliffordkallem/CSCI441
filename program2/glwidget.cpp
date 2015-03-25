#include "glwidget.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QTextStream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using glm::vec2;
using glm::vec3;
using glm::mat4;
using glm::perspective;
using glm::normalize;
using glm::length;
using glm::cross;
using glm::dot;
using glm::rotate;
using glm::value_ptr;
using glm::lookAt;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) { 
}

GLWidget::~GLWidget() {
}

void GLWidget::initializeGrid() {
    glGenVertexArrays(1, &gridVao);
    glBindVertexArray(gridVao);

    // Create a buffer on the GPU for position data
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    vec3 pts[84];
    for(int i = -10; i <= 10; i++) {

        pts[2*(i+10)] = vec3(i, -.5f, 10);
        pts[2*(i+10)+1] = vec3(i, -.5f, -10);

        pts[2*(i+10)+42] = vec3(10,-.5f, i);
        pts[2*(i+10)+43] = vec3(-10,-.5f, i);
    }

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    GLuint program = loadShaders(":/grid_vert.glsl", ":/grid_frag.glsl");
    glUseProgram(program);
    gridProg = program;

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information
    // is stored in our vertex array object.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    gridProjMatrixLoc = glGetUniformLocation(program, "projection");
    gridViewMatrixLoc = glGetUniformLocation(program, "view");
    gridModelMatrixLoc = glGetUniformLocation(program, "model");
    gridTrackballMatrixLoc = glGetUniformLocation(program, "trackball");
}

void GLWidget::initializeCube() {
    // Create a new Vertex Array Object on the GPU which
    // saves the attribute layout of our vertices.
    glGenVertexArrays(1, &cubeVao);
    glBindVertexArray(cubeVao);

    // Create a buffer on the GPU for position data
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);


    glGenBuffers(1, &colorBuffer);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);

    vec3 pts[] = {
        // top
        vec3(1,1,1),    // 0
        vec3(1,1,-1),   // 1
        vec3(-1,1,-1),  // 2
        vec3(-1,1,1),   // 3

        // bottom
        vec3(1,-1,1),   // 4
        vec3(-1,-1,1),  // 5
        vec3(-1,-1,-1), // 6
        vec3(1,-1,-1),  // 7

        // front
        vec3(1,1,1),    // 8
        vec3(-1,1,1),   // 9
        vec3(-1,-1,1),  // 10
        vec3(1,-1,1),   // 11

        // back
        vec3(-1,-1,-1), // 12
        vec3(-1,1,-1),  // 13
        vec3(1,1,-1),   // 14
        vec3(1,-1,-1),  // 15

        // right
        vec3(1,-1,1),   // 16
        vec3(1,-1,-1),  // 17
        vec3(1,1,-1),   // 18
        vec3(1,1,1),     // 19

        // left
        vec3(-1,-1,1),  // 20
        vec3(-1,1,1),   // 21
        vec3(-1,1,-1),  // 22
        vec3(-1,-1,-1) // 23

    };

    for(int i = 0; i < 24; i++) {
        pts[i] *= .5;
    }
    //    colors[] = {
    //        // top
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),

    //        // bottom
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),

    //        // front
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),

    //        // back
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),

    //        // right
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),

    //        // left
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //        vec3(0.58823529411,0.09803921568,0.05490196078),
    //    };
    for(int i = 0; i<24; i++)
    {
        colors[i] =   vec3(0.58823529411,0.09803921568,0.05490196078);
    }
    for(int i = 0; i<24; i++)
    {
        motar[i] =   vec3(0.33725490196,0.31372549019,0.31764705882);
    }
    GLuint restart = 0xFFFFFFFF;
    GLuint indices[] = {
        0,1,2,3, restart,
        4,5,6,7, restart,
        8,9,10,11, restart,
        12,13,14,15, restart,
        16,17,18,19, restart,
        20,21,22,23
    };

    // Upload the position data to the GPU, storing
    // it in the buffer we just allocated.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    GLuint program = loadShaders(":/vert.glsl", ":/frag.glsl");
    glUseProgram(program);
    cubeProg = program;

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information
    // is stored in our vertex array object.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    GLint colorIndex = glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(colorIndex);
    glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    cubeProjMatrixLoc = glGetUniformLocation(program, "projection");
    cubeViewMatrixLoc = glGetUniformLocation(program, "view");
    cubeModelMatrixLoc = glGetUniformLocation(program, "model");
    cubeTrackballMatrixLoc = glGetUniformLocation(program, "trackball");
}
void GLWidget::render(mat4 thing)
{
    glUseProgram(cubeProg);
    glBindVertexArray(cubeVao);
    glUseProgram(cubeProg);
    glUniformMatrix4fv(cubeModelMatrixLoc, 1, false, value_ptr(thing));
    glUniformMatrix4fv(cubeProjMatrixLoc, 1, false, value_ptr(projMatrix));
    glUniformMatrix4fv(cubeViewMatrixLoc, 1, false, value_ptr(viewMatrix));
    //   glUniformMatrix4fv(cubeModelMatrixLoc, 1, false, value_ptr(modelMatrix));
    glUniformMatrix4fv(cubeTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));
    //paintGL();
    renderCube();
    //update();
}

void GLWidget::wall(vec3 pos)
{
    mat4 translate;
    mat4 scale;
    mat4 scaleMoS;
    mat4 scaleMT;
    // std::cout<<"here"<<std::endl;
    for(float j = 0;j<=3;j+=1.2){
        for(float i = -3; i<= 3; i+=1.2)    {
            translate = glm::translate(mat4(1.0f), vec3(0.0f+(float)i+pos.x,0+j+pos.y,3+pos.z));
            scale= glm::scale(mat4(1.0f), vec3(1,0.5f,.5 ));
            brink( translate*scale);

            //mat4 rotating = glm::rotate(mat4(1.0f),0.0f,vec3(0,1,0) );

            translate = glm::translate(mat4(1.0f), vec3(0.6f+(float)i+pos.x,.6+j+pos.y,3+pos.z));
            brink(translate*scale);
            // update();
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(motar), motar, GL_STATIC_DRAW);
//    for(float i=0;i<=3;i+=1.2){
//        for(float j = -3;j<=3;j+=1.2){
//            glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
//            glBufferData(GL_ARRAY_BUFFER, sizeof(motar), motar, GL_STATIC_DRAW);
//            scaleMoS= glm::scale(mat4(1.0f), vec3(.25,.5,.4));
//            translate = glm::translate(mat4(1.0f), vec3(.6+j,i,3));
//            brink( translate*scaleMoS);

//            scaleMoS= glm::scale(mat4(1.0f), vec3(.25,.5,.4));
//            translate = glm::translate(mat4(1.0f), vec3(j,i+.6,3));
//            brink( translate*scaleMoS);

//            scaleMoS= glm::scale(mat4(1.0f), vec3(1.2,.2,.4));
//            translate = glm::translate(mat4(1.0f), vec3(j,i-.25,3));
//            brink( translate*scaleMoS);
//            scaleMoS= glm::scale(mat4(1.0f), vec3(1.2,.2,.4));
//            translate = glm::translate(mat4(1.0f), vec3(j,i+.35,3));
//            brink( translate*scaleMoS);
//        }
//    }
    scaleMoS= glm::scale(mat4(1.0f), vec3(6*1.2,3*1.2,.4));
    translate = glm::translate(mat4(1.0f), vec3(.2,1.2*1.2,3));
    brink( translate*scaleMoS);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    //    for(float j = 0;j<=3;j+=1.2){
    //    for(float i = -3; i<= 3; i+=1.2)    {
    //        translate = glm::translate(mat4(1.0f), vec3(3.6,0.0+j+pos.y,(i+pos.x)));
    //        scale= glm::scale(mat4(1.0f), vec3(.5,0.5f,1 ));
    //        brink( translate*scale);
    //        translate = glm::translate(mat4(1.0f), vec3(3.6,.6f+ j+pos.y,(i+pos.x)-.5));
    //        brink(translate*scale);

    //    }
    //}
    for(float j = 0;j<=3;j+=1.2){
        for(float i = -3; i<= 3; i+=1.2)    {
            translate = glm::translate(mat4(1.0f), vec3(3.85,0.0+j+pos.y,(0.0-i+pos.x)-.25));
            scale= glm::scale(mat4(1.0f), vec3(.5,0.5f,1 ));
            brink( translate*scale);
            translate = glm::translate(mat4(1.0f), vec3(3.85,.6f+ j+pos.y,(0.0-i+pos.x)-.85));
            brink(translate*scale);


        }
    }
    vec3 colors2[] = {
        // top
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),

        // bottom
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),

        // front
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),

        // back
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),
        // right
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),

        // left
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),
    };
    //GLuint colorBuffer;
    //glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors2), colors2, GL_STATIC_DRAW);
    for(float j = 0;j<=3;j+=1.2){
        for(float i = -3; i<= 3; i+=1.2)    {
            translate = glm::translate(mat4(1.0f), vec3(-3.25,0.0+j+pos.y,(0.0-i+pos.x)-.85));
            scale= glm::scale(mat4(1.0f), vec3(.5,0.5f,1 ));
            brink( translate*scale);
            translate = glm::translate(mat4(1.0f), vec3(-3.25,.6f+ j+pos.y,(0.0-i+pos.x)-.25));
            brink(translate*scale);

        }
    }


    //glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    for(float j = 0;j<=3;j+=1.2){
        for(float i = -3; i<= 3; i+=1.2)    {
            translate = glm::translate(mat4(1.0f), vec3((0.6f+(float)i+pos.x),0+j+pos.y,-4.125+pos.z));
            scale= glm::scale(mat4(1.0f), vec3(1,0.5f,.5 ));
            //mat4 rotating = glm::rotate(mat4(1.0f),0.0f,vec3(0,1,0) );
            brink( translate*scale);
            translate = glm::translate(mat4(1.0f), vec3(((float)i+pos.x),.6+j+pos.y,-4.125+pos.z));
            brink(translate*scale);
            // update();
        }
    }
}
//pos = model *position *
//normal = transpose(inverse(model))*vec4(normal,0);
void GLWidget::brink(mat4 change)
{

    render(    modelMatrix*change);
    // update();
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    glEnable(GL_DEPTH_TEST);
    GLuint restart = 0xFFFFFFFF;
    glPrimitiveRestartIndex(restart);
    glEnable(GL_PRIMITIVE_RESTART);

    //initializeCube();
    //wall
    initializeCube();
    initializeGrid();
}

void GLWidget::resizeGL(int w, int h) {
    width = w;
    height = h;

    float aspect = (float)w/h;

    projMatrix = perspective(45.0f, aspect, 1.0f, 100.0f);
    viewMatrix = lookAt(vec3(0,0,-10),vec3(0,0,0),vec3(0,1,0));
    vec3 id = vec3(1.0f);
    //mat4 start = glm::translate(id, vec3(2,2,0));

    modelMatrix = mat4(1.0f);

    wall(id);
    //    glUseProgram(cubeProg);
    //    glUniformMatrix4fv(cubeProjMatrixLoc, 1, false, value_ptr(projMatrix));
    //    glUniformMatrix4fv(cubeViewMatrixLoc, 1, false, value_ptr(viewMatrix));
    //    glUniformMatrix4fv(cubeModelMatrixLoc, 1, false, value_ptr(modelMatrix));
    //    glUniformMatrix4fv(cubeTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));
    glUseProgram(gridProg);
    glUniformMatrix4fv(gridProjMatrixLoc, 1, false, value_ptr(projMatrix));
    glUniformMatrix4fv(gridViewMatrixLoc, 1, false, value_ptr(viewMatrix));
    glUniformMatrix4fv(gridModelMatrixLoc, 1, false, value_ptr(modelMatrix));
    glUniformMatrix4fv(gridTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGrid();

    //mat4 id = mat4(1.0f);
    vec3 id = vec3(0,0,0);
    wall(id);
    // renderCube();
}

void GLWidget::renderCube() {
    glUseProgram(cubeProg);
    glBindVertexArray(cubeVao);
    glDrawElements(GL_TRIANGLE_FAN, 29, GL_UNSIGNED_INT, 0);
}

void GLWidget::renderGrid() {
    glUseProgram(gridProg);
    glBindVertexArray(gridVao);
    glDrawArrays(GL_LINES, 0, 84);
}

GLuint GLWidget::loadShaders(const char* vertf, const char* fragf) {
    GLuint program = glCreateProgram();

    // read vertex shader from Qt resource file
    QFile vertFile(vertf);
    vertFile.open(QFile::ReadOnly | QFile::Text);
    QString vertString;
    QTextStream vertStream(&vertFile);
    vertString.append(vertStream.readAll());
    std::string vertSTLString = vertString.toStdString();

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
    QFile fragFile(fragf);
    fragFile.open(QFile::ReadOnly | QFile::Text);
    QString fragString;
    QTextStream fragStream(&fragFile);
    fragString.append(fragStream.readAll());
    std::string fragSTLString = fragString.toStdString();

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

    return program;
}

// Part 2 - As we click and drag, use the pointOnVirtualTrackball
// to construct an angle and axis which we can use to create a
// rotation matrix with using glm::rotate.
// Keep track of the last mouse click or movement along with the
// current one to perform the above calulation. Continuously
// multiply the matrices to accumulate a single trackball rotation
// matrix.
void GLWidget::mousePressEvent(QMouseEvent *event) {
    vec2 pt(event->x(), event->y());
    lastPt = pointOnVirtualTrackball(pt);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    vec2 pt(event->x(), event->y());
    vec3 pOVT = pointOnVirtualTrackball(pt);//

    //vec3 lastPt = pOVT;

    vec3 V1 = normalize(pOVT);//
    vec3 V2 = normalize(lastPt);

    vec3 axis = cross(V2, V1);//
    if( length(axis)> .000001)
    {
        axis = normalize(axis);
        float angle = acos(dot(V2,V1));

        mat4 rot = rotate(mat4(1.0), angle, axis);
        trackballMatrix = rot*trackballMatrix;

    }
    lastPt = pOVT;
    glUseProgram(cubeProg);
    glUniformMatrix4fv(cubeTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));
    glUseProgram(gridProg);
    glUniformMatrix4fv(gridTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));

    update();
}

vec3 GLWidget::pointOnVirtualTrackball(const vec2 &pt) {
    // Part 1 - implement the equations for a virtual trackball to
    // convert the provided pt parameter to a 3D point on the virtual
    // trackball surface.
    int r = 1;
    vec3 p;

    p.x = (-1+pt.x*(2.f/width));
    p.y = -(float)height/width*(1-pt.y*(2.f/height));

    if(p.x*p.x+p.y <= r*r*.5){
        p.z = sqrt(r*r-(p.x*p.x + p.y*p.y)); //etc
    }
    else{
        p.z = (r*r *.5)/sqrt(p.x*p.x +p.y*p.y);
    }
    return p;


}
/*
mat4 modelMatrix;

    int width;
    int height;

    vec3 lastPt;

    glm::vec3 pointOnVirtualTrackball(const glm::vec2 &pt);

*/

