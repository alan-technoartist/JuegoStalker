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
import time

# =====================================================================
# Parámetros
# =====================================================================
MAZE_SIZE   = 21
INPUT_SIZE  = 445   # 21*21 + 2 (stalker) + 2 (hero)
N_SAMPLES   = 100000
EPOCHS      = 300
BATCH_SIZE  = 64

# Deltas de movimiento: arriba, abajo, izquierda, derecha
ACTION_DR = [-2, 2,  0, 0]  # paso de 2 para celdas raíz
ACTION_DC = [ 0, 0, -2, 2]

# Deltas de movimiento para el juego (paso de 1 en el grid real)
MOVE_DR = [-1, 1,  0, 0]
MOVE_DC = [ 0, 0, -1, 1]

class VisualizadorIA(tf.keras.callbacks.Callback):
    def __init__(self, X_data):
        super().__init__()
        self.X_data = X_data  # Guardamos referencia al dataset real

    def on_epoch_end(self, epoch, logs=None):
        if epoch != 0 and (epoch + 1) % 5 != 0:
            return

        print(f"\n\n--- Simulando con datos REALES de entrenamiento (Época {epoch + 1}) ---")
        
        # 1. Tomamos un tensor aleatorio directamente de tu variable X
        idx = random.randint(0, len(self.X_data) - 1)
        muestra = self.X_data[idx]
        
        # 2. Ingeniería inversa: Reconstruimos el grid de 21x21 a partir de los primeros 441 elementos
        grid_plano = muestra[:441]
        test_grid = grid_plano.reshape((MAZE_SIZE, MAZE_SIZE)).tolist()
        
        # 3. Ingeniería inversa: Des-normalizamos las coordenadas (* MAZE_SIZE)
        stalker_r = int(round(muestra[441] * MAZE_SIZE))
        stalker_c = int(round(muestra[442] * MAZE_SIZE))
        hero_r = int(round(muestra[443] * MAZE_SIZE))
        hero_c = int(round(muestra[444] * MAZE_SIZE))
        
        stalker_start = (stalker_r, stalker_c)
        hero_start = (hero_r, hero_c)
        
        current_pos = stalker_start
        path = [current_pos]
        
        for paso in range(40):
            if current_pos == hero_start:
                print("¡ÉXITO! El modelo encontró al héroe.")
                break
                
            # Reconstruimos la entrada para predecir el siguiente paso
            entrada = build_input(test_grid, current_pos[0], current_pos[1], 
                                  hero_start[0], hero_start[1])
            
            entrada_lote = np.expand_dims(entrada, axis=0)
            
            prediccion = self.model.predict(entrada_lote, verbose=0)[0]
            accion = np.argmax(prediccion)
            
            nr = current_pos[0] + MOVE_DR[accion]
            nc = current_pos[1] + MOVE_DC[accion]
            
            if nr < 0 or nr >= MAZE_SIZE or nc < 0 or nc >= MAZE_SIZE or test_grid[nr][nc] == 1.0:
                print(f"FALLO: El modelo intentó chocar contra una pared en el paso {paso+1}.")
                break
                
            current_pos = (nr, nc)
            
            if current_pos in path:
                path.append(current_pos)
                print(f"BUCLE: El modelo osciló y se quedó atascado en el paso {paso+1}.")
                break
                
            path.append(current_pos)
        
        print_maze(test_grid, path, stalker_start, hero_start)
        print("-" * 60 + "\n")

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

def print_maze(grid, path=None, stalker=None, hero=None):
    path = set(path or [])

    for r in range(MAZE_SIZE):
        row = ""
        for c in range(MAZE_SIZE):

            if stalker == (r, c):
                row += "S"
            elif hero == (r, c):
                row += "H"
            elif (r, c) in path:
                row += "·"
            elif grid[r][c] == 1:
                row += "█"
            else:
                row += " "

        print(row)

def floor_cells(grid):
    """Devuelve todas las celdas navegables del grid."""
    return [(r, c) for r in range(MAZE_SIZE) for c in range(MAZE_SIZE)
            if grid[r][c] == 0]

