#!/bin/bash
#SBATCH --job-name=PruebaHilos      # Nombre del trabajo
#SBATCH --output=resultado_%j.txt  # Archivo donde verás el printf (%j es el ID del proceso)
#SBATCH --partition=kura          # O la partición que te indicara tu profesor
#SBATCH --nodes=1                  # Usar un solo nodo
#SBATCH --ntasks=1                 # Una sola tarea (tu programa)
#SBATCH --cpus-per-task=4          # Pide 4 núcleos para tus hilos

# Ejecutar el programa
./Hilos


