# Cài đặt

`CP-Stress-Gen` là thư viện header-only. Cài đặt thư viện nghĩa là tải project về và biên dịch trình sinh dữ liệu của bạn với đúng đường dẫn include.

## Yêu cầu

- Trình biên dịch hỗ trợ C++17.
- `g++` nếu bạn muốn dùng trực tiếp các lệnh bên dưới.
- Không cần thư viện phụ thuộc bên ngoài.

## Tải project

Cách 1: clone bằng Git.

```powershell
git clone https://github.com/tutithuybi123/CP-Stress-Gen.git
cd CP-Stress-Gen
```

Cách 2: tải file ZIP từ GitHub.

1. Mở `https://github.com/tutithuybi123/CP-Stress-Gen`.
2. Chọn `Code` -> `Download ZIP`.
3. Giải nén file ZIP.
4. Mở PowerShell trong thư mục `CP-Stress-Gen` vừa giải nén.

## Kiểm tra cấu trúc project

Project cần có các đường dẫn sau:

```text
include/cp_stress_gen.hpp
include/core/
include/modules/
include/anti/
```

Header tổng là:

```cpp
#include "cp_stress_gen.hpp"
```

## Cách 1: biên dịch ngay trong project

Từ thư mục `CP-Stress-Gen`:

```powershell
New-Item -ItemType Directory -Force .tmp_build
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/easy_array_sum.cpp -o .tmp_build/easy_array_sum.exe
.\.tmp_build\easy_array_sum.exe
```

`-Iinclude` cho trình biên dịch biết nơi chứa `cp_stress_gen.hpp`.

## Cách 2: dùng từ project khác

Đặt `CP-Stress-Gen` cạnh project của bạn:

```text
workspace/
  CP-Stress-Gen/
  MyGenerator/
    generator.cpp
```

Biên dịch từ thư mục `MyGenerator/`:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -I..\CP-Stress-Gen\include generator.cpp -o generator.exe
.\generator.exe
```

Bạn cũng có thể sao chép riêng thư mục `include/` vào project của mình và biên dịch với đường dẫn include đã sao chép.

## Trình sinh dữ liệu tối thiểu

Tạo file `generator.cpp`:

```cpp
#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(123);
    cp_stress_gen::core::Printer out(std::cout);

    auto values = cp_stress_gen::Array(8).range(1, 100).build(rng);

    out.line(values.size());
    out.vector(values);
    return 0;
}
```

Biên dịch từ một thư mục truy cập được `CP-Stress-Gen/include`:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -I..\CP-Stress-Gen\include generator.cpp -o generator.exe
.\generator.exe
```

## Xử lý lỗi thường gặp

- `cp_stress_gen.hpp: No such file or directory`: đường dẫn include đang sai. Hãy kiểm tra phần đường dẫn sau `-I`.
- Lỗi cú pháp C++ hoặc lỗi liên quan `if constexpr`: hãy biên dịch với `-std=c++17` hoặc mới hơn.
- Lỗi đường dẫn trên Windows: nên dùng đường dẫn tương đối thân thiện với PowerShell, ví dụ `..\CP-Stress-Gen\include`.
- `.tmp_build/` chỉ là thư mục output cục bộ khi biên dịch. Không commit thư mục này.

