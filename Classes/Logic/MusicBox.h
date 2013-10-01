#ifndef MUSICBOX_H
#define MUSICBOX_H
#include <string>
class MusicBox
{
public:
    void playWakeUpMusic();
    void stopWakeUpMusic();
private:
    void setMaxVolume();
    void restoreVolume();

    std::string getNextRingTon();
    friend class System;
    MusicBox();
};

#endif // MUSICBOX_H
