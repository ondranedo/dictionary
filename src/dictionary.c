#include <stdlib.h>
#include <string.h>
#include <time.h>

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
static void decodeISO639_1toSTR(DicLanguage lang, char* dest, unsigned int dest_size);
static void mapLessons(DicWord word);

DicWord dicCreateWord(const DicFetchType fetchType, const void* extraData)
{ 
	DicWord r = NULL;

	switch (fetchType)
	{
	case DIC_FETCH_CONSOLE: DIC_LOG_ERR("Console is not yet supported"); break;
	case DIC_FETCH_CSVFILE: r = createWordFromCsv(extraData); break;
	}

	mapLessons(r);

	return r;
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

	// deletes lesson map
	free(word->lessonMap.lesson_arr);

	// deletes word data it self
	free(word);

	word = NULL;
}

void dicPrintWord(DicWord word)
{
	size_t i = 0;
	char from[5], to[5];

	dicGetFromLangage(word, from, 5);
	dicGetToLangage(word, to, 5);


	printf("Word [%s_%s] has: %d words\n", from,to, word->word_count);
	for (size_t i = 0; i < word->word_count; i++)
	{
		printf("%d\t%s\t%s\n",
			word->word_entry_array[i].lesson,
			word->word_entry_array[i].from_word,
			word->word_entry_array[i].to_word
			);
	}
}

void dicPrintLesson(DicWord word, unsigned int lesson)
{
	size_t i = 0;
	char from[5], to[5];

	dicGetFromLangage(word, from, 5);
	dicGetToLangage(word, to, 5);

	printf("printing %d. lesson in word [%s_%s] \n", lesson, from, to);
	for (size_t i = 0; i < word->word_count; i++)
	{
		if(word->word_entry_array[i].lesson == lesson)

		printf("\t%s\t%s\n",
			word->word_entry_array[i].from_word,
			word->word_entry_array[i].to_word
		);
	}
}

void dicPrintLessonList(DicWord word)
{
	unsigned int i;
	char from[5], to[5];

	dicGetFromLangage(word, from, 5);
	dicGetToLangage(word, to, 5);

	printf("Lesson overview for word [%s_%s]\n", from, to);
	printf("\tID\twords\n");

	for (i = 0; i < word->lessonMap.lesson_count; i++)
	{
		printf("\t%d\t%d\n", word->lessonMap.lesson_arr[i].lesson,
						     word->lessonMap.lesson_arr[i].word_count
						     );
	}

}

void dicTranslate(const DicWord word, const char* src, char* dest, unsigned int dest_size)
{
	char* buff; // Finds word
	unsigned int i;

	buff = (char*)malloc(1);
	memset(dest, NULL, dest_size);

	for (i = 0; i < word->word_count; i++)
	{
		buff = realloc(buff,sizeof(char)*(strlen(word->word_entry_array[i].from_word)+1));
		strcpy(buff, word->word_entry_array[i].from_word);
		strlwr(buff);
		strlwr(src);

		if (!strcmp(buff, src))
		{
			if (strlen(word->word_entry_array[i].to_word) < dest_size)
				strcpy(dest, word->word_entry_array[i].to_word);
			else break;
		}
	}


	free(buff);
}

void dicTranslateBack(const DicWord word, const char* src, char* dest, unsigned int dest_size)
{
	char* buff; // Finds word
	unsigned int i;

	buff = (char*)malloc(1);
	memset(dest, NULL, dest_size);

	for (i = 0; i < word->word_count; i++)
	{
		buff = realloc(buff, sizeof(char) * (strlen(word->word_entry_array[i].to_word) + 1));
		strcpy(buff, word->word_entry_array[i].to_word);
		strlwr(buff);
		strlwr(src);

		if (!strcmp(buff, src))
		{
			if (strlen(word->word_entry_array[i].from_word) < dest_size)
				strcpy(dest, word->word_entry_array[i].from_word);
			else break;
		}
	}

	free(buff);
}

