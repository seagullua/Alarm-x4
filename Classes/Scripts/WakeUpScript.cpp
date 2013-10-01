#include "WakeUpScript.h"
#include "Logic/System.h"
#include "Widgets/FlowHelperWidget.h"
#include "Widgets/AlarmWidget.h"
#include "Logic/PrefetchAction.h"
#include "Logic/IntentParams.h"
#include "Scripts/MainScript.h"
using namespace cocos2d;
WakeUpScript::WakeUpScript()
    : _delayed_snooze(0)
{
}

class WakeUpScript::FlowDelegate : public FlowGame::Delegate
{
public:
    FlowDelegate(WakeUpScript* parent)
        : _parent(parent)
    {}

    void onWin()
    {
        _parent->step2_final();
    }

private:
    WakeUpScript* _parent;
};



void WakeUpScript::onInit()
{
    CCLog("WakeUpScript");

    setQuickSnoozeBackBarrier();

    _main_widget = addWidget<AlarmWidget>(getApi(),
                                          delegate<AlarmWidget::Delegate>(
                                              this, callfunc_selector(WakeUpScript::step1_snooze),
                                              this, callfunc_selector(WakeUpScript::step1_turnOffAlarm)));

    _delayed_snooze = CCSequence::createWithTwoActions(
                CCDelayTime::create(120),
                CCCallFunc::create(this, callfunc_selector(WakeUpScript::quickSnooze)));
    MTileManager::sharedManager().runAction(_delayed_snooze);

    PrefetchAction::performPrefetch();

    System::getMusicBox().playWakeUpMusic();
}

void WakeUpScript::setQuickSnoozeBackBarrier()
{
    getApi()->addBackCallback(this,
                              callfunc_selector(WakeUpScript::quickSnooze));
}

void WakeUpScript::quickSnooze()
{
    performSnooze(Alarm::ShortSnooze);
}

void WakeUpScript::step1_snooze()
{
    performSnooze(Alarm::LongSnooze);
}

void WakeUpScript::step1_turnOffAlarm()
{
    turnOffAutoSnooze();
    getApi()->hideWidget(_main_widget);

    setQuickSnoozeBackBarrier();
    _flow_helper  = addWidget<FlowHelperWidget>(
                getApi(),FlowGame::DelegatePtr(new FlowDelegate(this)));

}

void WakeUpScript::step2_final()
{
    turnOffAlarm();
    getApi()->runScript(ScriptPtr(new MainScript));
}

void WakeUpScript::autoSnooze_delegate(float)
{
    performSnooze(Alarm::ShortSnooze);
}
void WakeUpScript::turnOffAlarm()
{
    System::getAlarm().onWakeUpAction();
    IntentParams::getInstance().justSetMode(IntentParams::LaunchMode);
    turnOffAutoSnooze();
    System::getMusicBox().stopWakeUpMusic();

}

void WakeUpScript::turnOffAutoSnooze()
{
    if(_delayed_snooze)
    {
        MTileManager::sharedManager().cancelAction(_delayed_snooze);
        CCLog("Auto snooze canceled");
        _delayed_snooze = 0;
    }
}

void WakeUpScript::performSnooze(const Alarm::SnoozeMode m)
{
    turnOffAutoSnooze();
    CCLog("Alarm snooze before");
    System::getAlarm().setSnooze(m);
    CCLog("Alarm snooze set");
    CCDirector::sharedDirector()->end();
}


