#include "Lang.h"
R::Translation R::Language::_trans;
R::LanguageFuncPtr R::Language::_helper;
std::string R::Language::_lang_code="";
#include "Translations.h"
void loadTranslation(const R::LangCode lang)
{
    R::Translation& tr = R::Language::getTranslation();

    R::LanguageFuncPtr ptr;
    if(lang == R::UK)
    {
        ptr = loadUK(tr);
        R::Language::setLangCode("uk");
    }
    else if(lang == R::RU)
    {
        ptr = loadRU(tr);
        R::Language::setLangCode("ru");
    }
    else
    {
        ptr = loadEN(tr);
        R::Language::setLangCode("en");
    }

    R::Language::setLangFunc(ptr);
}
