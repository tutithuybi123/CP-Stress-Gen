# Ví dụ

Mọi ví dụ đều biên dịch trực tiếp bằng C++17:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/easy_array_sum.cpp -o .tmp_build/easy_array_sum.exe
```

## Dễ

- `easy_array_sum.cpp`: một test case mảng.
- `easy_string_palindrome.cpp`: chuỗi đối xứng và chuỗi tuần hoàn.
- `easy_tree_shapes.cpp`: cây dạng chuỗi, cây sao, và cây ngẫu nhiên.

## Trung bình

- `medium_graph_cases.cpp`: đường đi, đồ thị liên thông, DAG, và đồ thị hai phía.
- `medium_geometry_points.cpp`: điểm trong hình chữ nhật, điểm duy nhất, điểm biên, và điểm cụm.
- `medium_batch_tests.cpp`: nhiều test case trộn mảng và chuỗi trong cùng một output.

## Khó

- `hard_stress_compare.cpp`: khung so sánh brute và optimized ngay trong một file.
- `hard_graph_suite.cpp`: thành phần liên thông, rừng, layered DAG, và đồ thị dày.
- `hard_anti_patterns.cpp`: các mẫu an toàn cho sắp xếp, cây, đồ thị, và chuỗi.

Các ví dụ cũ như `array_basic.cpp`, `graph_basic.cpp`, và `printer_basic.cpp` vẫn hữu ích khi cần kiểm tra nhanh từng module.
