# Printers

`cp_stress_gen::core::Printer` writes common generated structures to an `std::ostream`.

```cpp
cp_stress_gen::core::Printer out(std::cout);
out.line(3, 4, 5);
out.vector(std::vector<int>{1, 2, 3});
```

## Methods

- `line(args...)`: print arguments separated by spaces, then a newline.
- `space(args...)`: print arguments separated by spaces without forcing a final newline.
- `vector(container)`: print a one-line space-separated container.
- `edges(container)`: print edge structs with fields `u`, `v`, `w`, and `weighted`.

## Edge Convention

Tree and graph edges use the same printable shape:

```cpp
int u;
int v;
long long w;
bool weighted;
```

If `weighted` is `false`, `Printer::edges` prints `u v`. If `weighted` is `true`, it prints `u v w`.
