#ifndef PLATFORM_H
#define PLATFORM_H

class Platform
{
public:
    static void beforeLongTermAction();
    static void afterLongTermAction();
private:
    Platform();
};

#endif // PLATFORM_H
