#ifndef TEFRI_MAPPING_OPERATOR_H
#define TEFRI_MAPPING_OPERATOR_H

#include <functional>

namespace tefri
{
    struct MappingOperatorTag {};

    template <typename Callable>
    class Mapping : public MappingOperatorTag
    {
    public:
        Mapping();

        Mapping(const Mapping &);

        Mapping(Mapping &&);

        Mapping(const Callable &);

        Mapping(Callable &);

        Mapping(Callable &&);

        Mapping &operator=(const Mapping &);

        Mapping &operator=(Mapping &&);

        virtual ~Mapping();

    protected:
        Callable callable;
    };
}

#endif // TEFRI_MAPPING_OPERATOR_H