#!/bin/bash

# Verificar que se pase exactamente un argumento
if [ $# -ne 1 ]; then
    echo -ne "No se admite un argumento que no seaun directorio\n"
    echo "Uso $0 <directorio>"
    exit 1
fi

# Guardar el argumento en una variable
directorio="$1"

# Verificar que la ruta sea un directorio
if [ ! -d "$directorio" ]; then
    echo "$directorio no es un directorio"
    exit 1
fi

# Iterar por cada fichero en el directorio
for archivo in "$directorio"/*; do
    if [ -f "$archivo" ]; then  # Comprobar si es un fichero regular
        palabras=$(wc -w < "$archivo")  # Contar número de palabras
        echo "$archivo tiene $palabras palabras"
    fi
done
