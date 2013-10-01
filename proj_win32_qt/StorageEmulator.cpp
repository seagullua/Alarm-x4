#include "StorageEmulator.h"
#include "Core/Binary.h"
#include <fstream>

const char* const _file = "storage.ad";

StorageEmulator::StorageEmulator()
{
    std::ifstream input(_file);
    InputBinaryStream is(input);

    unsigned int size = 0;
    is >> size;

    for(unsigned int i=0; i<size; ++i)
    {
        unsigned int val = 0;
        unsigned int key = 0;
        is >> key >> val;
        _map[key]=val;
    }

}

int StorageEmulator::get(const int k)
{
    return _map[k];
}

void StorageEmulator::set(const int k, const int v)
{
    _map[k] = v;
}

StorageEmulator::~StorageEmulator()
{
    std::ofstream output(_file);
    OutputBinaryStream os(output, BinaryStream::MaxProtocolVersion);

    os << _map.size();
    for(Map::iterator i=_map.begin(); i!=_map.end(); ++i)
    {
        os << unsigned int(i->first) << unsigned int(i->second);
    }
}
