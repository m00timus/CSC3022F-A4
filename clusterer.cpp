#include<iostream>
#include"Header.h"
using namespace std;

int main(int argc, char const *argv[])
{
    GRLMAT001::PPM PPM;

    if (argc < 2)
    {   
        //input error catching
        cout << "Incorrect input parameters!\n";
    } else if (argc == 2)
    {
        //run standard implementation
        PPM.input(argc,argv);
        PPM.read(argv[1]);
        PPM.makeHistogram();
        PPM.genCluster();
        while (PPM.part1() == false)
        {
            PPM.part1();
        }
        PPM.outFile(PPM);

    } else if (argc == 4 || argc == 6 || argc == 8)
    {
        //optional command/s
        PPM.input(argc,argv);
        PPM.read(argv[1]);
        PPM.makeHistogram();
        PPM.genCluster();
        while (PPM.part1() == false)
        {
            PPM.part1();
        }
        PPM.outFile(PPM);
        
    } else {cout << "incorrect input parameters!\n";}

    return 0;
}
