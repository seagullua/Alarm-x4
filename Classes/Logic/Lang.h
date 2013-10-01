#ifndef TRANSLATE_H
#define TRANSLATE_H
#include <map>
#include <string>
#include <memory>
namespace R
{
enum LangCode
{
    EN = 0,
    RU = 1,
    UK = 2
};
enum TranslateKeys
{
    AlarmTimeChooser_AlarmIsSet=100,
    AlarmTimeChooser_TimeToSleep,
    AlarmTimeChooser_TimeToGetReady,
    AlarmTimeChooser_ExitTime,
    AlarmTimeChooser_SetAlarm,
    AlarmTimeChooser_Cancel,

    AlarmWidget_TimeToWakeUp=200,
    AlarmWidget_TurnOff,
    AlarmWidget_Snooze,

    FlowHelperWidget_TurnOffRules=300,
    FlowHelperWidget_PlayRules,
    FlowHelperWidget_Restart,
    FlowHelperWidget_Exit,

    JokesWidget_Joke = 400,

    StartFlowGameWidget_MorningPuzzle = 420,

    TimeSelector_Hours = 440,
    TimeSelector_Minutes,

    TurnOnAlarmMenuWidget_AlarmPurpose = 500,
    TurnOnAlarmMenuWidget_WakeUp,
    TurnOnAlarmMenuWidget_WakeUpAndGoOut,

    TurnOnAlarmWidget_AlarmIsSetFor = 600,
    TurnOnAlarmWidget_GoOutTime,
    TurnOnAlarmWidget_TurnOnAlarm,
    TurnOnAlarmWidget_TurnOffAlarm,

    Weather_Sunny = 700,
    Weather_PartialyCloud,
    Weather_Cloudy,
    Weather_HeavyCloudy,
    Weather_ShortRain,
    Weather_Rain,
    Weather_Lightning,
    Weather_Hail,
    Weather_RainSnow,
    Weather_Snow,
    Weather_HeavySnow,
    Weather_Today,
    Weather_Tomorrow,
    Waether_DoNotForgetUmbrella,

    Weather_LangKey = 800
};

typedef std::map<TranslateKeys, std::string> Translation;

class LanguageFunc
{
public:
    LanguageFunc()
    {}

    virtual ~LanguageFunc()
    {}

    virtual std::string formatDate(const int year,
                                   const int month,
                                   const int day,
                                   const int week_day)=0;
};
typedef std::auto_ptr<LanguageFunc> LanguageFuncPtr;

class Language
{
public:
    static Translation& getTranslation()
    {
        return _trans;
    }

    static void setLangCode(const std::string& lang)
    {
        _lang_code = lang;
    }

    static std::string getLangCode()
    {
        return _lang_code;
    }

    static const char* tr(const TranslateKeys k)
    {
        return _trans[k].c_str();
    }

    static LanguageFunc& f()
    {
        return *_helper;
    }

    static void setLangFunc(LanguageFuncPtr ptr)
    {
        _helper = ptr;
    }

private:
    static Translation _trans;
    static LanguageFuncPtr _helper;
    static std::string _lang_code;
};

inline const char* tr(const TranslateKeys k)
{
    return Language::tr(k);
}
inline LanguageFunc& f()
{
    return Language::f();
}


}
#endif // TRANSLATE_H
