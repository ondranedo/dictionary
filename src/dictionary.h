// Created by Ondrej Nedojedly
// 
// Date: 20.01.2023

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdio.h>

// Only for MSVC (Visual studio) compiler
#ifdef _MSC_VER
#define DIC_LOG_ERR(x) fprintf_s(stderr, "[%s] -> %s\n", __FUNCSIG__, x)
#else
#define DIC_LOG_ERR(x) fprintf_s(stderr, "%s\n", x)
#endif

#define DIC_CHECK_FILE_V(file,val) if(!file){ DIC_LOG_ERR("File can't be oppened"); return val;}
#define DIC_CHECK_FILE(file) if(!file){ DIC_LOG_ERR("File can't be oppened"); return;}

// typ1es of langugages
// Based on ISO-639-1
typedef enum DicLanguage {
	DIC_LANG_CS,
	DIC_LANG_EN
} DicLanguage;

//lang of the word
typedef struct DicChosenlanguage {
	DicLanguage from;
	DicLanguage to;
} DicChosenlanguage;

// ways of creating word
typedef enum DicFetchType {
	DIC_FETCH_CONSOLE,
	DIC_FETCH_CSVFILE
} DicFetchType;

// one line in word
typedef struct DicWordEntry {
    unsigned short lesson;
    char* from_word;
    char* to_word;
} DicWordEntry;

// Dictionary f.e. cs -> en
typedef struct DicWordData {
	unsigned int word_count;
	DicChosenlanguage lang;
	DicWordEntry* word_entry_array; // Array of lines
} DicWordD;

typedef DicWordD* DicWord;

/**
*	Creates a word in your program
* 
*	fetchType - tells the program where we want to fetch some data: 
*		DIC_FETCH_CONSOLE  - do not specify extraData, set extraData as NULL
*		DIC_FETCH_TEXTFILE - specify the path in extraData for the .csv file, which contains the dic
* 
*/
DicWord dicCreateWord(const DicFetchType fetchType, const void* extraData);

/**
*	Destroys a word, HAVE TO BE CALLED!, heap alloc.
*
*/
void dicDestroyWord(DicWord word);

/**
*	Prints the dictionary to the console
*
*/
void dicPrintWord(DicWord word);

#endif