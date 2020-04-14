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
#ifndef _LOCKED_QUEUE_H
#define _LOCKED_QUEUE_H

#include <ace/Guard_T.h>
#include <ace/Thread_Mutex.h>
#include <deque>
template < typename DataType, typename LockType, typename StorageType = std::deque<DataType> >
class LockedQueue
{
public:
    LockedQueue(){}
    ~LockedQueue(){}
public:
    void add( const DataType& data )
    {
        lock();
        m_queue.push_back(data);
        unlock();
    }
    bool next( DataType& data )
    {
        lock();
        if ( m_queue.empty() )
        {
            unlock();
            return false;
        }
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
    LockType    m_lock;
    StorageType m_queue;
};

#endif
