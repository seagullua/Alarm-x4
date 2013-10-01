#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H

#include "Lang.h"




R::LanguageFuncPtr loadEN(R::Translation& tr);
R::LanguageFuncPtr loadRU(R::Translation& tr);
R::LanguageFuncPtr loadUK(R::Translation& tr);
void loadTranslation(const R::LangCode lang);

R::LangCode getDeviceLang();




#endif // TRANSLATIONS_H
