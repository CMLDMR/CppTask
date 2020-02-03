//
// Created by CMLDMR on 27.01.2020.
//

#include "Renderer.h"

#include "Model2D.h"

#include "Delaunator/Delaunator.h"


static float tPoints[1000];
static float tColor[1000];

static Renderer* mRenderer = nullptr;

Renderer* Renderer::instance( const int& _width , const int& _height  )
{
    if( mRenderer == nullptr )
    {
        mRenderer = new Renderer();
        LOGE("new Renderer");
    }else{
        LOGE("Exist Renderer");
    }

    mRenderer->mWidth = _width;
    mRenderer->mHeight = _height;

    return mRenderer;
}

GLuint Renderer::CreateProgram(const char *vertexSource, const char *fragmentSource) {

    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader)
    {
        return 0;
    }
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader)
    {
        return 0;
    }
    mProgram = glCreateProgram();
    if (mProgram)
    {
        glAttachShader(mProgram , vertexShader);
        glAttachShader(mProgram, fragmentShader);
        glLinkProgram(mProgram);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(mProgram , GL_LINK_STATUS, &linkStatus);
        if( linkStatus != GL_TRUE)
        {
            GLint bufLength = 0;
            glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength)
            {
                char* buf = (char*) malloc(bufLength);
                if (buf)
                {
                    glGetProgramInfoLog(mProgram, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(mProgram);
            mProgram = 0;
        }
    }
    return mProgram;
}

GLuint Renderer::loadShader(GLenum shaderType, const char *shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader)
    {
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen)
            {
                char * buf = (char*) malloc(infoLen);
                if (buf)
                {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not Compile Shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint Renderer::getProgram() const {
    return mProgram;
}

void Renderer::initVertexShader() {
    this->vPosition = glGetAttribLocation(this->mProgram,"vPosition");
    this->fLocation = glGetAttribLocation(this->mProgram,"vertexColour");
    glViewport( 0 , 0 , this->mWidth , this->mHeight );
}

Renderer::Renderer() : mProgram(0){



}

void Renderer::updateScene() {

    this->clearGL();
    for( auto model : mModelList )
    {
        Model2D* model2 = static_cast<Model2D*>(model);
        if( model2->type() == AbstractModel<Point>::ModelType::model2D )
        {
            static_cast<Model2D*>(model)->update();
        }
        if( model2->type() == AbstractModel<Point>::ModelType::model3D )
        {
            // Eğer Model Model3D is Farklı Bir Method Çalıştırılmalı
        }
    }
}

void Renderer::drawPoints(float *pVertices, float *color, const int &verticeSize) {

    glUseProgram(this->mProgram);

    glVertexAttribPointer(this->vPosition, 2, GL_FLOAT, GL_FALSE, 0 ,pVertices);
    glEnableVertexAttribArray(this->vPosition);

    glVertexAttribPointer(this->fLocation, 3, GL_FLOAT, GL_FALSE, 0, color);
    glEnableVertexAttribArray(this->fLocation);

    glDrawArrays(GL_POINTS, 0, verticeSize);

}

void Renderer::drawTriangle(Triangle triangle , float red , float green , float blue ) {


    int index = 0;
    tPoints[index++] = triangle.p1.x;
    tPoints[index++] = triangle.p1.y;

    tPoints[index++] = triangle.p2.x;
    tPoints[index++] = triangle.p2.y;

    tPoints[index++] = triangle.p3.x;
    tPoints[index++] = triangle.p3.y;

    for( int i = 0 ; i < index*2 ; i += 3 )
    {
        tColor[i] = red;
        tColor[i+1] = green;
        tColor[i+2] = blue;
    }




    glUseProgram(this->mProgram);

    glVertexAttribPointer(this->vPosition, 2, GL_FLOAT, GL_FALSE, 0 ,&tPoints);
    glEnableVertexAttribArray(this->vPosition);

    glVertexAttribPointer(this->fLocation, 3, GL_FLOAT, GL_FALSE, 0, tColor);
    glEnableVertexAttribArray(this->fLocation);

    glDrawArrays(GL_TRIANGLES, 0, index);

}

void Renderer::drawPoint(Point point, float red, float green, float blue) {

    tPoints[0] = point.x;
    tPoints[1] = point.y;

    tColor[0] = red;
    tColor[1] = green;
    tColor[2] = blue;

    glUseProgram(this->mProgram);

    glVertexAttribPointer(this->vPosition, 2, GL_FLOAT, GL_FALSE, 0 ,&tPoints);
    glEnableVertexAttribArray(this->vPosition);

    glVertexAttribPointer(this->fLocation, 3, GL_FLOAT, GL_FALSE, 0, &tColor);
    glEnableVertexAttribArray(this->fLocation);

    glDrawArrays(GL_POINTS, 0, 1);

}

void Renderer::clearGL() {
    glClearColor(0.55f, 0.55f, 0.55f, 1.0f);
    glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Renderer::drawLine(const Point &p1, const Point &p2, const RGB &color) {

    tPoints[0] = p1.x;
    tPoints[1] = p1.y;

    tPoints[2] = p2.x;
    tPoints[3] = p2.y;

    tColor[0] = color.r;
    tColor[1] = color.g;
    tColor[2] = color.b;

    tColor[3] = color.r;
    tColor[4] = color.g;
    tColor[5] = color.b;

    glUseProgram(this->mProgram);

    glLineWidth(2.0);

    glVertexAttribPointer(this->vPosition, 2, GL_FLOAT, GL_FALSE, 0 ,&tPoints);
    glEnableVertexAttribArray(this->vPosition);

    glVertexAttribPointer(this->fLocation, 3, GL_FLOAT, GL_FALSE, 0, &tColor);
    glEnableVertexAttribArray(this->fLocation);

    glDrawArrays(GL_LINES, 0, 2);

}

void Renderer::drawLineLoop(const std::vector<Point> &pList) {
    for( int i = 0 ; i < pList.size()-1 ; i++ )
    {
        RGB color{getRandomColor(80),getRandomColor(80),getRandomColor(80)};
        mRenderer->drawLine(pList[i],pList[i+1],color);
        if( (i+1) == pList.size()-1 )
        {
            mRenderer->drawLine(pList[0],pList[i+1],color);
        }
    }

}

void Renderer::drawPointLoop(const std::vector<Point> &pList, RGB color) {
    for( int i = 0 ; i < pList.size() ; i++ )
    {
        mRenderer->drawPoint(pList[i],color.r,color.g,color.b);
    }
}


void Renderer::updateColor(const RGB &color, const std::vector<Triangle> pList ) {

    for( auto item : pList )
    {
        this->drawTriangle(item,color.r,color.g,color.b);
    }
}

std::string Renderer::createModelID() {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1000, 9999);

    bool _createdNew = true;
    int _r ;

    while( _createdNew )
    {
        _r  = dis(gen);

        std::string newID = std::to_string(_r);
        bool exist = false;
        for( auto item : mModelIDList )
        {
           if( newID == item )
           {
               exist = true;
               break;
           }
        }

        if( !exist )
        {
            _createdNew = false;
            break;
        }

    }


    return std::to_string(_r);
}

void Renderer::removeModel(const std::string &modelID) {

    for( auto item = this->mModelList.begin() ; item != this->mModelList.end() ; item++ )
    {
        if(static_cast<Model2D*>(*item)->getModelID() == modelID )
        {
            this->mModelList.erase(item);
            break;
        }
    }

    for( auto item = this->mModelIDList.begin() ; item != this->mModelIDList.end() ; item++ )
    {
        if( *item == modelID )
        {
            mModelIDList.erase(item);
            break;
        }
    }

}






