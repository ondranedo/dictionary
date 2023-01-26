#include <stdlib.h>

#include "dictionary.h"

void menuAskForWords(DicWord word)
{
    char dest[50], usr[50];

    do {
        printf("Zadejte slovo ktere chcete prelozit [ukoncete '0']: ");
        gets_s(usr, 50);

        if (usr[0] == '0') break;

        dicTranslate(word, usr, dest, 50);

        if (dest[0] != '\0')  printf("Prelozene slovo: %s\n", dest);
        else	   printf("Slovo nenalezeno\n");
    } while (1);
}

int main(void)
{ 
    // Creates a word
    DicWord cs_en;
    
    cs_en = dicCreateWord(DIC_FETCH_CSVFILE, "C:\\dev\\dictionary\\resources\\word\\cs_en.csv");
  
  
    // dicPrintSection(cs_en, 1);
    // dicPrintSection(cs_en, 2);
    // dicPrintWord(cs_en);
    // menuAskForWords(cs_en); 
    
    while (1)
    {
        DicTest t1;
        dicTestWord(cs_en, &t1, 3);
    }
    
    // dicTeWordBackwards(cs_en, &t1, 3);

    // DicTest t2;
    // dicTestWordLesson(cs_en, &t2, 2, 4);
    //  dicTestWordLessonBackwards(cs_en, &t2, 2, 4);


    dicDestroyWord(cs_en);


    return 0;
 }