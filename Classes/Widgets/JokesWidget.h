#ifndef JOKESWIDGET_H
#define JOKESWIDGET_H
#include "Core/Widget.h"
#include "ContentProvider/Joke.h"

class JokesWidget : public Widget
{
public:
    JokesWidget();
private:
    class JokesDelegate;

    void onInit();
    void onSuspend();

    void showJoke(const Joke& joke_obj);
    void onTileClick(MTile *sender);
    void getNextJoke();

    void reserveSpace();
    MTileBlock _rs;
    MTile* _joke_tile;
    JokesDelegate* _delegate;
};

#endif // JOKESWIDGET_H