# =====================================================================
# BFS — experto que calcula la acción óptima del stalker hacia el hero
# =====================================================================
def bfs_action(grid, stalker_r, stalker_c, hero_r, hero_c):
    """
    Devuelve:
        action: acción óptima (0-3)
        path: lista de celdas desde el stalker hasta el hero
    """

    if stalker_r == hero_r and stalker_c == hero_c:
        return None, []

    visited = [[False] * MAZE_SIZE for _ in range(MAZE_SIZE)]
    prev = [[None] * MAZE_SIZE for _ in range(MAZE_SIZE)]

    queue = deque()
    queue.append((stalker_r, stalker_c))
    visited[stalker_r][stalker_c] = True

    while queue:
        r, c = queue.popleft()

        if (r, c) == (hero_r, hero_c):

            # Reconstruir camino completo
            path = []
            cur = (hero_r, hero_c)

            while cur is not None:
                path.append(cur)
                cur = prev[cur[0]][cur[1]]

            path.reverse()

            # Primer movimiento
            first = path[1]

            dr = first[0] - stalker_r
            dc = first[1] - stalker_c

            for action in range(4):
                if MOVE_DR[action] == dr and MOVE_DC[action] == dc:
                    return action, path

            return None, path

        for action in range(4):
            nr = r + MOVE_DR[action]
            nc = c + MOVE_DC[action]

            if (0 <= nr < MAZE_SIZE and
                0 <= nc < MAZE_SIZE and
                not visited[nr][nc] and
                grid[nr][nc] == 0):

                visited[nr][nc] = True
                prev[nr][nc] = (r, c)
                queue.append((nr, nc))

    return None, []


# =====================================================================
# Construcción del tensor de input
# =====================================================================
def build_input(grid, stalker_r, stalker_c, hero_r, hero_c):
    data = np.zeros(INPUT_SIZE, dtype=np.float32)

    # Reemplaza el doble bucle 'for' con esta operación vectorizada de NumPy:
    # Aplanamos el grid y lo asignamos a los primeros 441 espacios
    data[:441] = np.array(grid, dtype=np.float32).flatten()

    # Posiciones NORMALIZADAS
    data[441] = float(stalker_r) / MAZE_SIZE
    data[442] = float(stalker_c) / MAZE_SIZE
    data[443] = float(hero_r) / MAZE_SIZE
    data[444] = float(hero_c) / MAZE_SIZE

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

            action, _ = bfs_action(grid, sr, sc, hr, hc)
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
        
        tf.keras.layers.Dense(1024, activation='relu'),
        tf.keras.layers.BatchNormalization(),
        tf.keras.layers.Dropout(0.3), # <--- VITAL: Apaga 30% de neuronas
        
        tf.keras.layers.Dense(512, activation='relu'),
        tf.keras.layers.BatchNormalization(),
        tf.keras.layers.Dropout(0.3), # <--- VITAL
        
        tf.keras.layers.Dense(256, activation='relu'),
        tf.keras.layers.BatchNormalization(),
        
        tf.keras.layers.Dense(128, activation='relu'),
        
        tf.keras.layers.Dense(4)  # Salida
    ])
    return model


# =====================================================================
# Entrenamiento
# =====================================================================
def train(model, X, y):
    custom_optimizer = tf.keras.optimizers.Adam(learning_rate=0.0001)

    model.compile(
        optimizer=custom_optimizer,
        loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
        metrics=['accuracy']
    )
    
    early_stop = tf.keras.callbacks.EarlyStopping(
        monitor='val_loss', 
        patience=15, 
        restore_best_weights=True
    )
    
    # LE PASAMOS EL DATASET 'X' AL CALLBACK AQUÍ
    visualizador = VisualizadorIA(X)
    
    model.fit(X, y, epochs=EPOCHS, batch_size=BATCH_SIZE, validation_split=0.1, 
              callbacks=[early_stop, visualizador])
    
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

    grid = generate_maze(0)

    cells = floor_cells(grid)

    stalker = random.choice(cells)
    hero = random.choice(cells)

    action, path = bfs_action(
        grid,
        stalker[0], stalker[1],
        hero[0], hero[1]
    )

    print_maze(
        grid,
        path,
        stalker,
        hero
    )

    #build_input(grid, stalker[0], stalker[1], hero[0], hero[1])

    print("Generando dataset...")
    X, y = generate_dataset(N_SAMPLES)

    print("Entrenando modelo...")
    model = build_model()
    
    # CORRECCIÓN AQUÍ: Solo pasamos model, X y y
    model = train(model, X, y)

    print("Exportando a ONNX...")
    export_onnx(model)
