#ifndef TEFRI_DETAIL_RINGQUEUE_H
#define TEFRI_DETAIL_RINGQUEUE_H

#include <stdexcept>
#include <memory>
#include <limits>
#include <algorithm>

namespace tefri::detail
{
    class NoQueueIsSpecified : public std::runtime_error
    {
    public:
        NoQueueIsSpecified()
        : std::runtime_error("tefri::detail::RingQueueIterator: No queue is specified")
        {}
    };

    class DereferencingIteratorEnd : public std::runtime_error
    {
    public:
        DereferencingIteratorEnd()
        : std::runtime_error("tefri::detail::RingQueueIterator: Attempt to dereference RingQueue::end() value")
        {}
    };

    class IncrementIteratorOnEmptyQueue : public std::runtime_error
    {
    public:
        IncrementIteratorOnEmptyQueue()
        : std::runtime_error("tefri::detail::RingQueueIterator: Attempt to increment iterator on empty ring queue")
        {}
    };

    template <typename T>
    class RingQueueIterator
    {
    public:
        using value_type         = T;
        using difference_type    = long long int;
        using pointer            = T*;
        using reference          = T&;
        using iterator_category  = std::forward_iterator_tag;

        static constexpr size_t NO_INDEX = std::numeric_limits<size_t>::max();

        RingQueueIterator()
        : index(NO_INDEX), processed_elements(0), queue_size(0), queue(nullptr)
        {}

        RingQueueIterator(const RingQueueIterator &) = default;

        RingQueueIterator(RingQueueIterator &&) = default;

        RingQueueIterator(size_t index, size_t queue_size, std::shared_ptr<T> queue)
        : index(index), queue_size(queue_size), queue(queue)
        {}

        ~RingQueueIterator() = default;

        RingQueueIterator &operator=(const RingQueueIterator &) = default;

        RingQueueIterator &operator=(RingQueueIterator &&) = default;

        RingQueueIterator &operator++()
        {
            if(queue_size == 0)
                throw IncrementIteratorOnEmptyQueue();

            index = (index + 1) % queue_size;
            ++processed_elements;
            return *this;
        }

        RingQueueIterator operator++(int)
        {
            RingQueueIterator old(*this);
            ++*this;

            return old;
        }

        bool operator==(const RingQueueIterator &rhs) const
        {
            if(queue != nullptr && rhs.queue != nullptr)
            {
                if(index != NO_INDEX && rhs.index != NO_INDEX)
                    return index == rhs.index;
                else if(index == NO_INDEX)
                {
                    if(queue_size != 0)
                        return (processed_elements != 0) && (processed_elements % queue_size == 0);
                    else
                        return true;
                }
                else if(rhs.index == NO_INDEX)
                {
                    if(rhs.queue_size != 0)
                        return (rhs.processed_elements != 0) && (rhs.processed_elements % queue_size == 0);
                    else
                        return true;
                }
                else
                    return true;
            }
            else if((queue != nullptr && rhs.queue == nullptr) || (queue == nullptr && rhs.queue != nullptr))
                return false;
            else
                return true;
        }

        bool operator!=(const RingQueueIterator &rhs) const
        {
            return !(*this == rhs);
        }

        const T &operator*() const
        {
            return static_cast<RingQueueIterator>(this)->operator*();
        }

        reference operator*()
        {
            if(queue == nullptr)
                throw NoQueueIsSpecified();
            else if(index == NO_INDEX)
                throw DereferencingIteratorEnd();
            return queue.get()[index];
        }

        const pointer operator->() const
        {
            return static_cast<RingQueueIterator>(this)->operator->();
        }

        pointer operator->()
        {
            if(queue == nullptr)
                throw NoQueueIsSpecified();
            else if(index == NO_INDEX)
                throw DereferencingIteratorEnd();
            return &queue[index];
        }

    private:
        size_t               index;
        size_t               processed_elements;
        size_t               queue_size;
        std::shared_ptr<T> queue;
    };

    template <typename T>
    class RingQueue
    {
    public:
        using Iterator = RingQueueIterator<T>;

        RingQueue(size_t size)
        : size(size), last_index(0), queue(new T[size], std::default_delete<T[]>())
        {}

        RingQueue(size_t size, const T &value)
        : RingQueue(size) 
        {
            std::fill(&queue[0], &queue[size], value);
        }

        void push(const T &value)
        {
            queue.get()[last_index] = value;
            last_index = (last_index + 1) % size;
        }

        const T &operator[](size_t i) const
        {
            return const_cast<RingQueue>(this)->operator[](i);
        }

        T &operator[](size_t i)
        {
            return queue.get()[(i + last_index - 1) % size];
        }

        Iterator begin()
        {
            return Iterator((last_index - 1) % size, size, queue);
        }

        Iterator end()
        {
            return Iterator(Iterator::NO_INDEX, size, queue);
        }

    private:
        size_t               size;
        size_t               last_index;
        std::shared_ptr<T> queue;
    };
}

#endif // TEFRI_DETAIL_RINGQUEUE_H