#include "FlowTile.h"
#include "Widgets/Flow/Generator/FGenerator.h"
#include "Widgets/Flow/FlowGame.h"
#include "Core/MTileGrid.h"
#include <algorithm>
using namespace cocos2d;
FlowTile::FlowTile()
{
}

typedef std::vector<FlowTask> FlowTaskVec;



FlowTask getTask(unsigned int N)
{
    static std::vector<bool> loaded(15, false);
    static std::map<unsigned int, FlowTaskVec> tables;

    if(loaded[N] == false)
    {
        CCLog("Load started");
        //Load task
        std::stringstream fname;
        loaded[N] = true;
        fname << "puzzle_" << N << "x" << N << ".ad";

        std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(fname.str().c_str());
        unsigned long size = 0;
        unsigned char* file_cont = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "r", &size);

        CCLog("File readed: %d", size);

        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
        ss.write((char*)file_cont, size);

        for(unsigned int i=0; i<40; ++i)
        {
            CCLog("%d,", file_cont[i]);
        }

        InputBinaryStream is(ss);

        CCLog("Start task read");
        FlowTaskVec& inp = tables[N];
        CCLog("Start task read++");
        is >> inp;

        CCLog("Task readed %d", inp.size());
    }

    const FlowTaskVec& vec = tables[N];
    if(vec.size() == 0)
        return FlowTask();
    FlowTask task = vec[rand() % vec.size()];
    std::random_shuffle(task.begin(), task.end());

    CCLog("Task returned");
    return task;
}

void FlowTile::init(const MTilePosition& pos, const MTileSize& size,FlowGame::DelegatePtr d)
{
    //MTileBlock rs(pos,size);
    MTile::init("black.png", pos, size, 0, 0);
    MTile* tile = this;
    unsigned int N = 5 + rand() % 3;

    FlowTable table(N,N);
    FlowTask task = getTask(N);

    for(FlowTask::iterator it = task.begin(); it!=task.end(); ++it)
    {
        FlowStartEnd& s = *it;
        if(s.size() == 2)
        {
            table.addColor(s[0], s[1]);
        }
    }


    FlowGame* render = FlowGame::create(table, d);
    CCSize render_size = render->getContentSize();
    CCSize tile_size = tile->getInnerContentSize();

    render->setScale(MIN(tile_size.width / render_size.width,
                         tile_size.height / render_size.height));

    tile->addChild(render, Aligment::HCenter, Aligment::VCenter);


}

FlowTile* FlowTile::create(const MTilePosition& position,
                             const MTileSize& size, FlowGame::DelegatePtr d)
{
    FlowTile* obj = new FlowTile();
    if(obj)
    {
        obj->autorelease();
        obj->init(position, size, d);
    }
    return obj;
}
