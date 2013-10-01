#include "City.h"

namespace Weather
{
City::City(const std::string& name,
     const CityID id)
    : _name(name), _id(id)
{

}

void City::setName(const std::string& name)
{
    _name = name;
}

const std::string& City::getName() const
{
    return _name;
}

void City::setID(const CityID id)
{
    _id = id;
}

const CityID City::getID() const
{
    return _id;
}

}
