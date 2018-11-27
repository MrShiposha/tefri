#ifndef TEST_TEFRI_DETAIL_UNWRAPOBJECTHOLDERS_H
#define TEST_TEFRI_DETAIL_UNWRAPOBJECTHOLDERS_H

#include "TestTefri.h"

class TestUnwrapObjectHolders : public TestTefri
{
public:
    virtual bool test() override
    {
        bool result = true;
        result = result && test_unwrap_single_object_holder();
        result = result && test_unwrap_single_non_object_holder();
        result = result && test_unwrap_tuple_of_object_holders();
        result = result && test_unwrap_tuple_of_general_objects();
        result = result && test_unwrap_tuple_of_non_object_holders();

        return result;
    }

    bool test_unwrap_single_object_holder()
    {
        using namespace metaxxa;
        using namespace tefri;
        using namespace tefri::detail;

        static_assert
        (
            Type<UnwrapObjectHolder<ObjectHolder<double>>>() == Type<double>(), 
            "Invalid unwrapped object holder's type"
        );

        static_assert
        (
            Type<UnwrapObjectHolder<ObjectHolder<std::reference_wrapper<double>>>>() == Type<double>(), 
            "Invalid unwrapped object holder's type"
        );

        static_assert
        (
            Type<UnwrapObjectHolder<ObjectHolder<std::reference_wrapper<const double>>>>() == Type<const double>(), 
            "Invalid unwrapped object holder's type"
        );

        return true;
    }

    bool test_unwrap_single_non_object_holder()
    {
        using namespace metaxxa;
        using namespace tefri;
        using namespace tefri::detail;

        static_assert
        (
            Type<UnwrapObjectHolder<double>>() == Type<double>(), 
            "Invalid unwrapped non-object-holder's type"
        );

        static_assert
        (
            Type<UnwrapObjectHolder<std::reference_wrapper<double>>>() == Type<double>(), 
            "Invalid unwrapped non-object-holder's type"
        );

        static_assert
        (
            Type<UnwrapObjectHolder<std::reference_wrapper<const double>>>() == Type<const double>(), 
            "Invalid unwrapped non-object-holder's type"
        );

        static_assert
        (
            Type<UnwrapObjectHolder<Tuple<const double>>>() == Type<Tuple<const double>>(), 
            "Invalid unwrapped non-object-holder's type"
        );

        return true;
    }

    bool test_unwrap_tuple_of_object_holders()
    {
        using namespace metaxxa;
        using namespace tefri;
        using namespace tefri::detail;

        static_assert
        (
            Type<UnwrapObjectHolders<Tuple<ObjectHolder<double>, ObjectHolder<int>>>>() == Type<Tuple<double, int>>(),
            "Invalid unwrapped tuple of object holders"
        );

        static_assert
        (
            Type
            <
                UnwrapObjectHolders
                <
                    Tuple
                    <
                        ObjectHolder<std::reference_wrapper<double>>, 
                        ObjectHolder<std::reference_wrapper<int>>
                    >
                >
            >() == Type<Tuple<double, int>>(),
            "Invalid unwrapped tuple of object holders"
        );

        static_assert
        (
            Type
            <
                UnwrapObjectHolders
                <
                    Tuple
                    <
                        ObjectHolder<std::reference_wrapper<const double>>, 
                        ObjectHolder<std::reference_wrapper<const int>>
                    >
                >
            >() == Type<Tuple<double, int>>(),
            "Invalid unwrapped tuple of object holders"
        );

        static_assert
        (
            Type<UnwrapObjectHolders<ObjectHolder<double>>>() == Type<double>(),
            "Invalid unwrapped tuple of object holders"
        );

        static_assert
        (
            Type<UnwrapObjectHolders<ObjectHolder<std::reference_wrapper<double>>>>() == Type<double>(),
            "Invalid unwrapped tuple of object holders"
        );

        static_assert
        (
            Type<UnwrapObjectHolders<ObjectHolder<std::reference_wrapper<const double>>>>() == Type<const double>(),
            "Invalid unwrapped tuple of object holders"
        );


        static_assert
        (
            Type<UnwrapObjectHolders<std::reference_wrapper<double>>>() == Type<double>(),
            "Invalid unwrapped tuple of object holders"
        );

        static_assert
        (
            Type<UnwrapObjectHolders<std::reference_wrapper<const double>>>() == Type<const double>(),
            "Invalid unwrapped tuple of object holders"
        );

        return true;
    }

    bool test_unwrap_tuple_of_general_objects()
    {
        using namespace metaxxa;
        using namespace tefri;
        using namespace tefri::detail;

        static_assert
        (
            Type
            <
                UnwrapObjectHolders
                <
                    Tuple
                    <
                        ObjectHolder<double>, 
                        char, 
                        ObjectHolder<int>
                    >
                >
            >() == Type<Tuple<double, char, int>>(),
            "Invalid unwrapped tuple of general objects"
        );

        static_assert
        (
            Type
            <
                UnwrapObjectHolders
                <
                    Tuple
                    <
                        ObjectHolder<std::reference_wrapper<double>>, 
                        char, 
                        ObjectHolder<std::reference_wrapper<const int>>,
                        std::reference_wrapper<float>,
                        std::vector<int>
                    >
                >
            >() == Type<Tuple<double, char, int, float, std::vector<int>>>(),
            "Invalid unwrapped tuple of general objects"
        );

        return true;
    }

    bool test_unwrap_tuple_of_non_object_holders()
    {
        using namespace metaxxa;
        using namespace tefri;
        using namespace tefri::detail;

        static_assert
        (
            Type
            <
                UnwrapObjectHolders
                <
                    Tuple
                    <
                        double, 
                        char, 
                        int
                    >
                >
            >() == Type<Tuple<double, char, int>>(),
            "Invalid unwrapped tuple of general objects"
        );

        static_assert
        (
            Type
            <
                UnwrapObjectHolders
                <
                    Tuple
                    <
                        std::reference_wrapper<double>, 
                        char, 
                        std::reference_wrapper<const int>,
                        std::reference_wrapper<float>,
                        std::vector<int>
                    >
                >
            >() == Type<Tuple<double, char, int, float, std::vector<int>>>(),
            "Invalid unwrapped tuple of general objects"
        );

        return true;
    }
};

#endif // TEST_TEFRI_DETAIL_UNWRAPOBJECTHOLDERS_H