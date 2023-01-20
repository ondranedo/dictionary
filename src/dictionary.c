#include "dictionary.h"

#include <stdlib.h>

DicInstance* dicCreateDic(const DicLanguage langFrom, const DicLanguage langTo)
{
	DicInstance* dicInstance = (DicInstance*)malloc(sizeof(DicInstance));

	// setUpData(dicInstance);
		dicInstance->language.from = langFrom;
		dicInstance->language.from = langTo;

	return dicInstance;
}

void dicDestroyDic(DicInstance* pInstance)
{

}

void dicAddWord(DicInstance* pInstance, const DicFetchType fetchType, const void* extraData)
{
		
}