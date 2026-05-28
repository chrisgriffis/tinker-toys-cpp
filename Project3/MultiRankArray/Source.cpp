#include <iostream>
#include <vector>
#include <array>
#include <iterator>
using namespace std;

template<class T>
struct slice {
    T* data;
    std::size_t const* stride;
    slice operator[](std::size_t I)const {
        return{ data + I* *stride, stride + 1 };
    }
    operator T&()const { return *data; }
    T& operator=(T in) { *data = std::move(in); return *data; }
};


template<class T>
struct MultiRankArray {
    std::vector<T> data_;
    std::vector<std::size_t> strides_;

    MultiRankArray(std::vector<std::size_t> sizes) :strides_(sizes) {
        std::size_t scale = 1;
        for (std::size_t i = 0; i < sizes.size(); ++i) {
            auto next = strides_[sizes.size() - 1 - i] * scale;
            strides_[sizes.size() - 1 - i] = scale;
            scale = next;
        }
        data_.resize(scale);
    }
    const std::vector<std::size_t>& strides() const { return strides_; };
    T* data() { return data_.data(); }
    slice<T> get() { return{ data_.data(), strides_.data() }; }
    slice<T> operator[](std::size_t I) { return get()[I]; }

};


int main() {
    // your code goes here

    struct foo { double dbl; float fl; int i;  void* ptr; };
    MultiRankArray<foo> threeDimArray({ 2,5,3 });

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                threeDimArray[i][j][k] = foo{ .3 * (.5 * (i)+j) + k + .1, .3f * (.5f * (i)+j) + k + .1f, 3 * (5 * (i)+j) + k + 1, nullptr };
            }
        }
    }
    copy(reinterpret_cast<char*>(threeDimArray.data()), reinterpret_cast<char*>(threeDimArray.data()) + 2 * 5 * 3 * sizeof(foo), ostream_iterator<char>(cout, " "));
    return 0;
}
