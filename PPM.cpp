#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <random>
#include "string.h"
#include "Header.h"
#include "dirent.h"

using namespace std;

namespace GRLMAT001
{

//default constructor
PPM::PPM()
{
    mindist = __DBL_MAX__;
    width = 0;
    height = 0;
    size = width * height;
    hist = 1;
    clusters = 10;
    output = "std::cout";
    imageNo = 0;
    it = 0;
    clusterNo.resize(100);
    minDists.resize(100);
}

//destructor
PPM::~PPM()
{
}

void PPM::input(int argc, const char *argv[]) //input argument decider
{

    filedir = argv[1];    //set file directory
    output = ""; //default
    clusters = 10;        //default
    hist = 1;             //default

    if (argc >= 4)
    {
        if (!strcmp(argv[2], "-o"))
        {
            output = argv[3];
            cout << "output set to " << output << "\n";
        }
        else if (!strcmp(argv[2], "-k"))
        {
            clusters = stoi(argv[3]);
            cout << "number of clusters set to " << clusters << "\n";
        }
        else if (!strcmp(argv[2], "-bin"))
        {
            hist = stoi(argv[3]);
            cout << "width of histogram set to " << hist << "\n";
        }
        else
        {
            cout << "optional input parameters on first argument are incorrect!\n";
        }
    }
    if (argc >= 6)
    {
        if (!strcmp(argv[4], "-o"))
        {
            output = argv[5];
            cout << "output set to " << output << "\n";
        }
        else if (!strcmp(argv[4], "-k"))
        {
            clusters = stoi(argv[5]);
            cout << "number of clusters set to " << clusters << "\n";
        }
        else if (!strcmp(argv[4], "-bin"))
        {
            hist = stoi(argv[5]);
            cout << "width of histogram set to " << hist << "\n";
        }
        else
        {
            cout << "optional input parameters on second argument are incorrect!\n";
        }
    }
    if (argc == 8)
    {
        if (!strcmp(argv[6], "-o"))
        {
            output = argv[7];
            cout << "output set to " << output << "\n";
        }
        else if (!strcmp(argv[6], "-k"))
        {
            clusters = stoi(argv[7]);
            cout << "number of clusters set to " << clusters << "\n";
        }
        else if (!strcmp(argv[6], "-bin"))
        {
            hist = stoi(argv[7]);
            cout << "width of histogram set to " << hist << "\n";
        }
        else
        {
            cout << "optional input parameters on first argument are incorrect!\n";
        };
    }
}

void PPM::read(const string &filedir)       //runs the readImage method for amount of images
{
    cout << "reading in images...";
    DIR *x = opendir(filedir.c_str());
    struct dirent *dir;
    string t; 

    while ((dir = readdir(x)) != NULL)
    {
        t = dir->d_name;
        if (t != "." && t != "..")
        {
            readImage(t, filedir, imageNo);
            imageNo++;
            imageNames.push_back(t);
        }
    }
    cout << "images read in successfully\n";
    closedir(x);
}

void PPM::readImage(string &filename, const string &filedir, int &imageNo) //reads in each image into memory
{
    string fullpath = filedir + "/" + filename;
    ifstream in(fullpath.c_str(), ios::in | ios::binary); //creating input file stream in
    if (in.is_open())
    {
        string line = "";
        getline(in, line); //get next line in file
        if (line != "P6")  //check first line
        {
            cout << "Error - not a PPM file!\n";
        }
        getline(in, line);
        while (line[0] == '#')
        {
            getline(in, line); //loop if comment detected
        }
        try
        {
            string delimiter = " ";
            size_t pos = line.find(delimiter);
            string Nrows = line.substr(0, pos);
            string Ncols = line.erase(0, pos + delimiter.length()); //splits width and height and stores vals
            width = stoi(Nrows);
            height = stoi(Ncols);
            size = width * height;
            getline(in, line);
            string maxS = line;
            max = stoi(maxS);
        }
        catch (exception e)
        {
            cout << "error: Header format error"; //catches if format is different/fails
            return;
        }

        try
        {
            char temp;
            images.push_back(new unsigned char *[width]);
            for (int j = 0; j < width; j++)
            {
                images[imageNo][j] = new unsigned char[height];
                for (int k = 0; k < height; k++)
                {
                    int pixel;
                    in.read(&temp, 1);
                    r = (int)temp;
                    in.read(&temp, 1);
                    g = (int)temp;
                    in.read(&temp, 1);
                    b = (int)temp;
                    pixel = 0.21 * r + 0.72 * g + 0.07 * b;
                    images[imageNo][j][k] = pixel;
                }
            }
        }
        catch (exception e)
        {
            cout << "error reading in unsigned char data!\n"
                 << e.what() << "\n";
        }
    } //end of open file statement error catching
    else
    {
        cout << "error opening file\n";
    }
    in.close();
}

void PPM::makeHistogram() //generates the histogram data to be used for clustering
{
    cout << "making histogram...";
    hSize = round(256 / hist); //gets number of histogram points
    for (int i = 0; i < imageNo; i++)
    {
        histogram.push_back(new int[hSize]); //push back histogram for given bin size

        for (int z = 0; z < hSize; z++) //iterate to initialize values to 0
        {
            histogram[i][z] = 0;
        }

        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < height; k++)
            {
                try
                {
                    int point = (int)images[i][j][k]; //gets value relative to input preferences
                    int val = round(point / hist);
                    histogram[i][val]++;
                }
                catch (exception e)
                {
                    cout << "error in histogram generation: " << e.what() << "\n";
                }
            }
        }
    }
    cout << "histogram successfully generated\n";
}