void dicTestWord(const DicWord word, DicTest* ptest, unsigned int words_to_test)
{
	unsigned int i,j;
	unsigned int chosen_word;
	unsigned int* indexed_words;
	unsigned int  indexed_words_size = 0;

	unsigned int* incorrect_index;
	unsigned int incorrect_index_count = 0;

	char usr_buff[100];

	srand(time(0));
	memset(ptest, NULL, sizeof(DicTest));

	if (words_to_test > word->word_count)
	{
		printf("words to test are higer then the number of words in the dictionary\n");
		ptest->words_tested = word->word_count;
	} else
	ptest->words_tested = words_to_test;

	indexed_words = (unsigned int*)malloc(ptest->words_tested*sizeof(unsigned int));
	memset(indexed_words, NULL, ptest->words_tested);

	incorrect_index = (unsigned int*)malloc(ptest->words_tested*sizeof(unsigned int));
	memset(incorrect_index, NULL, ptest->words_tested);

	for (i = 0; i < ptest->words_tested; i++)
	{
		char match;
		unsigned int indexed_word;

		// find index of word to test
		do
		{
			match = 1;
			indexed_word = rand() % (word->word_count);
			for (j = 0; j < indexed_words_size; j++)
			{
				if (indexed_words[j] == indexed_word)
					match = 0;
			}

			if(match)
				indexed_words[indexed_words_size++] = indexed_word;

		} while (!match);

		printf("[%s]: ", word->word_entry_array[indexed_word].to_word);
		gets_s(usr_buff, 100);
	
		if (!strcmp(usr_buff, word->word_entry_array[indexed_word].from_word))
			ptest->correct++;
		else
		{
			ptest->uncorrect++;
			incorrect_index[incorrect_index_count++] = indexed_word;
		}
	}
		
	ptest->percentage_of_succes = ((float)ptest->correct/ (float)ptest->words_tested ) * 100;
	printf("Succes rate %.1f%%\n", ptest->percentage_of_succes);
	
	if (incorrect_index_count)
	{
		for (i = 0; i < incorrect_index_count; i++)
		{
			printf("\t%s\t->\t%s\n", word->word_entry_array[incorrect_index[i]].to_word, word->word_entry_array[incorrect_index[i]].from_word);
		}
	}

	free(indexed_words);
	free(incorrect_index);
}

void dicTestWordLesson(const DicWord word, DicTest* ptest, unsigned int lesson, unsigned int words_to_test)
{
	unsigned int i,j;
	unsigned int word_count = 0;
	unsigned int* indexed_words;
	unsigned int indexed_words_size = 0;

	unsigned int* incorrect_words;
	unsigned int incorrect_words_size = 0;

	char usr_buff[100];

	// find words
	for (i = 0; i < word->lessonMap.lesson_count; i++)
		if (word->lessonMap.lesson_arr[i].lesson == lesson)
			{ word_count = word->lessonMap.lesson_arr[i].word_count; break; }
	if (!word_count)
	{
		printf("There is no known lesson with index %d\n", lesson);
		return;
	}

	// test setup
	srand(time(0));
	memset(ptest, NULL, sizeof(DicTest));

	// check word count to test
	if (words_to_test > word_count)
	{
		printf("words to test are higer then the number of words in the dictionary\n");
		ptest->words_tested = word_count;
	}
	else
		ptest->words_tested = words_to_test;

	// prep data
	indexed_words = (unsigned int*)malloc(sizeof(unsigned int) * ptest->words_tested);
	memset(indexed_words, NULL, sizeof(unsigned int) * ptest->words_tested);

	incorrect_words = (unsigned int*)malloc(sizeof(unsigned int) * ptest->words_tested);
	memset(incorrect_words, NULL, sizeof(unsigned int) * ptest->words_tested);


	//find word and test it
	for (i = 0; i < ptest->words_tested; i++)
	{
		char match;
		unsigned int indexed_word;

		// find index of word to test
		do
		{
			match = 1;
			indexed_word = rand() % (word->word_count);
			

			if (word->word_entry_array[indexed_word].lesson != lesson)
				match = 0;
			
			for (j = 0; j < indexed_words_size; j++)
			{
				if (indexed_words[j] == indexed_word)
					match = 0;
			}

			if (match)
				indexed_words[indexed_words_size++] = indexed_word;

		} while (!match);

		printf("[%s]: ", word->word_entry_array[indexed_word].to_word);
		gets_s(usr_buff, 100);

		if (!strcmp(usr_buff, word->word_entry_array[indexed_word].from_word))
			ptest->correct++;
		else
		{
			ptest->uncorrect++;
			incorrect_words[incorrect_words_size++] = indexed_word;
		}
	}

	ptest->percentage_of_succes = ((float)ptest->correct / (float)ptest->words_tested) * 100;
	printf("Succes rate %.1f%%\n", ptest->percentage_of_succes);

	if (incorrect_words_size)
	{
		for (i = 0; i < incorrect_words_size; i++)
		{
			printf("\t%s\t->\t%s\n", word->word_entry_array[incorrect_words[i]].to_word, word->word_entry_array[incorrect_words[i]].from_word);
		}
	}

	// destroy data
	free(indexed_words);
	free(incorrect_words);
}

