#include <stdlib.h>

#include "dictionary.h"

int main(void)
{ 
    // Creates a word
    DicWord cs_en;
    
    cs_en = dicCreateWord(DIC_FETCH_CSVFILE, "C:\\dev\\dictionary\\resources\\word\\cs_en.csv");
  
    
    // dicPrintWord(_d);      //Prints local dic. f.e. EN -> CZ
    // dicPrintSection(_d, 2) // Prints lection f.e.   EN -> CZ

    dicPrintWord(cs_en);

    // dicTranslate(cs_en,
    //      DIC_CONSOLE,NULL
    //      DIC_FILE,"path/to/file"

    // dicParameters _par;
    // dicGerParameters(cs_en, _par);

    dicDestroyWord(cs_en);


    return 0;
 }