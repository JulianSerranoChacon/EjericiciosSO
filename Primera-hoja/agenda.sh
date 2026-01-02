#!/bin/bash

clear
# Fichero donde se almacenan los datos de la agenda
AGENDA="agenda.txt"

# Comprobar si el fichero existe, si no, crearlo
if [ ! -f "$AGENDA" ]; then
    touch "$AGENDA"
fi

listar() {
    if [ ! -s "$AGENDA" ]; then
        echo "La agenda está vacía."
    else
        i=1
        while IFS=: read -r nombre telefono mail
        do
            echo "Registro $i:"
            echo "  Nombre: $nombre"
            echo "  Teléfono: $telefono"
            echo "  Mail: $mail"
            echo
            ((i++))
        done < "$AGENDA"
    fi
}

# Función para buscar registros por patrón
buscar(){
    read -p "Buscar: " patron
    i=1
    while IFS=: read -r nombre telefono mail
        do
            if [ "$patron" = "$nombre" ]; then
                echo "Registro $i:"
                echo "  Nombre: $nombre"
                echo "  Teléfono: $telefono"
                echo "  Mail: $mail"
                echo
            fi
            ((i++))
        done < "$AGENDA"
}

borrar(){
        read -p "Nombre a borrar: " nombre
    if grep -iq "^$nombre:" "$AGENDA"; then
        # Crear un fichero temporal sin el registro a borrar
        grep -iv "^$nombre:" "$AGENDA" > "${AGENDA}.tmp"
        mv "${AGENDA}.tmp" "$AGENDA"
        echo "Registro borrado."
    else
        echo "No se encontró el registro con ese nombre."
    fi
}

# Función para añadir un registro
anadir() {
    read -p "Nombre: " nombre
    read -p "Teléfono: " telefono
    read -p "Mail: " mail
    echo "${nombre}:${telefono}:${mail}" >> "$AGENDA"
    echo "Registro añadido."
}

# Función principal del menú usando select y case
menu() {
    PS3="#? "  # Prompt del menú
    options=("Listar" "Buscar" "Borrar" "Añadir" "Salir")
    select opt in "${options[@]}"
    do
        case $opt in
            "Listar")
                listar
                ;;
            "Buscar")
                buscar
                ;;
            "Borrar")
                borrar
                ;;
            "Añadir")
                anadir
                ;;
            "Salir")
                echo "Saliendo..."
                break
                ;;
            *)
                echo "Opción incorrecta"
                ;;
        esac
    done
}

# Ejecutar el menú
menu