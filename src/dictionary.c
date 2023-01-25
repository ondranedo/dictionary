#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

static DicWord createWordFromCsv(const char* path);
static DicChosenlanguage getLanguageFromTxtFile(FILE* file);
static DicWordEntry* getHeapWordEntries(FILE* file, size_t worde_count);

static void findNOccuOfChar(const char* src,
							const unsigned int src_size,
							char* des,
							const unsigned int des_size,
							const char ch,
							const unsigned int n);
static DicLanguage decodeLangFromString(const char* str);
static size_t getWordEntryCount(const char* path);

DicWord dicCreateWord(const DicFetchType fetchType, const void* extraData)
{
	switch (fetchType)
	{
	case DIC_FETCH_CONSOLE: DIC_LOG_ERR("Console is not yet supported"); break;
	case DIC_FETCH_CSVFILE: return createWordFromCsv(extraData); break;
	}

	return NULL;
}

void dicDestroyWord(DicWord word)
{
	size_t i;

	// deletes all from's and to's
	for (i = 0; i < word->word_count; i++)
	{
		free(word->word_entry_array[i].from_word);
		free(word->word_entry_array[i].to_word);
	}

	// deletes word entry
	free(word->word_entry_array);

	// deletes word data it self
	free(word);

	word = NULL;
}

void dicPrintWord(DicWord word)
{
	size_t i = 0;

	printf("Dic has: %d words\n", word->word_count);
	for (size_t i = 0; i < word->word_count; i++)
	{
		printf("%d\t%s\t%s\n",
			word->word_entry_array[i].lesson,
			word->word_entry_array[i].from_word,
			word->word_entry_array[i].to_word
			);
	}
}

void dicTranslate(const DicWord word, const char* src, char* dest, unsigned int dest_size)
{
	char* buff; // Finds word
	unsigned int i;

	buff = (char*)malloc(1);

	for (i = 0; i < word->word_count; i++)
	{
		buff = realloc(buff,sizeof(char)*(strlen(word->word_entry_array[i].from_word + 1)));
		strcpy(buff, word->word_entry_array[i].from_word);
		strlwr(buff);
		strlwr(src);

		if (!strcmp(buff, src))
		{
			if (strlen(word->word_entry_array[i].to_word) > dest_size) break;

			strcpy(dest, word->word_entry_array[i].to_word);
			return;
		}
	}

	memset(dest, NULL, dest_size);
}

/***************\
*	STATIC DEFINITIONS
\***************/

static DicWord createWordFromCsv(const char* path)
{
	FILE* fr;
 	DicWord word = (DicWord)malloc(sizeof(DicWordD));

	word->word_count = getWordEntryCount(path);

	fr = fopen(path, "r");
	DIC_CHECK_FILE_V(fr, NULL);

	word->lang = getLanguageFromTxtFile(fr);

	word->word_entry_array = getHeapWordEntries(fr, word->word_count);

	fclose(fr);

	return word;
}

static DicChosenlanguage getLanguageFromTxtFile(FILE* file)
{
	char line_buff[14], lang_from[3], lang_to[3];
	DicChosenlanguage lang_pair;
	DicLanguage from, to;

	fgets(line_buff, 14, file);

	// 0 - lesson
	findNOccuOfChar(line_buff, 14, lang_from, 3, ';', 1);
	findNOccuOfChar(line_buff, 14, lang_to  , 3, ';', 2);

	from = decodeLangFromString(lang_from);
	to = decodeLangFromString(lang_to);

	lang_pair.from = from;
	lang_pair.to = to;

	return lang_pair;
}

static void findNOccuOfChar(const char* src,
							const unsigned int src_size,
							char* des, 
							const unsigned int des_size,
							const char ch, 
							const unsigned int n)
{
	unsigned int counter = 0, left_offset = 0, right_offset = 0, output_length = 0, i;

	//finds the offsets
	for (i = 0; i < src_size; right_offset = ++i)
	{
		if (src[i] != ch && src[i] != '\n') continue;
		if (++counter - 1 == n) break;
		
		left_offset = i + 1; // +1 for the 'ch'
	}

	//cuts and sets the string
	output_length = right_offset - left_offset;
	
	if (output_length > des_size - 1) // -1 for the '0' char;
	{
		DIC_LOG_ERR("output_length is grater than des_size");
		return;
	}
	
	memcpy(des, src + left_offset, output_length);
	des[output_length] = '\0';
}

static DicLanguage decodeLangFromString(const char* str)
{
	// From ISO-639-1
	if (!strcmp(str, "cs")) return DIC_LANG_CS;
	if (!strcmp(str, "en")) return DIC_LANG_EN;

	DIC_LOG_ERR("unrecognized language");
	return 0;
}

static size_t getWordEntryCount(const char* path)
{
	size_t count = 0;
	char char_buff = 0;
	FILE* fr = fopen(path, "r");
	DIC_CHECK_FILE_V(fr,0);

	while (char_buff != EOF)
	{
		if (char_buff == '\n')
			count++;

		char_buff = getc(fr);
	}

	fclose(fr);

	return count - 1; // -1 - for lang spec.
}

static DicWordEntry* getHeapWordEntries(FILE* file, size_t worde_count)
{
	size_t i = 0;
	char line_buff[100];
	char from_word[48], to_word[48], lesson[5];

	DicWordEntry* buff = (DicWordEntry*)malloc(sizeof(DicWordEntry) * worde_count);

	for(i = 0; i < worde_count; i++)
	{
		fgets(line_buff, 100, file);

		findNOccuOfChar(line_buff, 100, lesson   , 5, ';', 0);
		findNOccuOfChar(line_buff, 100, from_word,48, ';', 1);
		findNOccuOfChar(line_buff, 100, to_word  ,48, ';', 2);

		buff[i].lesson = atoi(lesson);

		buff[i].from_word = (char*)malloc((strlen(from_word) + 1) * sizeof(char));
		memcpy(buff[i].from_word, from_word, strlen(from_word) + 1);
		
		buff[i].to_word = (char*)malloc((strlen(to_word) + 1) * sizeof(char));
		memcpy(buff[i].to_word, to_word, strlen(to_word) + 1);
	}

	return buff;
}
