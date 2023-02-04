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
        fflush(stdin);
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

int menuLoadInt(int from, int to)
{
    int u;
    do {
        printf("> ");
        fflush(stdin);
    }
    while (scanf("%d", &u) != 0 && (u < from || u > to));

    return u;
}

void menuPrintList(DicWord word)
{
    int lesson;
    int exit = 0;

    while (!exit) {
        exit = 1;
        printf("Zadejte cislo lekce kterou chcete vypsat: ");
        scanf("%d", &lesson);
        if (!dicCheckIfLessonExists(word, lesson)) {
            printf("Lekce neexistuje!\n");
            exit = 0;
        }
    }

    dicPrintLesson(word, lesson);
}

void menuTest(DicWord word)
{
    int usr, word_count = 1, lesson, max;
    system("cls");
    printf("Testovani ze slovnku\n");
    printf("\t0 - testovani z jedne lekce\n");
    printf("\t1 - testovani z celeho slovnku\n");

    usr = menuLoadInt(0, 1);
    if (!usr) {
        int exit = 0;
        while (!exit) {
            exit = 1;
            printf("Zadejte cislo lekce kterou chcete testovat: ");
            scanf("%d", &lesson);
            if (!dicCheckIfLessonExists(word, lesson)) {
                printf("Lekce neexistuje!\n");
                exit = 0;
            }
        }
    }

    if (!usr)
    {
        int i;
        for (i = 0; i < word->lessonMap.lesson_count; i++)
        {
            if (word->lessonMap.lesson_arr[i].lesson == lesson)
                max = word->lessonMap.lesson_arr[i].word_count;
        }
    }
    else
       max = word->word_count;

    printf("Zadejte pocet slov, ze kterych chcete byt testovan [max: %d]\n", max);
    word_count = menuLoadInt(1, max);
    

    if (!usr)
        menuTestLesson(word, lesson, word_count);
    else
        menuTestWholeDic(word, word_count);

}

void menuEdit(DicWord edit)
{
    printf("cls");

}

void menuStart()
{
    int running = 1;
    DicWord word = NULL;
    char path[100];
    char from[5], to[5];
    int eddited = 0;

    while (running)
    {
        system("cls");
        if (word == NULL) {
            printf("Slovnik neni nacten\n");
            printf("\t0 - Ukonci program\n");
            printf("\t1 - Nacte slovnik\n");
            switch (menuLoadInt(0, 1)){
                case 0: running = 0; break;
                case 1:
                {
                    printf("Zadejte cestu k CSV souboru: ");
                    scanf("%s", path);
                    word = dicCreateWord(DIC_FETCH_CSVFILE, path);
                }break;
            }
        }
        else
        {
            dicGetFromLangage(word, from, 5);
            dicGetToLangage(word, to, 5);
            printf("Nacten slovnik %s_%s s %d slovy\n", from, to, word->word_count);
            if (eddited) printf("[NEULOZENO]\n");
            printf("\t0 - ukonci program\n");
            printf("\t1 - vypise cely slovnik\n");
            printf("\t2 - vypise urcitou lekci\n");
            printf("\t3 - vypise prehled lekci\n");
            printf("\t4 - spusti prekladac\n");
            printf("\t5 - sputsti testovani\n");
            printf("\t6 - prida slova do slovnkinu\n");
            printf("\t7 - Editace slovniku\n");
            printf("\t8 - uloz slovnik\n");

            switch (menuLoadInt(0,8))
            {
            case 0: if (eddited) {
                printf("Mate neulozene zmeny!\n\t0 - zahodit zmeny\n\t1 - ulozit\n");
                if (menuLoadInt(0, 1))
                    dicSaveWord(word, path);
            } running = 0; break;
            case 1: dicPrintWord(word); system("pause"); break;
            case 2: menuPrintList(word); system("pause"); break;
            case 3: dicPrintLessonList(word); system("pause"); break;
            case 4: menuTranslator(word); break;
            case 5: menuTest(word); system("pause"); break;
            case 6: dicAddWord(word, DIC_FETCH_CONSOLE, NULL); eddited = 1; break;
            case 7: menuEdit(word); eddited = 1; break;
            case 8: dicSaveWord(word, path); eddited = 0; break;
            }
        }
    }
    
    if(word)
        dicDestroyWord(word);
}

int main(void)
{ 
    menuStart();

    return 0;
 }