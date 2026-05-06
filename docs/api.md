# CP-Stress-Gen API Reference

This page summarizes the public C++17 API exposed by `include/cp_stress_gen.hpp`. The
library is header-only and uses `std::invalid_argument` for invalid generator
configurations. Copy-only workflows may use the generated `dist/cp_stress_gen.hpp`
bundle, but `include/` remains the source of truth.

The API includes both preset generators for common shapes and composable builders for
custom tree/graph structures. Algorithm-targeted anti-pattern helpers are documented as
stress candidates, not guaranteed exploit generators.

## Core

### `core::Random`

Purpose: fast deterministic random generation based on Xoshiro256**.

Common API:

- `Random(seed)`
- `next_u64()`, `next_u32()`
- `integer<T>(left, right)`
- `real<T>(left, right)`
- `boolean()`, `boolean(probability_true)`

Example:

```cpp
cp_stress_gen::core::Random rng(42);
int x = rng.integer<int>(1, 100);
```

Validation: invalid integer or real ranges throw.

Limitations: this is not a cryptographic random generator.

### `core::Validate`

Purpose: shared validation helpers for generators.

Common API:

- `require(condition, message)`
- `require_range(left, right, message)`
- `require_positive(value, message)`

Example:

```cpp
cp_stress_gen::core::require(n > 0, "n must be positive");
```

Validation: failed checks throw `std::invalid_argument`.

Limitations: helpers intentionally stay small and do not define custom exception types.

### `core::Printer`

Purpose: print generated data to an `std::ostream`.

Common API:

- `line(args...)`
- `space(args...)`
- `indexed(shift)`
- `separator(value)`
- `vector(container)`
- `matrix(rows)`, `grid(rows)`
- `edges(container)`
- `edges(n, edges)`, `edges(n, m, edges)`
- `with_n(container)`, `with_n_m(n, m, edges)`
- `testcases(count, callable)`

Example:

```cpp
cp_stress_gen::core::Printer out(std::cout);
out.vector(std::vector<int>{1, 2, 3});
out.indexed(1).edges(edges);
```

Validation: no runtime validation beyond stream behavior.

Limitations: `edges()` expects edge objects with `u`, `v`, `w`, and `weighted` fields.
`testcases()` intentionally supports one simple callable form: `callable(case_index,
printer)`.

## Modules

### `Array`

Purpose: generate integer arrays.

Common API:

- `range(left, right)`
- `fill(value)`
- `iota(start, step)`
- `shuffle()`
- `almost_sorted(swaps)`
- `many_equal(distinct_values)`
- `blocky(block_size)`

Example:

```cpp
auto a = cp_stress_gen::Array(10).range(1, 100).build(rng);
```

Validation: invalid ranges, zero distinct counts, zero block sizes, and excessive swap
counts throw.

Limitations: values are `long long`; `almost_sorted` starts from `1..n`.

### `String`

Purpose: generate strings from alphabets and simple patterns.

Common API:

- `alphabet(chars)`
- `lowercase()`, `uppercase()`, `digits()`, `binary()`
- `range(left, right)`
- `palindrome()`
- `almost_palindrome(changes)`
- `periodic(pattern)`
- `prefix_heavy(prefix_char, tail_char)`
- `border_heavy()`
- `runs(run_length)`
- `periodic_with_noise(pattern, changes)`
- `random_palindromic_blocks(block_size)`
- `String::de_bruijn_binary(k)`
- `String::thue_morse`, `thue_morse_pair`, `fibonacci_word`, `de_bruijn`
- `String::border_chain`, `periodic_blocks`, `runs`, `kmp_worst_prefix`, `anti_z`
- `StringHash::hash_string`, `same_hash`, `collision_like_pair`
- `StringHash::hash_u64`, `same_hash_u64`, `thue_morse_pair_power2`

Example:

```cpp
auto s = cp_stress_gen::String(12).periodic("abc").build(rng);
auto h = cp_stress_gen::StringHash::hash_string(s, 911382323, 1000000007);
auto pair = cp_stress_gen::StringHash::thue_morse_pair_power2(10);
```

Validation: empty alphabets, invalid ranges, empty periodic/noise patterns, excessive
mutation counts, invalid run/block sizes, and invalid hash parameters throw.

Limitations: string generation is byte-oriented and does not handle Unicode grapheme
logic. `thue_morse_pair_power2()` is scoped to unsigned 64-bit overflow polynomial
hashes with odd bases; it is not a universal hash collision generator.

### `Permutation`

Purpose: generate permutations of consecutive integer labels.

