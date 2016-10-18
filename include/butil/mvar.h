#ifndef BUTIL_MVAR_H_INCLUDED
#define BUTIL_MVAR_H_INCLUDED

#include <chrono>
#include <utility> // move, forward

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

namespace butil {

/** \brief Synchronized variable
 *
 * Inspired by Control.Concurrent.MVar from Haskell
 *
 * MVar can be either empty (contains T()) or contain a value.
 * Reading empty mvar blocks until another thread writes it.
 * Writing full mvar blocks until another thread reads it.
 *
 * T must behave like smart pointer :
 *  * must have operator bool
 *  * (bool)T() == false
 *
 * value() requires T to be copyable
 *
 * Use shared_ptr, unique_ptr, boost::optional or something similar
 */ 
template<typename T>
class mvar
{
public :
    typedef T value_type;
private :
    mutable boost::mutex m_mutex;

    boost::condition_variable m_condition_empty;
    boost::condition_variable m_condition_full;

    value_type m_value;
public :
    mvar() = default;
    ~mvar() = default;

    mvar(mvar const &) = delete;
    mvar & operator=(mvar const &) = delete;

    /** \brief Tests for emptiness
     *
     * \return true if not empty
     */
    explicit operator bool() const
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        return static_cast<bool>(m_value);
    }

    void clear()
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        m_value = value_type();
        // signal empty mvar
        lock.unlock();
        m_condition_empty.notify_one();
    }

    /** \brief Get contents without emptying the mvar.
     */
    value_type value()
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        // wait until mvar is full
        m_condition_full.wait(lock, [this](){ return m_value; });
        // retrieve value
        return m_value;
    }

    /** \brief Get contents
     *
     * Blocks when empty
     *
     * Empty after get returns.
     */
    value_type get()
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        // wait until mvar is full
        m_condition_full.wait(lock, [this](){ return m_value; });
        // retrieve value
        value_type value(std::move(m_value));
        m_value = value_type();
        // signal empty mvar
        lock.unlock();
        m_condition_empty.notify_one();
        return value;
    }

    /** \brief Put contents
     *
     * Blocks when full
     *
     * Full after put returns
     */
    template<typename U>
    void put(U && value)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        // wait until mvar is empty
        m_condition_empty.wait(lock, [this](){ return !m_value; });
        // store value
        m_value = std::forward<U>(value);
        // signal full mvar
        lock.unlock();
        m_condition_full.notify_one();
    }

    /** \brief Put contents (interruptible)
     *
     * When full, update callback is checked periodically. If it returns false, put
     * interrupts and returns false.
     */
    template<typename U, typename Update, typename Rep, typename Period>
    bool put(U && value, Update update,
            boost::chrono::duration<Rep,Period> period = boost::chrono::seconds(1)
        )
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        // wait until mvar is empty
        while(m_value)
        {
            // wait for a moment
            if(m_condition_empty.wait_for(lock, period) == boost::cv_status::timeout)
            {
                // mvar is full
                // - check interrupt callback
                if(!update())
                    return false;
            }
        }
        // store value
        m_value = std::forward<U>(value);
        // signal full mvar
        lock.unlock();
        m_condition_full.notify_one();
        return true;
    }
};

}//namespace butil

#endif//BUTIL_MVAR_H_INCLUDED
