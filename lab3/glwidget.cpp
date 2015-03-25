#include "glwidget.h"
#include <iostream>

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QTextStream>

using namespace std;


GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), outline(false), drawMode(0) {
    num_pts = 0;
    num_mode = 0;
    drawMode = GL_POINTS;
}

GLWidget::~GLWidget() {
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_C:
            cout << "Cleared all the points. Make sure to "
                    "update this once you modify how the points "
                    "are stored." << endl;
            pts.clear();
           num_pts = 0;

            glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
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
        case Qt::Key_Space:
            num_mode ++;
            if(num_mode > 6)
                num_mode = 0;
            switch(num_mode){
            case 0:
                cout << "GL_POINTS" << endl;
                drawMode = GL_POINTS;
                break;
            case 1:
                cout << "GL_LINES"  << endl;
                drawMode = GL_LINES;
                break;
            case 2:
                cout << "GL_LINES_STRIP" << endl;
                drawMode = GL_LINE_STRIP;
                break;
            case 4:
                cout << "GL_LINE_LOOP" << endl;
                drawMode = GL_TRIANGLES;
                break;
            case 5:
                cout << "GL_TRIANGELE_STRIP" << endl;
                drawMode = GL_TRIANGLE_STRIP;
                break;
            case 6:
                cout << "GL_TRIANGLE_FAN" << endl;
                drawMode = GL_TRIANGLE_FAN;
                break;
            }

            /*
              Cycle between the following draw modes:
                GL_POINTS,
                GL_LINES,
                GL_LINE_STRIP,
                GL_LINE_LOOP,
                GL_TRIANGLES,
                GL_TRIANGLE_STRIP,
                GL_TRIANGLE_FAN

                */
            break;
    }
    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
<<<<<<< HEAD
    //    if(num_pts < 3) {
            // devicePixelRatio will be 1 on standard displays.
            // On retina display devices (mac book pro retina, tablets, phones, etc)
            // devicePixelRatio will be greater than 1.
            // The coordinates of the event are in points (like font points,
            // not vertex points) rather than pixels.
            // There are devicePixelRatio pixels in a single point, so here
            // we are converting to pixels.
            qreal pixelRatio = this->devicePixelRatio();
            vec2 temp = vec2(event->x()*pixelRatio,event->y()*pixelRatio);
            pts.push_back(temp);

          //  pts.resize(num_pts);
    //        pts[num_pts].x = event->x()*pixelRatio;
    //        pts[num_pts].y = event->y()*pixelRatio;

            cout << "Added point (" << pts[num_pts].x << ", " << pts[num_pts].y << ") " << endl;
            cout << "Make sure your orthographic projection matrix "
                    "is set up so you can see the points." << endl;
    num_pts++;

            cout<<"Amount"<<pts.size()<<endl;
           // cout<<"the size of "<<sizeof(pts)<<endl;
            glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
            glBufferData(GL_ARRAY_BUFFER, pts.size()*GL_FLOAT, &pts[0], GL_DYNAMIC_DRAW);
            update();
    //        vec2 temp[num_pts];
    //        for(int i =0;i<num_pts-1;i++)
=======
//    if(num_pts < 3) {
        // devicePixelRatio will be 1 on standard displays.
        // On retina display devices (mac book pro retina, tablets, phones, etc)
        // devicePixelRatio will be greater than 1.
        // The coordinates of the event are in points (like font points,
        // not vertex points) rather than pixels.
        // There are devicePixelRatio pixels in a single point, so here
        // we are converting to pixels.
        qreal pixelRatio = this->devicePixelRatio();
        vec2 temp = vec2(event->x()*pixelRatio,event->y()*pixelRatio);
        pts.push_back(temp);
                num_pts++;
        pts.resize(num_pts);
