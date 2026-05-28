#include <iostream>
#include <vector>
#include <array>
#include <iterator>
using namespace std;

template<class T>
struct slice {
    T* data;
    std::size_t const* stride;
    slice operator[](std::size_t I) const {
        return{ data + *stride * I, stride + 1 };
    }
    operator T&()const { return *data; }
    T& operator=(T in) { *data = std::move(in); return *data; }
};


template<class T>
struct MDArray {
    MDArray(std::vector<std::size_t> sizes) :strides_(sizes) {
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
    slice<T> operator[](std::size_t I) { return get()[I]; }
private:
    std::vector<T> data_;
    std::vector<std::size_t> strides_;
    slice<T> get() { return{ data_.data(), strides_.data() }; }

};

int main() {
    // your code goes here
    MDArray<int> b({ 2,5,3 });

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                b[i][j][k] = 3 * (5 * (i)+j) + k + 1;
            }
        }
    }
    copy(b.data(), b.data() + 2 * 5 * 3, ostream_iterator<int>(cout, " "));
    return 0;
}
