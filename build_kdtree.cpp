#include "config.h"
#include "Point.h"
#include "KDTree.h"

int main(int argc, char ** argv)
{
    if(argc < 3) {
        cerr << "Usage: " << argv[0] << " sample_data.csv kdtree.log"<< endl;
        exit(-1);
    }
    
    vector<Point> sample_points;
    import_data(argv[1], sample_points);
    
    KDTree kd_tree(sample_points);
    
    kd_tree.save(argv[2]);
    
    return 0;
}
