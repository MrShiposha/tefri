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
        template <typename... Seq>
        void operator()(Seq&&... args)
        {
            *(static_cast<Monad*>(this))(std::forward<Seq>(args)...);
        }

        template <typename... Seq>
        void operator()(const Seq &... args)
        {
            *(static_cast<Monad*>(this))(args...);
        }
    };
}

#endif // TEFRI_MONADBASE_H