#pragma once
#include "Maze.h"
#include "Rect.h"
#include "StepGenerator.h"
#include <atomic>
#include <omp.h>
#include <deque>

class MazeSolver {
	Maze* maze;
	int stepx;
	int stepy;
	
public:
	MazeSolver(Maze* maze) : maze(maze) {
		stepx = maze->w / maze->cols / 4;
		stepy = maze->h / maze->rows / 4;
	}

	std::pair<unsigned long, unsigned long> run(unsigned int p, unsigned int n, bool draw=false) {
		auto start = std::chrono::high_resolution_clock::now();

		Point start_p = maze->start_p;
		Rect end = maze->end_region;

		unsigned long result_iters = 0;
		Path result_path;

		std::atomic<bool> running{ true };

		#pragma omp parallel for num_threads(n)
		for (int t = 0; t < n; t++) { // 1 ciclo per ogni thread

			// inizializzazione vettori di generatori, paths e iterazioni
			std::vector<Path> paths(p);
			std::vector<StepGenerator> gens;
			gens.reserve(p);
			for (int i = 0; i < (int)p; i++) {
				paths[i].points.push_back(start_p);
				gens.emplace_back(stepx, stepy);
			}

			std::vector<long> local_iters(p, 0);

			while (running.load(std::memory_order_relaxed)) {
				for (int i = 0; i < (int)p; i++) { // per ogni particella del thread

					// genera prossimo punto (p2) e calcola i rimbalzi
					Path& path = paths[i];
					Point p1 = path.points.back();
					StepGenerator& gen = gens[i];

					Point p2_intent(p1.x + gen.getX(), p1.y + gen.getY());
					Point p2 = maze->move(p1, p2_intent);
					path.points.push_back(p2);

					// incrementa iterazioni
					local_iters[i]++;

					if (end.inside(p2)) { // entrata nella regione di arrivo
						bool expected = true;
						if (running.compare_exchange_strong(expected, false, std::memory_order_relaxed)) {
							// salva iterazioni e percorso trovato
							result_iters = local_iters[i];
							result_path = path;
						}
						break;
					}
				}
			}
		}

		// calcola tempo trascorso
		auto now = std::chrono::high_resolution_clock::now();
		unsigned long result_elapsed_ms = static_cast<unsigned long>(std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());

		if (draw) {
			maze->drawMaze(result_path);
		}

		return std::pair<unsigned long, unsigned long>(result_iters, result_elapsed_ms);
	}
};