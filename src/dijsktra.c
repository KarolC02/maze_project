#include "dijsktra.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

void dijkstra2(Maze *maze){
    // Initialize distances, visited, and previous node arrays
    int **distances = (int**)malloc(maze->height * sizeof(int * ) );
    bool **visited = (bool**)malloc(maze->width * sizeof(bool*) );
    if (distances == NULL || visited == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("HELLO\n");
    maze->prevX = (int **)malloc(maze->height * sizeof(int *));
    maze->prevY = (int **)malloc(maze->height * sizeof(int *));

    if (maze->prevX == NULL || maze->prevY == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for each row of prevX and prevY
    for (int i = 0; i < maze->height; ++i) {
        maze->prevX[i] = (int *)malloc(maze->width * sizeof(int));
        maze->prevY[i] = (int *)malloc(maze->width * sizeof(int));

        distances[i] = (int *)malloc(maze->width * sizeof(int));
        visited[i] = (bool *)malloc(maze->width * sizeof(bool));

        if (maze->prevX[i] == NULL || maze->prevY[i] == NULL || distances[i] == NULL || visited[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        for(int j = 0; j < maze->width; j++ ){
            distances[i][j] = INT_MAX;
            visited[i][j] = false;
            maze->prevX[i][j] = -1;
            maze->prevY[i][j] = -1;
            // If it's a wall mark it as visited
            if( maze->grid[i][j] == 'X' ){
                visited[i][j] = true;
            }
        }
    }

    MinHeap heap;
    initHeap(&heap, MAX_SIZE * MAX_SIZE);
    distances[maze->startX][maze->startY] = 0;
    insert(&heap, (Node){maze->startX, maze->startY, 0});
    
    // printf("The size of the heap is now %d\n", heap.size);
    while (heap.size > 0) {
        Node current = Poll(&heap);
        // printf("The size of the heap is now %d\n", heap.size);
        // printf("Aktualnie jestem na (%d,%d)\n", current.y, current.x);

        if (visited[current.y][current.x]) {
            // printf("JUŻ TU BYŁEM\n");
            // printf("A jestem na (%d,%d)\n\n", current.y,current.x);
            continue;
        }

        visited[current.y][current.x] = true;

        if (current.x == maze->endX && current.y == maze->endY) {
            // printf("JUPOIIASIDIA\n\n");
            break; // Reached the destination
        }

        // Add neighboring nodes to the heap
        int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // Right, Down, Left, Up
        // printf("Currently at node (%d, %d)\n", current.y, current.x);
        for (int i = 0; i < 4; ++i) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];
            // printf("Currently checking if I can go to (%d, %d)\n", ny, nx);

            if (nx >= 0 && nx < maze->width && ny >= 0 && ny < maze->height && maze->grid[ny][nx] != 'X' && !visited[ny][nx]) {
                // printf("I'm procsessing (%d,%d)\n", ny, nx);
                // printf("I just went to (%d, %d)", ny, nx);
                int newDist = distances[current.y][current.x] + 1;
                if (newDist < distances[ny][nx]) {
                    distances[ny][nx] = newDist;
                    maze->prevX[ny][nx] = current.x;
                    maze->prevY[ny][nx] = current.y;
                    // printf("PREV OF (%d,%d) is (%d,%d)\n", ny, nx, prevY[ny][nx], prevX[ny][nx]);
                    insert(&heap, (Node){nx, ny, newDist});
                    // printf("Just inserted a node! The size of the heap is now %d\n", heap.size);
                }
            }
        }
    }

    reconstruct_path(maze->endX, maze->endY, maze);
    freeHeap(&heap);


}

void reconstruct_path(int currentX, int currentY, Maze *maze) {
    // Check if there K was found
    if ( maze->prevX[maze->endY][maze->endX] == -1 || maze->prevY[maze->endY][maze->endX] == -1 ){
        printf("Path was not found\n");
        return;
    }
    // Starting from the end node, we will go back through the previous nodes to find the path
    while (currentX != -1 && currentY != -1) {
        if (maze->grid[currentY][currentX] != 'K' && maze->grid[currentY][currentX] != 'P'){
            maze->grid[currentY][currentX] = '*'; // Mark the path
        }
        // printf("curently at (%d,%d)\n", currentY, currentX);
        int tempX = maze->prevX[currentY][currentX];
        int tempY = maze->prevY[currentY][currentX];
        // printf("I wanna go to (%d,%d)\n", tempY, tempX);
        if( maze->grid[tempY][tempX] == 'P'){
            break;
        }
        assert (tempX >= 0);
        assert (tempY >= 0);
        currentX = tempX;
        currentY = tempY;
        // printf("Now at (%d,%d)\n", currentY, currentX);
    }
}
