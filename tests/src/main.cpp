#include <tefri/Pipeline.h>

struct F1
{
    double operator()(int)
    {}
};

struct F2
{
    float operator()(char)
    {}
};

int main()
{
    using namespace tefri;
    using namespace metaxxa;

    Pipeline<Tuple<F1, F2>> pipeline;

    static_assert(std::is_same_v<typename Pipeline<Tuple<F1, F2>>::InputTuple, Tuple<int>>, "input types are not equal (1)");
    static_assert(std::is_same_v<typename Pipeline<Tuple<F2, F1>>::InputTuple, Tuple<char>>, "input types are not equal (2)");

    static_assert(std::is_same_v<typename Pipeline<Tuple<F1, F2>>::OutputTuple, Tuple<float>>, "output types are not equal (1)");
    static_assert(std::is_same_v<typename Pipeline<Tuple<F2, F1>>::OutputTuple, Tuple<double>>, "output are not equal (2)");


    return 0;
}