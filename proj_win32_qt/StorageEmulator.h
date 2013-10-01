#ifndef STORAGEEMULATOR_H
#define STORAGEEMULATOR_H
#include <map>
class StorageEmulator
{
public:
    StorageEmulator();
    int get(const int);
    void set(const int, const int);
    ~StorageEmulator();
private:
    typedef std::map<int, int> Map;
    Map _map;

};

#endif // STORAGEEMULATOR_H
