#ifndef Header
#define Header
#include <string>
#include<vector>

namespace GRLMAT001
{

using namespace std;

class PPM
{
private:
    string filedir;
    string filename;
    string output;
    int imageNo;
    int clusters;
    int hist;
    int hSize;
    int width;
    int height;
    int max;
    int size;
    int r,g,b;
    double mindist;
    int it;
    
public:
    PPM();  // default constructor
    ~PPM(); // destructor

    void input(int argc,char const *argv[]);
    void readImage(string &filename,const string &filedir,int &imageNo);
    void read(const string &filedir);
    void makeHistogram();
    bool part2();
    void genCluster();
    bool part1();
    void outFile(PPM p);

    vector<unsigned char **> images; 
    vector<int *> histogram;
    vector<int *> cluster;
    vector<int> clusterNo;
    vector<double> minDists;
    vector<int *>previousCluster;
    vector<string> imageNames;

    friend ostream& operator<<( ostream& os, const PPM& PPM);
};

ostream& operator<<(ostream& os, const PPM& PPM);

}
#endif
