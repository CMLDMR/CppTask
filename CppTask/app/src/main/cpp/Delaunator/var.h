//
// Created by CMLDMR on 29.01.2020.
//

#ifndef CPPTASK_VAR_H
#define CPPTASK_VAR_H

#include <stdlib.h>
#include <android/log.h>
#include <stdio.h>
#include <chrono>
#include <vector>
#include <type_traits>
#include <random>

#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <limits>
#include <memory>
#include <utility>
#include <time.h>
#include <string>


#define LOG_TAG "libNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


/**
 * 2D Şekil Koordinat Yapısı
 */
struct Point{
    float x;
    float y;

    bool operator==( const Point& other )
    {
        if( this->x != other.x ) return false;
        if( this->y != other.y ) return false;
        return true;
    }

    bool operator==( Point&& other )
    {
        if( this->x != other.x ) return false;
        if( this->y != other.y ) return false;
        return true;
    }

    bool operator!=( const Point& other )
    {
        if( this->x == other.x ) return false;
        if( this->y == other.y ) return false;
        return true;
    }

    bool operator!=( Point&& other )
    {
        if( this->x == other.x ) return false;
        if( this->y == other.y ) return false;
        return true;
    }


    friend bool operator==( const Point& p1 , const Point& p2 )
    {
        if( p1.x != p2.x ) return false;
        if( p1.y != p2.y ) return false;
        return true;
    }

    friend bool operator!=( const Point& p1 , const Point& p2 )
    {
        if( p1.x == p2.x ) return false;
        if( p1.y == p2.y ) return false;
        return true;
    }


};

/**
 * 3D Şekil Koordinat Yapısı
 */
struct Point3D{
    float x;
    float y;
    float z;
};

/**
 * RGB Renk Yapısı
 */
struct RGB{
    float r;
    float g;
    float b;
};

/**
 * Polygon Yapısı
 */
struct Triangle{
    Point p1;
    Point p2;
    Point p3;

    /**
     * Polygon Ağırlık Merkezini Koordinatları Geri Döndürür
     * @return
     */
    Point getMedium(){
        float x = p1.x + p2.x + p3.x;
        float y = p1.y + p2.y + p3.y;
        Point newPoint;
        newPoint.x = x / 3.0;
        newPoint.y = y / 3.0;
        return newPoint;
    }

    bool operator==( const Triangle& other )
    {
        if( other.p1 != this->p1 ) return false;
        if( other.p2 != this->p2 ) return false;
        if( other.p3 != this->p3 ) return false;
        return true;
    }

    bool operator==( Triangle&& other )
    {
        if( other.p1 != this->p1 ) return false;
        if( other.p2 != this->p2 ) return false;
        if( other.p3 != this->p3 ) return false;
        return true;
    }


    bool operator!=( const Triangle& other )
    {
        if( other.p1 == this->p1 ) return false;
        if( other.p2 == this->p2 ) return false;
        if( other.p3 == this->p3 ) return false;
        return true;
    }

    bool operator!=( Triangle&& other )
    {
        if( other.p1 == this->p1 ) return false;
        if( other.p2 == this->p2 ) return false;
        if( other.p3 == this->p3 ) return false;
        return true;
    }
};








static const char glVertexShader[] =
        "attribute vec4 vPosition;\n"
        "attribute vec3 vertexColour;\n"
        "varying vec3 fragColour;\n"
        "void main()\n"
        "{\n"
        "  gl_Position = vPosition;\n"
        "  fragColour = vertexColour;\n"
        "  gl_PointSize = 30.0;\n"
        "}\n";


static const char glFragmentShader[] =
        "precision mediump float;\n"
        "varying vec3 fragColour;\n"
        "void main()\n"
        "{\n"
        "   gl_FragColor = vec4(fragColour, 1.0);\n"
        "}\n";

#endif //CPPTASK_VAR_H
