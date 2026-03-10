#include <iostream>
#include <iomanip>
#include <cmath>
#include <omp.h>

double func(double x) {
    return std::exp(-x * x);
}

const double exact_value = std::sqrt(M_PI);

double integrate_omp(double a, double b, int nsteps) {
    double h = (b - a) / static_cast<double>(nsteps);
    double global_sum = 0.0;

    #pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int tid = omp_get_thread_num();
        int chunk = nsteps / nthreads;
        int start = tid * chunk;
        int end = (tid == nthreads - 1) ? nsteps : start + chunk;

        double local_sum = 0.0;
        for (int i = start; i < end; ++i) {
            double x = a + (i + 0.5) * h;
            local_sum += func(x);
        }

        #pragma omp atomic
        global_sum += local_sum;
    }

    return global_sum * h;
}

double integrate_serial(double a, double b, int nsteps) {
    double h = (b - a) / static_cast<double>(nsteps);
    double sum = 0.0;
    for (int i = 0; i < nsteps; ++i) {
        double x = a + (i + 0.5) * h;
        sum += func(x);
    }
    return sum * h;
}

int main(int argc, char** argv) {
    const double a = -4.0;
    const double b = 4.0;
    const int nsteps = 40'000'000;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <num_threads>\n";
        return 1;
    }

    int num_threads = std::stoi(argv[1]);

    double t_serial = 0.0;
    {
        omp_set_num_threads(1);
        double t0 = omp_get_wtime();
        double res = integrate_serial(a, b, nsteps);
        t_serial = omp_get_wtime() - t0;
        volatile double dummy = res; (void)dummy;
    }

    omp_set_num_threads(num_threads);
    double t0 = omp_get_wtime();
    double res_parallel = integrate_omp(a, b, nsteps);
    double t_parallel = omp_get_wtime() - t0;

    double speedup = t_serial / t_parallel;
    double error = std::abs(res_parallel - exact_value);

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "nsteps=" << nsteps
              << ", threads=" << num_threads
              << ", T_serial=" << t_serial
              << ", T_parallel=" << t_parallel
              << ", Speedup=" << speedup
              << ", Error=" << error << "\n";

    return 0;
}