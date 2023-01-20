#ifndef DICTIONARY_H
#define DICTIONARY_H

typedef enum   DicLanguage;
typedef struct DicInstance;
typedef struct DicChosenlanguage;
typedef enum   DicFetchType;

typedef struct {
	DicChosenlanguage language;
	//DicWordBuffer* wordBuffer;
} DicInstance;

typedef enum
{
	NONE = 0,
	DIC_LANG_CZ,
	DIC_LANG_EN
} DicLanguage;

typedef struct {
	DicLanguage from;
	DicLanguage to;
} DicChosenlanguage;

typedef enum {
	NONE = 0,
	DIC_FETCH_CONSOLE,
	DIC_FETCH_TEXTFILE
} DicFetchType;


/**
*	[! **HAVE TO BE CALLED ** !]
*	Sets the Dictionary localization and creates HEAP allocation
*/ 
DicInstance* dicCreateDic(const DicLanguage langFrom, const DicLanguage langTo);

/**
*	[! **HAVE TO BE CALLED ** ! ]
*	deletes all data created on heap
*/
void dicDestroyDic(DicInstance* pInstance);

/**
*	fetchType - tells the program where we want to fetch some data: 
*		DIC_FETCH_CONSOLE  - do not specify extraData, set extraData as NULL
*		DIC_FETCH_TEXTFILE - specify the path in extraData for the .txt file, which contains this program
*							 friendly data format
*/
void dicAddWord(DicInstance* pInstance,const DicFetchType fetchType, const void* extraData);



#endif