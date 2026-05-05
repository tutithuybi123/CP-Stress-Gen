# Mẫu phản ví dụ an toàn

Namespace `cp_stress_gen::anti` chứa các hàm tạo mẫu dữ liệu đơn giản. Tên hàm mô tả hình dạng dữ liệu, không bảo đảm làm hỏng một thuật toán cụ thể.

## Mẫu cho sắp xếp

- `SortKiller::reversed(n, first)`
- `SortKiller::nearly_sorted(n, swaps, rng)`
- `SortKiller::many_duplicates(n, distinct, rng)`

## Mẫu cho cây

- `TreeKiller::chain(n)`
- `TreeKiller::star(n)`
- `TreeKiller::chain_with_branches(n, branches, rng)`

## Mẫu cho đồ thị

- `GraphKiller::dense(n, rng)`
- `GraphKiller::long_dag_chain(n)`
- `GraphKiller::sparse_connected(n, rng)`

## Mẫu cho chuỗi

- `StringKiller::repeated(n, ch)`
- `StringKiller::alternating(n, a, b)`
- `StringKiller::prefix_heavy(count, length)`

Các trình sinh va chạm hash, mẫu riêng cho Dijkstra, và các mẫu đối kháng rủi ro cao chưa được cài đặt.
