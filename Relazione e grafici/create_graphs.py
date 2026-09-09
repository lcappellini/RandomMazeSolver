import csv
import os
from collections import defaultdict

import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from mpl_toolkits.mplot3d import Axes3D

def median(lst):
    n = len(lst)
    lst = tuple(sorted(lst))
    if n % 2 == 0:
        return (lst[n//2] + lst[n//2-1]) / 2
    else:
        return lst[n//2]


sns.set_theme(style="whitegrid", context="talk")

os.makedirs("graphs", exist_ok=True)

times = defaultdict(list)
iters_data = defaultdict(list)

for file in [e for e in os.listdir() if e.endswith(".csv")]:
    with open(file, "r", newline="", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            threads = int(row["threads"])
            particles = int(row["particles"]) // threads
            times[(particles, threads)].append(float(row["elapsed_ms"]))
            iters_data[(particles, threads)].append(int(row["iters"]))

particle_values = sorted(set(p for p, t in times.keys()))
thread_values = sorted(set(t for p, t in times.keys()))

median_times = {k: median(v) for k, v in times.items()}
median_iters = {k: median(v) for k, v in iters_data.items()}

time_matrix = np.array([[median_times[(p, t)] for t in thread_values] for p in particle_values])
iters_matrix = np.array([[median_iters[(p, t)] for t in thread_values] for p in particle_values])
speedup_matrix = np.array([[median_times[(p, thread_values[0])] / median_times[(p, t)] for t in thread_values] for p in particle_values])
speedup_particles_matrix = np.array([[median_times[(particle_values[0], t)] / median_times[(p, t)] for t in thread_values] for p in particle_values])
speedup_1_1_matrix = np.array([[median_times[(particle_values[0], thread_values[0])] / median_times[(p, t)] for t in thread_values] for p in particle_values])

X, Y = np.meshgrid(thread_values, particle_values)

# Mesh 3D del tempo mediano
fig = plt.figure(figsize=(12, 9))
ax = fig.add_subplot(111, projection="3d")
surf = ax.plot_surface(X, Y, time_matrix, cmap="viridis", edgecolor="none")
ax.set_xlabel("Threads (n)")
ax.set_ylabel("Particles (p)")
ax.set_zlabel("Elapsed time (ms)")
ax.set_title("Elapsed time surface")
ax.set_xticks(thread_values)
ax.set_yticks(particle_values)
fig.colorbar(surf, shrink=0.5, aspect=10, label="Elapsed time (ms)")
plt.tight_layout()
plt.savefig("Graphs\\Graph Mesh Time.png")

# Mesh 3D delle iterazioni alla soluzione
fig = plt.figure(figsize=(12, 9))
ax = fig.add_subplot(111, projection="3d")
surf = ax.plot_surface(X, Y, iters_matrix, cmap="viridis", edgecolor="none")
ax.set_xlabel("Threads (n)")
ax.set_ylabel("Particles (p)")
ax.set_zlabel("Iterations")
ax.set_title("Iterations surface")
ax.set_xticks(thread_values)
ax.set_yticks(particle_values)
fig.colorbar(surf, shrink=0.5, aspect=10, label="Iterations to solution")
plt.tight_layout()
plt.savefig("Graphs\\Graph Mesh Iters.png")

# Heatmap dello speedup (p, 1)
fig, ax = plt.subplots(figsize=(12, 9))
im = ax.pcolormesh(thread_values, particle_values, speedup_matrix, cmap="viridis", shading="nearest", edgecolors="black", linewidth=0.8)
ax.set_xlabel("Threads (n)")
ax.set_ylabel("Particles (p)")
ax.set_title("Speedup (fixed p)")
ax.set_xticks(thread_values)
ax.set_yticks(particle_values)

for i in range(len(particle_values)):
    for j in range(len(thread_values)):
        ax.text(thread_values[j], particle_values[i], f"{speedup_matrix[i, j]:.1f}", ha="center", va="center", color="white")

cbar = fig.colorbar(im, ax=ax)
cbar.set_label("Speedup")
plt.tight_layout()
plt.savefig("Graphs\\Heatmap Speedup p-1.png", dpi=300)

# Heatmap dello speedup (1, n)
fig, ax = plt.subplots(figsize=(12, 9))
im = ax.pcolormesh(thread_values, particle_values, speedup_particles_matrix, cmap="viridis", shading="nearest", edgecolors="black", linewidth=0.8)
ax.set_xlabel("Threads (n)")
ax.set_ylabel("Particles (p)")
ax.set_title("Speedup (fixed n)")
ax.set_xticks(thread_values)
ax.set_yticks(particle_values)

for i in range(len(particle_values)):
    for j in range(len(thread_values)):
        ax.text(thread_values[j], particle_values[i], f"{speedup_particles_matrix[i, j]:.1f}", ha="center", va="center", color="white")

cbar = fig.colorbar(im, ax=ax)
cbar.set_label("Speedup")
plt.tight_layout()
plt.savefig("Graphs\\Heatmap Speedup 1-n.png", dpi=300)

# Heatmap dello speedup 1, 1)
fig, ax = plt.subplots(figsize=(12, 9))
im = ax.pcolormesh(thread_values, particle_values, speedup_1_1_matrix, cmap="viridis", shading="nearest", edgecolors="black", linewidth=0.8)
ax.set_xlabel("Threads (n)")
ax.set_ylabel("Particles (p)")
ax.set_title("Speedup vs 1 particle 1 thread")
ax.set_xticks(thread_values)
ax.set_yticks(particle_values)

for i in range(len(particle_values)):
    for j in range(len(thread_values)):
        ax.text(thread_values[j], particle_values[i], f"{speedup_1_1_matrix[i, j]:.1f}", ha="center", va="center", color="white")

cbar = fig.colorbar(im, ax=ax)
cbar.set_label("Speedup")
plt.tight_layout()
plt.savefig("Graphs\\Heatmap Speedup 1-1.png", dpi=300)
