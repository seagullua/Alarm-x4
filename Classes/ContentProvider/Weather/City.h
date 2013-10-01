#ifndef CITY_H
#define CITY_H
#include <string>
namespace Weather
{
typedef unsigned int CityID;
class City
{
public:
    City(const std::string& name="",
         const CityID id=0);
    void setName(const std::string& name);
    const std::string& getName() const;

    void setID(const CityID id);
    const CityID getID() const;
private:
    std::string _name;
    CityID  _id;
};

}

#endif // CITY_H
