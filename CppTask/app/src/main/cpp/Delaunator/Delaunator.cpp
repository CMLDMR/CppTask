//
// Created by CMLDMR on 29.01.2020.
//

#include "Delaunator.h"

size_t delaunator::fast_mod(const size_t i, const size_t c) {
    return i >= c ? i % c : i;
}

double delaunator::sum(const std::vector<double>& x) {
    double sum = x[0];
    double err = 0.0;

    for (size_t i = 1; i < x.size(); i++) {
        const double k = x[i];
        const double m = sum + k;
        err += std::fabs(sum) >= std::fabs(k) ? sum - m + k : k - m + sum;
        sum = m;
    }
    return sum + err;
}

double delaunator::dist(
        const double ax,
        const double ay,
        const double bx,
        const double by) {
    const double dx = ax - bx;
    const double dy = ay - by;
    return dx * dx + dy * dy;
}

double delaunator::circumradius(
        const double ax,
        const double ay,
        const double bx,
        const double by,
        const double cx,
        const double cy) {
    const double dx = bx - ax;
    const double dy = by - ay;
    const double ex = cx - ax;
    const double ey = cy - ay;

    const double bl = dx * dx + dy * dy;
    const double cl = ex * ex + ey * ey;
    const double d = dx * ey - dy * ex;

    const double x = (ey * bl - dy * cl) * 0.5 / d;
    const double y = (dx * cl - ex * bl) * 0.5 / d;

    if ((bl > 0.0 || bl < 0.0) && (cl > 0.0 || cl < 0.0) && (d > 0.0 || d < 0.0)) {
        return x * x + y * y;
    } else {
        return std::numeric_limits<double>::max();
    }
}

bool delaunator::orient(
        const double px,
        const double py,
        const double qx,
        const double qy,
        const double rx,
        const double ry) {
    return (qy - py) * (rx - qx) - (qx - px) * (ry - qy) < 0.0;
}


std::pair<double, double> delaunator::circumcenter(
        const double ax,
        const double ay,
        const double bx,
        const double by,
        const double cx,
        const double cy) {
    const double dx = bx - ax;
    const double dy = by - ay;
    const double ex = cx - ax;
    const double ey = cy - ay;

    const double bl = dx * dx + dy * dy;
    const double cl = ex * ex + ey * ey;
    const double d = dx * ey - dy * ex;

    const double x = ax + (ey * bl - dy * cl) * 0.5 / d;
    const double y = ay + (dx * cl - ex * bl) * 0.5 / d;

    return std::make_pair(x, y);
}

bool delaunator::compare::operator()(std::size_t i, std::size_t j) {
    const double d1 = dist(coords[2 * i], coords[2 * i + 1], cx, cy);
    const double d2 = dist(coords[2 * j], coords[2 * j + 1], cx, cy);
    const double diff1 = d1 - d2;
    const double diff2 = coords[2 * i] - coords[2 * j];
    const double diff3 = coords[2 * i + 1] - coords[2 * j + 1];

    if (diff1 > 0.0 || diff1 < 0.0) {
        return diff1 < 0;
    } else if (diff2 > 0.0 || diff2 < 0.0) {
        return diff2 < 0;
    } else {
        return diff3 < 0;
    }
}


bool delaunator::in_circle(
        const double ax,
        const double ay,
        const double bx,
        const double by,
        const double cx,
        const double cy,
        const double px,
        const double py) {
    const double dx = ax - px;
    const double dy = ay - py;
    const double ex = bx - px;
    const double ey = by - py;
    const double fx = cx - px;
    const double fy = cy - py;

    const double ap = dx * dx + dy * dy;
    const double bp = ex * ex + ey * ey;
    const double cp = fx * fx + fy * fy;

    return (dx * (ey * cp - bp * fy) -
            dy * (ex * cp - bp * fx) +
            ap * (ex * fy - ey * fx)) < 0.0;
}


bool delaunator::check_pts_equal(double x1, double y1, double x2, double y2) {
    return std::fabs(x1 - x2) <= delaunator::EPSILON &&
           std::fabs(y1 - y2) <= delaunator::EPSILON;
}

