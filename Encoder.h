#ifndef ENCODER_H
#define ENCODER_H

#include<iostream>
#include<cstdlib>
#include<string>
#include<fstream>
#include<map>
#include<cmath>
#include"color.h"

using namespace std;

class Encoder
{
private:
    map<char,double> lookupTable;
    map<char,double> freq;
    map<char,int> occ;
    short cumfreq;

    //metodo che restituisce la frequenza dei caratteri
    void readFreq(string in);

    //calcolo probabilità dei singoli caratteri
    void model();

    //aggiornamento mappa
    void updateMap(double low, double high);
    
    //calcola dell'entropia teorica
    void entropia();
    
    //scrittura codifica su file
    void writeOutFile(string out, double code);

public:
    Encoder(){cumfreq = 0;}

    //processo di codifica
    void encoding(string inFile,string outFile);

    ~Encoder(){}
};

#endif //ENCODER_H
