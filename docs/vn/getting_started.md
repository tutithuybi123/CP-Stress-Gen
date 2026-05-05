# Bắt đầu

Thư viện là header-only. Chỉ cần thêm thư mục `include/` vào include path của trình biên dịch và include header tổng.

```cpp
#include "cp_stress_gen.hpp"
```

## Trình sinh dữ liệu đầu tiên

```cpp
#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(123);
    auto a = cp_stress_gen::Array(5).range(1, 10).build(rng);

    cp_stress_gen::core::Printer out(std::cout);
    out.line(a.size());
    out.vector(a);
}
```

Biên dịch từ thư mục gốc của project:

```powershell
New-Item -ItemType Directory -Force .tmp_build
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/easy_array_sum.cpp -o .tmp_build/easy_array_sum.exe
```

## Tái lập kết quả

Dùng cùng một seed cho `core::Random` để nhận lại cùng một kết quả ngẫu nhiên.

```cpp
cp_stress_gen::core::Random rng(2026);
auto edges = cp_stress_gen::Graph(6).edges(7).sparse_connected().build(rng);
```

Chỉ nên dùng phiên bản `build()` không tham số cho các ví dụ nhanh khi không cần tái lập kết quả.

## Cấu hình không hợp lệ

Khoảng giá trị sai, số cạnh không thể tạo, alphabet rỗng, hoặc yêu cầu điểm duy nhất vượt quá sức chứa đều ném `std::invalid_argument`.
