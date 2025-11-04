// Maze Solver using Exploratory Decomposition with OpenMP
// Author: Ashish Bhambure | Batch B2 | WCE Sangli

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAXN 1000
#define INF 1000000

int N, M;
int maze[MAXN][MAXN];
int visited[MAXN][MAXN];
int start_x = 0, start_y = 0;
int end_x, end_y;

typedef struct {
    int x, y;
} Node;

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

// Check valid move
int valid(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < M && maze[x][y] == 0;
}

// Sequential BFS
int sequential_bfs() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            visited[i][j] = 0;

    Node *frontier = malloc(N * M * sizeof(Node));
    int f_size = 0;
    frontier[f_size++] = (Node){start_x, start_y};
    visited[start_x][start_y] = 1;

    while (f_size > 0) {
        Node *next = malloc(N * M * sizeof(Node));
        int next_size = 0;
        for (int i = 0; i < f_size; i++) {
            Node cur = frontier[i];
            if (cur.x == end_x && cur.y == end_y) {
                free(frontier);
                free(next);
                return 1; // found
            }
            for (int d = 0; d < 4; d++) {
                int nx = cur.x + dx[d];
                int ny = cur.y + dy[d];
                if (valid(nx, ny) && !visited[nx][ny]) {
                    visited[nx][ny] = 1;
                    next[next_size++] = (Node){nx, ny};
                }
            }
        }
        free(frontier);
        frontier = next;
        f_size = next_size;
    }
    free(frontier);
    return 0; // not found
}

// Parallel BFS (Exploratory)
int parallel_bfs(int num_threads) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            visited[i][j] = 0;

    Node *frontier = malloc(N * M * sizeof(Node));
    int f_size = 0;
    frontier[f_size++] = (Node){start_x, start_y};
    visited[start_x][start_y] = 1;

    int found = 0;
    omp_set_num_threads(num_threads);

    while (f_size > 0 && !found) {
        Node *next = malloc(N * M * sizeof(Node));
        int next_size = 0;

        #pragma omp parallel
        {
            Node *local_next = malloc(N * M * sizeof(Node));
            int local_count = 0;

            #pragma omp for schedule(static)
            for (int i = 0; i < f_size; i++) {
                Node cur = frontier[i];
                if (cur.x == end_x && cur.y == end_y) {
                    found = 1;
                    continue;
                }
                for (int d = 0; d < 4; d++) {
                    int nx = cur.x + dx[d];
                    int ny = cur.y + dy[d];
                    if (valid(nx, ny) && !visited[nx][ny]) {
                        if (__sync_bool_compare_and_swap(&visited[nx][ny], 0, 1)) {
                            local_next[local_count++] = (Node){nx, ny};
                        }
                    }
                }
            }

            #pragma omp critical
            {
                for (int k = 0; k < local_count; k++) {
                    next[next_size++] = local_next[k];
                }
            }

            free(local_next);
        }

        free(frontier);
        frontier = next;
        f_size = next_size;
    }

    free(frontier);
    return found;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <rows> <cols> <num_threads>\n", argv[0]);
        return 0;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    int num_threads = atoi(argv[3]);
    end_x = N - 1;
    end_y = M - 1;

    // Generate random maze
    srand(42);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            maze[i][j] = (rand() % 5 == 0) ? 1 : 0; // 20% obstacles
    maze[start_x][start_y] = 0;
    maze[end_x][end_y] = 0;

    // Sequential
    double t1 = omp_get_wtime();
    int found_seq = sequential_bfs();
    double t2 = omp_get_wtime();
    double seq_time = (t2 - t1) * 1000;

    // Parallel
    double t3 = omp_get_wtime();
    int found_par = parallel_bfs(num_threads);
    double t4 = omp_get_wtime();
    double par_time = (t4 - t3) * 1000;

    printf("\n--- Maze Solver (Exploratory Decomposition) ---\n");
    printf("Grid Size: %dx%d | Threads: %d\n", N, M, num_threads);
    printf("Path Found Sequential: %s\n", found_seq ? "YES" : "NO");
    printf("Path Found Parallel  : %s\n", found_par ? "YES" : "NO");
    printf("Sequential Time: %.2f ms\n", seq_time);
    printf("Parallel Time  : %.2f ms\n", par_time);
    printf("Speedup        : %.2fx\n", seq_time / par_time);
    printf("Wasted Work    : ~0%% (Exploratory - no speculation)\n");
    printf("-----------------------------------------------\n");

    return 0;
}
