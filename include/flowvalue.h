#ifndef TEFRI_FLOWVALUE_H
#define TEFRI_FLOWVALUE_H

#include <chrono>
#include <atomic>

#include "def.h"
#include "priority.h"

namespace tefri
{
    class FlowValue
    {
    public:
        using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

        tefri_inline FlowValue(unsigned long long priority = static_cast<unsigned long long>(Priority::NORMAL));

        tefri_inline FlowValue(Priority);

        tefri_inline FlowValue(const FlowValue &);

        tefri_inline FlowValue(FlowValue &&);

        tefri_inline virtual ~FlowValue();

        tefri_inline FlowValue &operator=(const FlowValue &) = delete;

        tefri_inline FlowValue &operator=(FlowValue &&) = delete;

        tefri_inline const TimePoint &creation_time() const;

        tefri_inline std::size_t operator_index() const;

        tefri_inline unsigned long long priority() const;

        tefri_inline void priority(unsigned long long);

        tefri_inline void priority(Priority);

        tefri_inline void next_operator_index();

    private:
        const TimePoint    creation;
        std::atomic_size_t index;
        std::atomic_ullong priority_;
    };
}

#endif // TEFRI_FLOWVALUE_H