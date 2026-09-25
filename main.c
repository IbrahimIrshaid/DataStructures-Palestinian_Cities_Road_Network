#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Ibrahim Irshaid 1231870 sec 1

#define MAX_EDGES 200
#define MAX_VERTICES 401
#define MAX_NAME_LEN 50
#define INF 1000000000

// Edge structure
typedef struct {
    int src, dest, weight;
} Edge;

// Graph structure
typedef struct {
    int V, E;
    Edge edges[MAX_EDGES];
} Graph;

// Min Heap for Prim and Kruskal
typedef struct {
    Edge data[MAX_EDGES];
    int size;
} MinHeap;

void heapifyDown(MinHeap* heap, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size && heap->data[left].weight < heap->data[smallest].weight)
        smallest = left;
    if (right < heap->size && heap->data[right].weight < heap->data[smallest].weight)
        smallest = right;

    if (smallest != i) {
        Edge temp = heap->data[i];
        heap->data[i] = heap->data[smallest];
        heap->data[smallest] = temp;
        heapifyDown(heap, smallest);
    }
}

void heapifyUp(MinHeap* heap, int i) {
    while (i && heap->data[i].weight < heap->data[(i - 1) / 2].weight) {
        Edge temp = heap->data[i];
        heap->data[i] = heap->data[(i - 1) / 2];
        heap->data[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

void insertHeap(MinHeap* heap, Edge edge) {
    heap->data[heap->size++] = edge;
    heapifyUp(heap, heap->size - 1);
}

Edge extractMin(MinHeap* heap) {
    Edge root = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    heapifyDown(heap, 0);
    return root;
}

//Hash Map for city names
char cityNames[MAX_VERTICES][MAX_NAME_LEN];
int cityCount = 0;

int getCityIndex(char* name) {
    for (int i = 0; i < cityCount; i++) {
        if (strcmp(cityNames[i], name) == 0)
            return i;
    }
    strcpy(cityNames[cityCount], name);
    return cityCount++;
}

Graph graph;

//Disjoint Set for Kruskal
int parent[MAX_VERTICES];
int find(int i) {
    if (parent[i] != i)
        parent[i] = find(parent[i]);
    return parent[i];
}
void unionSets(int u, int v) {
    int pu = find(u);
    int pv = find(v);
    parent[pu] = pv;
}
// loading the graph from the cities file
void loadGraph() {
    FILE* f = fopen("cities.txt", "r");
    if (!f) {
        printf("Error Opening File\n");
        return;
    }
    graph.E = 0;
    char line[200];
    while (fgets(line, sizeof(line), f)) {
        char* token = strtok(line, "#\n");
        char city1[MAX_NAME_LEN];
        strcpy(city1, token);

        token = strtok(NULL, "#\n");
        char city2[MAX_NAME_LEN];
        strcpy(city2, token);

        token = strtok(NULL, "#\n");
        int dist = atoi(token);

        int u = getCityIndex(city1);
        int v = getCityIndex(city2);
        graph.edges[graph.E++] = (Edge){u, v, dist};
    }
    graph.V = cityCount;
    fclose(f);
    printf("Graph loaded: Order = %d, Size = %d\n", graph.V, graph.E);
}

void primMST(int src) {
    int key[MAX_VERTICES];
    int parent[MAX_VERTICES];
    int inMST[MAX_VERTICES] = {0};
    MinHeap heap = {.size = 0};

    for (int i = 0; i < graph.V; i++) {
        key[i] = INF;
        parent[i] = -1;
    }

    key[src] = 0;
    insertHeap(&heap, (Edge){-1, src, 0});

    int totalCost = 0;
    while (heap.size > 0) {
        Edge minEdge = extractMin(&heap);
        int u = minEdge.dest;
        if (inMST[u]) continue;

        inMST[u] = 1;
        totalCost += minEdge.weight;
        if (minEdge.src != -1)
            printf("%s -> %s : %d\n", cityNames[minEdge.src], cityNames[minEdge.dest], minEdge.weight);

        for (int i = 0; i < graph.E; i++) {
            Edge e = graph.edges[i];
            int v = (e.src == u) ? e.dest : ((e.dest == u) ? e.src : -1);
            if (v != -1 && !inMST[v] && e.weight < key[v]) {
                key[v] = e.weight;
                parent[v] = u;
                insertHeap(&heap, (Edge){u, v, e.weight});
            }
        }
    }
    printf("Total Cost: %d\n", totalCost);
}

void kruskalMST() {
    MinHeap heap = {.size = 0};
    for (int i = 0; i < graph.E; i++) {
        insertHeap(&heap, graph.edges[i]);
    }

    for (int i = 0; i < graph.V; i++)
        parent[i] = i;

    int totalCost = 0;
    int edgeCount = 0;

    printf("Kruskal's MST edges:\n");
    while (heap.size > 0 && edgeCount < graph.V - 1) {
        Edge e = extractMin(&heap);
        int u = e.src, v = e.dest;
        if (find(u) != find(v)) {
            printf("%s -> %s : %d\n", cityNames[u], cityNames[v], e.weight);
            totalCost += e.weight;
            unionSets(u, v);
            edgeCount++;
        }
    }
    printf("Total Cost: %d\n", totalCost);
}

void compare() {
    clock_t t1, t2;

    t1 = clock();
    primMST(0);
    t2 = clock();
    double primTime = ((double)(t2 - t1)) / CLOCKS_PER_SEC;

    t1 = clock();
    kruskalMST();
    t2 = clock();
    double kruskalTime = ((double)(t2 - t1)) / CLOCKS_PER_SEC;

    printf("\nComparison:\n");
    printf("Prim's Time: %f seconds\n", primTime);
    printf("Kruskal's Time: %f seconds\n", kruskalTime);
}

void displayMainMenu()
{

    printf("\nPlease Select an Operation <1-5>:\n");
    printf("1. Load Cities File\n2. Apply Prim's Algorithm\n3. Apply Kruskal's Algorithm\n4. Compare the two algorithms\n");
    printf("5. Exit\n\n");


}

int main() {
    int op = 0;
    while (op != 5) {

        displayMainMenu();
        scanf("%d", &op);

        switch (op) {
            case 1:
                loadGraph();
                break;
            case 2: {
                char city[MAX_NAME_LEN];
                printf("Enter source city: ");
                scanf("%s", city);
                int index = getCityIndex(city);
                clock_t start = clock();
                primMST(index);
                clock_t end = clock();
                printf("Time: %.9f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
                break;
            }
            case 3: {
                clock_t start = clock();
                kruskalMST();
                clock_t end = clock();
                printf("Time: %.9f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
                break;
            }
            case 4:
                compare();
                break;
            case 5: printf("\nExiting... GoodBye!");
                break;
            default:
                printf("No such Operation!\tPLease try again.\n\n");
        }
    }
    return 0;
}

