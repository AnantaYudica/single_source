#ifndef PATHNAME_H_
#define PATHNAME_H_

#include <string>
#include <memory>
#include <regex>

#include "Interval.h"

class Pathname
{
public:
    typedef std::shared_ptr<Pathname> PointerType;
    typedef std::weak_ptr<Pathname> WeakPointerType;
    typedef std::string StringType;
    typedef Interval<typename StringType::const_iterator> IntervalType;
private:
    static const std::string ms_norm_conj_dir_str;
    static const std::string ms_parent_dir_str;
    static const std::regex ms_rep_conj_dir_regex;
    static const std::regex ms_dir_filename_regex;
    static const std::regex ms_file_extname_regex;
    static const std::regex ms_search_dir_regex;
private:
    IntervalType m_dirname_strpos;
    IntervalType m_filename_strpos;
    IntervalType m_extname_strpos;
    StringType m_pathname_str;
private:
    static void Normalize(std::string & pathname);
    static void DirFileExtname(const std::string & pathname,
        IntervalType & dirname_strpos, IntervalType & filename_strpos,
        IntervalType & extname_strpos);
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
    bool operator==(const Pathname & pathname) const;
    bool operator!=(const Pathname & pathname) const;
public:
    std::string GetPathname() const;
    std::string GetDirname() const;
    std::string GetFilename() const;
    std::string GetExtname() const;
public:
    bool IsFile() const;
    bool IsDirectory() const;
public:
    operator bool() const;
};

#endif //!_PATHNAME_H_
