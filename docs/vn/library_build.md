# Cách dùng và biên dịch thư viện

`CP-Stress-Gen` không cần hệ thống biên dịch riêng. Toàn bộ thư viện nằm trong các header ở thư mục `include/`.

Nếu cần các bước tải project và cài đặt ban đầu, xem [hướng dẫn cài đặt](installation.md).

## Biên dịch trực tiếp

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/graph_basic.cpp -o .tmp_build/graph_basic.exe
```

Cờ quan trọng là:

```text
-Iinclude
```

Cờ này cho phép dùng:

```cpp
#include "cp_stress_gen.hpp"
```

## Sao chép sang project khác

Sao chép thư mục `include/` vào project của bạn và truyền đường dẫn include đó cho trình biên dịch.

```powershell
g++ -std=c++17 -Ipath\to\CP-Stress-Gen\include generator.cpp -o generator.exe
```

## Trạng thái đóng gói

Hiện chưa có CMake target, công thức cho trình quản lý gói, hoặc gói cài đặt hệ thống. Cách dùng ổn định hiện tại là truyền đường dẫn include trực tiếp.
