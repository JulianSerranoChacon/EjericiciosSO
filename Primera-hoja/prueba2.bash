#!/bin/bash

# Verificar que se pase exactamente un argumento
if [ $# -ne 1 ]; then
    echo -ne "No se admite más de un argumento para este programa\n"
    echo "Uso prueba2.sh <ruta>"
    exit 1
fi

# Guardar el argumento en una variable
ruta="$1"

# Comprobar si la ruta es un fichero regular
if [ -f "$ruta" ]; then
    # Contar el número de líneas del fichero
    lineas=$(wc -l < "$ruta")
    echo "El fichero $ruta tiene $lineas líneas"
else
    echo "El fichero $ruta no existe o no es regular"
    exit 1
fi
