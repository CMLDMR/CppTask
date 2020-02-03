//
// Created by CMLDMR on 30.01.2020.
//

#include "Model2D.h"
#include "Renderer.h"


Model2D::Model2D( Renderer* _renderer )
    : AbstractModel<Point>::AbstractModel(_renderer,AbstractModel::ModelType::model2D)
{
    this->setModelID(_renderer->createModelID());
    _renderer->addModel(this);
    color = RGB{ 1.0 , 1.0 , 1.0 };

    mColorState = ColorState::red;
}


void Model2D::update() {

    if( this->vertexEnabled() )
    {
        this->getRenderer()->drawPointLoop(getVertex());
    }

    if( this->edgeEnabled() )
    {
        this->getRenderer()->drawLineLoop(this->getVertex());
    }

    if( this->polygonEnabled() )
    {
        for( auto item : this->getTriangle() )
        this->getRenderer()->drawTriangle(item,
                color.r,
                color.g,
                color.b);
    }

}

void Model2D::Runner() {

    switch ( mColorState )
    {
        case ColorState::red:
        {
            _rColor++;
            color.r = ((float)_rColor)/255.0;

            _gColor--;
            color.g = ((float)_gColor)/255.0;

            _bColor--;
            color.b = ((float)_bColor)/255.0;

            if( color.r > 1.0 ) {
                color.r = 1.0;
                _rColor = 255;
                mColorState = ColorState::green;
            }
            if( color.g < 0.0 ) { _gColor = 0; color.g = 0.0; }
            if( color.b < 0.0 ) { _bColor = 0; color.b = 0.0; }
        }
            break;






        case ColorState::green:
        {
            _rColor--;
            color.r = ((float)_rColor)/255.0;

            _gColor++;
            color.g = ((float)_gColor)/255.0;

            _bColor--;
            color.b = ((float)_bColor)/255.0;

            if( color.g > 1.0 ) {
                color.g = 1.0;
                _gColor = 255;
                mColorState = ColorState::blue;
            }
            if( color.r < 0.0 ) { _rColor = 0; color.r = 0.0; }
            if( color.b < 0.0 ) { _bColor = 0; color.b = 0.0; }
        }

            break;





        case ColorState::blue:
        {
            _rColor--;
            color.r = ((float)_rColor)/255.0;

            _gColor--;
            color.g = ((float)_gColor)/255.0;

            _bColor++;
            color.b = ((float)_bColor)/255.0;

            if( color.b > 1.0 ) {
                color.b = 1.0;
                _bColor = 255;
                mColorState = ColorState::red;
            }
            if( color.r < 0.0 ) { _rColor = 0; color.r = 0.0; }
            if( color.g < 0.0 ) { _gColor = 0; color.g = 0.0; }
        }
            break;

        default:
            break;
    }
}

