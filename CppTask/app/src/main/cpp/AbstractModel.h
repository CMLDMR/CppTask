//
// Created by CMLDMR on 30.01.2020.
//

#ifndef CPPTASK_ABSTRACTMODEL_H
#define CPPTASK_ABSTRACTMODEL_H

#include "Delaunator/var.h"
//#include "Renderer.h"
#import  "Delaunator/Delaunator.h"
#include <thread>
#include <chrono>
#include <unistd.h>


//forward Declare
class Renderer;

//#include "Renderer.h"

template <typename T = Point>
class AbstractModel
{
public:
 /**
 * Model Type: 2D Yada 3D Model Seçimi ve Model Tipini Belirleme
 */
    enum class ModelType{
        model2D,
        model3D
    };

private:
    Renderer* mRenderer;
    bool mDrawPoints;
    bool mDrawLines;
    bool mDrawTriangle;
    std::vector<Triangle> mMesh;
    std::vector<T> mPoints;
    std::vector<Triangle> getConcave( const std::vector<Triangle> & list );

    ModelType _mModelType;

    float x;
    float y;

    bool mRunner;

    std::string mModelID;
public:
    /**
     *  Geçerli Yapıcı Fonksiyon
     * @param _renderer : Renderer Nesnesinin Adresi
     * @param type : Model Tipi Belirteci
     */
    AbstractModel(Renderer *_renderer, AbstractModel::ModelType type)
            : mRenderer(_renderer),
              mDrawPoints(false),
              mDrawLines(false),
              mDrawTriangle(true),
              _mModelType(type),
              mRunner(false){

    }

  /**
   * Yıkıcı Fonksiyon
   */
    virtual ~AbstractModel(){
      mMesh.clear();
      mPoints.clear();
  }



  std::string getModelID() const{
        return mModelID;
    }

    void setModelID( const std::string& modelID )
    {
        this->mModelID = modelID;
    }


    /**
     * Model Tipi Geri Döndürür. 2D yada 3D
     * @return
     */
    AbstractModel::ModelType type() const{
        return _mModelType;
    }


    /**
     * Model Nesnesine Point Ekler, Pointler 2D yada 3D Pointleri Olması Gerekir.
     * @param array
     * @param length
     * @return
     */
    AbstractModel& appendPoinst( float* array , int length ){
        auto cList  = getCoords(array,length);
        this->mPoints = getPointList(array,length);
        delaunator::Delaunator d(cList);
        auto tempMesh = d.getTriangleList();
        this->mMesh = this->getConcave(tempMesh);
        this->update();
        return *this;
    }


    /**
     * Nesnenin Vertex, Line , Polygon Görünümleri Enable Edildiğin de yada
     * Child sınıftan Custom olarak çağırıldığında.
     * Child Sınıfın reimplemente edilmiş
     * update fonksiyonu çağırılır.
     */
    virtual void update() = 0 ;


    /**
     * Timer Başlatıldığında Verilen Aralıkta Bu Method Çağırılır.
     */
    virtual void Runner() {};

    /**
     * Timer'i Başlatır.
     * @param miliSeconds
     */
    void startTimer( const int &miliSeconds = 1000 ){
        mRunner = true;
        std::thread _thread([=](){
            while ( mRunner )
            {
                this->Runner();
                usleep(miliSeconds*1000);
            }
        });
        _thread.detach();
    }


    /**
     * Timeri Durdurur.
     */
    void stopTimer(){
        mRunner = false;
    }


    /**
     * Renderer Nesnesini Geri Döndürür.
     * @return Renderer'a ait pointer
     */
    Renderer* getRenderer() const{
        return mRenderer;
    }



    /**
     * Sahnede Vertexlerin Görünmesini Etkinleştirir/Devre Dışı Bırakır Başlangıç Değeri = false
     * @param enableVertex
     * @return
     */
    AbstractModel& enableVertex( const bool &enableVertex = true ){
        mDrawPoints = enableVertex;
        this->update();
        return *this;
    }

    /**
     * Vertexlerin Görünüp Görünmedini Geri Döndürür. Görünüyorsa true
     * @return
     */
    bool vertexEnabled() const{
        return mDrawPoints;
    }


    /**
     * Line ların Görünmesini Etkinleştirir/Devre Dışı Bırakır Başlangıç Değeri = false
     * @param enableEdge
     * @return
     */
    AbstractModel& enableEdge( const bool& enableEdge = true ){
        mDrawLines = enableEdge;
        this->update();
        return *this;
    }
    /**
     * Line Görünüp Görünmedini Geri Döndürür.
     * @return Görünüyorsa true
     */
    bool edgeEnabled() const{
        return mDrawLines;
    }


    /**
     * Polygonların Görünmesini Etkinleştirir/Devre Dışı Bırakır. Başlangıç Değeri = true
     * @param enablePolygon
     * @return Nesnenin Referansı
     */
    AbstractModel& enablePolygon( const bool& enablePolygon = true ) {
        mDrawTriangle = enablePolygon;
        this->update();
        return *this;
    }

    /**
     * Polygonların Görünüp Görünmedini Geri Döndürür.
     * @return Eğer Etkinleştirilmiş ise <b>true</b> yoksa <b>false</b>
     */
    bool polygonEnabled() const{
        return mDrawTriangle;
    }



    /**
     * Mevcut Vertexleri container içerisinde Geri Döndürür.
     * @return <b>std::vector<T>&</b> T: template, <b>Point</b> ve <b>Point3D</b>
     */
    const std::vector<T> &getVertex() const{
        return this->mPoints;
    }

    /**
     * Polygonların container içerisinde Geri Döndürür
     * @return <b>std::vector<Triangle></b>
     */
    const std::vector<Triangle> &getTriangle() const{
        return this->mMesh;
    }




};











template<typename T>
std::vector<Triangle> AbstractModel<T>::getConcave(const std::vector<Triangle> &list) {
    std::vector<Triangle> _list;
    for(auto item : list )
    {
        auto checkPoint = item.getMedium();
        auto inside = checkInside(this->getVertex(),checkPoint);
        if( inside )
        {
            _list.push_back(item);
        }
    }
    return _list;
}




#endif //CPPTASK_ABSTRACTMODEL_H
