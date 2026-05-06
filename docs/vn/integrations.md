# Tích hợp quy trình sinh dữ liệu

CP-Stress-Gen không phụ thuộc Polygon, Themis hay trình chấm cục bộ nào. Các ví dụ
trong trang này là mẫu trình sinh dữ liệu portable, biên dịch được bằng `g++` thông
thường.

## Trình sinh kiểu Polygon

Trình sinh dữ liệu kiểu Polygon thường in một test ra standard output. Dùng:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/polygon_generator.cpp -o .tmp_build/polygon_generator.exe
.\.tmp_build\polygon_generator.exe --n 20 --m 30 --seed 1 --mode graph
```

Ví dụ hỗ trợ `--mode graph`, `--mode dag` và `--mode bipartite`.

## Trình sinh kiểu Themis

Quy trình Themis thường dùng các file `.inp`. Ví dụ có thể in ra stdout hoặc ghi file:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/themis_generator.cpp -o .tmp_build/themis_generator.exe
.\.tmp_build\themis_generator.exe --tests 3 --n 20 --seed 1 --out case
```

Các file `.inp` sinh ra chỉ là output cục bộ và không nên commit.

## Stress test cục bộ

`examples/local_stress_driver.cpp` đặt trình sinh dữ liệu, lời giải brute và lời giải
tối ưu trong cùng một file. Ví dụ này không chạy chương trình bên ngoài:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/local_stress_driver.cpp -o .tmp_build/local_stress_driver.exe
.\.tmp_build\local_stress_driver.exe --tests 200 --n 30 --seed 1
```

## Dùng single-header

Nếu muốn đưa thư viện vào template cá nhân, có thể dùng thư mục `dist/`:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Idist examples/single_header_basic.cpp -o .tmp_build/single_header_basic.exe
```
