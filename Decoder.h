#ifndef DECODER_H
#define DECODER_H

#include<iostream>
#include<cstdlib>
#include<string>
#include<fstream>
#include<map>
#include"color.h"

using namespace std;

class Decoder
{
private:
    map<char,double> lookupTable;
    map<char,double> freq;
    map<char,int> occ;
    short cumfreq;
    double code;

    //metodo che restituisce le informazioni necessarie per la decodifica
    void readInfo(string in);

    //calcolo probabilità dei singoli caratteri
    void model();

    //aggiornamento mappa
    void updateMap(double low, double high);

public:
    Decoder(){cumfreq = 0;code = 0.0;}

    //processo di codifica
    void decoding(string inFile,string outFile);

    ~Decoder(){}
};

#endif //DECODER_H
