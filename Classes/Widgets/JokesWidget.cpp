#include "JokesWidget.h"
#include "ContentProvider/JokesHolder.h"
using namespace cocos2d;
JokesWidget::JokesWidget()
    : _joke_tile(0), _delegate(0)
{

}

class JokesWidget::JokesDelegate : public JokesGetDelegate
{
public:
    JokesDelegate(JokesWidget& parent)
        : _parent(parent), _canceled(false)
    {}
    void cancel()
    {
        _canceled = true;
    }

    void jokeArrived(const Joke& joke_obj)
    {
        CCLog("Jokes arrived");
        if(!_canceled)
        {
            _parent._delegate = 0;
            _parent.showJoke(joke_obj);
        }
    }
private:
    bool _canceled;
    JokesWidget& _parent;
};

void JokesWidget::showJoke(const Joke& joke_obj)
{

    //No enough space
    if(!_rs.isDefined())
        return;

    //Get the scale factor
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();

    //Create tile
    MTile* tile = MTile::create(
                "orange.png",
                _rs.getPosition(),
                MTileSize(6,3),
                this,
                tile_selector(JokesWidget::onTileClick));


    //Get the tile size
    CCSize inner_tile_size = tile->getInnerContentSize();
    float content_max_width = inner_tile_size.width;

    //Create the joke label with width of the tile
    std::string joke = joke_obj.getJokeText();
    CCLabelTTF* joke_text_label = LF::label(
                joke.c_str(),
                LF::SMALL_FONT,
                CCSize(content_max_width, 0),
                kCCTextAlignmentLeft);

    //Calculate the space inside the tile
    float title_height = LF::BIG_FONT / scaled - MTileGrid::getPadding() / scaled / 2;
    float initial_font = LF::SMALL_FONT;
    float label_heigh = joke_text_label->getContentSize().height + title_height;


    //If there is not enough space for label, increase tile height
    if(label_heigh > inner_tile_size.height)
    {
        tile->setTileSize(MTileSize(tile->getTileSize().width(),
                                    tile->getTileSize().height() + 1));
    }

    //Get new size for content
    inner_tile_size = tile->getInnerContentSize();

    //If still is not enough space, decrease font untill fit
    while(label_heigh > inner_tile_size.height)
    {
        initial_font *= 0.8f;
        joke_text_label = LF::label(
                    joke.c_str(),
                    initial_font,
                    CCSize(content_max_width, 0),
                    kCCTextAlignmentLeft);
        label_heigh = title_height + joke_text_label->getContentSize().height;
    }


    //Place title label
    tile->addChild(LF::label(
                       R::tr(R::JokesWidget_Joke),
                       LF::BIG_FONT),
                   Aligment::Left, Aligment::Top,
                   0,
                   LF::vfix(LF::BIG_FONT));

    //Place text
    tile->addChild(joke_text_label,
                   Aligment::Left, Aligment::Top,
                   0,
                   LF::BIG_FONT / scaled);

    //Update tile
    getApi()->replaceTile(_joke_tile, tile);
    _joke_tile = tile;
}

void JokesWidget::onTileClick(MTile* tile)
{

    getApi()->removeTile(tile);
    MTileManager::sharedManager().runAction(
                CCSequence::createWithTwoActions(
                    CCDelayTime::create(0.3f),
                    CCCallFunc::create(
                        this,
                        callfunc_selector(
                            JokesWidget::getNextJoke)
                        )
                    )
                );
    _joke_tile = 0;

}
void JokesWidget::onSuspend()
{
    if(_delegate)
        _delegate->cancel();
}

void JokesWidget::getNextJoke()
{
    _delegate = new JokesDelegate(*this);
    JokesHolder::sharedHolder().getJoke(
                JokesGetDelegatePtr(_delegate));
}

void JokesWidget::reserveSpace()
{
    _rs = getApi()->reserveSpace(Anchor::Left, Anchor::Top,
                                 MTileSize(6,4));


}


void JokesWidget::onInit()
{
    _joke_tile = 0;
    reserveSpace();
    getNextJoke();
}
