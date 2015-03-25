#ifndef __GLWIDGET__INCLUDE__
#define __GLWIDGET__INCLUDE__

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMouseEvent>
#include <glm/glm.hpp>
#include <QtGui>
// glm by default uses degrees, but that functionality
// is deprecated so GLM_FORCE_RADIANS turns off some 
// glm warnings
#define GLM_FORCE_RADIANS

using glm::vec2;
class points{
    //glm::vec2 coord;


    public:
        int type;
        glm::vec2 center;
        int point;
        int size;
        int nothing=rand();
        //glm::vec2 window;
};
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    public:
    std::string in;
    bool input = false;
        GLWidget(QWidget *parent=0);
        ~GLWidget();
                QImage * image;

    protected:
        void GLWidget::Triangle(int x, int y);
        void GLWidget::Square(int x, int y);
        void GLWidget::Circle(int x, int y);
        void GLWidget::semiCircle(int x, int y);
        void GLWidget::Hexagon(int x, int y);
        void GLWidget::Star(int x, int y);

        void GLWidget::resizeTriangle(int x);
        void GLWidget::resizeTriangle2(int x);
        void GLWidget::resizeSquare(int x);
        void GLWidget::resizeHexagon(int x);
        void GLWidget::resizesemiCircle(int x);
        void GLWidget::resizeCircle(int x);

        void GLWidget::resizeTriangleT(int x);
        void GLWidget::resizeTriangleT2(int x);
        void GLWidget::resizeSquareT(int x);
        void GLWidget::resizeHexagonT(int x);
        void GLWidget::resizesemiCircleT(int x);
        void resizeCircleT(int x);

        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);

    private:
        bool inputColor;
        glm::vec3 inColor;
        vec2 sizeWid;
        GLuint loadShaders();
        static const GLchar* readShader(const char* filename);
        GLuint positionBuffer;
        GLuint colorBuffer;
        bool outline;
        int drawMode;
        glm::mat4 projection;
        GLint projectionIndex;
        GLuint currentBuffer;
        GLuint currentIndex;
        GLuint program;
        GLuint vao;
        std::vector<glm::vec3> pts;
        std::vector<glm::vec2> window;
        std::vector<glm::vec3> colors;
        std::vector<points> pts_data;
        int num_pts;
        int num_mode;
        float size_dif;
};

#endif
