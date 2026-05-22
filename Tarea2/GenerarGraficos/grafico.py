try:
	from matplotlib import pyplot as plt  # type: ignore
except Exception:
	import sys
	print("matplotlib not installed or could not be imported. Install with: pip install matplotlib")
	sys.exit(1)

# 1. Tus datos reales medidos en la simulación
hilos = [1, 2, 4, 8, 16]
tiempos = [150.2, 76.1, 39.5, 21.2, 12.8]  # <--- Pon aquí tus tiempos en segundos

# 2. Calcular el Speedup real (T1 / Tp)
t1 = tiempos[0]
speedup_real = [t1 / tp for tp in tiempos]

# 3. Calcular el Speedup ideal (Aceleración lineal perfecta: S = p)
speedup_ideal = hilos

# 4. Diseñar la gráfica
plt.figure(figsize=(8, 5))
plt.plot(hilos, speedup_real, marker='o', linewidth=2, color='blue', label='Speedup Real ')
plt.plot(hilos, speedup_ideal, linestyle='--', color='red', label='Speedup Ideal')

# Configuración de ejes y títulos
plt.title('Gráfica de Escalabilidad: Speedup vs Número de Hilos', fontsize=14, fontweight='bold')
plt.xlabel('Número de Hilos (p)', fontsize=12)
plt.ylabel('Speedup (S)', fontsize=12)
plt.xticks(hilos) # Forzar a que muestre solo tus configuraciones de hilos
plt.grid(True, linestyle=':', alpha=0.6)
plt.legend(fontsize=11)

# Guardar y mostrar
plt.savefig('grafica_speedup.png', dpi=300)
plt.show()