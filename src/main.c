#include <stdlib.h>

#include "dictionary.h"

void menuTranslator(DicWord word)
{
    char dest[50], usr[50], from[5], to[5];
    char lang_dir = 0; // 0 = basic

    printf("[zadanim znaku '0' ukoncete]\n");
    printf("[zadanim znaku '1' otocte jazyk]\n");

    dicGetFromLangage(word, from, 5);
    dicGetToLangage(word, to, 5);

    do {
        if (!lang_dir)
            printf("Zadejte slovo ktere chcete prelozit z [%s - > %s]: ", from, to);
        else
            printf("Zadejte slovo ktere chcete prelozit z [%s - > %s]: ", to, from);
        
        gets_s(usr, 50);

        if (usr[0] == '0') break;
        if (usr[0] == '1') { lang_dir = ++lang_dir & 1; continue; } // 00->01 & 01 -> 01 || 01->10 & 01 -> 00

        if (!lang_dir)
            dicTranslate(word, usr, dest, 50);
        else
            dicTranslateBack(word, usr, dest, 50);

        if (dest[0] != '\0')  printf("Prelozene slovo: %s\n", dest);
        else	   printf("Slovo nenalezeno\n");
    } while (1);
}

void menuTestWholeDic(DicWord word, unsigned int word_count)
{
    DicTest t;
    dicTestWord(word, &t, word_count);
}

void menuTestLesson(DicWord word, unsigned int lesson, unsigned int word_count)
{
    DicTest t;
    dicTestWordLesson(word, &t, lesson, word_count);
}

int main(void)
{ 
    // Creates a word
    DicWord cs_en;
    
    cs_en = dicCreateWord(DIC_FETCH_CSVFILE, "C:\\dev\\dictionary\\resources\\word\\cs_en.csv");

    // dicPrintLessonList(cs_en);
    // dicPrintLesson(cs_en, 1);
    // dicPrintWord(cs_en);
    
    // menuTranslator(cs_en);
    // menuTestWholeDic(cs_en, 2);
    // menuTestLesson(cs_en, 2, 3);


    //TODO: dicFixDuplicates(cs_en);
    //TODO: dicAddWords(cs_en, DIC_FETCH_CONSOLE, NULL);
    //TODO: dicAddWords(cs_en, DIC_FETCH_CSVFILE, "C:\\dev\\dictionary\\resources\\word\\en_cs.csv");
    //TODO: dicSaveWord(cs_en, "C:\\dev\\dictionary\\resources\\word\\cs_en-mod.csv");

    dicDestroyWord(cs_en);

    system("pause>nu");
    return 0;
 }