# Cookbook trình sinh dữ liệu

Các công thức dưới đây là điểm bắt đầu cho stress test thực tế. Mỗi đoạn code giả định
đã có:

```cpp
cp_stress_gen::core::Random rng(seed);
cp_stress_gen::core::Printer out(std::cout);
```

## Mảng có nhiều giá trị trùng

```cpp
auto values = cp_stress_gen::Array(n).many_equal(3).build(rng);
out.line(n);
out.vector(values);
```

## Cây dạng chuỗi để thử độ sâu đệ quy

```cpp
auto edges = cp_stress_gen::Tree(n).bamboo().build(rng);
out.edges(n, edges);
```

## DAG phân tầng

```cpp
auto edges = cp_stress_gen::Graph(n).edges(m).layered_dag(4).build(rng);
out.edges(n, edges);
```

## Đồ thị nhiều thành phần liên thông

```cpp
auto edges = cp_stress_gen::Graph(n).edges(m).connected_components(3).build(rng);
out.edges(n, edges);
```

## Đồ thị hai phía

```cpp
auto edges = cp_stress_gen::Graph(n).edges(m).bipartite(n / 2).build(rng);
out.edges(n, edges);
```

## Mẫu xâu cho KMP và Z

```cpp
auto kmp = cp_stress_gen::String::kmp_worst_prefix(n);
auto z = cp_stress_gen::String::anti_z(n);
out.line(kmp);
out.line(z);
```

## Điểm hình học và bao lồi

```cpp
auto points = cp_stress_gen::Geometry::points(n).rectangle(-100, -100, 100, 100).unique().build(rng);
auto hull = cp_stress_gen::Geometry::convex_hull(points);
out.line(points.size(), hull.size());
```

## Trường hợp toán học với gcd và coprime

```cpp
auto pair = cp_stress_gen::Math::with_gcd(12, 1, 100, rng);
auto coprime = cp_stress_gen::Math::random_coprime_pair(1, 100, rng);
out.line(pair.first, pair.second);
out.line(coprime.first, coprime.second);
```

## Mẫu anti pattern theo nhóm thuật toán

```cpp
auto chain = cp_stress_gen::anti::DfsBfsKiller::deep_chain(n);
auto duplicates = cp_stress_gen::anti::SortKiller::many_duplicates(n, 4, rng);
out.edges(n, chain);
out.vector(duplicates);
```

## Dùng Options để tái lập kết quả

```cpp
cp_stress_gen::core::Options opt(argc, argv);
int n = opt.get<int>("n", 1000);
long long seed = opt.get<long long>("seed", 1);
```

## Printer với header cạnh

```cpp
out.edges(n, edges);
out.indexed(1).edges(n, edges);
```

## Single-header

Dùng `dist/cp_stress_gen.hpp` khi muốn copy thư viện vào template cá nhân:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Idist generator.cpp -o generator.exe
```
