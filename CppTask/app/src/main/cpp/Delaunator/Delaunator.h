//
// Created by CMLDMR on 29.01.2020.
//

#ifndef CPPTASK_DELAUNATOR_H
#define CPPTASK_DELAUNATOR_H


#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <limits>
#include <memory>
#include <utility>
#include <vector>
#include<time.h>


#include "var.h"


/**
 * Java dan Gönderilen float[] std::vector<double> çevirir
 * @param array
 * @param length
 * @return
 */
std::vector<double> getCoords( float* array , int length );

/**
 * Java dan Gönderilen float[] Koordinatları std::vector<Point> olarak Geri Döndürür
 * @param array
 * @param length
 * @return
 */
std::vector<Point> getPointList( float* array , int length );

/**
 * Hazır Polygon Çizimi
 */
namespace delaunator {

    //@see https://stackoverflow.com/questions/33333363/built-in-mod-vs-custom-mod-function-improve-the-performance-of-modulus-op/33333636#33333636
    size_t fast_mod(const size_t i, const size_t c) ;

    // Kahan and Babuska summation, Neumaier variant; accumulates less FP error
    double sum(const std::vector<double>& x) ;

    double dist(
            const double ax,
            const double ay,
            const double bx,
            const double by) ;


    double circumradius(
            const double ax,
            const double ay,
            const double bx,
            const double by,
            const double cx,
            const double cy) ;





    bool orient(
            const double px,
            const double py,
            const double qx,
            const double qy,
            const double rx,
            const double ry) ;


    inline std::pair<double, double> circumcenter(
            const double ax,
            const double ay,
            const double bx,
            const double by,
            const double cx,
            const double cy) ;


    struct compare {

        std::vector<double> const& coords;
        double cx;
        double cy;

        bool operator()(std::size_t i, std::size_t j) ;
    };


    inline bool in_circle(
            const double ax,
            const double ay,
            const double bx,
            const double by,
            const double cx,
            const double cy,
            const double px,
            const double py) ;


    constexpr double EPSILON = std::numeric_limits<double>::epsilon();
    constexpr std::size_t INVALID_INDEX = std::numeric_limits<std::size_t>::max();

    bool check_pts_equal(double x1, double y1, double x2, double y2) ;

// monotonically increases with real angle, but doesn't need expensive trigonometry
    double pseudo_angle(const double dx, const double dy) ;


    struct DelaunatorPoint {
        std::size_t i;
        double x;
        double y;
        std::size_t t;
        std::size_t prev;
        std::size_t next;
        bool removed;
    };


    class Delaunator {

    public:
        std::vector<double> const& coords;
        std::vector<std::size_t> triangles;
        std::vector<std::size_t> halfedges;
        std::vector<std::size_t> hull_prev;
        std::vector<std::size_t> hull_next;
        std::vector<std::size_t> hull_tri;
        std::size_t hull_start;

        Delaunator(std::vector<double> const& in_coords);

        double get_hull_area();

        std::vector<Triangle> getTriangleList();

    private:
        std::vector<std::size_t> m_hash;
        double m_center_x;
        double m_center_y;
        std::size_t m_hash_size;
        std::vector<std::size_t> m_edge_stack;

        std::size_t legalize(std::size_t a);
        std::size_t hash_key(double x, double y) const;
        std::size_t add_triangle(
                std::size_t i0,
                std::size_t i1,
                std::size_t i2,
                std::size_t a,
                std::size_t b,
                std::size_t c);
        void link(std::size_t a, std::size_t b);
    };

}

struct line {
    Point p1, p2;
};


bool onLine(line l1, Point p);

float direction(Point a, Point b, Point c) ;

bool isIntersect(line l1, line l2) ;

bool checkInside(const std::vector<Point> &poly,const Point &p) ;

bool checkInside(Point poly[], int n, Point p) ;

/**
 * 0.0 - 1.0 aralığında rasgele sayı üretir.
 * @param begin
 * @param end
 * @return
 */
float getRandomColor(int begin = 0 , int end = 100 );



#endif //CPPTASK_DELAUNATOR_H