bool PPM::part2() //part 2 of cluster algorithm - calculates mean for clusters and restrts if change is evident
{
    bool result2 = true; //defaults to true
    for (int i = 0; i < clusters; i++)
    {
        //cout << cluster[i][1] << "\n";
        previousCluster.push_back(new int[hSize]);
        for (int n = 0; n < hSize; n++)
        {
            previousCluster[i][n] = cluster[i][n]; //copies cluster to compare for changes later
        }
            int count = 1;
            for (int l = 0; l < imageNo; l++)
            {
                if (clusterNo[l] == i)
                {
                    for (int j = 0; j < hSize; j++)
                    {
                    cluster[i][j] += histogram[l][j];   //adds histogram values for each occurence in the cluster
                    }
                    count++;
                }
            }

            for (int j = 0; j < hSize; j++)
            {
                cluster[i][j] = round(cluster[i][j]/count);     //rounds by number of histograms added
            }
            
        
    }

    for (int q = 0; q < clusters; q++)
    {
        for (int r = 0; r < hSize; r++)
        {
            if (cluster[q][r] != previousCluster[q][r]) //check if values changing
            {
                result2 = false;
                break;
            }
        }
    }
    return result2;
}

bool PPM::part1() //part 1 of cluster algorithm - calculates distances to find closest cluster and assigns each image accordingly
{
    bool result = false;
    int clusterA;

    for (int l = 0; l < imageNo; l++) //for each histogram
    {
        mindist = __DBL_MAX__;
        clusterA = -1;
        for (int m = 0; m < clusters; m++) //for cluster k to compare to
        {
            double dist = 0;
            for (int n = 0; n < hSize; n++) //for each part of histogram
            {
                dist += (histogram[l][n] - cluster[m][n]) * (histogram[l][n] - cluster[m][n]); //dist is sum of sqrt(E(p.x - x)^2)
            }
            dist = sqrt(dist);
            if (dist < mindist)
            {
                mindist = dist;
                clusterA = m;
            }
        }
        clusterNo[l] = clusterA;
    }
    if (part2() == true) //if values not changing
    {
        result = true;
    }
    else
    {
        result = false;     //if changing values
        it++;
        cout << it << " iterations\n";
    }
    return result;
}

void PPM::genCluster()  //generates randomly selected unique clusters
{
    srand(time(0)); //random seed selection
    vector<int> no;
    no.resize(imageNo);

    for (int i = 0; i < clusters; i++)
    {
        cluster.push_back(new int[hSize]);
        bool check;
        int n;
        do
        {
            n = rand() % imageNo;
            check = true;
            for (int j = 0; j < i; j++)
                if (n == no[j])
                {
                    check = false;
                    break;
                }
        } while (!check);
        no[i] = n;
        for (int k = 0; k < hSize; k++)
        {
            cluster[i][k] = histogram[no[i]][k];
        }
    }
}

void PPM::outFile(PPM p)        //prints to file using overloaded operator <<
{
    
    if (output == "")
    {
        cout << p;
    } else
    {
        ofstream of;
        of.open(output + ".txt");
        of << p;
    }
    
    
}

ostream& operator<<(ostream& os, const PPM& PPM )       //overloading output operator <<
{
        for (int i = 0; i < PPM.clusters; i++)
        { 
            os << "Cluster " << i << ": ";
            for (int j = 0; j < PPM.imageNo; j++)
            {
                if (PPM.clusterNo[j] == i)
                {
                    
                    os << PPM.imageNames[j] << " ";
                }     
            }
            os << "\n"; 
        }
             
        return os;
    
}

} // namespace GRLMAT001