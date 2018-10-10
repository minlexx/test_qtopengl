#include <QLoggingCategory>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_3_0>
#include <QOpenGLContext>
#include "widget.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

Widget::~Widget()
{

}

void Widget::resizeGL(int w, int h)
{
    qDebug() << Q_FUNC_INFO << w << h;
}

static const char *vertexShaderCode =
"                                    \
    in vec4 position;                \
    void main() {                    \
        gl_Position = position;      \
    }                                \
";

static const char *fragmentShaderCode =
"\
    #version 130\n\
                                              \
    out vec4 out_color;                       \
    void main() {                             \
        out_color = vec4(1.0, 1.0, 1.0, 1.0); \
    }                                         \
";


//static GLuint load_shader(QOpenGLFunctions *f, const char *shader_source, GLenum type)
static GLuint load_shader(QOpenGLFunctions_3_0 *f, const char *shader_source, GLenum type)
{
    GLuint  shader = f->glCreateShader(type);
    f->glShaderSource(shader, 1, &shader_source, nullptr);
    f->glCompileShader(shader);
    GLint test = 0;
    f->glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
    if(!test) {
        char compilation_log[512] = {0};
        f->glGetShaderInfoLog(shader, 511, nullptr, compilation_log);
        qWarning() << "Shader compilation failed with this message:" << compilation_log;
        exit(-1);
    }
    return shader;
}

void Widget::initializeGL()
{
    //QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QOpenGLFunctions_3_0 *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();
    if (!f) {
        return;
    }
    f->glClearColor(1.0f, .5f, .5f, 1.0f);

    // load shaders
    GLuint vertexShader = load_shader(f, vertexShaderCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = load_shader(f, fragmentShaderCode, GL_FRAGMENT_SHADER);

    // Attach the above shader to a program
    GLuint shaderProgram = f->glCreateProgram();
    f->glAttachShader(shaderProgram, vertexShader);
    f->glAttachShader(shaderProgram, fragmentShader);

    // Flag the shaders for deletion
    f->glDeleteShader(vertexShader);
    f->glDeleteShader(fragmentShader);

    // Link and use the program
    f->glLinkProgram(shaderProgram);
    f->glUseProgram(shaderProgram);

    static GLfloat vertices_position[24] = {
        0.0, 0.0,
        0.5, 0.0,
        0.5, 0.5,

        0.0, 0.0,
        0.0, 0.5,
        -0.5, 0.5,

        0.0, 0.0,
        -0.5, 0.0,
        -0.5, -0.5,

        0.0, 0.0,
        0.0, -0.5,
        0.5, -0.5,
    };

    // Use a Vertex Array Object
    f->glGenVertexArrays(1, &vao);
    f->glBindVertexArray(vao);

    // Create a Vector Buffer Object that will store the vertices on video memory
    GLuint vbo;
    f->glGenBuffers(1, &vbo);

    // Allocate space and upload the data from CPU to GPU
    f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position), vertices_position, GL_STATIC_DRAW);

    // Get the location of the attributes that enters in the vertex shader
    GLuint position_attribute = static_cast<GLuint>(f->glGetAttribLocation(shaderProgram, "position"));

    // Specify how the data for position can be accessed
    f->glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Enable the attribute
    f->glEnableVertexAttribArray(position_attribute);
}

void Widget::paintGL()
{
    // Draw the scene:
    QSize r = size();
    GLfloat ww = static_cast<GLfloat>(r.width());
    GLfloat wh = static_cast<GLfloat>(r.height());

    //QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QOpenGLFunctions_3_0 *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();

    f->glClear(GL_COLOR_BUFFER_BIT);

    f->glBindVertexArray(vao);
    f->glDrawArrays(GL_TRIANGLES, 0, 12);

    // old opengl, does not work on mobile
//    QOpenGLFunctions_2_0 *f2 = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_2_0>();
//    assert(f2 != nullptr);

//    f2->glClear(GL_COLOR_BUFFER_BIT);
//    f2->glMatrixMode(GL_MODELVIEW);
//    f2->glLoadIdentity();

//    f2->glMatrixMode(GL_PROJECTION);
//    f2->glLoadIdentity();
//    f2->glOrtho(0.0, static_cast<GLdouble>(ww), 0.0, static_cast<GLdouble>(wh), 0.0, 1.0);

//    GLfloat pointVertex[] = {ww / 2, wh / 2};

//    f2->glEnableClientState(GL_VERTEX_ARRAY);
//    f2->glVertexPointer(2, GL_FLOAT, 0, pointVertex);
//    f2->glPointSize(50.0f);
//    f2->glEnable(GL_POINT_SMOOTH);
//    f2->glDrawArrays(GL_POINTS, 0, 1);
//    f2->glDisableClientState(GL_VERTEX_ARRAY);
}
