import sys

# =========================================================================
# 1. Datos reales medidos en Kabré (Mapeo directo de Hilos -> Tiempo)
# =========================================================================
hilos = [2, 4, 8, 16, 32]

# N = 100
t_serial_100 = 0.75  
tiempos_100 = {2: 0.3917, 4: 0.2048, 8: 0.1160, 16: 0.0656, 32: 0.0695}

# N = 200
t_serial_200 = 11.2  
tiempos_200 = {2: 5.76351, 4: 3.78357, 8: 3.04937, 16: 2.58469, 32: 3.0008}

# N = 300
t_serial_300 = 38.5  
tiempos_300 = {2: 20.5323, 4: 13.0071, 8: 10.3575, 16: 10.9845, 32: 13.4586}


# =========================================================================
# 2. Impresión de la Tabla Formateada
# =========================================================================
print("=" * 82)
print(f"{'TABLA COMPARATIVA DE RENDIMIENTO (SPEEDUP Y EFICIENCIA)':^82}")
print("=" * 82)
print(f"{'Hilos (p)':<12} | {'Métrica':<15} | {'N = 100':<14} | {'N = 200':<14} | {'N = 300':<14}")
print("-" * 82)

# Iterar directamente sobre la lista de hilos configurada
for p in hilos:
    # Obtener tiempos
    t100 = tiempos_100[p]
    t200 = tiempos_200[p]
    t300 = tiempos_300[p]
    
    # Calcular Speedups (T_serial / T_paralelo)
    s_100 = t_serial_100 / t100
    s_200 = t_serial_200 / t200
    s_300 = t_serial_300 / t300
    
    # Calcular Eficiencias ((Speedup / p) * 100)
    e_100 = (s_100 / p) * 100
    e_200 = (s_200 / p) * 100
    e_300 = (s_300 / p) * 100
    
    # Fila 1: Tiempos de ejecución
    print(f"{p:<12} | {'Tiempo (s)':<15} | {t100:<14.4f} | {t200:<14.4f} | {t300:<14.4f}")
    # Fila 2: Speedup relativo
    print(f"{'':<12} | {'Speedup':<15} | {s_100:<14.2f} | {s_200:<14.2f} | {s_300:<14.2f}")
    # Fila 3: Eficiencia del hardware
    print(f"{'':<12} | {'Eficiencia':<15} | {f'{e_100:.1f}%':<14} | {f'{e_200:.1f}%':<14} | {f'{e_300:.1f}%':<14}")
    print("-" * 82)