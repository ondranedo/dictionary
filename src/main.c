#include <stdlib.h>
#include "dictionary.h"

/*
 * 1 - Add word
 * 2 - Print dic.
 * 3 - Print section.
 * 4 - Transaltion
*/

int main(void)
{
    // Creates an instance
    DicInstance* _d = dicCreateDic(DIC_LANG_CZ, DIC_LANG_EN);

    dicAddWord(_d, DIC_FETCH_CONSOLE, NULL);

    // dicPrintDic(_d); //Prints local dic. fe. EN -> CZ
    // dicPrintSection(_d, 2) // Prints lection fe. EN -> CZ


    // dicTranslate(_d,
    //      DIC_CONSOLE,NULL
    //      DIC_FILE,"path/to/file"

    // dicResult _res
    // dicGetResult(_d, &_res);

    // dicParameters _par;
    // dicGerParameters(_d, _par);


    dicDestroyDic(_d);

    return 0;
}