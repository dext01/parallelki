#include <iostream>
#include <vector>
#include <omp.h>
#include <iomanip>

//умножение матрицы на вектор
void matvec_serial(const std::vector<double>& a, const std::vector<double>& b, std::vector<double>& c, int m, int n) {
    for (int i = 0; i < m; ++i) {
        double sum = 0.0;
        for (int j = 0; j < n; ++j) {
            sum += a[i * n + j] * b[j];
        }
        c[i] = sum;
    }
}

//параллельно
void matvec_omp(const std::vector<double>& a, const std::vector<double>& b, std::vector<double>& c, int m, int n) {
    #pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int tid = omp_get_thread_num();
        int chunk = m / nthreads;
        int start_i = tid * chunk;
        int end_i = (tid == nthreads - 1) ? m : start_i + chunk;

        for (int i = start_i; i < end_i; ++i) {
            double sum = 0.0;
            for (int j = 0; j < n; ++j) {
                sum += a[i * n + j] * b[j];
            }
            c[i] = sum;
        }
    }
}

//подготока данных для параллельной версии
void init_data(std::vector<double>& a, std::vector<double>& b, std::vector<double>& c, int m, int n, int num_threads) {
    omp_set_num_threads(num_threads);

    a.assign(m * n, 0.0); //выделяем память
    b.assign(n, 0.0);
    c.assign(m, 0.0);

    #pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int tid = omp_get_thread_num();
        int chunk = m / nthreads;
        int start_i = tid * chunk;
        int end_i = (tid == nthreads - 1) ? m : start_i + chunk;

        for (int i = start_i; i < end_i; ++i) {
            for (int j = 0; j < n; ++j) {
                a[i * n + j] = static_cast<double>(i + j);
            }
            c[i] = 0.0;
        }
    }

    for (int j = 0; j < n; ++j) {
        b[j] = static_cast<double>(j);
    }
}

//время работы последовательной версии
double measure_serial(int m, int n) {
    std::vector<double> a(m * n);
    std::vector<double> b(n);
    std::vector<double> c(m);

    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            a[i * n + j] = static_cast<double>(i + j);
    for (int j = 0; j < n; ++j)
        b[j] = static_cast<double>(j);

    double t_start = omp_get_wtime();
    matvec_serial(a, b, c, m, n);
    return omp_get_wtime() - t_start;
}

//время работы параллельной версии
double measure_parallel(int m, int n, int num_threads) {
    std::vector<double> a, b, c;
    init_data(a, b, c, m, n, num_threads);

    double t_start = omp_get_wtime();
    matvec_omp(a, b, c, m, n);
    return omp_get_wtime() - t_start;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <N> <num_threads>\n";
        return 1;
    }

    int N = std::stoi(argv[1]);
    int num_threads = std::stoi(argv[2]);

    double t_serial = measure_serial(N, N);
    double t_parallel = measure_parallel(N, N, num_threads);

    double speedup = t_serial / t_parallel;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "N=" << N
              << ", Threads=" << num_threads
              << ", T_serial=" << t_serial
              << ", T_parallel=" << t_parallel
              << ", Speedup=" << speedup << "\n";

    return 0;
}
