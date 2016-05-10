#ifndef _KDTREE_H_
#define _KDTREE_H_

#include "Point.h"

class KDTree {
private:
    struct KDNode {
        Point &point;
        int split_dim;

        KDNode *left;
        KDNode *right;
        
        KDNode(Point &p, int d = -1, KDNode *l = NULL, KDNode *r = NULL):
        point(p), split_dim(d), left(l),right(r) {}

        ~KDNode()
        {
            delete left;
            delete right;
        }
    };

    KDNode *root;
    
    static Point NULL_POINT;    
    vector<Point> m_points;
    int m_points_size;
    int m_point_dim;

    KDNode* build(vector<Point*> &points, int l, int r, int depth);
    int partition(vector<Point*> &points, int l, int r, int dim);
    int splitPoint_method1(vector<Point*> &points, int l, int r);
    int splitPoint_method2(vector<Point*> &points, int depth);
    
    Point* findNearestUtil(KDNode *cur, Point query);
    void serializeTreeUtil(ofstream &fout, KDNode *cur);
    KDNode* deserializeTreeUtil(ifstream &fin);
    
public:
    KDTree();
    KDTree(vector<Point> &points);
    ~KDTree();
    Point &findNearest(Point query);
    int save(const char* filename);
    int load(const char* filename);
};



#endif // _KDTree_H_
