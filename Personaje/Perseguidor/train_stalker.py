"""
train_stalker.py — Grid Stalker CLI
Genera el dataset con BFS como experto y entrena el modelo MLP.
Exporta a ONNX con argmax incluido en el grafo.
"""

import numpy as np
import tensorflow as tf
import tf2onnx
from collections import deque
import random
import sys
import subprocess

# =====================================================================
# Parámetros
# =====================================================================
MAZE_SIZE   = 21
INPUT_SIZE  = 445   # 21*21 + 2 (stalker) + 2 (hero)
N_SAMPLES   = 50000
EPOCHS      = 10
BATCH_SIZE  = 64

# Deltas de movimiento: arriba, abajo, izquierda, derecha
ACTION_DR = [-2, 2,  0, 0]  # paso de 2 para celdas raíz
ACTION_DC = [ 0, 0, -2, 2]

# Deltas de movimiento para el juego (paso de 1 en el grid real)
MOVE_DR = [-1, 1,  0, 0]
MOVE_DC = [ 0, 0, -1, 1]


# =====================================================================
# Generación de laberinto — Recursive Backtracking
# =====================================================================
def generate_maze(seed):
    rng = random.Random(seed)
    grid = [[1] * MAZE_SIZE for _ in range(MAZE_SIZE)]

    def carve(r, c):
        grid[r][c] = 0
        dirs = list(range(4))
        rng.shuffle(dirs)
        for d in dirs:
            nr, nc = r + ACTION_DR[d], c + ACTION_DC[d]
            if 1 <= nr <= MAZE_SIZE - 2 and 1 <= nc <= MAZE_SIZE - 2 and grid[nr][nc] == 1:
                grid[r + ACTION_DR[d]//2][c + ACTION_DC[d]//2] = 0
                carve(nr, nc)

    carve(1, 1)

    # Derribar paredes adicionales para crear ciclos
    for r in range(1, MAZE_SIZE - 1, 2):
        for c in range(2, MAZE_SIZE - 1, 2):
            if grid[r][c] == 1 and rng.randint(0, 99) < 20:
                grid[r][c] = 0

    return grid


def floor_cells(grid):
    """Devuelve todas las celdas navegables del grid."""
    return [(r, c) for r in range(MAZE_SIZE) for c in range(MAZE_SIZE)
            if grid[r][c] == 0]


# =====================================================================
# BFS — experto que calcula la acción óptima del stalker hacia el hero
# =====================================================================
def bfs_action(grid, stalker_r, stalker_c, hero_r, hero_c):
    """
    Calcula el primer paso del camino más corto del stalker al hero.
    Devuelve la acción (0-3) o None si no hay camino.
    """
    if stalker_r == hero_r and stalker_c == hero_c:
        return None

    visited = [[False] * MAZE_SIZE for _ in range(MAZE_SIZE)]
    prev    = [[None]  * MAZE_SIZE for _ in range(MAZE_SIZE)]
    queue   = deque()

    queue.append((stalker_r, stalker_c))
    visited[stalker_r][stalker_c] = True

    while queue:
        r, c = queue.popleft()

        if r == hero_r and c == hero_c:
            # Reconstruir camino hasta el primer paso
            cur = (r, c)
            while prev[cur[0]][cur[1]] != (stalker_r, stalker_c):
                cur = prev[cur[0]][cur[1]]

            # Determinar acción según la dirección del primer paso
            dr = cur[0] - stalker_r
            dc = cur[1] - stalker_c
            for action in range(4):
                if MOVE_DR[action] == dr and MOVE_DC[action] == dc:
                    return action
            return None

        for action in range(4):
            nr, nc = r + MOVE_DR[action], c + MOVE_DC[action]
            if (0 <= nr < MAZE_SIZE and 0 <= nc < MAZE_SIZE
                    and not visited[nr][nc] and grid[nr][nc] == 0):
                visited[nr][nc] = True
                prev[nr][nc] = (r, c)
                queue.append((nr, nc))

    return None  # sin camino


# =====================================================================
# Construcción del tensor de input
# =====================================================================
def build_input(grid, stalker_r, stalker_c, hero_r, hero_c):
    data = np.zeros(INPUT_SIZE, dtype=np.float32)

    # Grid aplanado
    for r in range(MAZE_SIZE):
        for c in range(MAZE_SIZE):
            data[r * MAZE_SIZE + c] = float(grid[r][c])

    # Posiciones
    data[441] = float(stalker_r)
    data[442] = float(stalker_c)
    data[443] = float(hero_r)
    data[444] = float(hero_c)

    return data


# =====================================================================
# Generación del dataset
# =====================================================================
def generate_dataset(n_samples):
    X = np.zeros((n_samples, INPUT_SIZE), dtype=np.float32)
    y = np.zeros(n_samples, dtype=np.int64)

    i = 0
    seed = 0

    while i < n_samples:
        grid  = generate_maze(seed)
        cells = floor_cells(grid)
        seed += 1

        if len(cells) < 2:
            continue

        # Muestrear múltiples posiciones por laberinto
        attempts = min(50, n_samples - i)
        for _ in range(attempts):
            stalker_pos, hero_pos = random.sample(cells, 2)
            sr, sc = stalker_pos
            hr, hc = hero_pos

            action = bfs_action(grid, sr, sc, hr, hc)
            if action is None:
                continue

            X[i] = build_input(grid, sr, sc, hr, hc)
            y[i] = action
            i += 1

            if i >= n_samples:
                break

        if i % 5000 == 0:
            print(f"Generando dataset... {i}/{n_samples}")

    print(f"Dataset generado: {n_samples} muestras")
    return X, y


# =====================================================================
# Modelo
# =====================================================================
def build_model():
    model = tf.keras.Sequential([
        tf.keras.layers.Input(shape=(INPUT_SIZE,)),
        tf.keras.layers.Dense(16, activation='relu'),
        tf.keras.layers.Dense(4)  # Q-values
    ])
    return model


# =====================================================================
# Entrenamiento
# =====================================================================
def train(model, X, y):
    model.compile(
        optimizer='adam',
        loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
        metrics=['accuracy']
    )
    model.fit(X, y, epochs=EPOCHS, batch_size=BATCH_SIZE, validation_split=0.1)
    return model


# =====================================================================
# Exportar a ONNX con argmax en el grafo
# =====================================================================
def export_onnx(model, path="stalker.onnx"):
    saved_model_dir = "stalker_saved_model"

    model.export(saved_model_dir)

    subprocess.run([
        sys.executable,
        "-m",
        "tf2onnx.convert",
        "--saved-model",
        saved_model_dir,
        "--output",
        path,
        "--opset",
        "17",
    ], check=True)

    print(f"Modelo exportado a {path}")


# =====================================================================
# Main
# =====================================================================
if __name__ == "__main__":
    print("Generando dataset...")
    X, y = generate_dataset(N_SAMPLES)

    print("Entrenando modelo...")
    model = build_model()
    model = train(model, X, y)

    print("TensorFlow:", tf.__version__)
    print("Keras:", tf.keras.__version__)
    print("tf2onnx:", tf2onnx.__version__)


    print("Exportando a ONNX...")
    export_onnx(model)