// monotonically increases with real angle, but doesn't need expensive trigonometry
double delaunator::pseudo_angle(const double dx, const double dy) {
    const double p = dx / (std::abs(dx) + std::abs(dy));
    return (dy > 0.0 ? 3.0 - p : 1.0 + p) / 4.0; // [0..1)
}

delaunator::Delaunator::Delaunator(std::vector<double> const& in_coords)
        : coords(in_coords),
          triangles(),
          halfedges(),
          hull_prev(),
          hull_next(),
          hull_tri(),
          hull_start(),
          m_hash(),
          m_center_x(),
          m_center_y(),
          m_hash_size(),
          m_edge_stack() {
    std::size_t n = coords.size() >> 1;

    double max_x = std::numeric_limits<double>::min();
    double max_y = std::numeric_limits<double>::min();
    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    std::vector<std::size_t> ids;
    ids.reserve(n);

    for (std::size_t i = 0; i < n; i++) {
        const double x = coords[2 * i];
        const double y = coords[2 * i + 1];

        if (x < min_x) min_x = x;
        if (y < min_y) min_y = y;
        if (x > max_x) max_x = x;
        if (y > max_y) max_y = y;

        ids.push_back(i);
    }
    const double cx = (min_x + max_x) / 2;
    const double cy = (min_y + max_y) / 2;
    double min_dist = std::numeric_limits<double>::max();

    std::size_t i0 = INVALID_INDEX;
    std::size_t i1 = INVALID_INDEX;
    std::size_t i2 = INVALID_INDEX;

    // pick a seed point close to the centroid
    for (std::size_t i = 0; i < n; i++) {
        const double d = dist(cx, cy, coords[2 * i], coords[2 * i + 1]);
        if (d < min_dist) {
            i0 = i;
            min_dist = d;
        }
    }

    const double i0x = coords[2 * i0];
    const double i0y = coords[2 * i0 + 1];

    min_dist = std::numeric_limits<double>::max();

    // find the point closest to the seed
    for (std::size_t i = 0; i < n; i++) {
        if (i == i0) continue;
        const double d = dist(i0x, i0y, coords[2 * i], coords[2 * i + 1]);
        if (d < min_dist && d > 0.0) {
            i1 = i;
            min_dist = d;
        }
    }

    double i1x = coords[2 * i1];
    double i1y = coords[2 * i1 + 1];

    double min_radius = std::numeric_limits<double>::max();

    // find the third point which forms the smallest circumcircle with the first two
    for (std::size_t i = 0; i < n; i++) {
        if (i == i0 || i == i1) continue;

        const double r = circumradius(
                i0x, i0y, i1x, i1y, coords[2 * i], coords[2 * i + 1]);

        if (r < min_radius) {
            i2 = i;
            min_radius = r;
        }
    }

    if (!(min_radius < std::numeric_limits<double>::max())) {
        throw std::runtime_error("not triangulation");
    }

    double i2x = coords[2 * i2];
    double i2y = coords[2 * i2 + 1];

    if (orient(i0x, i0y, i1x, i1y, i2x, i2y)) {
        std::swap(i1, i2);
        std::swap(i1x, i2x);
        std::swap(i1y, i2y);
    }

    std::tie(m_center_x, m_center_y) = circumcenter(i0x, i0y, i1x, i1y, i2x, i2y);

    // sort the points by distance from the seed triangle circumcenter
    std::sort(ids.begin(), ids.end(), compare{ coords, m_center_x, m_center_y });

    // initialize a hash table for storing edges of the advancing convex hull
    m_hash_size = static_cast<std::size_t>(std::llround(std::ceil(std::sqrt(n))));
    m_hash.resize(m_hash_size);
    std::fill(m_hash.begin(), m_hash.end(), INVALID_INDEX);

    // initialize arrays for tracking the edges of the advancing convex hull
    hull_prev.resize(n);
    hull_next.resize(n);
    hull_tri.resize(n);

    hull_start = i0;

    size_t hull_size = 3;

    hull_next[i0] = hull_prev[i2] = i1;
    hull_next[i1] = hull_prev[i0] = i2;
    hull_next[i2] = hull_prev[i1] = i0;

    hull_tri[i0] = 0;
    hull_tri[i1] = 1;
    hull_tri[i2] = 2;

    m_hash[hash_key(i0x, i0y)] = i0;
    m_hash[hash_key(i1x, i1y)] = i1;
    m_hash[hash_key(i2x, i2y)] = i2;

    std::size_t max_triangles = n < 3 ? 1 : 2 * n - 5;
    triangles.reserve(max_triangles * 3);
    halfedges.reserve(max_triangles * 3);
    add_triangle(i0, i1, i2, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX);
    double xp = std::numeric_limits<double>::quiet_NaN();
    double yp = std::numeric_limits<double>::quiet_NaN();
    for (std::size_t k = 0; k < n; k++) {
        const std::size_t i = ids[k];
        const double x = coords[2 * i];
        const double y = coords[2 * i + 1];

        // skip near-duplicate points
        if (k > 0 && check_pts_equal(x, y, xp, yp)) continue;
        xp = x;
        yp = y;

        // skip seed triangle points
        if (
                check_pts_equal(x, y, i0x, i0y) ||
                check_pts_equal(x, y, i1x, i1y) ||
                check_pts_equal(x, y, i2x, i2y)) continue;

        // find a visible edge on the convex hull using edge hash
        std::size_t start = 0;

        size_t key = hash_key(x, y);
        for (size_t j = 0; j < m_hash_size; j++) {
            start = m_hash[fast_mod(key + j, m_hash_size)];
            if (start != INVALID_INDEX && start != hull_next[start]) break;
        }

        start = hull_prev[start];
        size_t e = start;
        size_t q;

        while (q = hull_next[e], !orient(x, y, coords[2 * e], coords[2 * e + 1], coords[2 * q], coords[2 * q + 1])) { //TODO: does it works in a same way as in JS
            e = q;
            if (e == start) {
                e = INVALID_INDEX;
                break;
            }
        }

        if (e == INVALID_INDEX) continue; // likely a near-duplicate point; skip it

        // add the first triangle from the point
        std::size_t t = add_triangle(
                e,
                i,
                hull_next[e],
                INVALID_INDEX,
                INVALID_INDEX,
                hull_tri[e]);

        hull_tri[i] = legalize(t + 2);
        hull_tri[e] = t;
        hull_size++;

        // walk forward through the hull, adding more triangles and flipping recursively
        std::size_t next = hull_next[e];
        while (
                q = hull_next[next],
                        orient(x, y, coords[2 * next], coords[2 * next + 1], coords[2 * q], coords[2 * q + 1])) {
            t = add_triangle(next, i, q, hull_tri[i], INVALID_INDEX, hull_tri[next]);
            hull_tri[i] = legalize(t + 2);
            hull_next[next] = next; // mark as removed
            hull_size--;
            next = q;
        }

        // walk backward from the other side, adding more triangles and flipping
        if (e == start) {
            while (
                    q = hull_prev[e],
                            orient(x, y, coords[2 * q], coords[2 * q + 1], coords[2 * e], coords[2 * e + 1])) {
                t = add_triangle(q, i, e, INVALID_INDEX, hull_tri[e], hull_tri[q]);
                legalize(t + 2);
                hull_tri[q] = t;
                hull_next[e] = e; // mark as removed
                hull_size--;
                e = q;
            }
        }

        // update the hull indices
        hull_prev[i] = e;
        hull_start = e;
        hull_prev[next] = i;
        hull_next[e] = i;
        hull_next[i] = next;

        m_hash[hash_key(x, y)] = i;
        m_hash[hash_key(coords[2 * e], coords[2 * e + 1])] = e;
    }
}


