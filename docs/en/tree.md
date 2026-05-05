# Tree

`cp_stress_gen::Tree(n)` generates `n - 1` edges for a valid tree when `n > 0`.

```cpp
cp_stress_gen::core::Random rng(4);
auto edges = cp_stress_gen::Tree(8).random().build(rng);
```

## Shapes

- `bamboo()`: a chain.
- `star()`: one center connected to all other vertices.
- `star(center)`: star with an explicit center label.
- `random()`: each new vertex attaches to a previous vertex.
- `binary()`: heap-like binary tree.
- `caterpillar(spine)`: a path spine with leaves attached to it.
- `deep_recursion(branches)`: long chain with extra branches.
- `shuffle()`: shuffle edge order.

## Labels And Weights

- `one_based()` uses labels `1..n` (default).
- `zero_based()` uses labels `0..n-1`.
- `first_node(x)` sets the first label.
- `weight(value)` assigns one fixed edge weight.
- `weighted(left, right)` samples weights in `[left, right]`.

Invalid centers, invalid spine sizes, invalid branch counts, and invalid weight ranges throw.
