#ifndef INTENTPARAMS_H
#define INTENTPARAMS_H

class IntentParams
{
public:
    enum Mode
    {
        LaunchMode=0,
        WakeUpMode=1
    };

    static IntentParams& getInstance();

    void setMode(const Mode);
    void justSetMode(const Mode);
    Mode getMode() const;
private:
    IntentParams();
    Mode _mode;
    static IntentParams _obj;
};

#endif // INTENTPARAMS_H
