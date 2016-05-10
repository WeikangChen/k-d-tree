#ifndef _POINT_H_
#define _POINT_H_

#include "config.h"

template<class T>
class PointTmpl {
private:
    vector<T> coordinates;
    int index;
    
public:
    PointTmpl(vector<T> xs = vector<T>(), int id = -1): coordinates(xs), index(id) {}
    T& operator[] (const int id);
    const T& operator[] (const int id) const;
    const size_t dimension() const;
    string getCoordsStr() const;
    int getIndex() const;
    
    template<class S>
    friend S distance(const PointTmpl<S> a, const PointTmpl<S> b);
    
    template<class S>
    friend void import_data(char *filename, vector<PointTmpl<S> > &points);
};

typedef PointTmpl<real> Point;


//#include "Point.cpp" // since we are using template


template<class T>
T& PointTmpl<T>::operator[] (const int id) {
    return coordinates[id];
}

template<class T>    
const T& PointTmpl<T>::operator[] (const int id) const {
    return coordinates[id];
}

template<class T>
const size_t PointTmpl<T>::dimension() const {
    return coordinates.size();
}

template<class T>
string PointTmpl<T>::getCoordsStr() const {

    const int dim = this->dimension();
    ostringstream oss;
    oss.precision(numeric_limits<long double>::digits10 + 1);
    
    oss << scientific;
    for (int d = 0; d < dim - 1; ++d)
        oss << coordinates[d] << ",";
    oss << coordinates[dim-1];

    return oss.str();
}

template<class T>
int PointTmpl<T>::getIndex() const {
    return index;
}

template<class T>
T distance(const PointTmpl<T> a, const PointTmpl<T> b) {
    T dis2 = 0.;
    const int dim = a.dimension();
    for (int d = 0; d < dim; ++d) {
        T tmp = a[d] -b[d];
        dis2 +=  tmp * tmp;
    }
    return sqrt(dis2);
}

template<class T>
void import_data(char *filename, vector<PointTmpl<T> > &points) {

    ifstream datafile(filename);
    if (datafile.fail())
    {
        cerr << "[Error] Failed to open file: " << filename  << endl;
        exit(-1);
    }
    
    string line, token;
    int id = 0;

    while (getline(datafile, line)) {
        istringstream iss(line);
        vector<T> pt;
        while (getline(iss, token, ',')) {
            pt.push_back(stod(token));
        }
        points.push_back(Point(pt, id++));
    }
    datafile.close();

    debug(debug_in << "Loaded " << points.size() << " points from " << filename << endl;);
    
}

#endif // _POINT_H_
