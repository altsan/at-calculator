#ifndef CALC_VERSION_H
#define CALC_VERSION_H

#define VER_FILEVERSION_STR         "0.0.1"
#define VER_PRODUCTVERSION_STR      VER_FILEVERSION_STR

#define VER_LEGALTRADEMARKS1_STR    "All Rights Reserved"
#define VER_LEGALTRADEMARKS2_STR    VER_LEGALTRADEMARKS1_STR

#define VER_FILEDESCRIPTION_STR     "Calculator"
#define VER_PRODUCTNAME_STR         VER_FILEDESCRIPTION_STR

#define VER_INTERNALNAME_STR        "ATCalc"
#define VER_ORIGINALFILENAME_STR    "calculator.exe"

#define VER_COMPANYNAME_STR         "Alexander Taylor"
#define VER_COMPANYDOMAIN_STR       "altsan.org"

#ifdef __OS2__
#define VER_LEGALCOPYRIGHT_STR      "Copyright ¸ 2018 Alexander Taylor"

#else
#define VER_LEGALCOPYRIGHT_STR      "Copyright © 2018 Alexander Taylor"
#define VER_FILEVERSION             0,0,1,0
#define VER_PRODUCTVERSION          VER_FILEVERSION

#endif  // else

#endif  // CALC_VERSION_H