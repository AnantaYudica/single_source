#ifndef INTERVAL_H_
#define INTERVAL_H_

#include <utility>

template<typename TIterator>
class Interval
{
public:
    typedef TIterator IteratorType;
private:
    IteratorType m_begin;
    IteratorType m_end;
public:
    Interval() = default;
    Interval(IteratorType begin, IteratorType end);
public:
    Interval(const Interval<TIterator> & cpy);
    Interval(Interval<TIterator> && mov);
public:
    Interval<TIterator> & operator=(const Interval<TIterator> & cpy);
    Interval<TIterator> & operator=(Interval<TIterator> && mov);
public:
    IteratorType Begin(IteratorType begin);
    IteratorType Begin() const;
    IteratorType End(IteratorType end);
    IteratorType End() const;
};

template<typename TIterator>
Interval<TIterator>::Interval(IteratorType begin, 
    IteratorType end) :
        m_begin(begin),
        m_end(end)
{}

template<typename TIterator>
Interval<TIterator>::Interval(const Interval<TIterator> & cpy) :
    m_begin(cpy.m_begin),
    m_end(cpy.m_end)
{}

template<typename TIterator>
Interval<TIterator>::Interval(Interval<TIterator> && mov) :
    m_begin(std::move(mov.m_begin)),
    m_end(std::move(mov.m_end))
{
    mov.m_begin = mov.m_end;
}

template<typename TIterator>
Interval<TIterator> & Interval<TIterator>::
    operator=(const Interval<TIterator> & cpy)
{
    m_begin = cpy.m_begin;
    m_end = cpy.m_end;
}

template<typename TIterator>
Interval<TIterator> & Interval<TIterator>::
    operator=(Interval<TIterator> && mov)
{
    m_begin = cpy.m_begin;
    m_end = cpy.m_end;
    mov.m_begin = mov.m_end;
}

template<typename TIterator>
typename Interval<TIterator>::IteratorType 
    Interval<TIterator>::Begin(IteratorType begin)
{
    return (m_begin = begin);
}

template<typename TIterator>
typename Interval<TIterator>::IteratorType 
    Interval<TIterator>::Begin() const
{
    return m_begin;
}

template<typename TIterator>
typename Interval<TIterator>::IteratorType 
    Interval<TIterator>::End(IteratorType end)
{
    return (m_end = end);
}

template<typename TIterator>
typename Interval<TIterator>::IteratorType 
    Interval<TIterator>::End() const
{
    return m_end;
}

#endif //!INTERVAL_H_
