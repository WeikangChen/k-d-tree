#include "config.h"
#include "Point.h"
#include "KDTree.h"

int main(int argc, char ** argv)
{
    if(argc < 3) {
        cerr << "Usage: " << argv[0] << " query_data.csv kdtree.log"<< endl;
        exit(-1);
    }
    
    KDTree kd_tree;
    if (kd_tree.load(argv[2]) == -1)
        return -1;
    
    vector<Point> query_points;
    import_data(argv[1], query_points);

    for (auto pt : query_points) {
        Point &nearest_pt = kd_tree.findNearest(pt);
        cout << nearest_pt.getIndex() << endl;
    }

    return 0;
}