double delaunator::Delaunator::get_hull_area() {
    std::vector<double> hull_area;
    size_t e = hull_start;
    do {
        hull_area.push_back((coords[2 * e] - coords[2 * hull_prev[e]]) * (coords[2 * e + 1] + coords[2 * hull_prev[e] + 1]));
        e = hull_next[e];
    } while (e != hull_start);
    return sum(hull_area);
}


std::size_t delaunator::Delaunator::legalize(std::size_t a) {
    std::size_t i = 0;
    std::size_t ar = 0;
    m_edge_stack.clear();

    // recursion eliminated with a fixed-size stack
    while (true) {
        const size_t b = halfedges[a];

        /* if the pair of triangles doesn't satisfy the Delaunay condition
        * (p1 is inside the circumcircle of [p0, pl, pr]), flip them,
        * then do the same check/flip recursively for the new pair of triangles
        *
        *           pl                    pl
        *          /||\                  /  \
        *       al/ || \bl            al/    \a
        *        /  ||  \              /      \
        *       /  a||b  \    flip    /___ar___\
        *     p0\   ||   /p1   =>   p0\---bl---/p1
        *        \  ||  /              \      /
        *       ar\ || /br             b\    /br
        *          \||/                  \  /
        *           pr                    pr
        */
        const size_t a0 = 3 * (a / 3);
        ar = a0 + (a + 2) % 3;

        if (b == INVALID_INDEX) {
            if (i > 0) {
                i--;
                a = m_edge_stack[i];
                continue;
            } else {
                //i = INVALID_INDEX;
                break;
            }
        }

        const size_t b0 = 3 * (b / 3);
        const size_t al = a0 + (a + 1) % 3;
        const size_t bl = b0 + (b + 2) % 3;

        const std::size_t p0 = triangles[ar];
        const std::size_t pr = triangles[a];
        const std::size_t pl = triangles[al];
        const std::size_t p1 = triangles[bl];

        const bool illegal = in_circle(
                coords[2 * p0],
                coords[2 * p0 + 1],
                coords[2 * pr],
                coords[2 * pr + 1],
                coords[2 * pl],
                coords[2 * pl + 1],
                coords[2 * p1],
                coords[2 * p1 + 1]);

        if (illegal) {
            triangles[a] = p1;
            triangles[b] = p0;

            auto hbl = halfedges[bl];

            // edge swapped on the other side of the hull (rare); fix the halfedge reference
            if (hbl == INVALID_INDEX) {
                std::size_t e = hull_start;
                do {
                    if (hull_tri[e] == bl) {
                        hull_tri[e] = a;
                        break;
                    }
                    e = hull_next[e];
                } while (e != hull_start);
            }
            link(a, hbl);
            link(b, halfedges[ar]);
            link(ar, bl);
            std::size_t br = b0 + (b + 1) % 3;

            if (i < m_edge_stack.size()) {
                m_edge_stack[i] = br;
            } else {
                m_edge_stack.push_back(br);
            }
            i++;

        } else {
            if (i > 0) {
                i--;
                a = m_edge_stack[i];
                continue;
            } else {
                break;
            }
        }
    }
    return ar;
}



