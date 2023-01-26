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
    DicTest t1;
    dicTestWord(word, &t1, word_count);
}

void menuTestLesson(DicWord word, unsigned int lesson, unsigned int word_count)
{
    DicTest t1;
    dicTestWordLesson(word, &t1, lesson, word_count);
}

int main(void)
{ 
    // Creates a word
    DicWord cs_en;
    
    cs_en = dicCreateWord(DIC_FETCH_CSVFILE, "C:\\dev\\dictionary\\resources\\word\\cs_en.csv");

    // menuTranslator(cs_en);
    // menuTestWholeDic(cs_en, 2);


    //TODO: dicSaveWord(cs_en, "C:\\dev\\dictionary\\resources\\word\\cs_en-mod.csv");
    dicDestroyWord(cs_en);

    return 0;
 }