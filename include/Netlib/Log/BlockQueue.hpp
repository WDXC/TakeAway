// Implementation of blocking queue
// app in multi-thread environment

#ifndef BLOCKQUEUE_H_
#define BLOCKQUEUE_H_


#include <mutex>
#include <deque>
#include <condition_variable>

template <class T>
class BlockQueue {
    public:
        explicit BlockQueue(int MaxSize = 1000);
        ~BlockQueue();

        bool isEmpty();
        bool isFull();
        int Size();
        int Capactity();

        T Front();
        T Back();
        void PushFront(const T& item);
        void PushBack(const T& item);
        bool PopFront(T& item);
        bool PopBack(T& item);
        void Flush();
        void Close();
    private:
        bool m_isClose;
        int m_maxSize;
        std::deque<T> m_deque;
        std::mutex m_mutex;
        std::condition_variable m_noFullConVar;
        std::condition_variable m_noEmptyConVar;
};

template <class T>
BlockQueue<T>::BlockQueue(int MaxSize) : 
    m_isClose(false),
    m_maxSize(MaxSize) {

}

template <class T>
BlockQueue<T>::~BlockQueue() {
    Close();
}

template <class T>
void BlockQueue<T>::Close() {
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_deque.clear();
        m_isClose = true;
    }
    m_noFullConVar.notify_all();
    m_noEmptyConVar.notify_all();
}

template <class T>
bool BlockQueue<T>::isEmpty() {
    std::unique_lock<std::mutex> locker(m_mutex);
    return m_deque.empty();
}

template <class T>
bool BlockQueue<T>::isFull() {
    std::unique_lock<std::mutex> locker(m_mutex);
    return m_deque.empty() >= m_maxSize;
}

template <class T>
int BlockQueue<T>::Size() {
    std::unique_lock<std::mutex> locker(m_mutex);
    return m_deque.size();
}

template <class T>
int BlockQueue<T>::Capactity() {
    std::unique_lock<std::mutex> locker(m_mutex);
    return m_maxSize;
}

template <class T>
T BlockQueue<T>::Front() {
    std::unique_lock<std::mutex> locker(m_mutex);
    return m_deque.front();
}

template <class T>
T BlockQueue<T>::Back() {
    std::unique_lock<std::mutex> locker(m_mutex);
    return m_deque.back();
}

template <class T>
void BlockQueue<T>::PushFront(const T& item) {
    std::unique_lock<std::mutex> locker(m_mutex);
    while (m_deque.size() >= m_maxSize) {
        m_noFullConVar.wait(locker);
    }
    m_deque.emplace_front(item);
    m_noEmptyConVar.notify_one();
}

template <class T>
void BlockQueue<T>::PushBack(const T& item) {
    std::unique_lock<std::mutex> locker(m_mutex);
    while (m_deque.size() >= m_maxSize) {
        m_noFullConVar.wait(locker);
    }
    m_deque.emplace_back(item);
    m_noEmptyConVar.notify_one();
}

template <class T>
bool BlockQueue<T>::PopFront(T& item) {
    std::unique_lock<std::mutex> locker(m_mutex);
    while (m_deque.empty()) {
        m_noEmptyConVar.wait(locker);
        if (m_isClose) return false;
    }
    item = m_deque.front();
    m_deque.pop_front();
    m_noFullConVar.notify_one();
    return true;
}

template <class T>
bool BlockQueue<T>::PopBack(T& item) {
    std::unique_lock<std::mutex> locker(m_mutex);
    while (m_deque.empty()) {
        m_noEmptyConVar.wait(locker);
        if (m_isClose) return false;
    }
    item = m_deque.back();
    m_deque.pop_back();
    m_noFullConVar.notify_one();
    return true;
}

template <class T>
void BlockQueue<T>::Flush() {
    std::unique_lock<std::mutex> locker(m_mutex);
    m_noEmptyConVar.notify_one();
}


#endif