#include <iostream>

// Strictly-typed integer template
template<int N>
struct Integer {
    static constexpr int value = N;
};

// Addition operation
template<int N, int M>
constexpr Integer<N + M> add(Integer<N> n, Integer<M> m) {
    return Integer<N + M>();
}

// Overloaded addition operator
template<int N, int M>
constexpr Integer<N + M> operator+(Integer<N> n, Integer<M> m) {
    return Integer<N + M>();
}

// Multiplication operation
template<int N, int M>
constexpr Integer<N * M> multiply(Integer<N> n, Integer<M> m) {
    return Integer<N * M>();
}

// Overloaded multiplication operator
template<int N, int M>
constexpr Integer<N * M> operator*(Integer<N> n, Integer<M> m) {
    return Integer<N * M>();
}

// Division operation
template<int N, int M>
constexpr Integer<N / M> divide(Integer<N> n, Integer<M> m) {
    return Integer<N / M>();
}

// Overloaded division operator
template<int N, int M>
constexpr Integer<N / M> operator/(Integer<N> n, Integer<M> m) {
    return Integer<N / M>();
}

// Power operation using parameter packs and fold expression
template<int N, int M>
struct Power {
    static constexpr int result = []() constexpr {
        int res = 1;
        for (int i = 0; i < M; ++i) {
            res *= N;
        }
        return res;
    }();
};


template<int N>
struct Power<N, 1> {
    static constexpr int result = N;
};

// Specialization for Power<N, 0> (N^0 = 1)
template<int N>
struct Power<N, 0> {
    static constexpr int result = 1;
};

// Logarithm operation
template<int Base, int N>
struct Logarithm {
    static constexpr int value = Logarithm<Base, N / Base>::value + 1;
};

template<int Base>
struct Logarithm<Base, 1> {
    static constexpr int value = 0;
};

// Square root operation
template<int N, int I = 1>
struct Sqrt {
    static constexpr double value = 0.5 * (Sqrt<N, I - 1>::value + N / Sqrt<N, I - 1>::value);
};

template<int N>
struct Sqrt<N, 0> {
    static constexpr double value = 1.0;
};

template<int N>
constexpr double squareRoot() {
    return Sqrt<N, 20>::value; // Adjust the number of iterations as needed
}

// Factorial operation
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

template<int N>
consteval int getValue(Integer<N> n) {
    return n.value;
}

int main() {
    // Perform calculations at compile time
    constexpr Integer<2> two;
    constexpr Integer<4> four;

    // Addition
    constexpr auto six = two + four * two;
    std::cout << "Two plus four is: " << getValue(six) << std::endl;

    // Multiplication
    constexpr auto eight = two * four;
    std::cout << "Two times four is: " << getValue(eight) << std::endl;

    // Division
    constexpr auto twoDivFour = four / two;
    std::cout << "Four divided by two is: " << getValue(twoDivFour) << std::endl;

    // Power (2^4)
    constexpr auto result = Power<2, 4>();
    std::cout << "Two to the power of four is: " << result.result << std::endl;


    // Logarithm (log base 2 of 8)
    constexpr auto logResult = Logarithm<2, 8>();
    std::cout << "Log base 2 of 8 is: " << logResult.value << std::endl;

    // Print the compile-time computed square roots
    std::cout << "Square root of 16: " << squareRoot<16>() << std::endl;
    std::cout << "Square root of 27: " << squareRoot<27>() << std::endl;

    // Factorial
    constexpr auto fact_4 = Factorial<4>();
    std::cout << "Factorial of 4 is: " << fact_4.value << std::endl;

    // Factorial
    constexpr auto fact_6 = Factorial<6>();
    std::cout << "Factorial of 6 is: " << fact_6.value << std::endl;

    return 0;
}
