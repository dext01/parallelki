## Сборка с выбором типа данных

```bash
# Очистка предыдущей сборки
rm -rf build

# Сборка с float
mkdir build && cd build
cmake .. -DUSE_DOUBLE=OFF && make
./sine_sum

# Мой результат:
Тип: float
Количество элементов: 10000000
Сумма: 0.291951

# Сборка с double
rm -rf build
mkdir build && cd build
cmake .. -DUSE_DOUBLE=ON && make
./sine_sum

# Мой результат:
Тип: double
Количество элементов: 10000000
Сумма: 4.89582e-11
```

## Пример мейка
```bash
all: sine_sum

sine_sum: CMakeFiles/sine_sum.dir/main.cpp.o
	g++ -O3 CMakeFiles/sine_sum.dir/main.cpp.o -o sine_sum

CMakeFiles/sine_sum.dir/main.cpp.o: ../main.cpp
	g++ -O3 -std=gnu++14 \
	   -c ../main.cpp \
	   -o CMakeFiles/sine_sum.dir/main.cpp.o
```