//        pts[num_pts].x = event->x()*pixelRatio;
//        pts[num_pts].y = event->y()*pixelRatio;

        cout << "Added point (" << pts[num_pts].x << ", " << pts[num_pts].y << ") " << endl;
        cout << "Make sure your orthographic projection matrix "
                "is set up so you can see the points." << endl;


        cout<<"Amount"<<pts.size()<<endl;
       // cout<<"the size of "<<sizeof(pts)<<endl;
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, pts.size()*sizeof(glm::vec3), &pts[0], GL_DYNAMIC_DRAW);
        update();
//        vec2 temp[num_pts];
>>>>>>> origin/master
//        for(int i =0;i<num_pts-1;i++)
//        {
//            temp[i]= pts[i];
//        }
//        pts = temp;
//    } else {
//        cout << "Three points is the max. "
//                "You must change this to make "
//                "it so any number of points can be created." << endl;
//    }
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    // Create a new Vertex Array Object on the GPU which
    // saves the attribute layout of our vertices.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a buffer on the GPU for position data
    glGenBuffers(1, &positionBuffer);

    // Upload the position data to the GPU, storing
    // it in the buffer we just allocated.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    // Create a buffer on the GPU for position data
//    GLuint projectionBuffer;
//    glGenBuffers(1, &projectionBuffer);

    // Upload the position data to the GPU, storing
    // it in the buffer we just allocated.
//    glBindBuffer(GL_ARRAY_BUFFER, projectionBuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(projection), projection, GL_STATIC_DRAW);

    GLuint program = loadShaders();

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information
    // is stored in our vertex array object.
//cout<<"here"<<endl;
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
      //  glGetActiveUniform(projectionBuffer, positionIndex, sizeof(projection), );
 //   glBindBuffer(GL_ARRAY_BUFFER, projectionBuffer);
//setIdentityMatrix(projection,4);
     projectionIndex = glGetUniformLocation(program, "projection");
   // cout<<projectionIndex<<endl;

   // glVertexAttribPointer(projectionIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void GLWidget::resizeGL(int w, int h) {
    float projectionTemp[16];
    glViewport(0,0,w,h);
for(int i = 0; i<16;i++){

projectionTemp[i]=0;
}

        projectionTemp[0]=(float)2/w;
        projectionTemp[5]=(float)2/h;
        projectionTemp[15]=1;
     //   glm::vec4 thing =glm::vec4(50,50,0,1);
//        float thing2[4][4];
//        for(int i = 0; i<4;i++)
//        {
//            for(int j =0; j <4;j++)
//            {
//                thing2[i][j]= thing[i*4 + j];
//            }
//        }
       // mat4 m3(projection);
        projection = {
            glm::vec4(projectionTemp[0],projectionTemp[1],projectionTemp[2],projectionTemp[3]),
                            glm::vec4(projectionTemp[4],projectionTemp[5],projectionTemp[6],projectionTemp[7]),
                            glm::vec4(projectionTemp[8],projectionTemp[9],projectionTemp[10],projectionTemp[11]),
                            glm::vec4(projectionTemp[12],projectionTemp[13],projectionTemp[14],projectionTemp[15])};
     //   glm::vec4 out = projection * thing;
    //    out = glm::vec4(-1+out.x,1-out.y,0,1);
    glUniformMatrix4fv(projectionIndex, 1, false, &projection[0][0]);
//            cout<<out.x<<endl;
//            cout<<out.y<<endl;

    // When our OpenGL context is resized, we need to change
    // our projection matrix to match. Create an orthographic
    // projection matrix that converts window coordinates to normalized
    // device coordinates.  This is similar to our previous lab,
    // except the conversion will happen in our vertex shader.
    // This way we won't need to do any conversions on our mouse
    // event coordinates and when we resize the window the geometry
    // won't be scaled unevenly.
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    // draw primitives based on the current draw mode
    glDrawArrays(drawMode, 0, num_pts);

    // draw points so we can always see them
    // glPointSize adjusts the size of point
    // primitives
    glDrawArrays(GL_POINTS, 0, num_pts);
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
