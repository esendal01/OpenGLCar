#include "qopenglpanel.h"

QOpenGLPanel::QOpenGLPanel(QWidget *parent) :QOpenGLWidget(parent)
{

}

QOpenGLPanel::~QOpenGLPanel()
{

}

QOpenGLFunctions* QOpenGLPanel::getGLFunctions()
{
    return QOpenGLContext::currentContext()->functions();
}

QOpenGLExtraFunctions* QOpenGLPanel::getGLExtraFunctions()
{
    return QOpenGLContext::currentContext()->extraFunctions();
}

bool QOpenGLPanel::initializeShaderProgram(QString vertex, QString fragment, QOpenGLFunctions *f)
{
    progID = f->glCreateProgram();

    vertID = f->glCreateShader(GL_VERTEX_SHADER);
    const char* vertSource = readShaderSource(vertex);
    f->glShaderSource(vertID,1,&vertSource,nullptr);
    f->glCompileShader(vertID);
    f->glAttachShader(progID, vertID);

    fragID = f->glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragSource = readShaderSource(fragment);
    f->glShaderSource(fragID,1,&fragSource,nullptr);
    f->glCompileShader(fragID);
    f->glAttachShader(progID, fragID);

    f->glLinkProgram(progID);

    return checkGLError(f, "Linking Shader Program");
}

bool QOpenGLPanel::checkGLError(QOpenGLFunctions *f, QString functionCall)
{
    GLenum error = f->glGetError();
    if(error == GL_NO_ERROR){
        qDebug()<<"No OpenGL Error while "<<functionCall;
        return true;
    }else{
        qDebug()<<"Error "<<error<<" while "<<functionCall;
        return false;
    }
}

const char* QOpenGLPanel::readShaderSource(QString filename)
{
    const char* source = nullptr;
    QFile shaderFile(filename);
    if(!shaderFile.open(QFile::ReadOnly|QFile::Text)){
        qDebug()<<"Error while reading shader source file";
        return source;
    }

    QTextStream in(&shaderFile);
    QString text = in.readAll();
    shaderFile.close();
    QByteArray ba = text.toLocal8Bit();
    source = ba.data();

    return source;
}

void QOpenGLPanel::initializeGL()
{
    QOpenGLFunctions *f = getGLFunctions();
    f->initializeOpenGLFunctions();
    QOpenGLExtraFunctions *ef = getGLExtraFunctions();
    ef->initializeOpenGLFunctions();

    f->glClearColor(0.0, 0.4, 0.7, 0.0);

    initializeShaderProgram(":simple.vert", ":simple.frag",f);

    translateMatrixID = f->glGetUniformLocation(progID,"translateMatrix");
    rotateMatrixID = f->glGetUniformLocation(progID, "rotateMatrix");
    scaleMatrixID = f->glGetUniformLocation(progID, "scaleMatrix");
    reflectMatrixID = f->glGetUniformLocation(progID, "reflectMatrix");

    ef->glGenVertexArrays(2, &arrays);
    f->glGenBuffers(1,&triangleData);
    ef->glBindVertexArray(arrays);
    f->glBindBuffer(GL_ARRAY_BUFFER, triangleData);

    checkGLError(f, "Generating and Binding Vertex Arrays");

    float vertAndColors[1024] = {
        0.0f,  0.5f, 0.5f,         1.0f, 0.0f, 0.0f,   // A köşesi kırmızı renk
        -0.433f, -0.25f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.433f, -0.25f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk

        0.0f,  0.5f, 0.5f,         1.0f, 0.0f, 0.0f,   // A köşesi kırmızı renk
        -0.433f, 1.0f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.433f, 1.0f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk




        0.0f,  0.5f, 0.5f,         1.0f, 0.0f, 0.0f,   // A köşesi kırmızı renk
        0.866f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.433f, 1.0f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk

        0.0f,  0.5f, 0.5f,         1.0f, 0.0f, 0.0f,   // A köşesi kırmızı renk
        0.433f, -0.25f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        0.866f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk

        0.0f,  0.5f, 0.5f,         1.0f, 0.0f, 0.0f,   // A köşesi kırmızı renk
        -0.433f, -0.25f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        -0.866f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk

        0.0f,  0.5f, 0.5f,         1.0f, 0.0f, 0.0f,   // A köşesi kırmızı renk
        -0.866f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        -0.433f, 1.0f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk

        ///////////////////////////////////////////////////////////////

        0.0f,  0.5f, -0.5f,         1.0f, 0.0f, 0.0f,   // A köşesi kırmızı renk
        -0.433f, -0.25f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.433f, -0.25f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk

        0.0f,  0.5f, -0.5f,         1.0f, 0.0f, 0.0f,   // A köşesi kırmızı renk
        -0.433f, 1.0f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.433f, 1.0f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk

        0.0f,  0.5f, -0.5f,         1.0f, 0.0f, 0.0f,   // A köşesi kırmızı renk
        0.866f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.433f, 1.0f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk

        0.0f,  0.5f, -0.5f,         1.0f, 0.0f, 0.0f,   // A köşesi kırmızı renk
        0.433f, -0.25f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        0.866f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk

        0.0f,  0.5f, -0.5f,         1.0f, 0.0f, 0.0f,   // A köşesi kırmızı renk
        -0.433f, -0.25f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        -0.866f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk

        0.0f,  0.5f, -0.5f,         1.0f, 0.0f, 0.0f,   // A köşesi kırmızı renk
        -0.866f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        -0.433f, 1.0f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk

        /////////////////////////////////////////////////////////////
        0.433f, 1.0f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        -0.433f, 1.0f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.433f, 1.0f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk

        0.433f, 1.0f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        -0.433f, 1.0f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        -0.433f, 1.0f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk

        0.433f, -0.25f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        -0.433f, -0.25f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.433f, -0.25f, 0.5f,      0.0f, 0.0f, 1.0f ,


        -0.433f, -0.25f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.433f, -0.25f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        -0.433f, -0.25f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk

        0.866f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.866f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.433f, 1.0f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk

        0.433f, 1.0f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        0.433f, 1.0f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        0.866f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk

        0.866f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.433f, -0.25f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        0.866f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk

        0.433f, -0.25f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        0.866f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        0.433f, -0.25f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk


        -0.866f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        -0.866f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        -0.433f, 1.0f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk

        -0.433f, 1.0f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        -0.433f, 1.0f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        -0.866f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk

        -0.866f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        -0.433f, -0.25f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        -0.866f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk

        -0.433f, -0.25f, 0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk
        -0.866f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   // B köşesi yeşil renk
        -0.433f, -0.25f, -0.5f,      0.0f, 0.0f, 1.0f ,   // C köşesi mavi renkk








    };



    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertAndColors), vertAndColors, GL_STATIC_DRAW);

    position = f->glGetAttribLocation(progID, "position");
    f->glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);
    f->glEnableVertexAttribArray(position);

    color = f->glGetAttribLocation(progID, "color");
    f->glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)(sizeof(float)*3));
    f->glEnableVertexAttribArray(color);


    tX = 0.0f, tY=0.0f, tZ=0.0f;
    rX = 1.0f, rY=1.0f, rZ=0.0f;
    sX = 1.0f, sY=1.0f, sZ=0.0f;
    reX = 0.0f, reY = 1.0F, reZ = 0.0f;
    rDegree = 0.0f;

}

