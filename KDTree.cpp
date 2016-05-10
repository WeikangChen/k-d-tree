#include "KDTree.h"

KDTree::KDTree(): root(NULL), m_points(), m_points_size(0) { }

KDTree::KDTree(vector<Point> &points): root(NULL), m_points(points), m_points_size(0) {

    if(m_points.empty()) return;
    m_points_size = m_points.size();
    m_point_dim = points[0].dimension();

    vector<Point*> ppts;
    for (auto &pt: m_points) ppts.push_back(&pt);
    root = build(ppts, 0, m_points_size - 1, 0);
}

KDTree::~KDTree() {
    delete root;
}

KDTree::KDNode* KDTree::build(vector<Point*> &points, int l, int r, int depth) {

    const int n = r - l + 1;
    if (n <= 0)
        return NULL;
    if (n == 1) 
        return new KDNode(*points[l], -1);

    //int split_axis = splitPoint_method1(points, l, r);
    int split_axis = splitPoint_method2(points, depth);
    int l1 = l, r1 = r, m = l + (r-l) / 2;
    int p = partition(points, l, r, split_axis);
    while(m != p)
    {
        if(m < p)
            r1 = p - 1;
        else
            l1 = p + 1;
        p = partition(points, l1, r1, split_axis);
    }
    KDNode *cur = new KDNode(*points[m], split_axis);
    cur->left = build(points, l, m, depth+1);
    cur->right = build(points, m+1, r, depth+1);
    return cur;
}

int KDTree::partition(vector<Point*> &points, int l, int r, int d)
{
    int q = l + 1.0 * rand() / RAND_MAX * (r-l+1);
    real x = (*points[q])[d];
    swap(points[q], points[r]);

    int p = l;
    for(int i = l; i < r; ++i) {
        if((*points[i])[d] < x)
            swap(points[p++], points[i]);
    }
    swap(points[p], points[r]);
    return p;
}

int KDTree::splitPoint_method1(vector<Point*> &points, int l, int r) {
    const int n = r - l + 1;
    Point x_min(*points[l]);
    Point x_max(*points[l]);
    
    for (int i = 1; i < n; ++i) {
        for(int d = 0; d < m_point_dim; ++d) {
            x_min[d] =  min(x_min[d], (*points[i])[d]);
            x_max[d] =  max(x_max[d], (*points[i])[d]);
        }
    }

    double max_variance = x_max[l] - x_min[l];
    int max_variance_dim = 0;

    for (int d = 1; d < m_point_dim; ++d) {
        if (x_max[d] - x_min[d] > max_variance) {
            max_variance = x_max[d] - x_min[d];
            max_variance_dim = d;
        }
    }

    debug(
        debug_in << "max - min = " << max_variance << " on dim " << max_variance_dim << endl;
        debug_in << x_min.getCoordsStr() << endl;
        debug_in << x_max.getCoordsStr() << endl;
        );

    return max_variance_dim;
}

int KDTree::splitPoint_method2(vector<Point*> &points, int depth) {
    
    return depth % m_point_dim;
}


Point& KDTree::findNearest(Point query) {
    Point* pNearest = findNearestUtil(root, query);
    if (!pNearest) return NULL_POINT;
    return *pNearest;
}

Point* KDTree::findNearestUtil(KDNode *cur, Point query) {
    
    if (cur == NULL)
        return NULL;
    
    if (!cur->left && !cur->right)
        return &(cur->point);
        
    int dim = cur->split_dim;
    real axis_distance =  query[dim] - (cur->point)[dim];
    real oneside_distance;
    real another_distance;  
    Point *oneside_nearest = NULL;
    Point *another_nearest = NULL;
  

    if (axis_distance < 0) {
        oneside_nearest = findNearestUtil(cur->left, query);
        oneside_distance = distance(query, *oneside_nearest);
        if (oneside_distance > fabs(axis_distance)) {
            another_nearest = findNearestUtil(cur->right, query);
            another_distance = distance(query, *another_nearest);
        }
    }
    else {
        oneside_nearest = findNearestUtil(cur->right, query);
        oneside_distance = distance(query, *oneside_nearest);
        if (oneside_distance > fabs(axis_distance)) {
            another_nearest = findNearestUtil(cur->left, query);
            another_distance = distance(query, *another_nearest);
        }
    }

    if (another_nearest && another_distance < oneside_distance)
        return another_nearest;
                
    return oneside_nearest;
}

void KDTree::serializeTreeUtil(ofstream &fout, KDNode *cur) {
    if (cur) {
        fout << cur->point.getIndex() << ":" << cur->split_dim << " ";
        serializeTreeUtil(fout, cur->left);
        serializeTreeUtil(fout, cur->right);
    } else {
        fout << "$ ";
    }
}

KDTree::KDNode* KDTree::deserializeTreeUtil(ifstream &fin) {
    string token;
    fin >> token;
    if (token.empty() || token == "$") return NULL;
    
    istringstream iss(token);
    int index, split_dim;
    char delim;
    iss >> index >> delim >> split_dim;
    
    if (index > m_points_size) {
        cerr << "[Error] index > m_points_size: " << index << endl;
        return NULL;
    }
    
    KDNode* cur = new KDNode(m_points[index], split_dim);
    cur->left = deserializeTreeUtil(fin);
    cur->right = deserializeTreeUtil(fin);
    return cur;
}

int KDTree::save(const char *filename) {
    
    ofstream fout(filename);
    if (fout.fail())
    {
        cerr << "[Error] Failed to save KDTree to file: " << filename  << endl;
        return -1;
    }

    fout << m_points.size() << endl;
    for (auto &pt: m_points) {
        fout << pt.getCoordsStr() << endl;
    }

    serializeTreeUtil(fout, root);
    
    fout.close();
    return 0;
}

int KDTree::load(const char *filename) {
    
    ifstream fin(filename);
    if (fin.fail())
    {
        cerr << "[Error] Failed to recover KDTree from file: " << filename  << endl;
        return -1;
    }
    
    string line, token;
    getline(fin, line);
    m_points_size = stoi(line);
    
    m_points.clear();
    for (int i = 0; i < m_points_size; ++i) {
        getline(fin,line);
        istringstream iss(line);
        vector<real> pt;
        while (getline(iss, token, ',')) {
            pt.push_back(stod(token));
        }
        m_points.push_back(Point(pt, i));
    }

    delete root;
    root = deserializeTreeUtil(fin);
    fin.close();
    
    if (!root)
        return -1;
    return 0;
}


Point KDTree::NULL_POINT = Point(vector<real>());
