#ifndef BINARY_H
#define BINARY_H
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <stdint.h>
class BinaryStream
{
public:
    static const uint32_t Mark = 0x41434453;
    static const uint32_t MaxArraySize = 255 * 100;
    static const uint32_t MaxProtocolVersion = 1;
};

class InputBinaryStream
{
public:
    explicit InputBinaryStream(std::istream& is) : _is(is), _ok(true)
    {
        uint32_t mark = 0;
        uint32_t protocol = 0;
        (*this) >> mark >> protocol;

        if(mark != BinaryStream::Mark)
            _ok = false;
        _protocol_version = protocol;
    }
    bool isOK() const
    {
        return _is.good() && _ok;
    }
    void setUndefiedProtocol()
    {
        _ok = false;
    }
    void setError()
    {
        _ok = false;
    }
	
    void readBytes(char* x, unsigned int size)
    {
        _is.read(x, size);
    }

    void readBytes(char& x)
    {
        readBytes(&x, 1);
    }
    unsigned int protocolVersion() const
    {
        return _protocol_version;
    }
	InputBinaryStream& operator>>(uint32_t& x)
    {
        typedef uint32_t Ui;
        typedef unsigned char Uc;

        char r[4];
        readBytes(r, 4);

        x = Ui(Uc(r[0])) + Ui(Uc(r[1])) * 0x100
                + Ui(Uc(r[2])) * 0x10000 + Ui(Uc(r[3])) * 0x1000000;
        return *this;
    }

    InputBinaryStream& operator>>(uint64_t& x)
    {
        typedef uint32_t Ui;
        Ui a=0;
        Ui b=0;

        *this >> b >> a;
        x = (uint64_t(a) << 32) | uint64_t(b);


        return *this;
    }

    InputBinaryStream& operator>>(bool& t)
    {
        char buf = 0;
        readBytes(buf);
        if(buf == 0)
            t = false;
        else
            t = true;
        return *this;
    }

    InputBinaryStream& operator>>(uint16_t& x)
    {
        typedef uint16_t Ui;
        typedef unsigned char Uc;

        char r[2];
        readBytes(r, 2);

        x = Ui(Uc(r[0])) + Ui(Uc(r[1])) * 0x100;
        return *this;
    }
private:

    InputBinaryStream(const InputBinaryStream&);
    InputBinaryStream& operator=(const InputBinaryStream&);
    std::istream& _is;
    uint32_t _protocol_version;
    bool _ok;
};

class OutputBinaryStream
{
public:
    explicit OutputBinaryStream(std::ostream& os, const unsigned int protocol)
        : _ok(true), _os(os), _protocol_version(protocol)
    {
        (*this) << BinaryStream::Mark << protocol;
    }

    void writeBytes(const char* x, unsigned int size)
    {
        _os.write(x, size);
    }

    void writeBytes(const char x)
    {
        writeBytes(&x, 1);
    }
    bool isOK() const
    {
        return _os.good() && _ok;
    }
    unsigned int protocolVersion() const
    {
        return _protocol_version;
    }
    void setUndefiedProtocol()
    {
        _ok = false;
    }
    OutputBinaryStream& operator<<(bool t)
    {
        char bc = 1;
        if(!t)
            bc = 0;
        this->writeBytes(bc);
        return *this;
    }

    OutputBinaryStream& operator<<(uint32_t t)
    {
        char buf[4];
        buf[0] = t % 256;
        t /= 256;
        buf[1] = t % 256;
        t /= 256;
        buf[2] = t % 256;
        t /= 256;
        buf[3] = t % 256;
        writeBytes(buf, 4);
        return *this;
    }
	OutputBinaryStream& operator<<(uint16_t t)
    {
        char buf[2];
        buf[0] = t % 256;
        t /= 256;
        buf[1] = t % 256;
        writeBytes(buf, 2);
        return *this;
    }

    OutputBinaryStream& operator<<(uint64_t t)
    {
        uint32_t a = 0;
        uint32_t b = 0;

        uint64_t modulus = (uint64_t(1) << 32);
        a = uint32_t(t % modulus);
        t /= modulus;
        b = uint32_t(t % modulus);
        *this << a << b;
        return *this;
    }
private:
    OutputBinaryStream(const OutputBinaryStream&);
    OutputBinaryStream& operator=(const OutputBinaryStream&);
    std::ostream& _os;
    uint32_t _protocol_version;
    bool _ok;
};




inline OutputBinaryStream& operator<<(OutputBinaryStream& os, const std::string& str)
{
    assert(os.protocolVersion() <= BinaryStream::MaxProtocolVersion);
	uint16_t str_size = static_cast<uint16_t>(str.size());
    assert(str.size() < BinaryStream::MaxArraySize);

    os << str_size;
    os.writeBytes(&str[0], str_size);
    return os;
}

template <class T>
inline OutputBinaryStream& operator<<(OutputBinaryStream& os, const std::vector<T>& vec)
{
    assert(os.protocolVersion() <= BinaryStream::MaxProtocolVersion);
    uint16_t vec_size = static_cast<uint16_t>(vec.size());
    assert(vec.size() < BinaryStream::MaxArraySize);

    os << vec_size;
    for(uint16_t i=0; i<vec_size; ++i)
    {
        os << vec[i];
    }
    return os;
}

inline OutputBinaryStream& operator<<(OutputBinaryStream& os, const std::vector<char>& vec)
{
    assert(os.protocolVersion() <= BinaryStream::MaxProtocolVersion);
    uint16_t vec_size = static_cast<uint16_t>(vec.size());
    assert(vec.size() < BinaryStream::MaxArraySize);

    os << vec_size;
    os.writeBytes(&vec[0], vec_size);
    return os;
}

inline InputBinaryStream& operator>>(InputBinaryStream& is, std::string& str)
{
    if(is.protocolVersion() > BinaryStream::MaxProtocolVersion)
    {
        is.setUndefiedProtocol();
    }
    else
    {
        uint16_t size = 0;
        is >> size;
        if(is.isOK() && size < BinaryStream::MaxArraySize)
        {
            str.resize(size);
            if(size > 0)
                is.readBytes(&str[0], size);
        }
        else
        {
            is.setError();
        }
    }
    return is;
}
//#include "cocos2d.h"
template <class T>
inline InputBinaryStream& operator>>(InputBinaryStream& is, std::vector<T>& vec)
{
    if(is.protocolVersion() > BinaryStream::MaxProtocolVersion)
    {
        is.setUndefiedProtocol();
    }
    else
    {
        uint16_t size = 0;
        is >> size;

        //cocos2d::CCLog("Array read started: %d", size);
        if(is.isOK() && size < BinaryStream::MaxArraySize)
        {
            vec.resize(0);
            vec.reserve(size);
            T buffer;
            for(uint16_t i=0; i<size; ++i)
            {
                is >> buffer;
                vec.push_back(buffer);
            }
        }
        else
        {
            is.setError();
        }
    }
    return is;
}





inline InputBinaryStream& operator>>(InputBinaryStream& is, std::vector<char>& str)
{
    if(is.protocolVersion() > BinaryStream::MaxProtocolVersion)
    {
        is.setUndefiedProtocol();
    }
    else
    {
        uint16_t size = 0;
        is >> size;
        if(is.isOK() && size < BinaryStream::MaxArraySize)
        {
            str.resize(size);
            is.readBytes(&str[0], size);
        }
        else
        {
            is.setError();
        }
    }
    return is;
}

#endif // BINARY_H