inline std::size_t delaunator::Delaunator::hash_key(const double x, const double y) const {
    const double dx = x - m_center_x;
    const double dy = y - m_center_y;
    return fast_mod(
            static_cast<std::size_t>(std::llround(std::floor(pseudo_angle(dx, dy) * static_cast<double>(m_hash_size)))),
            m_hash_size);
}


std::size_t delaunator::Delaunator::add_triangle(
        std::size_t i0,
        std::size_t i1,
        std::size_t i2,
        std::size_t a,
        std::size_t b,
        std::size_t c) {
    std::size_t t = triangles.size();
    triangles.push_back(i0);
    triangles.push_back(i1);
    triangles.push_back(i2);
    link(t, a);
    link(t + 1, b);
    link(t + 2, c);
    return t;
}


void delaunator::Delaunator::link(const std::size_t a, const std::size_t b) {
    std::size_t s = halfedges.size();
    if (a == s) {
        halfedges.push_back(b);
    } else if (a < s) {
        halfedges[a] = b;
    } else {
        throw std::runtime_error("Cannot link edge");
    }
    if (b != INVALID_INDEX) {
        std::size_t s2 = halfedges.size();
        if (b == s2) {
            halfedges.push_back(a);
        } else if (b < s2) {
            halfedges[b] = a;
        } else {
            throw std::runtime_error("Cannot link edge");
        }
    }
}

std::vector<Triangle> delaunator::Delaunator::getTriangleList() {

    std::vector<Triangle> list;

    for(std::size_t i = 0; i < this->triangles.size(); i+=3) {
        Triangle triangle;

        Point p1;
        p1.x = (float)this->coords[2 * this->triangles[i]];
        p1.y = (float)this->coords[2 * this->triangles[i] + 1];
        triangle.p1 = p1;

        p1.x = (float)this->coords[2 * this->triangles[i + 1]];
        p1.y = (float)this->coords[2 * this->triangles[i + 1] + 1];
        triangle.p2 = p1;

        p1.x = (float)this->coords[2 * this->triangles[i + 2]];
        p1.y = (float)this->coords[2 * this->triangles[i + 2] + 1];
        triangle.p3 = p1;
        list.push_back(triangle);
    }


    return list;
}


