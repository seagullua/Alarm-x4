#ifndef VALUESTORAGE_H
#define VALUESTORAGE_H

class ValueStorage
{
public:
    typedef int Key;


    static int get(const Key);
    static void set(const Key, const int value);

private:
    friend class System;
};

#endif // VALUESTORAGE_H
