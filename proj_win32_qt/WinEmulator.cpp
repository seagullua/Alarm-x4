#include "StorageEmulator.h"
#include "Logic/ValueStorage.h"
#include "Logic/DelayedActions.h"
#include "Logic/Location.h"
#include "Logic/PrefetchAction.h"
#include "Logic/MusicBox.h"
std::auto_ptr<StorageEmulator> s(0);

int ValueStorage::get(const Key k)
{
    if(s.get() == 0)
        s = std::auto_ptr<StorageEmulator>(new StorageEmulator);
    return s->get(k);
}

void ValueStorage::set(const Key k, const int value)
{
    if(s.get() == 0)
        s = std::auto_ptr<StorageEmulator>(new StorageEmulator);
    s->set(k, value);
}
#include "cocos2d.h"
using namespace cocos2d;
void DelayedActions::setActionTime(const Action action, const Time t)
{
    CCLog("Delayed action set %d on %d:%d", action, t.getHour(), t.getMinute());

}
void DelayedActions::cancelAction(const Action action)
{
    CCLog("Delayed action canceled %d", action);
}
//Харків 36.27163,49.9599
//Велика Бакта 22.662172,48.1585
//Мукачево 22.723381,48.433096
//Костопіль 26.459376,50.869266
//Дача 30.228748,50.178487
//Автралія 150.271475,-34.417168
//Плзень 13.443716,49.798396
double Location::getLongitude()
{
    return 22.723381;
}

double Location::getLatitude()
{
    return 48.433096;
}

void MusicBox::setMaxVolume()
{
    CCLog("Max volume set");
}

void MusicBox::restoreVolume()
{
    CCLog("Volume restored");
}
#include "Logic/Lang.h"
R::LangCode getDeviceLang()
{
    return R::UK;
}
