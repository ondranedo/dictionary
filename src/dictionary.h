// Created by Ondrej Nedojedly
// 
// Date: 20.01.2023
// Dead-line: 31.01.2023
// Teacher: Ing. Bozena Ralbovska
// 
// Task given:
// 
// Translated with www.DeepL.com / Translator(free version):
// 
//		GLOSSARY
//			Create a program to work with a dictionary that represents a text file. On each line 
//			there is an English word, an English word and a lesson number, all separated by a 
//			suitable separator. A word can be a general phrase (with spaces). In Czech words, 
//			for simplicity, we do not use hooks,...
//			
//				CREATE A USER-FRIENDLY MENU
//					1. TRANSLATION AJ CZ, CZ AJ
//						• It can translate a given word
//						• Separate function
//						• Treat error conditions
//						• Solve upper / lower case problem
//					2. VOCABULARY TEST FROM ONE LESSON
//						• Lists all the Czech words from the lesson in turn and for each one expects its
//						  translation, immediately lists correct / wrong
//						• Separate function
//						• Treat error conditions
//						• Solve upper / lower case problem
//						• List testing statistics
//						• Time limit for testing can be added
//					3. RANDOM TESTING FROM THE WHOLE DICTIONARY
//						• The user specifies how many words he wants to select
//						• The program randomly generates a specified number of words and allows testing
//						• The words are not repeated
//						• Recommendations(good to do at the beginning of random testing)
//						  o Create a custom function to determine the number of words in the
//						  dictionary
//						  o Generate 1 random number(= word order in the dictionary) and do
//						  testing for 1 random word
//						• Separate function
//						• Treat error conditions
//						• Solve upper / lower case problem
//						• List testing statistics
//					4. ADDS A WORD TO THE FILE
//						• Adds the Czech word, English wordand lesson number in the desired format to
//						  the end of the file
//						• We only add a word if it is not in the dictionary
//						• Separate function
//						• Treat error conditions
//						• Solve upper / lower case problem
//					5. EDIT EXPRESSION IN THE DICTIONARY(premium task)
//						• Separate function
//						• The function corrects a dictionary error(overwrites the wrong expression)
//						• Another option - removes the entire line containing the specified word from the
//						  dictionary
//	
//
///////\\ How to read this file /////////////////////////////////////////////////////////
//
// NamingConvetions:
//		dicXxxxx  -  function name				f.e. dicCreateDic(...)
//		DicXxxxx  -  struct/enum/union name		f.e. DicLanguage
//		DIC_XXXX  -  enum/macro value			f.e. DIC_FETCH_CONSOLE
//
//
// FileConvetions:
//			-CSV(MS_DOS)
// 
//		first line:  lesson;[from];[to]			f.e. lesson;cs;en
//		2........N:  [lesson];[from];[tp]		f.e. 2;ahoj;hello
// 
//		NOTE: For [from] and [to] use valid ISO-639-1 language code 
//      (list of valid codes: https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes).
//
// 
// Meaning of words:
//		dic			- dictionary
//		word		- an actual dictionary (CSV file)
//		fetch		- fetch data from
//		word entry	- one line in word (one line in the csv file)
//		from		- languge from we translate
//		to			- language to we translate
//		lesson		- lesson in which we shloud learn the word ( it's really weird,
//					  but the task has been given, soo...)
//		
//	
//
//

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