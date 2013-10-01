#include "System.h"

Alarm* System::_alarm=0;
DelayedActions* System::_actions=0;
ValueStorage* System::_storage=0;
MusicBox* System::_music_box=0;


System::Loader System::_ld;
