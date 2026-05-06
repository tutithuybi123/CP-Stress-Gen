# Cài đặt

CP-Stress-Gen là thư viện header-only. Bạn không cần package manager, CMake hay thư
viện phụ thuộc bên ngoài. Chỉ cần tải project rồi biên dịch trình sinh dữ liệu với
đường dẫn include đúng, hoặc dùng single-header trong thư mục `dist/`.

## Yêu cầu

- Trình biên dịch hỗ trợ C++17.
- `g++` nếu muốn dùng trực tiếp các lệnh bên dưới.
- PowerShell nếu muốn chạy script có sẵn trên Windows.

## Tải project

Clone bằng Git:

```powershell
git clone https://github.com/tutithuybi123/CP-Stress-Gen.git
cd CP-Stress-Gen
```

Hoặc tải file ZIP từ GitHub:

1. Mở `https://github.com/tutithuybi123/CP-Stress-Gen`.
2. Chọn `Code` -> `Download ZIP`.
3. Giải nén file ZIP.
4. Mở PowerShell trong thư mục `CP-Stress-Gen` vừa giải nén.

## Kiểm tra cấu trúc

Các header gốc nằm trong:

```text
include/cp_stress_gen.hpp
include/core/
include/modules/
include/anti/
```

Single-header đã sinh sẵn nằm tại:

```text
dist/cp_stress_gen.hpp
```

## Cách 1: dùng thư mục include

Dùng cách này khi bạn giữ nguyên cả repository:

```powershell
New-Item -ItemType Directory -Force .tmp_build
g++ -std=c++17 -Wall -Wextra -pedantic `
    -Iinclude examples/easy_array_sum.cpp `
    -o .tmp_build/easy_array_sum.exe
.\.tmp_build\easy_array_sum.exe
```

Nếu dùng từ một project đặt cạnh `CP-Stress-Gen`:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic `
    -I..\CP-Stress-Gen\include generator.cpp `
    -o generator.exe
```

## Cách 2: dùng single-header

Dùng cách này khi muốn copy một file vào template cá nhân:

```cpp
#include "cp_stress_gen.hpp"
```

Biên dịch với thư mục `dist/`:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic `
    -Idist examples/single_header_basic.cpp `
    -o .tmp_build/single_header_basic.exe
```

Bạn có thể copy `dist/cp_stress_gen.hpp` vào thư mục template cá nhân rồi thêm thư mục
đó vào đường dẫn include khi biên dịch.

## Cập nhật single-header

Thư mục `include/` là nguồn chính. Sau khi sửa public header, sinh lại
`dist/cp_stress_gen.hpp` bằng lệnh:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\bundle_single_header.ps1
```

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

Biên dịch với `-Iinclude`, `-Idist`, hoặc đường dẫn đến thư mục chứa header bạn đã copy.

## Xử lý lỗi thường gặp

- `cp_stress_gen.hpp: No such file or directory`: đường dẫn sau `-I` đang sai.
- Lỗi cú pháp C++ hoặc `if constexpr`: hãy thêm `-std=c++17`.
- PowerShell chặn script: chạy với `-ExecutionPolicy Bypass` như ví dụ ở trên.
- `.tmp_build/`, `.exe`, `.inp` và `.out` là output cục bộ, không nên commit.
