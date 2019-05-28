#ifndef TEFRI_MAPPING_OPERATOR_H
#define TEFRI_MAPPING_OPERATOR_H

namespace tefri
{
    struct MappingOperatorTag {};

    template <typename Callable>
    class Mapping : public MappingOperatorTag
    {
    public:
        Mapping(const Callable &);

        Mapping(Callable &);

        Mapping(Callable &&);

        template <typename Args, typename Next>
        void operator()(const Args &, Next &);
        
    private:
        Callable callable;
    };
}

#endif // TEFRI_MAPPING_OPERATOR_H