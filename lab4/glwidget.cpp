#include "glwidget.h"
#include <iostream>
#include <QGLPixelBuffer>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QTextStream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::value_ptr;
using glm::ortho;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    tx = 0;
    ty = 0;
    tz = 0;

    sx = 1;
    sy = 1;
    sz = 1;

    rx = 0;
    ry = 0;
    rz = 0;
    camY=20;
    camAngle = 0;
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
    viewLoc2 = glGetUniformLocation(program, "view");
    // Part 2 - Get any uniform variable locations that you'll need.
}

void GLWidget::initializeCube() {
    // Create a new Vertex Array Object on the GPU which
    // saves the attribute layout of our vertices.
    glGenVertexArrays(1, &cubeVao);
    glBindVertexArray(cubeVao);

    // Create a buffer on the GPU for position data
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    GLuint colorBuffer;
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

    vec3 colors[] = {
        // top
        vec3(0,1,0),
        vec3(0,1,0),
        vec3(0,1,0),
        vec3(0,1,0),

        // bottom
        vec3(0,.5f,0),
        vec3(0,.5f,0),
        vec3(0,.5f,0),
        vec3(0,.5f,0),

        // front
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),
        vec3(0,0,1),

        // back
        vec3(0,0,.5f),
        vec3(0,0,.5f),
        vec3(0,0,.5f),
        vec3(0,0,.5f),

        // right
        vec3(1,0,0),
        vec3(1,0,0),
        vec3(1,0,0),
        vec3(1,0,0),


        // left
        vec3(.5f,0,0),
        vec3(.5f,0,0),
        vec3(.5f,0,0),
        vec3(.5f,0,0)
    };

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

    //GLuint  program1 = loadShaders(":/vert.glsl", ":/frag.glsl");

   GLuint  program1 = loadShaders(":/vert.glsl", ":/frag.glsl");

    glUseProgram(program1);
    cubeProg = program1;

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information
    // is stored in our vertex array object.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program1, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    GLint colorIndex = glGetAttribLocation(program1, "color");
    glEnableVertexAttribArray(colorIndex);
    glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    cubeProjMatrixLoc = glGetUniformLocation(program1, "projection");
    viewLoc = glGetUniformLocation(program1, "view");

    // std::cout<<viewLoc<<endl;
    modelLoc = glGetUniformLocation(program1, "model");
    //     std::cout<<modelLoc<<endl;

    // Part 2 & 3 - Get any uniform variable locations that you'll need.
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    glEnable(GL_DEPTH_TEST);
    GLuint restart = 0xFFFFFFFF;
    glPrimitiveRestartIndex(restart);
    glEnable(GL_PRIMITIVE_RESTART);

    initializeCube();
    initializeGrid();
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0,0,w,h);
    //std::cout<<"here"<<endl;
    float aspect = (float)w/h;
    std::cout<<w<<std::endl;
    std::cout<<h<<std::endl;
    float width = (float)w;
    float height = (float)h;

    //    float n =100;
    //    float f =1;
    // projMatrix = glm::ortho(-5.0f*aspect, 5.0f*aspect, -5.0f, 5.0f, -10.0f, 100.0f);
    // Part 1 - Instead of using ortho, construct your own perspective matrix.
    // Do not use glm::perspective.
    //   glm::mat4 projMatrix1 = glm::mat4{glm::vec4(n/(width),0,0,0),
    //                           glm::vec4(0,n/(height),0,0),
    //                           glm::vec4(0,0,-1*((f+n)/(f-n)),-1.0),
    //                           glm::vec4(0,0,-2*(f*n)/(f-n),0)};
    float n=1;
    float  f = 100;


    //projMatrix = glm::perspective(45.0f,aspect,1.0f,100.0f);
    for(int i = 0; i<4; i++)
    {
        for(int j =0;j<4;j++)
        {
            std::cout<<projMatrix[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"here"<<std::endl;
    //   for(int i = 0; i<4; i++)
    //   {
    //       for(int j =0;j<4;j++)
    //       {
    //           std::cout<<projMatrix1[i][j]<<" ";
    //       }
    //       std::cout<<std::endl;
    //   }

    glm::mat4 projMatrix2 = glm::mat4{
            glm::vec4(1/(aspect*tan(M_PI/180*45.0f/2)),0,0,0),
            glm::vec4(0,1/tan(M_PI/180*45.0f/2),0,0),
            glm::vec4(0,0,-1*((f+n)/(f-n)),-1.0),
            glm::vec4(0,0,-2*(f*n)/(f-n),0)};
    for(int i = 0; i<4; i++)
    {
        for(int j =0;j<4;j++)
        {
            std::cout<<projMatrix2[i][j]<<" ";
        }
        std::cout<<std::endl;
    }

    view = glm::lookAt(
                glm::vec3(20*cos(camAngle),camY,20*sin(camAngle)),
                glm::vec3(0,0,0),
                glm::vec3(0,1,0));
    model=mat4{vec4(1.0f,0.0f,0.0f,0.0f),

            vec4(0.0f,1.0f,0.0f,0.0f),
            vec4(0.0f,0.0f,1.0f,0.0f),
            vec4(0.0f,0.0f,0.0f,1.0f)};
    //    view=mat4{vec4(1.0f,0.0f,0.0f,0.0f),
    //               vec4(0.0f,1.0f,0.0f,0.0f),
    //               vec4(0.0f,0.0f,1.0f,0.0f),
    //               vec4(0.0f,0.0f,0.0f,1.0f)};
    glBindVertexArray(cubeVao);
    glBindVertexArray(gridVao);
    //  glUniformMatrix4fv(modelLoc, 1, false, &model[0][0]);
    glUseProgram(cubeProg);
    glUniformMatrix4fv(modelLoc, 1, false, &model[0][0]);
    glUseProgram(cubeProg);
    glUniformMatrix4fv(viewLoc, 1, false, &view[0][0]);
    glUseProgram(gridProg);
    glUniformMatrix4fv(viewLoc2, 1, false, &view[0][0]);
    // projMatrix=glFrustum(0,width,height,0,-20.0,20.0);
    glUseProgram(cubeProg);
    glUniformMatrix4fv(cubeProjMatrixLoc, 1, false, value_ptr(projMatrix2));

    glUseProgram(gridProg);
    glUniformMatrix4fv(gridProjMatrixLoc, 1, false, value_ptr(projMatrix2));
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGrid();
    renderCube();
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

void GLWidget::translateX(int x) {
    tx = (float)x/10;
    std::cout << "Translate X " << tx << std::endl;
    updateModelMatrix();
}

void GLWidget::translateY(int y) {
    ty = (float)y/10;
    std::cout << "Translate Y " << ty << std::endl;
    updateModelMatrix();
}

void GLWidget::translateZ(int z) {
    tz = (float)z/10;
    std::cout << "Translate Z " << tz << std::endl;
    updateModelMatrix();
}

void GLWidget::rotateX(int x) {
    std::cout << "Rotate X " << x << std::endl;
    rx = (float)x*M_PI/180;
    updateModelMatrix();
}

void GLWidget::rotateY(int y) {
    std::cout << "Rotate Y " << y << std::endl;
    ry = (float)y*M_PI/180;
    updateModelMatrix();
}

void GLWidget::rotateZ(int z) {
    std::cout << "Rotate Z " << z << std::endl;
    rz = (float)z*M_PI/180;
    updateModelMatrix();
}

void GLWidget::scaleX(int x) {
    sx = (float)x/10;
    std::cout << "Scale X " << sx << std::endl;
    updateModelMatrix();
}

void GLWidget::scaleY(int y) {
    sy = (float)y/10;
    std::cout << "Scale Y " << sy << std::endl;
    updateModelMatrix();
}

void GLWidget::scaleZ(int z) {
    sz = (float)z/10;
    std::cout << "Scale Z " << sz << std::endl;
    updateModelMatrix();
}

void GLWidget::cameraPosition(int angle) {
    camAngle = angle*M_PI/180;
    std::cout << "Camera angle " << angle << std::endl;
    updateViewMatrix();
}

void GLWidget::cameraY(int y) {
    camY = (float)y/10;
    std::cout << "Camera Y " << y << std::endl;
    updateViewMatrix();
}

void GLWidget::updateModelMatrix() {

    //    glUseProgram(cubeProg);
   // glBindVertexArray(cubeVao);
//    model =mat4{vec4(sx*cos(ry)*cos(rz), sin(rz),           -sin(ry),              0),
//            vec4(-sin(rz),           sy*cos(rx)*cos(rz), sin(rx),              0),
//            vec4(sin(ry),           -sin(rx),            sz*cos(rx)*cos(ry),   0),
//            vec4(tx,                 ty,                -tz,                   1)};
//    //    model =mat4{vec4(sx, 0,0,0),
    //                vec4(0,sy, 0,0),
    //                vec4(0,0,sz,0),
    //                vec4(tx,ty,-tz,1)};
    mat4 temp1 = mat4{
            vec4(1,0,0,0),
            vec4(0,1,0,0),
            vec4(0,0,1,0),
            vec4(tx,ty,tz,1)};
//    mat4 temp4 = glm::inverse(temp1);
    mat4 temp2 =mat4{vec4(sx, 0,0,0),
            vec4(0,sy, 0,0),
            vec4(0,0,sz,0),
            vec4(0,0,0,1)};
//    mat4 temp3 =mat4{vec4(cos(ry)*cos(rz),    -sin(rz),           sin(ry),               0),
//            vec4(sin(rz),            cos(rx)*cos(rz),    -sin(rx),              0),
//            vec4(-sin(ry),           sin(rx),            cos(rx)*cos(ry),       0),
//            vec4(0,                  0,                  0,                     1)};
    mat4 temp6 = mat4{vec4(1,0,0,0),
                      vec4(0,cos(rx),sin(rx),0),
                      vec4(0,-sin(rx),cos(rx),0),
                      vec4(0,0,0,1)};
    mat4 temp7 = mat4{vec4(cos(ry),0, -sin(ry),0),
            vec4(0,1,0,0),
            vec4(sin(ry),0,cos(ry),0),
            vec4(0,0,0,1)};
    mat4 temp8 = mat4{vec4(cos(rz), sin(rz), 0,0),
            vec4( -sin(rz),cos(rz),0,0),
            vec4(0,0,1,0),
            vec4(0,0,0,1)};

//    mat4 temp3 = glm::inverse(temp6)*glm::inverse(temp7)*glm::inverse(temp8);
    mat4 temp3 = temp6*temp7*temp8;
    model = temp1*temp3*temp2;

    // mat4 invtmodel = glm::inverse(model);
    //    model=mat4{vec4(1.0f,1.0f,1.0f,1.0f),
    //               vec4(1.0f,1.0f,1.0f,1.0f),
    //               vec4(1.0f,1.0f,1.0f,1.0f),
    //               vec4(1.0f,1.0f,1.0f,1.0f)};
    //vec4 temp = model * vec4(1,1,1,1);
    //std::cout<<std::endl<<temp.x<<", "<<temp.y<<", "<<temp.z<<std::endl;
    //vec4 temp2 = model * vec4(-1,-1,-1,1);
    //std::cout<<temp2.x<<", "<<temp2.y<<", "<<temp2.z<<std::endl;
    //vec4 temp3 = model * vec4(1,-1,1,1);
    //std::cout<<temp3.x<<", "<<temp3.y<<", "<<temp3.z<<std::endl;
    //  glUniformMatrix4fv(modelLoc, 1, false, &model[0][0]);
    glUseProgram(cubeProg);
    glUniformMatrix4fv(modelLoc, 1, false, &model[0][0]);

    // glUseProgram(gridProg);
    // glUniformMatrix4fv(modelLoc, 1, false,&model[0][0]);

    // Part 3 - Construct a combined model matrix from the translation,
    // rotation and scale values and upload it as a uniform variable to
    // the cube program (don't use it to change the grid). Update your
    // vertex shader accordingly.
}

void GLWidget::updateViewMatrix() {

//    glUseProgram(cubeProg);
//    glBindVertexArray(cubeVao);

    view = glm::lookAt(
                glm::vec3(20*cos(camAngle),camY,20*sin(camAngle)),
                glm::vec3(0,0,0),
                glm::vec3(0,1,0));
//   view =mat4{vec4(1.0f,0.0f,0.0f,0.0f),
//              vec4(0.0f,1.0f,0.0f,0.0f),
//              vec4(0.0f,0.0f,1.0f,0.0f),
//              vec4(0.0f,0.0f,0.0f,1.0f)};
   // glUniformMatrix4fv(viewLoc, 1, false, &view[0][0]);

    glUseProgram(cubeProg);
    glUniformMatrix4fv(viewLoc, 1, false, &view[0][0]);
    glUseProgram(gridProg);
    glUniformMatrix4fv(viewLoc2, 1, false, &view[0][0]);
    // glUseProgram(gridProg);
    // glUniformMatrix4fv(viewLoc, 1, false,&view[0][0]);// Part 2 - Construct a view matrix and upload as a uniform variable
    // to the cube and grid programs. Update your vertex shader accordingly.
}
