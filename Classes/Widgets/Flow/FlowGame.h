#ifndef FLOWGAME_H
#define FLOWGAME_H
#include "cocos2d.h"
#include "FlowRenderer.h"
class FlowGame : public cocos2d::CCNode, public cocos2d::CCTouchDelegate
{
public:

    class Delegate
    {
    public:
        Delegate(){}
        virtual void onWin()
        {}

        virtual ~Delegate()
        {}
    };
    typedef std::auto_ptr<Delegate> DelegatePtr;

    FlowGame(const FlowTable& table, DelegatePtr delegate);
    static FlowGame* create(const FlowTable& table, DelegatePtr delegate);
private:
    FlowRenderer* _renderer;
    const FlowTable* _table;

    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    bool touchStart(const FlowPoint&);
    void touchMoved(const FlowPoint&);
    void touchEnded(const FlowPoint&);

    void cutTheTrace(const FlowPoint& start, const FlowColor color,
                     const bool remove_highlight);
    FlowColor getPointColor(const FlowPoint& p);
    void connectPoints(FlowPointState& a, FlowPointState& b);
    void disconnectPoints(FlowPointState& a, FlowPointState& b);
    void updateHighlighted();
    void removeHighlight(FlowPointState &st);
    void restoreTraces();
    bool checkWinCondition();
    void updateActiveCircle(cocos2d::CCPoint p);

    FlowPoint touchToCellCordinates(cocos2d::CCPoint p);
    cocos2d::CCPoint touchToLocalCords(cocos2d::CCTouch* touch);

    typedef std::vector<FlowPoint> Trace;
    typedef std::vector<Trace> Traces;

    bool _is_touch_active;
    bool _finish_lock;
    int _active_touch_id;
    Traces _active_traces;
    Traces _submitted_traces;

    FlowPoint _last_point;
    FlowColor _working_color;
    DelegatePtr _delegate;

    void traceDebug() const;
    void traceDebug(std::ostream& os, const Traces& tr) const;
};

#endif // FLOWGAME_H
