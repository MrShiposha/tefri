#ifndef TEFRI_MONADBASE_H
#define TEFRI_MONADBASE_H

#include "tuple.h"

namespace tefri
{
    template <typename Monad, typename InputTupleVariants>
    class MonadBase // : public AbstractMonad<InputTupleVariants>
    {
    public:
        // TODO
    };

    template <typename Monad>
    class MonadBase<Monad, Tuple<>>
    {
    public:
        template <typename... Args>
        void operator()(Args&&... args)
        {
            *(static_cast<Monad*>(this))(std::forward<Args>(args)...);
        }

        template <typename... Args>
        void operator()(const Args &... args)
        {
            *(static_cast<Monad*>(this))(args...);
        }
    };
}

#endif // TEFRI_MONADBASE_H