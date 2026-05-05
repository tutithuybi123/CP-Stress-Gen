# In dữ liệu

`cp_stress_gen::core::Printer` ghi các cấu trúc thường dùng ra `std::ostream`.

```cpp
cp_stress_gen::core::Printer out(std::cout);
out.line(3, 4, 5);
out.vector(std::vector<int>{1, 2, 3});
```

## Phương thức

- `line(args...)`: in các giá trị cách nhau bằng dấu cách, rồi xuống dòng.
- `space(args...)`: in các giá trị cách nhau bằng dấu cách, không tự ép xuống dòng cuối.
- `vector(container)`: in container trên một dòng.
- `edges(container)`: in danh sách cạnh có các trường `u`, `v`, `w`, và `weighted`.

## Quy ước cạnh

Cạnh của cây và đồ thị có cùng dạng để in dữ liệu:

```cpp
int u;
int v;
long long w;
bool weighted;
```

Nếu `weighted == false`, `Printer::edges` in `u v`. Nếu `weighted == true`, hàm sẽ in `u v w`.
