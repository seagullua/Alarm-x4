#ifndef FILE_EXISTS_H
#define FILE_EXISTS_H
#include <string>
#include <fstream>
inline bool fileExists(const char* name)
{
    FILE *fp = fopen(name, "r");
    bool bRet = false;

    if (fp)
    {
        bRet = true;
        fclose(fp);
    }

    return bRet;
}

inline std::string getFileNameFromUrl(const std::string& url)
{
    size_t last_slash = url.find_last_of('/');
    if(last_slash != std::string::npos)
        return url.substr(last_slash+1);
    return url;
}

#endif // FILE_EXISTS_H
