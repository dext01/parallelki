#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

#ifdef USE_DOUBLE
using Real = double;
#else
using Real = float;
#endif

int main() {
    const size_t N = 10000000;
    const Real PI = 3.14159265358979323846;
    const Real TWO_PI = 2.0 * PI;

    std::vector<Real> arr(N);

    for (size_t i = 0; i < N; ++i) {
        Real x = (i * TWO_PI) / static_cast<Real>(N);
        arr[i] = std::sin(x);
    }

    Real sum = 0.0;
    for (size_t i = 0; i < N; ++i) {
        sum += arr[i];
    }

    std::cout << "Тип: " << (sizeof(Real) == 8 ? "double" : "float") << std::endl;
    std::cout << "Количество элементов: " << N << std::endl;
    std::cout << "Сумма: " << sum << std::endl;

    return 0;
}
