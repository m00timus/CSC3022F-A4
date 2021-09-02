***README FOR A4***

Unsupervised image processing
Built by GRLMAT001 @home

Instructions of use:
1)    compile program using: make release

2)    application designed to take input in any order eg. ./clusterer Gradient_Numbers_PPMS -o outFile -k 25 -bin 4
                                                        ./clusterer Gradient_Numbers_PPMS -k 10 -o outputFile
    ensure -k and -bin are < 100 (obviously) otherwise might cause errors..

File Descriptions:
    Header.h: contains all values being used in the program as well as defines methods used

    clusterer.cpp: the Main() file for this program, is where all the methods are called and input handling is done

    PPM.cpp: the bulk of our program, majority of the main methods are here

Outputs:
    the application has some outputs which tell you at what point in the program it is at via cout messages. i decided to leave some of these in to help error catch should the program fail.

***END***
------------------------------------------------------------------------------------------------------------------


