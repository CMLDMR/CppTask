//
// Created by CMLDMR on 30.01.2020.
//

#ifndef CPPTASK_MODEL2D_H
#define CPPTASK_MODEL2D_H

#include "AbstractModel.h"

class Model2D : public AbstractModel<Point> {

public:
    explicit Model2D( Renderer* _renderer );


    /**
     * Sahnede Nesne Görünümünü Günceller.
     */
    virtual void update() override ;


    /**
     * Timer verilen zaman dolduğunda bu fonksiyon çalıştırılır.
     * Ayrı Bir Thread içinde Asenkron Olarak Çalışır
     */
    virtual void Runner() override ;

private:
    RGB color;

    enum class ColorState{
        red,
        green,
        blue
    };

    ColorState mColorState;
    int _rColor = 120;
    int _gColor = 120;
    int _bColor = 120;

};


#endif //CPPTASK_MODEL2D_H
