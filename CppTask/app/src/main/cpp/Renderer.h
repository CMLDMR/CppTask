//
// Created by CMLDMR on 27.01.2020.
//

#ifndef CPPTASK_RENDERER_H
#define CPPTASK_RENDERER_H





#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <jni.h>
#include "Delaunator/var.h"
#include "AbstractModel.h"



class Renderer {




public:

    /**
     * Render Nesnesi Geri Dödürür. Eğer Nesne Yoksa Bir Adet Oluşturur var ise olanı Geri Döndürür
     * @param _width : Ekran Width
     * @param _height: Ekran Height
     * @return
     */
    static Renderer *instance( const int& _width , const int& _height );

    /**
     * OpenGL ES Programı Oluşturur ve Geri Döndürür.
     * @param vertexSource
     * @param fragmentSource
     * @return
     */
    GLuint CreateProgram(const char* vertexSource, const char * fragmentSource);

    /**
     * Veri Shaderları Yükler
     * @param shaderType
     * @param shaderSource
     * @return
     */
    GLuint loadShader(GLenum shaderType , const char* shaderSource );

    /**
     * Mevcut Programı Geri Döndürür
     * @return
     */
    GLuint getProgram() const;


    /**
     * Shaderları Atamasını yapar ve indeksleri saklar
     */
    void initVertexShader() ;

    /**
     * Sahneyi Günceller
     */
    void updateScene();


    /**
     * Verilen Noktaları Verilen Renklerde Ekrana Çizer
     * @param pVertices
     * @param color
     * @param verticeSize
     */
    void drawPoints(float* pVertices , float* color , const int& verticeSize );

    /**
     * Verilen Triangle Veri Renklerde Ekrana Çizdirir.
     * @param triangle
     * @param red
     * @param green
     * @param blue
     */
    void drawTriangle( Triangle triangle , float red , float green , float blue );

    /**
     * Verilen Vertexleri Ekrana Çizdirir
     * @param point
     * @param red
     * @param green
     * @param blue
     */
    void drawPoint( Point point , float red , float green , float blue );

    /**
     * Verilen Noktaların tamamını ekrana çizdirir
     * @param pList
     * @param color
     */
    void drawPointLoop( const std::vector<Point> &pList , RGB color = RGB{1.0,1.0,1.0});

    /**
     * Verilen Noktalar Arası Line Çizer
     * @param p1
     * @param p2
     * @param color
     */
    void drawLine( const Point& p1 , const Point &p2 , const RGB& color );

    /**
     * Verilen Noktalar arasında Line Çizer. Başlangıç ve Bitiş Arası da Dahil
     * @param pList
     */
    void drawLineLoop( const std::vector<Point> &pList );

    /**
     * Verilen Trianglerin Renklerini Günceller
     * @param color
     * @param pList
     */
    void updateColor( const RGB &color , const std::vector<Triangle> pList );

    /**
     * Ekranı Temizler
     */
    void clearGL();


    /**
     * Container Mevcut Modelleri Pointerları Ekler
     * @tparam T
     * @param _model
     */
    template < typename T>
    void addModel( T* _model ){
        mModelList.push_back(static_cast<void*>(_model));
    }


    /**
     * Model için Özgün ID Oluşturur.
     * @return
     */
    std::string createModelID();

    /**
     * ModelID verilen Modeli Siler
     * @param modelID
     * @return
     */
    void removeModel( const std::string& modelID );

private:
    Renderer();
    GLuint mProgram;
    GLuint vPosition;
    GLuint fLocation;

    int mWidth;
    int mHeight;

    std::vector<void*> mModelList;

    std::vector<std::string> mModelIDList;



};





#endif //CPPTASK_RENDERER_H
