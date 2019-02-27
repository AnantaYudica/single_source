#include "Pathname.h"

#include <stack>
#include <utility>

using namespace std;

const string Pathname::ms_norm_conj_dir_str("/");

const string Pathname::ms_parent_dir_str("../");

const regex Pathname::ms_rep_conj_dir_regex("\\\\");

const regex Pathname::ms_dir_filename_regex("(.*[\\/\\\\])*(.*)");

const regex Pathname::ms_file_extname_regex("(.*)(\\..*)");

const regex Pathname::ms_search_dir_regex("([^\\/]*\\/)");

void Pathname::Normalize(string & pathname)
{
    pathname = regex_replace(pathname, ms_rep_conj_dir_regex, 
        ms_norm_conj_dir_str);
    vector<string> name_split;
    ptrdiff_t diff = 0;
    cmatch rslt;
    pair<const char *, const char *> m_last_suffix;
    while (regex_search(pathname.c_str() + diff, rslt, ms_search_dir_regex) &&
        rslt.size() > 1)
    {
        name_split.push_back(rslt[1].str());
        diff = rslt.suffix().first - pathname.c_str();
        m_last_suffix.first = rslt.suffix().first;
        m_last_suffix.second = rslt.suffix().second;
    }
    stack<string> name_stack;
    string filename = string(m_last_suffix.first, m_last_suffix.second);
    size_t count_skip = 0;
    for(auto it = name_split.rbegin(); it != name_split.rend(); ++it)
    {
        if (it->compare(ms_parent_dir_str) == 0)
            ++count_skip;
        else
        {
            if (count_skip != 0)
                --count_skip;
            else
                name_stack.push(std::move(*it));
        }
    }
    while(!name_stack.empty())
    {
        pathname.append(name_stack.top());
        name_stack.pop();
    }
    pathname.append(filename);
}

void Pathname::DirFileExtname(const string & pathname,
    IntervalType & dirname_strpos, IntervalType& filename_strpos,
    IntervalType & extname_strpos)
{
    smatch s_rslt;
    cmatch c_rslt;
    ptrdiff_t diff = 0;
    if (regex_match(pathname, s_rslt, ms_dir_filename_regex) && 
        s_rslt.size() > 2)
    {
        dirname_strpos.Begin(s_rslt[1].first);
        dirname_strpos.End(s_rslt[1].second);
        diff = s_rslt[2].first - pathname.begin();
    }
    else
    {
        dirname_strpos.Begin(pathname.begin());
        dirname_strpos.End(pathname.end());
    }
    if (regex_match(pathname.c_str() + diff, c_rslt, ms_file_extname_regex) && 
        c_rslt.size() > 2)
    {
        filename_strpos.Begin(pathname.begin() + 
            (c_rslt[1].first - pathname.c_str()));
        filename_strpos.End(pathname.begin() + 
            (c_rslt[1].second - pathname.c_str()));
        extname_strpos.Begin(pathname.begin() + 
            (c_rslt[2].first - pathname.c_str()));
        extname_strpos.End(pathname.begin() + 
            (c_rslt[2].second - pathname.c_str()));
    }
    else
    {
        filename_strpos.Begin(pathname.end());
        filename_strpos.End(pathname.end());
        extname_strpos.Begin(pathname.end());
        extname_strpos.End(pathname.end());
    }
}

Pathname::Pathname() :
    m_pathname_str(""),
    m_dirname_strpos(m_pathname_str.end(), m_pathname_str.end()),
    m_filename_strpos(m_pathname_str.end(), m_pathname_str.end()),
    m_extname_strpos(m_pathname_str.end(), m_pathname_str.end())
{}
    
Pathname::Pathname(string pathname_str) :
    m_pathname_str(std::move(pathname_str))
{
    DirFileExtname(m_pathname_str, m_dirname_strpos, 
        m_filename_strpos, m_extname_strpos);
}

Pathname::Pathname(const Pathname & cpy) :
    m_pathname_str(cpy.m_pathname_str),
    m_dirname_strpos(cpy.m_dirname_strpos),
    m_filename_strpos(cpy.m_filename_strpos),
    m_extname_strpos(cpy.m_extname_strpos)
{}

Pathname::Pathname(Pathname && mov) :
    m_pathname_str(std::move(mov.m_pathname_str)),
    m_dirname_strpos(std::move(mov.m_dirname_strpos)),
    m_filename_strpos(std::move(mov.m_filename_strpos)),
    m_extname_strpos(std::move(mov.m_extname_strpos))
{}

Pathname & Pathname::operator=(const Pathname & cpy)
{
    m_pathname_str = cpy.m_pathname_str;
    m_dirname_strpos = cpy.m_dirname_strpos;
    m_filename_strpos = cpy.m_filename_strpos;
    m_extname_strpos = cpy.m_extname_strpos;
    return *this;
}

Pathname & Pathname::operator=(Pathname && mov)
{
    m_pathname_str = std::move(mov.m_pathname_str);
    m_dirname_strpos = std::move(mov.m_dirname_strpos);
    m_filename_strpos = std::move(mov.m_filename_strpos);
    m_extname_strpos = std::move(mov.m_extname_strpos);
    return *this;
}

bool Pathname::operator==(const Pathname & pathname) const
{
    return m_pathname_str.compare(pathname.m_pathname_str) == 0; 
}

bool Pathname::operator!=(const Pathname & pathname) const
{
    return !operator==(pathname);
}

string Pathname::GetPathname() const
{
    return m_pathname_str;
}

string Pathname::GetDirname() const
{
    return string(m_dirname_strpos.Begin(), m_dirname_strpos.End());
}

string Pathname::GetFilename() const
{
    return string(m_filename_strpos.Begin(), m_filename_strpos.End());
}

std::string Pathname::GetExtname() const
{
    return string(m_extname_strpos.Begin(), m_extname_strpos.End());
}

bool Pathname::IsFile() const
{
    return m_filename_strpos.Begin() != m_filename_strpos.End() ||
        m_extname_strpos.Begin() != m_extname_strpos.End();
}

bool Pathname::IsDirectory() const
{
    return m_dirname_strpos.Begin() != m_dirname_strpos.End() && 
        m_filename_strpos.Begin() == m_filename_strpos.End() &&
        m_extname_strpos.Begin() == m_extname_strpos.End();
}

Pathname::operator bool() const
{
    return m_dirname_strpos.Begin() != m_dirname_strpos.End() && 
        m_filename_strpos.Begin() != m_filename_strpos.End() &&
        m_extname_strpos.Begin() != m_extname_strpos.End();
}

