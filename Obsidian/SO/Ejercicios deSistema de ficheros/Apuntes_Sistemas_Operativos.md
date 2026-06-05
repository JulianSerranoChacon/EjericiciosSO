# 📘 Apuntes de Sistemas Operativos
## Práctica de copia de bloques, llamadas al sistema y ficheros sparse

---

# 1. Lecturas parciales con `read()`

## Problema
Una llamada a `read(fd, buffer, n)` **no garantiza** que se lean `n` bytes.

Puede devolver menos datos en:

- Pipes
- Terminales
- Sockets
- Dispositivos especiales

## Solución

Acumular lecturas hasta completar el bloque o alcanzar EOF.

```c
size_t totalLeidos = 0;

while (totalLeidos < nsize) {
    ssize_t n = read(fd, buffer + totalLeidos,
                     nsize - totalLeidos);

    if (n == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    if (n == 0)
        break;

    totalLeidos += n;
}
```

---

# 2. Escrituras parciales con `write()`

## Problema

`write()` tampoco garantiza escribir todos los bytes solicitados.

## Solución

Repetir llamadas hasta escribir todo el contenido leído.

```c
size_t totalEscritos = 0;

while (totalEscritos < totalLeidos) {
    ssize_t n = write(fd,
                      buffer + totalEscritos,
                      totalLeidos - totalEscritos);

    if (n == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    totalEscritos += n;
}
```

---

# 3. Uso correcto de `lseek()`

## Problema

No puede hacerse `lseek()` sobre:

- Terminales
- Pipes
- Entrada/salida estándar redirigida

Ejemplo:

```bash
./midd entrada - 512 10 0
```

## Solución

```c
if (fOut != STDOUT_FILENO) {
    lseek(...);
}
```

---

# 4. Comprobación correcta de `-`

Incorrecto:

```c
if(argv[1][0] == '-')
```

Aceptaría:

```text
-
-a
-archivo
----
```

Correcto:

```c
#include <string.h>

if(strcmp(argv[1], "-") == 0)
```

---

# 5. Permisos de creación

El enunciado pide:

```text
rw-rw-r--
```

Modo octal:

```c
0664
```

No:

```c
0666
```

---

# 6. Mensaje final del programa

```c
printf("Copiados %d bloques de %zu bytes\n",
       bloquesCopiados,
       nsize);
```

---

# 7. Uso de `strace`

Permite observar las llamadas al sistema.

Ejemplo:

```bash
strace -e read,write ./midd fichero salida 512 10 0
```

Muestra:

```text
read(...)
write(...)
```

Muy útil para comprobar lecturas y escrituras parciales.

---

# 8. Prueba con pipes

Comando del ejercicio:

```bash
((echo "0123"; sleep 1; echo "4567") &) | \
./midd - prueba2 8 2 0
```

## Qué ocurre

Primer proceso:

```text
0123\n
```

espera 1 segundo

```text
4567\n
```

Total:

```text
10 bytes
```

El programa intenta leer bloques de 8 bytes.

Resultado:

```text
Bloque 1 -> 8 bytes
Bloque 2 -> 2 bytes
```

---

# 9. Ficheros Sparse

## Definición

Un fichero sparse contiene huecos lógicos que no ocupan espacio físico.

---

## Creación

```c
lseek(fd, 100*1024*1024, SEEK_SET);
write(fd, "A", 1);
```

---

## Resultado

Tamaño lógico:

```bash
ls -lh fichero
```

Tamaño real:

```bash
du -h fichero
```

El tamaño lógico puede ser 100 MB mientras que el espacio real ocupado es sólo unos pocos KB.

---

# 10. Vista lógica vs física

## Vista lógica

```text
| DATA | HOLE | DATA |
```

## Vista física

```text
| DATA | DATA |
```

Los huecos no se almacenan.

---

# 11. `fallocate()`

Reserva espacio físico desde el inicio.

```bash
fallocate -l 100M archivo.bin
```

Características:

- Todos los bloques quedan reservados.
- Evita errores de falta de espacio posteriores.
- Reduce fragmentación.

---

# 12. Comparación: Sparse vs Prealocado

| Característica | Sparse | fallocate |
|---------------|---------|------------|
| Espacio inicial | mínimo | reservado |
| Bloques físicos | no | sí |
| Riesgo ENOSPC | sí | no |
| Tamaño lógico | grande | grande |

---

# 13. Extents en ext4

## Qué es un extent

Describe una región contigua:

```text
(logical_start,
 physical_start,
 length)
```

Ejemplo:

```text
0 -> 1000 (100 bloques)
```

---

## Ventaja

Menos metadatos y menos fragmentación que almacenar bloques individuales.

---

# 14. Sparse en ext4

Los huecos no aparecen en el árbol de extents.

```text
[ HOLE ][ HOLE ][ DATA ]
```

Sólo existe físicamente la zona con datos.

---

# 15. Extent Splitting

Si se escribe en mitad de un extent:

Antes:

```text
[--------------------]
```

Después:

```text
[------][X][---------]
```

Consecuencia:

- Más extents
- Más fragmentación
- Menor rendimiento

---

# 16. Organización física del disco

## Block Groups

Ext4 divide el sistema de ficheros en grupos de bloques.

## Buddy Allocator

Intenta:

- Asignar bloques contiguos
- Mantener los datos próximos
- Reducir fragmentación

---

# 17. Delayed Allocation

Ext4 no asigna bloques inmediatamente.

Proceso:

```text
write()
 ↓
Page Cache
 ↓
Asignación diferida
 ↓
Disco
```

Ventajas:

- Menos fragmentación
- Mejor rendimiento

---

# 18. SEEK_HOLE y SEEK_DATA

Permiten localizar huecos y datos dentro de un fichero sparse.

```c
lseek(fd, pos, SEEK_HOLE);
lseek(fd, pos, SEEK_DATA);
```

---

# 19. Cómo verificar un fichero sparse

## Tamaño lógico

```bash
ls -lh fichero
```

## Espacio real

```bash
du -h fichero
```

## Información completa

```bash
stat fichero
```

Un fichero sparse se reconoce porque:

```text
Tamaño lógico >> Espacio realmente ocupado
```

---

# 🧾 Resumen final

- `read()` puede leer menos bytes de los solicitados.
- `write()` puede escribir menos bytes de los solicitados.
- Hay que reintentar ambas llamadas hasta completar la operación.
- `lseek()` no funciona sobre pipes ni terminales.
- `strace` permite inspeccionar llamadas al sistema.
- Un fichero sparse tiene huecos lógicos sin bloques físicos.
- `fallocate()` reserva bloques físicos desde el inicio.
- Ext4 usa extents para representar regiones contiguas.
- Delayed allocation mejora rendimiento y reduce fragmentación.
