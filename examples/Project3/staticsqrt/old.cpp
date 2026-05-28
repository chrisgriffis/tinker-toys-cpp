#include <iostream>
#include <type_traits>

using namespace std;

constexpr unsigned long long constexpr_midval(unsigned  l, unsigned r)
{
    return (l < r) ? l + ((r - l) >> 1) : l;
}
constexpr unsigned constexpr_compute(bool b, unsigned L, unsigned R, unsigned k)
{
    return !b
        ? (L - 1)
        : ((constexpr_midval(L, R) * constexpr_midval(L, R)
            <= static_cast<unsigned long long>(k))
            ? constexpr_compute(
                L <= R,
                static_cast<unsigned>(constexpr_midval(L, R)) + 1,
                R,
                k)
            : constexpr_compute(
                L <= R,
                L,
                static_cast<unsigned>(constexpr_midval(L, R)) - 1,
                k));
}
constexpr unsigned constexpr_introot(unsigned k)
{
    return constexpr_compute(true, 1, k, k);
}

template<unsigned  l, unsigned r>
struct midval
{
    enum {
        val = (l < r) ? l + ((r - l) >> 1) : l
    };
};

template<bool, unsigned L, unsigned R, unsigned k>
class compute
{
    using Res = typename conditional<
        midval<L, R>::val * midval<L, R>::val <= k,
        typename compute<L <= R, midval<L, R>::val + 1, R, k>::Res,
        typename compute<L <= R, L, midval<L, R>::val - 1, k>::Res
    >::type;
    template<bool, unsigned, unsigned, unsigned>
    friend class compute;

public:
    enum {
        val = Res::value
    };
};

template<unsigned L, unsigned R, unsigned k>
class compute<false, L, R, k>
{
    template<bool, unsigned, unsigned, unsigned>
    friend class compute;
    using Res = integral_constant<unsigned, L - 1>;
};

template<unsigned k>
struct introot
{
    enum {
        val = compute<true, 1, k, k>::val
    };
};

int main1()
{
    constexpr long long expected_result = 1 << 4;
    constexpr unsigned num = constexpr_introot(expected_result*expected_result);
    char* x = new char[num]; //compiles!
    cout << "expected_result: " << expected_result
        << "; max intermediate val: " << (expected_result / 2)*(expected_result / 2)
        << "; calculated val (constexpr): " << num << endl << endl;
    delete[] x;

    constexpr unsigned num2 = introot<expected_result*expected_result>::val;
    x = new char[num2]; //compiles!
    cout << "expected_result: " << expected_result
        << "; max intermediate val: " << (expected_result / 2)*(expected_result / 2)
        << "; calculated val (TMP): " << num2 << endl << endl;
    return 0;
}
