#include "MusicBox.h"
#include "SimpleAudioEngine.h"
#include "System.h"
#include <cstdlib>
using namespace CocosDenshion;

MusicBox::MusicBox()
{
}
void MusicBox::playWakeUpMusic()
{
    setMaxVolume();
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(getNextRingTon().c_str(),
                                                           true);
}

void MusicBox::stopWakeUpMusic()
{
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    restoreVolume();
}

std::string MusicBox::getNextRingTon()
{
    unsigned int snooze_times = System::getAlarm().getSnoozeTimes();

    if(snooze_times > 3)
    {
        //hard music
        return "music/loud1.ogg";
    }
    else
    {
        unsigned int num = rand() % 2;
        if(num == 0)
            return "music/norm1.ogg";
        else
            return "music/norm2.ogg";
    }


}

