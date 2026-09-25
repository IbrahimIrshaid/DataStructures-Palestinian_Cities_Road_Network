# Palestinian Cities Road Network: Prim vs. Kruskal

![C](https://img.shields.io/badge/C-00599C?logo=c&logoColor=white)
![Graphs](https://img.shields.io/badge/algorithms-MST-555)

This program finds the cheapest road network connecting **50 Palestinian cities** (a graph with 200 weighted edges) by building a **minimum spanning tree** two ways, then timing both.

- **Prim's algorithm** grows the tree from a chosen source city, using a binary **min-heap** of candidate edges.
- **Kruskal's algorithm** takes edges in increasing weight order from the heap, uses **union-find** to skip any edge that would form a cycle, and stops once it has V − 1 edges.
- **Compare** runs both back to back, printing each tree's edges and total cost along with its `clock()` time.

Both algorithms, the heap and the disjoint-set structure are implemented from scratch in [`main.c`](main.c).

## Run

```bash
gcc main.c -o roads && ./roads
```

```
1. Load Cities File          ← reads cities.txt
2. Apply Prim's Algorithm    ← asks for a source city, e.g. Jerusalem
3. Apply Kruskal's Algorithm
4. Compare the two algorithms
5. Exit
```

`cities.txt` holds one undirected road per line, in the form `CityA#CityB#distance`:

```
Tulkarem#Salfit#47
Rafat#Jiftlik#21
```

---

*Data Structures (COMP2421), Birzeit University, Spring 2025.*
