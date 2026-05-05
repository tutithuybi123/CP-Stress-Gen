# Graph

`cp_stress_gen::Graph(n)` generates graph edge lists. Use `.edges(m)` when you need a specific edge count.

```cpp
cp_stress_gen::core::Random rng(5);
auto edges = cp_stress_gen::Graph(8)
    .edges(12)
    .directed()
    .dag()
    .no_multi_edges()
    .build(rng);
```

## Common Options

- `directed()` and `undirected()`.
- `one_based()`, `zero_based()`, and `first_node(x)`.
- `edges(m)` to request an edge count.
- `no_multi_edges()` or `allow_multi_edges()`.
- `weight(value)` or `weighted(left, right)`.
- `shuffle()` to shuffle edge order.

## Modes

- `dag()`: directed acyclic graph with edges from lower labels to higher labels.
- `layered_dag(layers)`: directed edges from earlier layers to later layers.
- `dense()`: many unique edges.
- `sparse_connected()`: undirected connected graph.
- `complete()`: complete graph.
- `cycle()`: cycle graph.
- `path()`: path graph.
- `forest(components)`: undirected forest with fixed component count.
- `connected_components(k)`: undirected graph with internally connected components.
- `bipartite(left_size)`: bipartite graph with a fixed left side.

Impossible edge counts, invalid layer counts, invalid partitions, and incompatible modes throw.
