# Examples

All examples compile directly with C++17:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/easy_array_sum.cpp -o .tmp_build/easy_array_sum.exe
```

## Easy

- `easy_array_sum.cpp`: one array test case.
- `easy_string_palindrome.cpp`: palindrome and periodic strings.
- `easy_tree_shapes.cpp`: bamboo, star, and random tree shapes.

## Medium

- `medium_graph_cases.cpp`: path, connected graph, DAG, and bipartite graph.
- `medium_geometry_points.cpp`: rectangle, unique, boundary, and clustered points.
- `medium_batch_tests.cpp`: mixed test cases in one output stream.

## Hard

- `hard_stress_compare.cpp`: in-process brute vs optimized comparison skeleton.
- `hard_graph_suite.cpp`: components, forest, layered DAG, and dense graph.
- `hard_anti_patterns.cpp`: safe sort, tree, graph, and string patterns.

Existing focused examples such as `array_basic.cpp`, `graph_basic.cpp`, and `printer_basic.cpp` remain useful for quick module checks.
