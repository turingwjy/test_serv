/*
 * =====================================================================================
 *
 *       Filename:  LockedQueue.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年12月28日 16时59分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <ace/Guard_T.h>
#include <ace/Thread_Mutex.h>
#include <deque>

template <typename T, typename LOCK_TYPE, typename StorageType = std::deque<T> >
class LockedQueue
{
public:
    LockedQueue(){}
    ~LockedQueue(){}
public:
    void add( const T& data )
    {
        lock();
        m_queue.push_back(data);
        unlock();
    }
    bool next( T& data )
    {
        lock();
        if ( m_queue.empty() )
            return false;
        data = m_queue.front();
        m_queue.pop_front();
        unlock();
        return true;
    }
    size_t size()
    {
        return m_queue.size();
    }
    bool empty()
    {
        return m_queue.empty();
    }
    void lock()
    {
        m_lock.acquire();
    }
    void unlock()
    {
        m_lock.release();
    }
private:
    LOCK_TYPE   m_lock;
    StorageType m_queue;
};
