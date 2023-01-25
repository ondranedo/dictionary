#include <stdlib.h>

#include "dictionary.h"

int main(void)
{ 
    // Creates a word
    DicWord cs_en;
    
    cs_en = dicCreateWord(DIC_FETCH_CSVFILE, "C:\\dev\\dictionary\\resources\\word\\cs_en.csv");
  
    
    // dicPrintWord(_d);      // Prints local dic. f.e. EN -> CZ
    // dicPrintSection(_d, 2) // Prints lection f.e.   EN -> CZ

    dicPrintWord(cs_en);
    

    char dest[50], word[50];

    do {
        printf("Zadejte slovo ktere chcete prelozit [ukoncete '0']: ");
        gets_s(word, 50);

        if (word[0] == '0') break;

        dicTranslate(cs_en, word, dest, 50);

        if (dest[0] != '\0')  printf("Prelozene slovo: %s\n", dest);
        else	   printf("Slovo nenalezeno\n");
    } while (1);

    
    // dicTranslate(cs_en,
    //      DIC_CONSOLE,NULL
    //      DIC_FILE,"path/to/file"

    // dicParameters _par;
    // dicGerParameters(cs_en, _par);

    dicDestroyWord(cs_en);


    return 0;
 }