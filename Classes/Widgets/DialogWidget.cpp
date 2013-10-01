#include "DialogWidget.h"

DialogWidget::DialogWidget(MTile *main_tile,
                           const std::string &accept_text,
                           const std::string &decline_text,
                           DelegatePtr delegate)
    : _ask_tile(main_tile),
      _accept_text(accept_text),
      _decline_text(decline_text),
      _delegate(delegate),
      _x_anchor(Anchor::Left),
      _y_anchor(Anchor::Top)
{
    _ask_tile->retain();
}

DialogWidget::~DialogWidget()
{
    _ask_tile->release();
}

void DialogWidget::onInit()
{
    MTileSize label_size = _ask_tile->getTileSize();

    _rs = getApi()->reserveSpace(
                _x_anchor, _y_anchor,
                MTileSize(label_size.width(), label_size.height()+2));

    initUI();
}

void DialogWidget::initUI()
{
    if(_rs.isDefined())
    {
        MTilePosition my_position = _rs.getPosition();
        MTileSize my_size = _rs.getSize();
        MTileSize label_size = _ask_tile->getTileSize();

        _ask_tile->setTilePosition(my_position);
        getApi()->addTile(_ask_tile);

        unsigned int buttons_height = my_size.height() - label_size.height();
        MTileSize button_size(label_size.width()/2, buttons_height);

        MTile* accept_button = MTile::create(
                    "blue.png",
                    MTilePosition(my_position.x()+button_size.width(),
                                  my_position.y()+label_size.height()),
                    button_size,
                    this, tile_selector(DialogWidget::onAcceptClick));

        MTile* decline_button = MTile::create(
                    "blue.png",
                    MTilePosition(my_position.x(),
                                  my_position.y()+label_size.height()),
                    button_size,
                    this, tile_selector(DialogWidget::onDeclineClick));

        getApi()->addTile(accept_button);
        getApi()->addTile(decline_button);

        accept_button->addChild(LF::label(_accept_text.c_str(),
                                          LF::MEDIUM_FONT),
                                Aligment::HCenter, Aligment::VCenter,
                                0, LF::vfix(LF::MEDIUM_FONT)/2);
        decline_button->addChild(LF::label(_decline_text.c_str(),
                                           LF::MEDIUM_FONT),
                                 Aligment::HCenter, Aligment::VCenter,
                                 0, LF::vfix(LF::MEDIUM_FONT)/2);

    }
}

void DialogWidget::onAcceptClick(MTile*)
{
    if(_delegate.get() != 0)
        _delegate->onAcceptClick();
}

void DialogWidget::onDeclineClick(MTile*)
{
    if(_delegate.get() != 0)
        _delegate->onDeclineClick();
}

void DialogWidget::setAnchor(const Anchor::Horizontal x_a,
               const Anchor::Vertical y_a)
{
    _x_anchor = x_a;
    _y_anchor = y_a;
}