bool onLine(line l1, Point p) {        //check whether p is on the line or not
    if(p.x <= std::fmax(l1.p1.x, l1.p2.x) && p.x <= std::fmin(l1.p1.x, l1.p2.x) &&
       (p.y <= std::fmax(l1.p1.y, l1.p2.y) && p.y <= std::fmin(l1.p1.y, l1.p2.y)))
        return true;

    return false;
}


float direction(Point a, Point b, Point c) {
    float val = (b.y-a.y)*(c.x-b.x)-(b.x-a.x)*(c.y-b.y);
    if (val == 0.0)
        return 0.0;           //colinear
    else if(val < 0.0)
        return 2.0;          //anti-clockwise direction
    return 1.0;          //clockwise direction
}


bool isIntersect(line l1, line l2) {
    //four direction for two lines and points of other line
    float dir1 = direction(l1.p1, l1.p2, l2.p1);
    float dir2 = direction(l1.p1, l1.p2, l2.p2);
    float dir3 = direction(l2.p1, l2.p2, l1.p1);
    float dir4 = direction(l2.p1, l2.p2, l1.p2);

    if(dir1 != dir2 && dir3 != dir4)
        return true;           //they are intersecting
    if(dir1==0.0 && onLine(l1, l2.p1))        //when p2 of line2 are on the line1
        return true;
    if(dir2==0.0 && onLine(l1, l2.p2))         //when p1 of line2 are on the line1
        return true;
    if(dir3==0.0 && onLine(l2, l1.p1))       //when p2 of line1 are on the line2
        return true;
    if(dir4==0.0 && onLine(l2, l1.p2)) //when p1 of line1 are on the line2
        return true;
    return false;
}


bool checkInside(const std::vector<Point> &poly,const Point &p) {
    int n = poly.size();
    if(n < 3)
        return false;                  //when polygon has less than 3 edge, it is not polygon
    line exline = {p, {9999.0, p.y}};   //create a point at infinity, y is same as point p
    int count = 0;
    int i = 0;
    do {
        line side = {poly[i], poly[(i+1)%n]};     //forming a line from two consecutive points of poly
        if(isIntersect(side, exline)) {          //if side is intersects exline
            if(direction(side.p1, p, side.p2) == 0.0)
                return onLine(side, p);
            count++;
        }
        i = (i+1)%n;
    } while(i != 0);

//    LOGE("Counter: %d N:%d exline.p1.x:%f, y:%f",count,n,exline.p1.x,exline.p1.y);
    return count&1;             //when count is odd
}


bool checkInside(Point poly[], int n, Point p) {
    if(n < 3)
        return false;                  //when polygon has less than 3 edge, it is not polygon
    line exline = {p, {999999, p.y}};   //create a point at infinity, y is same as point p
    int count = 0;
    int i = 0;
    do {
        line side = {poly[i], poly[(i+1)%n]};     //forming a line from two consecutive points of poly
        if(isIntersect(side, exline)) {          //if side is intersects exline
            if(direction(side.p1, p, side.p2) == 0.0)
                return onLine(side, p);
            count++;
        }
        i = (i+1)%n;
    } while(i != 0);
    return count&1;             //when count is odd
}

std::vector<double> getCoords(float *array, int length) {
    std::vector<double> list;
    for(int i = 0 ; i < length ; i++ )
    {
        list.push_back(array[i]);
    }
    return list;
}

std::vector<Point> getPointList(float *array, int length) {

    std::vector<Point> list;

    if( length%2 == 1 )
    {
//        LOGE("Mod% 2 != 0");
        return list;
    }

    for(int i = 0 ; i < length ; i++  )
    {
        if( i+1 < length )
        {
            Point p;
            p.x = array[i];
            p.y = array[i+1];
            list.push_back(p);
            i++;
        }
    }

    return list;
}



float getRandomColor( int begin , int end ) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(begin, end);
    int _r  = dis(gen);
    float r = ((float)_r) / 100.0f;
    return r;
}


