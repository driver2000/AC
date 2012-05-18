#include "Decoder.h"

using namespace std;

//metodo che restituisce la frequenza dei caratteri
void Decoder::readInfo(string in)
{
    const char* inFile = in.c_str();
    ifstream ifile(inFile,ios::binary);
    if (ifile.fail())
    {
        cerr<<RED<<"Impossibile aprire il file di input"<<endl;
        exit(1);
    }
    else
    {
        char ch = 0,c = 0;
        //Leggo cumfreq
        ch = ifile.get();
        cumfreq = (cumfreq<<8)|(ch&255);
        int z = cumfreq;
        do
        {
            int val = 0;
            
            //Leggo carattere
            ch = ifile.get();
            
            //Leggo occorrenza
            c = ifile.get();
            
            val = (val<<8)|(c&255);
            occ.insert(pair<char,int>(ch,val));
            freq.insert(pair<char,double>(ch,0.0));
            lookupTable.insert(pair<char,double>(ch,0.0));
            z -= val;
        }
        while (ifile.good() && z > 0);
    
        //intero a 64 bit
        long long iCode = 0;
        for (int i = 0;i < 8; ++i)
        {
            ch = ifile.get();
            iCode = (iCode << 8)|(ch&255);

        }
        code = (double &)iCode;
        cout<<YELLOW<<" >> Codice : "<<code<<endl;
    }
    ifile.close();
}

//calcolo occorrenze totali
void Decoder::model()
{
    map<char,double>::iterator itFreq = freq.begin();
    map<char,int>::iterator itOcc = occ.begin();
    while ( itFreq != freq.end())
    {
        itFreq->second = static_cast<double>(itOcc->second) / cumfreq;
        ++itFreq;
        ++itOcc;
    }
}

//aggiornamento mappa
void Decoder::updateMap(double low, double high)
{
    map<char,double>::iterator itTable = lookupTable.begin();
    map<char,double>::iterator itFreq = freq.begin();
    double lowOld = low;
    while ( itTable != lookupTable.end())
    {
        itTable->second = low;
        low += (high - lowOld) * itFreq->second;
        ++itTable;
        ++itFreq;
    }
}

//processo di decodifica
void Decoder::decoding(string in,string out)
{
    readInfo(in);
    cout<<YELLOW<<" >> Calcolo delle occorrenze..."<<endl;
    model();
    cout<<YELLOW<<" >> Calcolo delle probabilità..."<<endl;
    updateMap(0.0,1.0);
    cout<<YELLOW<<" >> Processo di decodifica avviato..."<<endl<<"    ";;
    const char* outFile = out.c_str();
    ofstream ofile(outFile,ios::trunc | ios::binary);

    if (ofile.fail())
    {
        cerr<<RED<<"Impossibile aprire il file"<<endl;
        exit(1);
    }
        double high = 1.0,low = 0.0,lowOld;
        char ch = 0;
        int i = 0;
        map<char,double>::iterator it,itOld;
        while ( i < cumfreq)
        {
            cout<<GREEN<<".";
            it = lookupTable.begin();
            while ( code >= it->second && it != lookupTable.end() )
            {
                itOld = it;
                ++it;
            }
            ch = itOld->first;
            ofile.put((ch&255));
            low = itOld->second;
            if (it != lookupTable.end())
            {
                high = it->second;
            }
            updateMap(low,high);
            ++i;
        }
    ofile.close();
    cout<<GREEN<<endl<<"\a >> File decodificato"<<endl;
}
