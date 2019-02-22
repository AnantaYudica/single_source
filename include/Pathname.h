#ifndef PATHNAME_H_
#define PATHNAME_H_

#include <string>
#include <regex>

#include "Interval.h"

class Pathname
{
public:
    typedef std::string StringType;
    typedef Interval<typename StringType::const_iterator> IntervalType;
private:
    static const std::regex ms_pathname_regex;
private:
    IntervalType m_dirname_strpos;
    IntervalType m_filename_strpos;
    StringType m_pathname_str;
private:
    static void DirFilename(const std::string & pathname,
        IntervalType & dirname_strpos, IntervalType & filename_strpos);
public:
    Pathname();
    Pathname(std::string pathname_str);
public:
    Pathname(const Pathname & cpy);
    Pathname(Pathname && mov);
public:
    Pathname & operator=(const Pathname & cpy);
    Pathname & operator=(Pathname && mov);
public:
    std::string GetPathname() const;
    std::string GetDirname() const;
    std::string GetFilename() const;

};

#endif //!_PATHNAME_H_