Common API:

- `one_based()`, `zero_based()`
- `shuffle()`
- `reversed()`
- `almost_sorted(swaps)`
- `cyclic_shift(k)`
- `few_swaps(count)`
- `blocks(block_size)`

Example:

```cpp
auto p = cp_stress_gen::Permutation(8).zero_based().shuffle().build(rng);
```

Validation: size must fit in `int`; excessive swap counts and zero block size throw.

Limitations: values are consecutive `int` labels only.

### `Tree`

Purpose: generate valid tree edge lists.

Common API:

- `one_based()`, `zero_based()`, `first_node(first)`
- `weight(value)`, `weighted(left, right)`
- `bamboo()`, `star()`, `star(center)`, `random()`
- `degree_limit(limit)` for random trees
- `binary()`, `caterpillar(spine)`, `deep_recursion(branches)`
- `broom(chain_len, leaves)`, `double_star(left_leaves, right_leaves)`, `k_ary(k)`
- `shuffle()`
- `Tree::from_edges(n, edges, first)`
- `TreeBuilder::bamboo(n)`, `star(n)`, `broom(chain_len, leaves)`, `double_star(left, right)`, `k_ary(n, k)`
- `TreeBuilder::attach(...)`, `merge(...)`, `renumber(...)`, `shuffle_vertices(rng)`, `shuffle_edges(rng)`, `build()`

Example:

```cpp
auto edges = cp_stress_gen::Tree(10).weighted(1, 100).random().build(rng);
auto composed = cp_stress_gen::TreeBuilder::bamboo(5)
    .attach(cp_stress_gen::TreeBuilder::star(3), 5, 1)
    .build();
```

Validation: invalid labels, centers, spine sizes, branch counts, degree limits,
non-random degree-limit use, duplicate builder edges, disconnected explicit trees,
invalid attach nodes, and weight ranges throw.

Limitations: degree limits currently apply only to `random()` trees; `TreeBuilder`
bridge edges are unweighted while existing component edge weights are preserved.

### `Graph`

Purpose: generate graph edge lists.

Common API:

- `edges(m)`
- `directed()`, `undirected()`
- `one_based()`, `zero_based()`, `first_node(first)`
- `no_multi_edges()`, `allow_multi_edges()`
- `weight(value)`, `weighted(left, right)`
- `dag()`, `layered_dag(layers)`, `dense()`
- `sparse_connected()`, `complete()`, `cycle()`, `path()`
- `forest(components)`, `connected_components(k)`, `bipartite(left_size)`
- `wheel()`, `grid(rows, cols)`, `complete_bipartite(left, right)`, `tournament()`
- `shuffle()`
- `Graph::from_edges(n, edges, directed, first)`
- `GraphBuilder::add_edge`, `add_path`, `add_cycle`, `add_clique`, `add_bipartite`
- `GraphBuilder::merge`, `renumber`, `shuffle_vertices`, `shuffle_edges`, `remove_duplicate_edges`, `complement`, `build`

Example:

```cpp
auto edges = cp_stress_gen::Graph(8).edges(12).dag().no_multi_edges().build(rng);
auto custom = cp_stress_gen::GraphBuilder(5)
    .add_path({1, 2, 3})
    .add_clique({3, 4, 5})
    .remove_duplicate_edges()
    .build();
```

Validation: impossible edge counts, invalid partitions, invalid layer counts, invalid
structure sizes, invalid builder labels, duplicate builder edges, self-loops, and
incompatible directed modes throw.

Limitations: advanced graph families and weighted shortest-path adversarial cases are
intentionally deferred. `GraphBuilder::complement()` supports simple unweighted graphs
only.

### `Math`

Purpose: small math helpers useful in generators.

Common API:

- `gcd(a, b)`, `lcm(a, b)`
- `divisors(n)`
- `primes_up_to(n)`, `is_prime(n)`
- `sieve_spf(n)`, `factorize_spf(value, spf)`
- `euler_phi(n)`, `mobius_up_to(n)`
- `divisor_count_from_factorization(factors)`, `divisor_sum_from_factorization(factors)`
- `random_prime(left, right, rng)`
- `random_prime_sieve(left, right, rng)`
- `random_composite(left, right, rng)`
- `coprime_pair(left, right, rng)`
- `random_coprime_pair(left, right, rng)`
- `random_number_with_many_divisors(limit, rng)`
- `with_gcd(g, multiplier_left, multiplier_right, rng)`
- `mod_add(a, b, mod)`, `mod_mul(a, b, mod)`
- `mod_pow(base, exponent, mod)`
- `extended_gcd(a, b)`
- `mod_inverse(value, mod)`
- `crt_pair(a1, m1, a2, m2)`
- `factorize_trial(value)`
- `binom_small(n, k)`, `factorials_mod(n, mod)`