void dicGetFromLangage(const DicWord word, char* dest, unsigned int dest_size)
{
	decodeISO639_1toSTR(word->lang.from, dest, dest_size);
}

void dicGetToLangage(const DicWord word, char* dest, unsigned int dest_size)
{
	decodeISO639_1toSTR(word->lang.to, dest, dest_size);
}

/***************\
*	STATIC DEFINITIONS
\***************/

static DicWord createWordFromCsv(const char* path)
{
	FILE* fr;
 	DicWord word = (DicWord)malloc(sizeof(DicWordD));

	word->word_count = getWordEntryCount(path);

	word->lessonMap.lesson_arr = NULL;
	word->lessonMap.lesson_count = NULL;

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

static void decodeISO639_1toSTR(DicLanguage lang, char* dest, unsigned int dest_size)
{
	if (dest_size <= 2) return;

	switch (lang)
	{
	case DIC_LANG_CS: strcpy(dest, "cs"); break;
	case DIC_LANG_EN: strcpy(dest, "en"); break;

	default: strcpy(dest, "");
	}
}

static void mapLessons(DicWord word)
{
	unsigned int i, j;
	DicLessonMap tmp_map = { 0,0 };
	tmp_map.lesson_arr = (DicLesson*)malloc(sizeof(DicLesson)*word->word_count); // max pos. number of words
	memset(tmp_map.lesson_arr, NULL, sizeof(DicLesson) * word->word_count);

	//checks for one row
	for (i = 0; i < word->word_count; i++) {
		unsigned int is_unique = 1;
		DicLesson new_lesson;

		new_lesson.lesson = word->word_entry_array[i].lesson;
		new_lesson.word_count = 0;

		// is unique?
		for (j = 0; j < word->word_count; j++)
			if (new_lesson.lesson == tmp_map.lesson_arr[j].lesson) { is_unique = 0; break; }
		if (!is_unique) continue;

		// count lessons
		for (j = 0; j < word->word_count; j++) {
			if (new_lesson.lesson == word->word_entry_array[j].lesson) new_lesson.word_count++;
		}

		// stores new unique lesson
		memcpy(&tmp_map.lesson_arr[tmp_map.lesson_count++], &new_lesson, sizeof(DicLesson));
	}

	word->lessonMap.lesson_count = tmp_map.lesson_count;
	word->lessonMap.lesson_arr = (DicLesson*)malloc(sizeof(DicLesson) * tmp_map.lesson_count);
	memcpy(word->lessonMap.lesson_arr, tmp_map.lesson_arr, sizeof(DicLesson) * tmp_map.lesson_count);
	
	free(tmp_map.lesson_arr);
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
