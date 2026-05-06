# Benchmark

CP-Stress-Gen có một số ví dụ benchmark nhỏ để thử trên máy cá nhân. Các ví dụ dùng
`std::chrono` và in thời gian theo mili-giây kèm checksum để chương trình thật sự sử
dụng dữ liệu đã sinh.

Đây không phải cam kết hiệu năng cho mọi môi trường. Kết quả phụ thuộc trình biên dịch,
máy chạy, cờ tối ưu và kiểu trình sinh dữ liệu bạn chọn.

## Benchmark mảng

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -Iinclude examples/benchmark_array.cpp -o .tmp_build/benchmark_array.exe
.\.tmp_build\benchmark_array.exe --n 100000 --seed 1
```

## Benchmark đồ thị

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -Iinclude examples/benchmark_graph.cpp -o .tmp_build/benchmark_graph.exe
.\.tmp_build\benchmark_graph.exe --n 1000 --m 2000 --seed 1
```

## Benchmark xâu

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -Iinclude examples/benchmark_string.cpp -o .tmp_build/benchmark_string.exe
.\.tmp_build\benchmark_string.exe --n 100000 --seed 1
```

Khi đang chỉnh ví dụ, nên dùng kích thước nhỏ. Chỉ tăng kích thước khi bạn thật sự muốn
đo một trình sinh dữ liệu cụ thể trên máy của mình.
