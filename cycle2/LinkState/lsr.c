#include <stdio.h>
#include <string.h>
#include <math.h>

#define INF 99
int dijkstra(int cost[][10], int source, int target);

int N;
int main()
{
    int cost[10][10], w, co, source, target;
    printf("Enter the number of routers\n");
    int res[10][10];
    scanf("%d", &N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cost[i][j] = INF;
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            printf("Enter the distance of the path between routers %c and %c:(99 for no direct link) ", i + 65, j + 65);
            scanf("%d", &w);
            cost[i][j] = cost[j][i] = w;
        }
        printf("\n");
    }
    // printf("\nEnter the source:");
    // scanf("%d", &source);
    // printf("\nEnter the target:");
    // scanf("%d", &target);
    // co = dijkstra(cost, source, target);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("\nShortest distance between %c and %c is %d\n\n", i + 65, j + 65, dijkstra(cost, i, j));
        }
    }
    // printf("\nThe Shortest Path: %d", co);
}
int dijkstra(int cost[][10], int source, int target)
{
    int dist[N], prev[N], selected[10] = {0}, i, m, min, start, d, j;
    char path[N], temp[N];
    for (i = 0; i < N; i++)
    {
        dist[i] = INF;
        prev[i] = -1;
    }
    start = source;
    selected[start] = 1;
    dist[start] = 0;
    while (selected[target] == 0)
    {
        min = INF;
        m = 0;
        for (i = 0; i < N; i++)
        {
            d = dist[start] + cost[start][i];
            if (d < dist[i] && selected[i] == 0)
            {
                dist[i] = d;
                prev[i] = start;
            }
            if (min > dist[i] && selected[i] == 0)
            {
                min = dist[i];
                m = i;
            }
        }
        start = m;
        selected[start] = 1;
    }
    start = target;
    j = 0;
    while (start != -1)
    {
        path[j++] = start + 65;
        start = prev[start];
    }
    path[j] = '\0';
    for (int i = strlen(path) - 1; i >= 0; i--)
    {
        printf("%c", path[i]);
    }
    // printf("%s", path);
    return dist[target];
}