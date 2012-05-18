#include <cstring>
#include "Encoder.h"
#include "Decoder.h"

using namespace std;

int main(int argc,char *argv[])
{
    
    if (argc == 4 && strcmp(argv[1],"-d") == 0 )
    {
        Decoder decod;
        string input = argv[2];
        string output = argv[3];
        decod.decoding(input,output);
    }
    else if (argc == 3 && argv[1][strlen(argv[1])-1] != 'c')
    {
         Encoder cod;           
         string input = argv[1];
         string output = argv[2];
         cod.encoding(input,output);
    }
    else
    {
        cerr<<RED<<"Modo d'uso : "<<argv[0]<<" [-d] fileI fileO"<<endl;
        cerr<<RED<<"\t -d : decomprime\n\t fileI : file di input"<<endl;
        cerr<<RED<<"\t fileO : file di output"<<endl;
        exit(1);
    }       
    cout<<DEFAULT;
    return 0;
}

