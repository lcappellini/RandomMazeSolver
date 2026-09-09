// Random Maze Solver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Maze.h"
#include "MazeSolver.h"
#include <omp.h>

int main()
{
    Maze maze(1280, 720);
    MazeSolver solver(&maze);

    const int MIN_PARTICLES = 1;
    const int MAX_PARTICLES = 20;
    const int MIN_THREADS = 1;
    const int MAX_THREADS = 16;
    const int REPEATS = 20;

    // creazione immagine di esempio
    auto sample_res = solver.run(1, 1, true);
    std::cout << "Sample image solved in " << sample_res.first << " iterations" << std::endl;

    // misurazioni
    std::ofstream csv("maze_results.csv");
    csv << "particles,threads,iters,elapsed_ms\n";
    for (unsigned int n = MIN_THREADS; n <= MAX_THREADS; n++) { // per ogni numero di threads
        for (unsigned int p = MIN_PARTICLES; p <= MAX_PARTICLES; p++) { // per ogni numero di particelle per thread
            for (int rep = 0; rep < REPEATS; rep++) { //ripeti la misurazione 
                auto result = solver.run(p, n); // avvia ricerca

                csv << p*n << "," << n << "," << result.first << "," << result.second << "\n";
                csv.flush();

                std::cout << "p=" << p * n << " n=" << n << " rep=" << rep + 1 << "/" << REPEATS << " -> it=" << result.first << " ms=" << result.second << std::endl;
            }
        }
    }

    csv.close();
    return 0;
}

