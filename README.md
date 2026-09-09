# Random Maze Solver

Trovare l'uscita da un labirinto simulando il movimento casuale di una "particella".

Il progetto è stato sviluppato per l'assignment del corso di Parallel Computing, il quale richiede l'implementazione del programma in modo che sfrutti almeno uno degli approcci di parallelizzazione o vettorizzazione presentati durante il corso (OpenMP, vettorizzazione SIMD, CUDA), a partire da una versione sequenziale dello stesso algoritmo, corredata da un'analisi delle performance in termini di speed-up.

## Presentazione

La presentazione completa del progetto, con dettagli su approcci, implementazione e risultati sperimentali, è disponibile qui:

[**Relazione Random Maze Solver**](Relazione%20e%20grafici/Relazione%20Random%20Maze%20Solver.pdf)

## Immagini d'esempio

Rappresentazioni del labirinto e del percorso effettuato da una particella per raggiungere l'uscita (rettangolo rosso in basso a destra) partendo dall'angolo in alto a sinistra.

### Caso ottimo (350000 steps)

![Caso ottimo](Relazione%20e%20grafici/Graphs/maze.png)

### Caso pessimo (5000000 steps)
![Caso pessimo](Relazione%20e%20grafici/Graphs/maze2.png)