void QOpenGLPanel::paintGL()
{
    QOpenGLFunctions *f = getGLFunctions();
    QOpenGLExtraFunctions *ef = getGLExtraFunctions();
    f->glClear(GL_COLOR_BUFFER_BIT);


    translateMatrix.setToIdentity();
    //     1.0, 0.0, 0.0, 0.0
    //     0.0, 1.0, 0.0, 0.0
    //     0.0, 0.0, 1.0, 0.0
    //     0.0, 0.0, 0.0, 1.0
    tX+=0.000;
    tY+=0.000;
    tZ+=0.000;
    // translateMatrix(0,3)=tX;
    // translateMatrix(1,3)=tY;
    // translateMatrix(2,3)=tZ;
    translateMatrix.translate(tX,tY,tZ);

    rotateMatrix.setToIdentity();
   rDegree+=1.0f;
   float radian = qDegreesToRadians(rDegree);
   // //rotate X axis
   // rotateMatrix(1,1)=qCos(radian);
   // rotateMatrix(1,2)=-qSin(radian);
   // rotateMatrix(2,1)=qSin(radian);
   // rotateMatrix(2,2)=qCos(radian);
   // //rotate Y axis
   // rotateMatrix(0,0)=qCos(radian);
   // rotateMatrix(0,2)=-qSin(radian);
   // rotateMatrix(2,0)=qSin(radian);
   // rotateMatrix(2,2)=qCos(radian);
   // //rotate Z axis
   // rotateMatrix(0,0)=qCos(radian);
   // rotateMatrix(0,1)=-qSin(radian);
   // rotateMatrix(1,0)=qSin(radian);
   // rotateMatrix(1,1)=qCos(radian);
   rotateMatrix.rotate(rDegree, rX, rY, rZ);


    scaleMatrix.setToIdentity();
       // sX+=0.01f, sY+=0.01f, sZ+=0.01f;
       // scaleMatrix(0,0) = sX;
       // scaleMatrix(1,1)= sY;
       // scaleMatrix(2,2) = sZ;
       // scaleMatrix.scale(sX,sY,sZ);


    reflectMatrix.setToIdentity();
    //     1.0, 0.0, 0.0, 0.0
    //     0.0, 1.0, 0.0, 0.0
    //     0.0, 0.0, 1.0, 0.0
    //     0.0, 0.0, 0.0, 1.0
    // reflectMatrix(0,0)=-1.0f;


    f->glUseProgram(progID);

    f->glUniformMatrix4fv(translateMatrixID,1,GL_FALSE,translateMatrix.constData());
    f->glUniformMatrix4fv(rotateMatrixID,1,GL_FALSE,rotateMatrix.constData());
    f->glUniformMatrix4fv(scaleMatrixID,1,GL_FALSE,scaleMatrix.constData());
    f->glUniformMatrix4fv(reflectMatrixID,1,GL_FALSE,reflectMatrix.constData());

    ef->glBindVertexArray(arrays);
    f->glDrawArrays(GL_TRIANGLES, 0, 128);

    update();
}

void QOpenGLPanel::resizeGL(int width, int height)
{


}