Example:

```cpp
auto pair = cp_stress_gen::Math::with_gcd(6, 1, 20, rng);
auto crt = cp_stress_gen::Math::crt_pair(2, 3, 3, 5);
```

Validation: invalid ranges, impossible prime/composite/coprime requests, invalid modular
parameters, invalid SPF lookups, invalid factorization metadata, incompatible helper
inputs, and missing modular inverses throw.

Limitations: primality and factor-related helpers use simple algorithms and do not
target huge ranges. CRT and divisor helpers use `long long` arithmetic and do not claim
overflow-proof arbitrary-precision behavior.

### `Geometry`

Purpose: integer point generation and simple shapes.

Common API:

- `Point<T>`
- `Geometry::points(count).rectangle(...).unique()`
- `Geometry::collinear(count).from(...).step(...)`
- `Geometry::rectangle_boundary_points(count).rectangle(...)`
- `Geometry::clustered_points(count).center(...).radius(...)`
- `Geometry::duplicate_points(count).point(...)`
- `Geometry::rectangle_polygon(x1, y1, x2, y2)`
- `Geometry::triangle(a, b, c)`
- `Geometry::cross`, `orientation`, `polygon_area2`, `polygon_area`, `is_convex`
- `Geometry::dot`, `dist2`, `manhattan`
- `Geometry::on_segment`, `segments_intersect`
- `Geometry::bounding_box`, `is_simple_polygon`, `convex_hull`
- `Geometry::circle_points`, `grid_points`, `convex_hull_of_random_points`
- `Geometry::regular_polygon(n, radius)`, `convex_polygon_candidate(n, radius)`

Example:

```cpp
auto points = cp_stress_gen::Geometry::points(9).rectangle(1, 1, 3, 3).unique().build(rng);
```

Validation: invalid rectangles, impossible unique requests, invalid collinear steps,
negative cluster radii, degenerate simple polygons, too-small polygons, empty bounding
boxes, invalid grid sizes, and invalid regular/circle radii throw.

Limitations: arbitrary integer convex polygon generation is not implemented.
`convex_polygon_candidate()` returns a regular/on-circle candidate.

## Anti Pattern Modules

Purpose: provide honest, safe pattern helpers for local stress testing.

Common API:

- `anti::SortKiller::reversed`, `nearly_sorted`, `many_duplicates`
- `anti::SortKiller::organ_pipe`, `alternating_high_low`
- `anti::TreeKiller::chain`, `star`, `chain_with_branches`, `broom`
- `anti::GraphKiller::dense`, `long_dag_chain`, `sparse_connected`, `dense_dag`
- `anti::StringKiller::repeated`, `alternating`, `prefix_heavy`, `kmp_prefix_pattern`
- `anti::DfsBfsKiller::deep_chain`, `many_components`, `huge_branching`, `broom_tree`, `grid_maze_like`
- `anti::DijkstraKiller::many_equal_distances`, `zero_weight_edges`,
  `dense_weighted_trap`, `layered_equal_shortest_paths`
- `anti::DsuKiller::redundant_edges`, `delayed_connectivity`, `reverse_union_sequence`, `many_queries_same_component`
- `anti::DpKiller::all_equal_array`, `monotonic_array`, `alternating_array`, `boundary_sizes`, `knapsack_tight_capacity`
- `anti::GreedyKiller::local_optimum_trap_array`, `interval_scheduling_trap`, `coin_change_greedy_trap`
- `anti::BinarySearchKiller::off_by_one_boundaries`, `lower_upper_bound_duplicates`,
  `all_false_then_true`, `all_true_then_false`
- `anti::StringPatternKiller::kmp_worst_prefix`, `z_repeated_blocks`, `border_heavy`, `periodic_with_break`
- `anti::HashKiller::repeated_base_sensitive_shapes`, `collision_like_patterns`

Example:

```cpp
auto values = cp_stress_gen::anti::SortKiller::many_duplicates(20, 3, rng);
auto chain = cp_stress_gen::anti::DfsBfsKiller::deep_chain(100000);
```

Validation: invalid sizes or counts throw where applicable.

Limitations: these names describe input shapes. They do not guarantee that a specific
algorithm will fail. Hash utilities provide collision-like patterns only; no true hash
collision generator is implemented.
