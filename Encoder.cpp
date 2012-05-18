#include "Encoder.h"

//metodo che restituisce la frequenza dei caratteri
void Encoder::readFreq(string in)
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
        while (ifile.good())
        {
            char ch = ifile.get();
            if ( (ch&255)!=255 && (ch&255)!=10 )
            {
                ++cumfreq;
                if (occ.find(ch) != occ.end())
                    ++occ[ch];

                else
                {
                    occ.insert(pair<char,int>(ch,1));
                    freq.insert(pair<char,double>(ch,0.0));
                    lookupTable.insert(pair<char,double>(ch,0.0));
                }
            }
        }
    }
    ifile.close();
}

//calcolo occorrenze totali
void Encoder::model()
{
    map<char,double>::iterator itFreq = freq.begin();
    map<char,int>::iterator itOcc = occ.begin();
    while ( itFreq != freq.end())
    {
        itFreq->second = itOcc->second / static_cast<double>(cumfreq);
        ++itFreq;
        ++itOcc;
    }
}

//aggiornamento mappa
void Encoder::updateMap(double low, double high)
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

//scrittura codifica su file
void Encoder::writeOutFile(string out, double code)
{
    const char* outFile = out.c_str();
    ofstream ofile(outFile,ios::trunc | ios::binary);
    if (ofile.fail())
    {
        cerr<<RED<<"Impossibile aprire il file"<<endl;
        exit(1);
    }
       ofile.put(cumfreq&255);
       map<char,int>::iterator itOcc = occ.begin();
       while ( itOcc != occ.end())
       {
           ofile.put(itOcc->first);
           ofile.put(itOcc->second & 255);
           ++itOcc;
       }
        long long iCode =(long long &) code;
        for (short i = 7;i >= 0;i--)
        {
            ofile.put((iCode >> (i*8))&255);
        }
     cout<<YELLOW<<"\n >> Codice : "<<code<<endl;
    ofile.close();
}

//processo di codifica
void Encoder::encoding(string in,string out)
{
    readFreq(in);
    cout<<YELLOW<<" >> Calcolo delle occorrenze..."<<endl;
    model();
    cout<<YELLOW<<" >> Calcolo delle probabilità..."<<endl;
    updateMap(0.0,1.0);
    cout<<YELLOW<<" >> Processo di codifica avviato..."<<endl<<"    ";
    const char* inFile = in.c_str();
    ifstream ifile(inFile,ios::binary);
    if (ifile.fail())
    {
        cerr<<RED<<"Impossibile aprire il file"<<endl;
        exit(1);
    }
    else
    {
        double high = 1.0,low = 0.0;
        char ch = 0;
        int i = 0;
        while (i < cumfreq)
        {
            cout<<GREEN<<".";
            ch = ifile.get();
            if ( (ch&255)!=255 && (ch&255)!=10 )
            {
                map<char,double>::iterator it = lookupTable.find(ch);
 
                low = it->second;                
                ++it;
                if (it != lookupTable.end())
                {
                    high = it->second;
                    
                }
                updateMap(low,high);
                ++i;
            }
        }
        writeOutFile(out,low);
    }
    ifile.close();
    short sizeOutput = (lookupTable.size()*2 + 9);
    float ratio = 100 - ( sizeOutput * 100 / cumfreq );
    cout<<YELLOW<<" >> Size Input  : "<<cumfreq<<" byte"<<endl;
    cout<<YELLOW<<" >> Size Output : "<<sizeOutput<<" byte"<<endl;
    entropia();
    cout<<YELLOW<<" >> Rapporto di compressione : "<<ratio<<" %"<<endl;
    cout<<GREEN<<"\a >> File codificato"<<endl;
}

void Encoder::entropia()
{
    double H = 0.0;
    map<char,double>::iterator itFreq = freq.begin();
    while ( itFreq != freq.end())
    {
        H +=  itFreq->second * log2(itFreq->second);
        ++itFreq;
    }
    cout<<YELLOW<<" >> Entropia teorica : "<<-H<<" bits"<<endl;
}
