# Tổng quan

`CP-Stress-Gen` là thư viện C++17 dạng header-only để sinh dữ liệu kiểm thử cho lập trình thi đấu. Thư viện tập trung vào API dễ đọc, khả năng tái lập kết quả khi dùng seed, và báo lỗi rõ ràng khi gặp cấu hình không hợp lệ.

## Thư viện hiện có

| Nhóm | API chính |
| --- | --- |
| Ngẫu nhiên | `cp_stress_gen::core::Random` |
| In dữ liệu | `cp_stress_gen::core::Printer` |
| Mảng | `cp_stress_gen::Array` |
| Chuỗi | `cp_stress_gen::String` |
| Hoán vị | `cp_stress_gen::Permutation` |
| Cây | `cp_stress_gen::Tree` |
| Đồ thị | `cp_stress_gen::Graph` |
| Toán học | `cp_stress_gen::Math` |
| Hình học | `cp_stress_gen::Geometry`, `cp_stress_gen::Point<T>` |
| Mẫu phản ví dụ an toàn | `cp_stress_gen::anti::*Killer` |

## Nguyên tắc thiết kế

- Dùng trực tiếp bằng `#include "cp_stress_gen.hpp"`.
- Nền tảng C++17, không có thư viện phụ thuộc bên ngoài.
- Dùng `build(rng)` để tái lập kết quả khi cần.
- Cung cấp các dạng dữ liệu thường dùng trong stress test cục bộ.
- Ném ngoại lệ khi cấu hình không hợp lệ thay vì tự ý đổi ý định của người dùng.

## Giới hạn hiện tại

`CP-Stress-Gen` chưa phải là thư viện kiểm định hoặc bộ tương tác đầy đủ. Hiện chưa có tích hợp CMake hay trình quản lý gói, chưa có giao diện GUI, chưa bảo đảm sinh đa giác lồi, chưa tối ưu sinh số nguyên tố trong khoảng rất lớn, và chưa có các mẫu nâng cao như va chạm hash hoặc mẫu riêng cho Dijkstra.

## Nên đọc tiếp

- [Bắt đầu](getting_started.md)
- [Ví dụ](examples.md)
- [Đồ thị](graph.md)
- [Cây](tree.md)
