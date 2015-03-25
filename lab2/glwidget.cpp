#include <QApplication>
#include "glwidget.h"
#include <iostream>

using namespace std;

//int width;
//int height;
GLWidget::GLWidget(const QGLFormat &format, QWidget *parent) : QGLWidget(format, parent) {
    setAutoBufferSwap(false);


GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {

}

GLWidget::~GLWidget() {
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Create a new Vertex Array Object on the GPU which
    // saves the attribute layout of our vertices.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // position data for a single triangle
 /*   Point pts[3] = {
        Point(0,1),
        Point(1,0),
        Point(0,-1)
    };
*/
	cout<< "Enter 3 points (enter a point as x,y:r,g,b)\n";
	float points[3][2];
	float color[3][3];
	char stuff;
	int i = 0;
	for(i = 0; i<3;i++)
	{
		cin >> points[i][0];
		cin >> stuff;
		cin >> points[i][1];
		cin >> stuff;
		cin >> color[i][0];
		cin >> stuff;
		cin >> color[i][1];
		cin >> stuff;
		cin >> color[i][2];
	}
	cout<<endl;
    for(i = 0; i < 3; i++)
    {
        cout << points[i][0] <<", " << points[i][1] << " : ";
        cout << color[i][0] << ", " <<color[i][1] << ", " <<color[i][2] << "\n";
    }
	Point pts[3] = {
        Point(points[0][0],points[0][1]),

        Point(points[1][0],points[1][1]),
        Point(points[2][0],points[2][1])
    };
	
    pts[0] = w2nd(pts[0]);
    pts[1] = w2nd(pts[1]);
    pts[2] = w2nd(pts[2]);
    // Create a buffer on the GPU for position data
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    // Upload the position data to the GPU, storing
    // it in the buffer we just created.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);


    // Create a buffer on the GPU for position data
    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);

    // Upload the position data to the GPU, storing
    // it in the buffer we just created.
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);


        // Load our vertex and fragment shaders into a program object
    // on the GPU
    GLuint program = loadShaders();

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information 
    // is stored in our vertex array object.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex,2 , GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    GLint colorIndex = glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(colorIndex);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

}

void GLWidget::resizeGL(int w, int h) {
    width = w;
    height = h;
    glViewport(0,0,w,h);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

GLuint GLWidget::loadShaders() {
    GLuint program = glCreateProgram();
 //   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    const GLchar* vertSource = 
        "#version 330\n"
        "in vec2 position;\n"
        "in vec3 color;\n"
        "out vec4 theColor;\n"
        "void main() {\n"
        "  gl_Position = vec4(position.x, position.y, 0, 1);\n"
        "  theColor = vec4(color.r,color.g,color.b,1.0);\n"
        "}\n";
       // "  gl_Color = vec4(position.r, position.g, position.b, 1.0);\n"
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
    
    const GLchar* fragSource = 
        "#version 330\n"
        "in vec4 theColor;\n"
        "out vec4 color_out;\n"
        "void main() {\n"
                "  color_out = theColor;\n"
        "}\n";
               // "in vec4 gl_Color;\n"
              //  "  color_out = gl_Color;\n"
    //  "  color_out = vec4(position.r,position.g,postion.b,1.0);\n"
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

Point GLWidget::w2nd(Point pt_w) {
    pt_w.x = -1 + (pt_w.x *(2/width));
    pt_w.y = 1-(pt_w.y*(2/height));
    return pt_w;
    /* convert pt_w to normalized device coordinates */
    /* use this method to convert your input coordinates to
       normalized device coordinates */
    return pt_w;
}


