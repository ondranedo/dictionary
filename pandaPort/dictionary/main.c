#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

void flushI()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void menuTranslator(DicWord word)
{
	char dest[50], usr[50], from[5], to[5];
	char lang_dir = 0; // 0 = basic
	
	printf("[zadanim znaku '0' ukoncete]\n");
	printf("[zadanim znaku '1' otocte jazyk]\n");
	
	dicGetFromLangage(word, from, 5);
	dicGetToLangage(word, to, 5);
	flushI();
	
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
		fflush(stdin);
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
			fflush(stdin);
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

void menuEdit(DicWord word)
{
	char usr_buff[60], from[30], to[30];
	int i, lesson;
	system("cls");
	dicPrintWord(word);
	printf("---------------------\n");
	printf("Zadejte slovo (levy sloupec), ktery checete editovat: ");
	flushI();
	gets_s(usr_buff, 60);
	for(i = 0; i < word->word_count; i++)
		if (!strcmp(word->word_entry_array[i].from_word, usr_buff))
	{
		system("cls");
		printf("Vybrane slovo: %i %s %s\n", word->word_entry_array[i].lesson, word->word_entry_array[i].from_word, word->word_entry_array[i].to_word);
		printf("Chcete editovat pouze lekci? (1 - ano, 0 - ne");
		if (menuLoadInt(0, 1))
		{
			printf("Zadejte nove cislo lekce: ");
			scanf("%d", &lesson);
			strcpy(from, word->word_entry_array[i].from_word);
			strcpy(to, word->word_entry_array[i].to_word);
		}
		else
		{
			printf("Zadejte nove cislo lekce: ");
			scanf("%d", &lesson);
			printf("Zadejte slovo z ktereho se preklada (levy sloupec): ");
			flushI();
			gets_s(from, 30);
			if (dicCheckForDuplicate(word, from))
			{
				printf("Slovo %s jiz existuje ve slovniku!\n", from);
				return;
			}
			printf("Zadejte slovo do ktereho se preklada (pravy sloupec): ");
			gets_s(to, 30);
		}
		
		dicReplaceWordEntrie(word, usr_buff, lesson, from, to);
		printf("Editovane slovo: %i %s %s\n", word->word_entry_array[i].lesson, word->word_entry_array[i].from_word, word->word_entry_array[i].to_word);
		break;
	}
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
					flushI();
					gets_s(path, 100);
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
				case 7: menuEdit(word); eddited = 1; system("pause"); break;
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
