💻 Ejercicio 1.  Compila el siguiente programa:

```C
#include <stdio.h>

#include <unistd.h>

int num = 22;

int mul;

  

const char *msg = "El resultado es:\n";

  

int main(void) {

    static int factor = 2;

  

    mul = num * factor;

    printf("%s%i\n", msg, mul);

  

    sleep(600);

  

    return 0;

}
```
Consulta los segmentos definidos en el ejecutable generado con el comando readelf -l  y completa la siguiente tabla.

- Los segmentos de la tabla corresponden a las secciones de tipo LOAD. La primera de ellas PHDR es la 00, INTERP la 01 y así sucesivamente.
    
- Algunas secciones del programa pueden estar asignadas al mismo segmento del ejecutable.
    

|   |   |   |   |   |
|---|---|---|---|---|
|Segmento|Código C correspondiente|Offset|Dir.virtual|Flags|
|.text|||||
|.rodata|||||
|.bss|||||
|.data|||||

Ejecutar ahora el programa y obtener los segmentos de memoria virtual accediendo al fichero maps del directorio del proceso en /proc. Nota: Se puede identificar la correspondencia de los segmentos del ejecutable y los segmentos de memoria virtual comparando el offset y los flags:

  

|   |   |   |   |   |   |
|---|---|---|---|---|---|
|Segmento|Rango de direcciones virtuales|Offset del fichero|Flags|Tipo map/anónimo|Ruta del fichero|
|.text||||||
|.rodata||||||
|.bss||||||
|.data||||||
|[heap]||||||
|[stack]||||||

Nota: Alternativamente, el programa pmap(1) puede utilizarse para consultar la información sobre los segmentos de memoria (misma información que proc/`<pid>`/maps)

Nota: Para identificar los segmentos de memoria virtual tener en cuenta que deben alinearse al tamaño de página. Por ejemplo, un dirección virtual 0x3dd0 se alinea a 4K (ALIGN = 0x1000) es decir se colocaría en 0x3000 (0x3dd0 & 0x0FFF = 0x3000). 

Cuestiones:

- En qué segmento(s) de memoria virtual está la cadena "El resultado es:\n". El fichero proyectado del segmento de memoria virtual se puede acceder desde el directorio del proceso /proc/`<pid proceso>`/map_files/`<rango del segmento>` ¿Qué tipo de ficheros son?.
    
- En la salida del comando readelf se muestra el inicio del programa (entry point). ¿En qué segmento del fichero está ubicado? ¿Y en qué segmento de memoria virtual?
    
- Dibuja esquemáticamente (como en las transparencias de teoría) el espacio de direcciones del proceso con las regiones correspondientes.
    
- Investiga cuál es el contenido y propósito de las áreas de memoria [vdso] y [vvar].



Respuesta:


![[imagenes/Pasted image 20260609223627.png]]

``` bash
~/Documentos/SO/EjerciciosSO/Cuarta-hoja main*
❯ readelf -l Ejercicio1.exe

El tipo del fichero elf es DYN (Position-Independent Executable file)
Entry point 0x1050
There are 14 program headers, starting at offset 64

Encabezados de Programa:
  Tipo           Desplazamiento     DirVirtual         DirFísica
                 TamFichero         TamMemoria          Opts   Alineación
  PHDR           0x0000000000000040 0x0000000000000040 0x0000000000000040
                 0x0000000000000310 0x0000000000000310  R      0x8
  INTERP         0x00000000000003b4 0x00000000000003b4 0x00000000000003b4
                 0x000000000000001c 0x000000000000001c  R      0x1
      [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
  LOAD           0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000690 0x0000000000000690  R      0x1000
  LOAD           0x0000000000001000 0x0000000000001000 0x0000000000001000
                 0x00000000000001a5 0x00000000000001a5  R E    0x1000
  LOAD           0x0000000000002000 0x0000000000002000 0x0000000000002000
                 0x00000000000000dc 0x00000000000000dc  R      0x1000
  LOAD           0x0000000000002dd0 0x0000000000003dd0 0x0000000000003dd0
                 0x0000000000000260 0x0000000000000268  RW     0x1000
  DYNAMIC        0x0000000000002de0 0x0000000000003de0 0x0000000000003de0
                 0x00000000000001e0 0x00000000000001e0  RW     0x8
  NOTE           0x0000000000000350 0x0000000000000350 0x0000000000000350
                 0x0000000000000040 0x0000000000000040  R      0x8
  NOTE           0x0000000000000390 0x0000000000000390 0x0000000000000390
                 0x0000000000000024 0x0000000000000024  R      0x4
  NOTE           0x00000000000020bc 0x00000000000020bc 0x00000000000020bc
                 0x0000000000000020 0x0000000000000020  R      0x4
  GNU_PROPERTY   0x0000000000000350 0x0000000000000350 0x0000000000000350
                 0x0000000000000040 0x0000000000000040  R      0x8
  GNU_EH_FRAME   0x000000000000201c 0x000000000000201c 0x000000000000201c
                 0x0000000000000024 0x0000000000000024  R      0x4
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     0x10
  GNU_RELRO      0x0000000000002dd0 0x0000000000003dd0 0x0000000000003dd0
                 0x0000000000000230 0x0000000000000230  R      0x1

 Asignación de Sección a Segmento:
  Segmento Secciones...
   00
   01     .interp
   02     .note.gnu.property .note.gnu.build-id .interp .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt
   03     .init .plt .text .fini
   04     .rodata .eh_frame_hdr .eh_frame .note.ABI-tag
   05     .init_array .fini_array .dynamic .got .got.plt .data .bss
   06     .dynamic
   07     .note.gnu.property
   08     .note.gnu.build-id
   09     .note.ABI-tag
   10     .note.gnu.property
   11     .eh_frame_hdr
   12
   13     .init_array .fini_array .dynamic .got
```

``` bash
~/Documentos/SO/EjerciciosSO/Cuarta-hoja main*
readelf -lS Ejercicio1.exe
There are 36 section headers, starting at offset 0x3920:

Encabezados de Sección:
  [Nr] Nombre            Tipo             Dirección         Despl
       Tamaño            TamEnt           Opts   Enl   Info  Alin
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .note.gnu.pr[...] NOTE             0000000000000350  00000350
       0000000000000040  0000000000000000   A       0     0     8
  [ 2] .note.gnu.bu[...] NOTE             0000000000000390  00000390
       0000000000000024  0000000000000000   A       0     0     4
  [ 3] .interp           PROGBITS         00000000000003b4  000003b4
       000000000000001c  0000000000000000   A       0     0     1
  [ 4] .gnu.hash         GNU_HASH         00000000000003d0  000003d0
       000000000000001c  0000000000000000   A       5     0     8
  [ 5] .dynsym           DYNSYM           00000000000003f0  000003f0
       00000000000000c0  0000000000000018   A       6     1     8
  [ 6] .dynstr           STRTAB           00000000000004b0  000004b0
       0000000000000095  0000000000000000   A       0     0     1
  [ 7] .gnu.version      VERSYM           0000000000000546  00000546
       0000000000000010  0000000000000002   A       5     0     2
  [ 8] .gnu.version_r    VERNEED          0000000000000558  00000558
       0000000000000030  0000000000000000   A       6     1     8
  [ 9] .rela.dyn         RELA             0000000000000588  00000588
       00000000000000d8  0000000000000018   A       5     0     8
  [10] .rela.plt         RELA             0000000000000660  00000660
       0000000000000030  0000000000000018  AI       5    23     8
  [11] .init             PROGBITS         0000000000001000  00001000
       000000000000001b  0000000000000000  AX       0     0     4
  [12] .plt              PROGBITS         0000000000001020  00001020
       0000000000000030  0000000000000010  AX       0     0     16
  [13] .text             PROGBITS         0000000000001050  00001050
       0000000000000147  0000000000000000  AX       0     0     16
  [14] .fini             PROGBITS         0000000000001198  00001198
       000000000000000d  0000000000000000  AX       0     0     4
  [15] .rodata           PROGBITS         0000000000002000  00002000
       000000000000001c  0000000000000000   A       0     0     4
  [16] .eh_frame_hdr     PROGBITS         000000000000201c  0000201c
       0000000000000024  0000000000000000   A       0     0     4
  [17] .eh_frame         PROGBITS         0000000000002040  00002040
       000000000000007c  0000000000000000   A       0     0     8
  [18] .note.ABI-tag     NOTE             00000000000020bc  000020bc
       0000000000000020  0000000000000000   A       0     0     4
  [19] .init_array       INIT_ARRAY       0000000000003dd0  00002dd0
       0000000000000008  0000000000000008  WA       0     0     8
  [20] .fini_array       FINI_ARRAY       0000000000003dd8  00002dd8
       0000000000000008  0000000000000008  WA       0     0     8
  [21] .dynamic          DYNAMIC          0000000000003de0  00002de0
       00000000000001e0  0000000000000010  WA       6     0     8
  [22] .got              PROGBITS         0000000000003fc0  00002fc0
       0000000000000028  0000000000000008  WA       0     0     8
  [23] .got.plt          PROGBITS         0000000000003fe8  00002fe8
       0000000000000028  0000000000000008  WA       0     0     8
  [24] .data             PROGBITS         0000000000004010  00003010
       0000000000000020  0000000000000000  WA       0     0     8
  [25] .bss              NOBITS           0000000000004030  00003030
       0000000000000008  0000000000000000  WA       0     0     4
  [26] .comment          PROGBITS         0000000000000000  00003030
       000000000000001b  0000000000000001  MS       0     0     1
  [27] .debug_aranges    PROGBITS         0000000000000000  0000304b
       0000000000000030  0000000000000000           0     0     1
  [28] .debug_info       PROGBITS         0000000000000000  0000307b
       0000000000000121  0000000000000000           0     0     1
  [29] .debug_abbrev     PROGBITS         0000000000000000  0000319c
       00000000000000a3  0000000000000000           0     0     1
  [30] .debug_line       PROGBITS         0000000000000000  0000323f
       000000000000006b  0000000000000000           0     0     1
  [31] .debug_str        PROGBITS         0000000000000000  000032aa
       0000000000000097  0000000000000001  MS       0     0     1
  [32] .debug_line_str   PROGBITS         0000000000000000  00003341
       0000000000000061  0000000000000001  MS       0     0     1
  [33] .symtab           SYMTAB           0000000000000000  000033a8
       00000000000002b8  0000000000000018          34     7     8
  [34] .strtab           STRTAB           0000000000000000  00003660
       0000000000000155  0000000000000000           0     0     1
  [35] .shstrtab         STRTAB           0000000000000000  000037b5
       0000000000000166  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  D (mbind), l (large), p (processor specific)

El tipo del fichero elf es DYN (Position-Independent Executable file)
Entry point 0x1050
There are 14 program headers, starting at offset 64

Encabezados de Programa:
  Tipo           Desplazamiento     DirVirtual         DirFísica
                 TamFichero         TamMemoria          Opts   Alineación
  PHDR           0x0000000000000040 0x0000000000000040 0x0000000000000040
                 0x0000000000000310 0x0000000000000310  R      0x8
  INTERP         0x00000000000003b4 0x00000000000003b4 0x00000000000003b4
                 0x000000000000001c 0x000000000000001c  R      0x1
      [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
  LOAD           0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000690 0x0000000000000690  R      0x1000
  LOAD           0x0000000000001000 0x0000000000001000 0x0000000000001000
                 0x00000000000001a5 0x00000000000001a5  R E    0x1000
  LOAD           0x0000000000002000 0x0000000000002000 0x0000000000002000
                 0x00000000000000dc 0x00000000000000dc  R      0x1000
  LOAD           0x0000000000002dd0 0x0000000000003dd0 0x0000000000003dd0
                 0x0000000000000260 0x0000000000000268  RW     0x1000
  DYNAMIC        0x0000000000002de0 0x0000000000003de0 0x0000000000003de0
                 0x00000000000001e0 0x00000000000001e0  RW     0x8
  NOTE           0x0000000000000350 0x0000000000000350 0x0000000000000350
                 0x0000000000000040 0x0000000000000040  R      0x8
  NOTE           0x0000000000000390 0x0000000000000390 0x0000000000000390
                 0x0000000000000024 0x0000000000000024  R      0x4
  NOTE           0x00000000000020bc 0x00000000000020bc 0x00000000000020bc
                 0x0000000000000020 0x0000000000000020  R      0x4
  GNU_PROPERTY   0x0000000000000350 0x0000000000000350 0x0000000000000350
                 0x0000000000000040 0x0000000000000040  R      0x8
  GNU_EH_FRAME   0x000000000000201c 0x000000000000201c 0x000000000000201c
                 0x0000000000000024 0x0000000000000024  R      0x4
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     0x10
  GNU_RELRO      0x0000000000002dd0 0x0000000000003dd0 0x0000000000003dd0
                 0x0000000000000230 0x0000000000000230  R      0x1

 Asignación de Sección a Segmento:
  Segmento Secciones...
   00
   01     .interp
   02     .note.gnu.property .note.gnu.build-id .interp .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt
   03     .init .plt .text .fini
   04     .rodata .eh_frame_hdr .eh_frame .note.ABI-tag
   05     .init_array .fini_array .dynamic .got .got.plt .data .bss
   06     .dynamic
   07     .note.gnu.property
   08     .note.gnu.build-id
   09     .note.ABI-tag
   10     .note.gnu.property
   11     .eh_frame_hdr
   12
   13     .init_array .fini_array .dynamic .got
```


![[imagenes/Pasted image 20260609223857.png]]

## Ejecución del programa

Al ejecutar el programa se obtiene la siguiente salida:

```bash
❯ ./Ejercicio1.exe &
El resultado es:
44
```

El resultado es correcto ya que:

```c
mul = num * factor = 22 * 2 = 44
```

---

## 1. Segmentos definidos en el ejecutable (`readelf -l`)

A partir de la salida obtenida con:

```bash
readelf -lS Ejercicio1.exe
```

se identifican las siguientes secciones y su correspondencia con los segmentos `LOAD` del ejecutable.

| Segmento  | Código C correspondiente                                            | Offset   | Dirección virtual | Flags |
| --------- | ------------------------------------------------------------------- | -------- | ----------------- | ----- |
| `.text`   | Código ejecutable (`main`, `_start`, llamadas a `printf` y `sleep`) | `0x1000` | `0x1000`          | `R E` |
| `.rodata` | Cadena constante `"El resultado es:\n"`                             | `0x2000` | `0x2000`          | `R`   |
| `.data`   | `int num = 22;`, `const char *msg`, `static int factor = 2;`        | `0x3010` | `0x4010`          | `RW`  |
| `.bss`    | `int mul;`                                                          | `0x3030` | `0x4030`          | `RW`  |

### Correspondencia de variables

```c
int num = 22;
```

Variable global inicializada → **`.data`**

```c
int mul;
```

Variable global no inicializada → **`.bss`**

```c
const char *msg = "El resultado es:\n";
```

- El puntero `msg` → **`.data`**
    
- La cadena literal → **`.rodata`**
    

```c
static int factor = 2;
```

Variable estática inicializada → **`.data`**

El código de `main()` se encuentra en **`.text`**.

---

## 2. Segmentos de memoria virtual (`/proc/<pid>/maps`)

El PID del proceso en ejecución es:

```bash
4813
```

Obteniendo el mapa de memoria:

```bash
cat /proc/4813/maps
```

se identifican los siguientes segmentos:

|Segmento|Rango de direcciones virtuales|Offset fichero|Flags|Tipo|Ruta|
|---|---|---|---|---|---|
|`.text`|`562d7ba70000-562d7ba71000`|`00001000`|`r-xp`|Mapeado|`/home/verxxuxz/Documentos/SO/EjerciciosSO/Cuarta-hoja/Ejercicio1.exe`|
|`.rodata`|`562d7ba71000-562d7ba72000`|`00002000`|`r--p`|Mapeado|`/home/verxxuxz/Documentos/SO/EjerciciosSO/Cuarta-hoja/Ejercicio1.exe`|
|`.data`|`562d7ba73000-562d7ba74000`|`00003000`|`rw-p`|Mapeado|`/home/verxxuxz/Documentos/SO/EjerciciosSO/Cuarta-hoja/Ejercicio1.exe`|
|`.bss`|`562d7ba73000-562d7ba74000`|`00003000`|`rw-p`|Mapeado/Anónimo|`/home/verxxuxz/Documentos/SO/EjerciciosSO/Cuarta-hoja/Ejercicio1.exe`|
|`[heap]`|`562da1889000-562da18aa000`|`00000000`|`rw-p`|Anónimo|`[heap]`|
|`[stack]`|`7fff4a6e1000-7fff4a702000`|`00000000`|`rw-p`|Anónimo|`[stack]`|

**Nota:** Los segmentos `.data` y `.bss` aparecen en la misma página de memoria debido al alineamiento a páginas de 4 KiB.

---

## Cuestiones

### 1. ¿En qué segmento(s) de memoria virtual está la cadena `"El resultado es:\n"`?

La declaración es:

```c
const char *msg = "El resultado es:\n";
```

Existen dos componentes:

- La **cadena literal** `"El resultado es:\n"` se almacena en la sección **`.rodata`**, proyectada en memoria mediante el segmento:
    

```text
562d7ba71000-562d7ba72000 r--p
```

- El **puntero `msg`** se almacena en la sección **`.data`**, proyectada en:
    

```text
562d7ba73000-562d7ba74000 rw-p
```

#### ¿Qué tipo de ficheros son `/proc/<pid>/map_files/<rango>`?

Los ficheros contenidos en:

```bash
/proc/<pid>/map_files/
```

son **enlaces simbólicos especiales gestionados por el kernel** que apuntan al fichero original mapeado en memoria.

Permiten acceder al archivo que respalda una determinada región mapeada del espacio de direcciones del proceso.

No son copias independientes del fichero, sino referencias al objeto realmente proyectado en memoria.

---

### 2. Entry Point del programa

La salida de `readelf` muestra:

```text
Entry point 0x1050
```

La dirección `0x1050` coincide con el inicio de la sección:

```text
.text
```

dentro del ejecutable.

Por tanto:

- **Segmento del fichero:** `.text`
    
- **Segmento de memoria virtual:** región con permisos `r-xp`
    

```text
562d7ba70000-562d7ba71000
```

correspondiente al código ejecutable del programa.

---

### 3. Espacio de direcciones del proceso

Representación esquemática:

```text
Direcciones altas
┌─────────────────────────┐
│         [stack]         │
├─────────────────────────┤
│          [vvar]         │
├─────────────────────────┤
│      [vvar_vclock]      │
├─────────────────────────┤
│          [vdso]         │
├─────────────────────────┤
│   ld-linux-x86-64.so    │
├─────────────────────────┤
│        libc.so.6        │
├─────────────────────────┤
│         [heap]          │
├─────────────────────────┤
│      .data / .bss       │
├─────────────────────────┤
│        .rodata          │
├─────────────────────────┤
│         .text           │
└─────────────────────────┘
Direcciones bajas
```

---

### 4. Áreas de memoria `[vdso]` y `[vvar]`

#### `[vdso]` (Virtual Dynamic Shared Object)

Es una pequeña biblioteca proporcionada por el kernel y mapeada automáticamente en el espacio de direcciones del proceso.

Su función es acelerar determinadas llamadas al sistema permitiendo ejecutarlas desde espacio de usuario sin necesidad de realizar una transición completa al modo kernel.

Ejemplos de funciones que utilizan VDSO:

- `clock_gettime()`
    
- `gettimeofday()`
    
- `time()`
    

En este proceso aparece como:

```text
7f4b70648000-7f4b7064a000 r-xp [vdso]
```

Tiene permisos de lectura y ejecución.

---

#### `[vvar]` (Virtual Variables)

Es una región de memoria de solo lectura utilizada por VDSO para almacenar información proporcionada por el kernel, como:

- Datos temporales del sistema.
    
- Información para el cálculo eficiente del tiempo.
    
- Variables necesarias para implementar las funciones exportadas por VDSO.
    

En este proceso aparecen:

```text
7f4b70642000-7f4b70646000 r--p [vvar]
7f4b70646000-7f4b70648000 r--p [vvar_vclock]
```

Estas regiones contienen únicamente datos y no código ejecutable.

---

## Conclusiones

- Las variables globales inicializadas (`num`, `factor` y el puntero `msg`) se almacenan en **`.data`**.
    
- Las variables globales no inicializadas (`mul`) se almacenan en **`.bss`**.
    
- Las cadenas constantes se sitúan en **`.rodata`**.
    
- El código ejecutable reside en **`.text`**.
    
- El punto de entrada del programa (`0x1050`) se encuentra dentro del segmento **`.text`**.
    
- El espacio de direcciones del proceso incluye regiones especiales como **`[heap]`**, **`[stack]`**, **`[vdso]`** y **`[vvar]`**, cada una con funciones específicas dentro de la ejecución del programa.


---

#### Ejercicio 2.  Considera el siguiente programa y completa la siguiente tabla.


```c
#define CONSTANT 10

int num1 = CONSTANT;
int num2;

int main(int argc, char *argv[]) {
    int *i = malloc(sizeof(int));

    num2 = argc;

    for (*i = 0; *i < CONSTANT; *i = *i + 1) {
        fprintf(stdout, "%s: %d, argc: %d\n", string, num1--, num2);
    }

    return 0;
}
```

|   |   |   |
|---|---|---|
|Símbolo|Espacio en ejecutable (Sí/No)|Segmento|
|CONSTANT|||
|i|||
|num1|||
|num2|||
|“%s: %d, argc: %d”|||

Respuesta:

# Ejercicio 2 - Ubicación de símbolos en el ejecutable y memoria

Dado el siguiente programa:

se analiza dónde se almacena cada símbolo dentro del ejecutable y en qué segmento de memoria se ubicará durante la ejecución.

| Símbolo                | ¿Espacio en ejecutable? | Segmento                                                                                                                                                                                |
| ---------------------- | ----------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `CONSTANT`             | **No**                  | No ocupa memoria. El preprocesador sustituye todas las apariciones de `CONSTANT` por el valor literal `10` antes de la compilación.                                                     |
| `i`                    | **No**                  | La variable puntero `i` es una variable local automática y se almacena en la **pila (`stack`)**. El entero apuntado por `i`, reservado mediante `malloc()`, se almacena en el **heap**. |
| `num1`                 | **Sí**                  | **`.data`**. Es una variable global inicializada (`int num1 = CONSTANT;`).                                                                                                              |
| `num2`                 | **Sí**                  | **`.bss`**. Es una variable global no inicializada (`int num2;`).                                                                                                                       |
| `"%s: %d, argc: %d\n"` | **Sí**                  | **`.rodata`**. Las cadenas literales se almacenan en la sección de datos de solo lectura.                                                                                               |

## Explicación detallada

### `CONSTANT`

```c
#define CONSTANT 10
```

Es una **macro del preprocesador**. Durante la fase de preprocesado, el compilador sustituye:

```c
num1 = CONSTANT;
```

por:

```c
num1 = 10;
```

Por tanto:

- No existe como símbolo en el ejecutable.
    
- No ocupa espacio en memoria.
    

---

### `i`

```c
int *i = malloc(sizeof(int));
```

Hay que distinguir dos elementos:

- El **puntero `i`**:
    
    - Variable local de `main`.
        
    - Se almacena en la **pila (`stack`)**.
        
    - No forma parte de las secciones `.data` o `.bss` del ejecutable.
        
- La memoria obtenida mediante `malloc()`:
    
    - Se reserva dinámicamente durante la ejecución.
        
    - Se ubica en el **heap**.
        

---

### `num1`

```c
int num1 = CONSTANT;
```

Tras el preprocesado:

```c
int num1 = 10;
```

Es una:

- Variable global.
    
- Inicializada.
    

Por tanto, se almacena en:

```text
.data
```

y ocupa espacio tanto en el ejecutable como en la memoria del proceso.

---

### `num2`

```c
int num2;
```

Es una:

- Variable global.
    
- No inicializada.
    

El compilador la sitúa en:

```text
.bss
```

La sección `.bss` no almacena los datos en el fichero ejecutable; únicamente reserva espacio que el sistema operativo inicializa a cero al cargar el programa.

---

### Cadena literal

```c
"%s: %d, argc: %d\n"
```

Es una constante de tipo cadena.

Las cadenas literales se almacenan en:

```text
.rodata
```

(sección de datos de solo lectura).

Aunque el programa utiliza un puntero para acceder a ella, el contenido de la cadena forma parte del ejecutable.

## Resumen

- **`.text`** → código ejecutable.
    
- **`.data`** → variables globales/estáticas inicializadas.
    
- **`.bss`** → variables globales/estáticas no inicializadas.
    
- **`.rodata`** → constantes y cadenas literales.
    
- **Stack** → variables locales automáticas.
    
- **Heap** → memoria reservada dinámicamente mediante `malloc()`.



```
la casilla de la tabla de num2 no debería ser un 2, no ocupa espacio en el ejecutable, solo reserva memoria
```


Sí, tienes razón. Depende de cómo interprete tu profesor la columna **"Espacio en ejecutable (Sí/No)"**.

Hay dos formas de verlo:

1. **Desde el punto de vista lógico de las secciones ELF**:
    
    - `.bss` forma parte del ejecutable como sección.
        
    - Por ello, algunos apuntes ponen **"Sí"** porque el símbolo existe en el ejecutable.
        
2. **Desde el punto de vista del espacio físico ocupado en el fichero ELF** (que suele ser lo que preguntan en Sistemas Operativos):
    
    - La sección `.bss` es de tipo `NOBITS`.
        
    - **No ocupa bytes en el fichero ejecutable**.
        
    - El cargador del SO simplemente **reserva memoria y la inicializa a cero** al crear el proceso.
        

En vuestro contexto, dado que la pregunta es **"Espacio en ejecutable (Sí/No)"**, lo correcto sería:

|Símbolo|Espacio en ejecutable (Sí/No)|Segmento|
|---|---|---|
|`CONSTANT`|**No**|No aplica (macro del preprocesador)|
|`i`|**No**|Stack (`i`) y Heap (memoria de `malloc`)|
|`num1`|**Sí**|`.data`|
|`num2`|**No**|`.bss`|
|`"%s: %d, argc: %d\n"`|**Sí**|`.rodata`|

La justificación para `num2` sería:

> `num2` es una variable global no inicializada, por lo que se almacena en la sección `.bss`. Esta sección es de tipo `NOBITS`, por lo que **no ocupa espacio real en el fichero ejecutable**, aunque el ejecutable sí contiene la información necesaria para que el cargador reserve memoria para ella e inicialice su contenido a cero.

De hecho, si ejecutas:

```bash
readelf -S programa
```

verás algo parecido a:

```text
[25] .bss    NOBITS    0000000000004030 ...
```

El tipo `NOBITS` indica precisamente que **no hay datos almacenados en el ELF para esa sección**.

Así que, para entregar este ejercicio, yo pondría **NO** para `num2`.

```
que secciones ocupan espacio en el ejecutable
```

En un ejecutable ELF, **ocupan espacio real en el fichero** las secciones que contienen datos o código almacenados físicamente. Las secciones que son solo una reserva de memoria para el momento de ejecución **no ocupan espacio**.

## Secciones que SÍ ocupan espacio en el ejecutable

Estas secciones suelen ser de tipo `PROGBITS`:

|Sección|Contenido|
|---|---|
|`.text`|Código máquina del programa.|
|`.rodata`|Constantes y cadenas literales (`"Hola"`).|
|`.data`|Variables globales/estáticas inicializadas.|
|`.init`, `.fini`|Código de inicialización y finalización.|
|`.plt`, `.got.plt`|Soporte para enlace dinámico.|
|`.dynamic`|Información del enlazador dinámico.|
|`.dynsym`, `.dynstr`|Tabla de símbolos y cadenas dinámicas.|
|`.rela.dyn`, `.rela.plt`|Información de reubicación.|
|`.eh_frame`, `.eh_frame_hdr`|Información para manejo de excepciones y backtrace.|
|`.interp`|Ruta del cargador dinámico (`ld-linux`).|
|`.symtab`, `.strtab`|Información de depuración y símbolos (si no se ha hecho `strip`).|
|`.debug_*`|Información de depuración (solo si se compila con `-g`).|

Por ejemplo, en tu salida:

```text
[13] .text     PROGBITS ...
[15] .rodata   PROGBITS ...
[24] .data     PROGBITS ...
```

Todas ellas **sí ocupan espacio**.

---

## Secciones que NO ocupan espacio en el ejecutable

Estas secciones son normalmente de tipo `NOBITS`:

|Sección|Contenido|
|---|---|
|`.bss`|Variables globales/estáticas no inicializadas.|
|`.tbss`|Variables TLS no inicializadas (Thread Local Storage).|

Ejemplo de tu salida:

```text
[25] .bss NOBITS 0000000000004030 00003030
```

El tipo **`NOBITS`** significa:

- Existe una entrada en la tabla de secciones.
    
- El cargador sabe cuánto espacio reservar.
    
- **No hay datos almacenados en el fichero ELF.**
    

Por eso:

```c
int a = 5;    // .data → SÍ ocupa espacio
int b;        // .bss → NO ocupa espacio
```

---

## Regla rápida para el examen

- **¿Variable inicializada?** → `.data` → **Sí ocupa espacio**.
    
- **¿Variable no inicializada?** → `.bss` → **No ocupa espacio**.
    
- **¿Cadena literal o `const`?** → `.rodata` → **Sí ocupa espacio**.
    
- **¿Código?** → `.text` → **Sí ocupa espacio**.
    
- **¿Macro `#define`?** → **No existe en el ejecutable**.
    

---

### Cómo comprobarlo

Con:

```bash
readelf -S programa
```

mira la columna **Tipo**:

```text
.text    PROGBITS   ← ocupa espacio
.data    PROGBITS   ← ocupa espacio
.rodata  PROGBITS   ← ocupa espacio
.bss     NOBITS     ← NO ocupa espacio
```

La regla general es:

> **PROGBITS ⇒ Sí ocupa espacio en el ejecutable.**  
> **NOBITS ⇒ No ocupa espacio en el ejecutable.**


--- 

#### 💻 Ejercicio 3. Escribe un programa que cree una región de memoria (mmap(2)) con las siguientes características:

- Tamaño 1024 bytes
    
- Acceso privado
    
- Modo lectura y escritura
    

Una vez creada la región de memoria el proceso la inicializará con ‘\0’ (usar un bucle o la llamada memset(3)), mostrará la dirección del segmento (modificador de formato %p) y el PID del proceso. Finalmente el proceso  hará un sleep de 600s. Ejemplo de ejecución:
``` bash
$ ./mimap 

PID:2716 Dirección del segmento: 0x7fe4c56ff000
```

Usando el PID consultar el fichero maps e identificar el segmento creado en el espacio de memoria del proceso:

  

|   |   |   |   |
|---|---|---|---|
|Dirección Inicial|Dirección FInal|Offset|Flags|
|||||

Respuesta 

## Ejecución del programa

Al ejecutar el programa se obtiene:

```bash
❯ ./Ejercicio3.exe &
PID: 9334 Dirección del segmento: 0x7f128eae2000
```

El proceso crea correctamente una región de memoria anónima mediante `mmap()` y muestra tanto su PID como la dirección inicial del segmento asignado.

---

## Código utilizado

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main(void) {
    size_t size = 1024;

    void *ptr = mmap(NULL, size,
                     PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS,
                     -1, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    memset(ptr, '\0', size);

    printf("PID: %d Dirección del segmento: %p\n",
           getpid(), ptr);

    sleep(600);

    munmap(ptr, size);

    return 0;
}
```


![[imagenes/Pasted image 20260609232730.png]]

---

## Identificación del segmento en `/proc/<pid>/maps`

Se consulta el mapa de memoria del proceso:

```bash
cat /proc/9334/maps
```

La dirección devuelta por `mmap()` es:

```text
0x7f128eae2000
```

Buscando esta dirección en la salida de `maps` se encuentra:

```text
7f128eae2000-7f128eae3000 rw-p 00000000 00:00 0
```

Esta es la región creada mediante `mmap()`.

---

## Tabla solicitada

| Dirección Inicial | Dirección Final  | Offset     | Flags  |
| ----------------- | ---------------- | ---------- | ------ |
| `0x7f128eae2000`  | `0x7f128eae3000` | `00000000` | `rw-p` |

---

## Análisis del segmento

La entrada encontrada:

```text
7f128eae2000-7f128eae3000 rw-p 00000000 00:00 0
```

indica que:

- **Dirección inicial:** `0x7f128eae2000`
    
- **Dirección final:** `0x7f128eae3000`
    
- **Tamaño real asignado:**
    

```text
0x7f128eae3000 − 0x7f128eae2000 = 0x1000 bytes = 4096 bytes
```

Aunque el programa solicitó únicamente:

```c
size_t size = 1024;
```

el kernel asignó una **página completa de memoria (4 KiB)** debido a que los mapeos realizados con `mmap()` están alineados al tamaño de página del sistema.

- **Offset:** `00000000`
    
    Al utilizar `MAP_ANONYMOUS`, la región no está asociada a ningún fichero, por lo que el desplazamiento es cero.
    
- **Flags:** `rw-p`
    
    - `r`: permiso de lectura.
        
    - `w`: permiso de escritura.
        
    - `p`: mapeo privado (`MAP_PRIVATE`).
        
- **Dispositivo e inodo:** `00:00 0`
    
    Indican que se trata de un **mapeo anónimo**, es decir, no respaldado por ningún fichero del sistema de archivos.
    

---

## Conclusiones

- `mmap()` ha creado correctamente una región de memoria privada y anónima.
    
- La dirección obtenida mediante `mmap()` coincide con la mostrada en `/proc/9334/maps`.
    
- Aunque se solicitaron **1024 bytes**, el sistema operativo reservó una página completa de **4096 bytes**, que es el tamaño mínimo de asignación.
    
- La región aparece como:
    

```text
rw-p 00000000 00:00 0
```

lo que confirma que es un segmento:

- **legible**,
    
- **escribible**,
    
- **privado**,
    
- y **no asociado a ningún fichero**.

---


# Ejercicio 4 - Uso de `mmap()` para proyectar un fichero en memoria

## Objetivo

El objetivo de este ejercicio es reescribir el ejercicio 11 de la Hoja 3 utilizando **memoria proyectada sobre un fichero** mediante `mmap()`.

El proceso padre creará un fichero denominado `output.txt` y lo proyectará en el espacio de direcciones del proceso. Posteriormente, varios procesos hijos escribirán directamente sobre dicha región de memoria compartida, de manera que los cambios se reflejarán automáticamente en el fichero.

---

## Código del programa

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

#define NUM_HIJOS 5
#define SEGMENTO 5

int main() {
    int fd = open("output.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    
    if (fd == -1) 
    { 
        perror("open"); 
        exit(EXIT_FAILURE); 
    }

    /* Reservar espacio en el fichero */
    if (ftruncate(fd, (NUM_HIJOS + 1) * SEGMENTO) == -1) 
    {
        perror("ftruncate"); 
        exit(EXIT_FAILURE); 
    }

    /* Proyectar el fichero en memoria */
    char *map = mmap(NULL, (NUM_HIJOS+1) * SEGMENTO, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (map == MAP_FAILED) 
    { 
        perror("mmap"); 
        exit(EXIT_FAILURE); 
    }

    /* Cerrar descriptor */
    close(fd);

    /* Inicializar la primera región */
    for (int j = 0; j < SEGMENTO; j++) {
        map[j] = '0';
    }

    /* Crear procesos hijos */
    for (int i = 1; i <= NUM_HIJOS; i++) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); exit(EXIT_FAILURE); }
        else if (pid == 0) {
            for (int j = 0; j < SEGMENTO; j++) {
                map[SEGMENTO*i + j] = '0' + i;
            }
            exit(i);
        }
    }

    /* Esperar a todos los hijos */
    for (int i = 0; i < NUM_HIJOS; i++) {
        int status;
        pid_t hijo = wait(&status);
        if (WIFEXITED(status)) {
            printf("Hijo %d con PID %d terminó\n", WEXITSTATUS(status), hijo);
        }
    }

    /* Sincronizar cambios con el fichero */
    msync(map, (NUM_HIJOS + 1) * SEGMENTO, MS_SYNC);
    /* Liberar el mapeo */
    munmap(map, (NUM_HIJOS + 1) * SEGMENTO);

    printf("Escritura completada en output.txt usando mmap\n");
    return 0;
}

```

---

## Explicación del funcionamiento

### 1. Creación del fichero

El padre crea el fichero `output.txt`:

```c
int fd = open("output.txt",
              O_CREAT | O_RDWR | O_TRUNC,
              0644);
```

Opciones utilizadas:

- `O_CREAT`: crea el fichero si no existe.
    
- `O_RDWR`: permite lectura y escritura.
    
- `O_TRUNC`: elimina el contenido previo del fichero.
    

Permisos:

- Propietario: lectura y escritura.
    
- Grupo: lectura.
    
- Otros: lectura.
    

---

### 2. Reserva del tamaño del fichero

Antes de proyectar el fichero es necesario asegurar que tenga el tamaño suficiente:

```c
ftruncate(fd,
          (NUM_HIJOS + 1) * SEGMENTO);
```

Con:

```c
NUM_HIJOS = 5
SEGMENTO = 5
```

el tamaño será:

```text
(5 + 1) × 5 = 30 bytes
```

`ftruncate()` crea un **fichero sparse**, reservando espacio lógico sin necesidad de escribir físicamente todos los datos.

---

### 3. Proyección del fichero en memoria

```c
char *map = mmap(NULL,
                 (NUM_HIJOS + 2) * SEGMENTO,
                 PROT_READ | PROT_WRITE,
                 MAP_SHARED,
                 fd,
                 0);
```

Parámetros:

- `NULL`: el kernel decide la dirección virtual.
    
- Tamaño: 35 bytes.
    
- `PROT_READ | PROT_WRITE`: lectura y escritura.
    
- `MAP_SHARED`: los cambios son visibles para todos los procesos y se reflejan en el fichero.
    
- `fd`: descriptor del fichero.
    
- `0`: desplazamiento inicial.
    

El puntero se convierte a `char *` para manipular directamente cada posición del fichero como un array de caracteres.

---

### 4. Cierre del descriptor

```c
close(fd);
```

Una vez realizado el mapeo, el descriptor ya no es necesario.

El segmento proyectado permanece válido hasta ejecutar:

```c
munmap(...)
```

---

### 5. Inicialización de la región

El padre inicializa las primeras cinco posiciones:

```c
for (int j = 0; j < SEGMENTO; j++) {
    map[j] = '0';
}
```

Resultado:

```text
00000
```

---

### 6. Escritura realizada por los hijos

Cada hijo utiliza su identificador lógico (`i`) para determinar el desplazamiento:

```c
map[SEGMENTO * i + j]
```

y escribe:

```c
'0' + i
```

Aprovechando que los caracteres ASCII son consecutivos:

```text
'0' → 48
'1' → 49
'2' → 50
...
```

Cada hijo escribe cinco veces su número correspondiente.

Distribución de la memoria:

|Hijo|Posiciones|Contenido|
|---|---|---|
|Padre|0–4|`00000`|
|Hijo 1|5–9|`11111`|
|Hijo 2|10–14|`22222`|
|Hijo 3|15–19|`33333`|
|Hijo 4|20–24|`44444`|
|Hijo 5|25–29|`55555`|

---

### 7. Espera de finalización

El padre espera la terminación de todos los hijos:

```c
wait(&status);
```

Mostrando:

```text
Hijo 1 con PID XXXX terminó
...
Hijo 5 con PID XXXX terminó
```

---

### 8. Sincronización del fichero

```c
msync(map,
      (NUM_HIJOS + 2) * SEGMENTO,
      MS_SYNC);
```

`msync()` fuerza que las modificaciones realizadas en memoria se escriban en el fichero asociado.

La opción:

```c
MS_SYNC
```

hace que la llamada sea bloqueante hasta completar la sincronización.

---

### 9. Liberación del mapeo

```c
munmap(map,
       (NUM_HIJOS + 2) * SEGMENTO);
```

Elimina la región proyectada del espacio de direcciones del proceso.

---

## Resultado esperado

Contenido de `output.txt`:

```text
000001111122222333334444455555
```

Cada grupo de cinco caracteres corresponde a la escritura realizada por el padre o por uno de los hijos.

---

## Conclusiones

- `mmap()` permite acceder a un fichero como si fuese memoria convencional.
    
- `MAP_SHARED` garantiza que las modificaciones sean visibles entre procesos y persistan en el fichero.
    
- `ftruncate()` es necesario para asegurar que el fichero tenga el tamaño suficiente antes del mapeo.
    
- `msync()` sincroniza explícitamente la memoria con el almacenamiento secundario.
    
- El uso de memoria proyectada evita llamadas repetidas a `write()`, proporcionando una forma eficiente de compartir información entre procesos.

![[imagenes/Pasted image 20260610173756.png]]

---

Ejercicio 5. Compara la implementación del Ejercicio 4 y la realizada en el Ejercicio 11 de la Hoja 3:

- Respecto a E/S, describe qué acciones realiza el sistema. ¿Hay alguna diferencia entre ambas alternativas (e.g. número de escrituras en disco…)?. Nota: considera todos los componentes del VFS y memoria virtual.
    
- Respecto al SO, ¿cuál de las dos alternativas es preferible? ¿Y desde el punto de vista del programador?

Respuesta:

# Ejercicio 5 - Comparación entre el uso de `write()` y `mmap()`

En este ejercicio se comparan dos implementaciones para generar el fichero `output.txt`:

- **Ejercicio 11 (Hoja 3):** utilización de llamadas tradicionales de E/S (`open()`, `lseek()`, `write()`, `close()`).
    
- **Ejercicio 4:** utilización de memoria proyectada mediante `mmap()` y sincronización con `msync()`.
    

---

# 1. Comparación respecto a la E/S

## Implementación con `write()`

En esta alternativa, cada proceso hijo realiza explícitamente operaciones de entrada/salida sobre el fichero.

### Operaciones realizadas

Para cada hijo:

1. `open()`
    
    - El VFS busca la entrada correspondiente al fichero.
        
    - Se crea una estructura `file` en la tabla global de ficheros.
        
    - Se asigna un descriptor en la tabla de descriptores del proceso.
        
2. `lseek()`
    
    - Se actualiza el desplazamiento asociado al descriptor.
        
3. `write()`
    
    - El contenido se copia desde el espacio de usuario al **page cache** del kernel.
        
    - Se actualizan las estructuras del VFS (inode, páginas sucias).
        
    - El kernel marcará dichas páginas como **dirty pages**.
        
4. `close()`
    
    - Se libera el descriptor de fichero.
        

Posteriormente:

5. El subsistema de escritura diferida (**write-back**) escribirá las páginas modificadas al dispositivo de almacenamiento.
    

---

### Número de llamadas al sistema

Padre:

```text
open + write + close
```

Hijos (5):

```text
5 × (open + lseek + write + close)
```

Total:

| Llamada   | Número |
| --------- | ------ |
| `open()`  | 6      |
| `write()` | 6      |
| `lseek()` | 5      |
| `close()` | 6      |

Total aproximado:

```text
23 llamadas al sistema
```

---

## Implementación con `mmap()`

En esta alternativa se utiliza memoria proyectada.

### Operaciones realizadas

Inicialmente:

1. `open()`
    
2. `ftruncate()`
    
3. `mmap()`
    

Durante la ejecución:

- Los hijos modifican directamente la memoria virtual:
    

```c
map[offset] = valor;
```

Estas escrituras:

- generan faltas de página (_page faults_) la primera vez que se accede,
    
- modifican páginas del **page cache**,
    
- no requieren llamadas explícitas de escritura.
    

Finalmente:

4. `msync()`
    
    - fuerza la sincronización de las páginas modificadas con el disco.
        
5. `munmap()`
    
6. `close()`
    

---

### Número de llamadas al sistema

Padre:

```text
open
ftruncate
mmap
close
msync
munmap
```

Hijos:

```text
Ninguna llamada de E/S explícita
```

Total:

|Llamada|Número|
|---|---|
|`open()`|1|
|`ftruncate()`|1|
|`mmap()`|1|
|`close()`|1|
|`msync()`|1|
|`munmap()`|1|

Total:

```text
6 llamadas al sistema
```

---

## ¿Existe diferencia en el número de escrituras en disco?

### `write()`

Cada llamada a `write()`:

```text
Usuario → Kernel → Page Cache
```

Aunque las escrituras reales pueden agruparse posteriormente mediante write-back, el kernel recibe **6 peticiones independientes de escritura**.

---

### `mmap()`

Las modificaciones se realizan sobre memoria virtual:

```text
Usuario → Página mapeada → Page Cache
```

No existen llamadas `write()` explícitas.

El sistema operativo puede:

- agrupar modificaciones,
    
- retrasar la escritura,
    
- optimizar el acceso al disco.
    

`msync()` fuerza una única sincronización final.

---

## Diferencias principales

|Característica|`write()`|`mmap()`|
|---|---|---|
|Copia usuario → kernel|Sí|No|
|Llamadas `write()`|6|0|
|Uso de `lseek()`|Sí|No|
|Escritura mediante memoria virtual|No|Sí|
|Uso de page cache|Sí|Sí|
|Posibilidad de agrupar escrituras|Limitada|Mayor|
|Overhead de syscalls|Alto|Bajo|

---

# 2. Desde el punto de vista del Sistema Operativo

## Alternativa preferible: `mmap()`

### Ventajas

- Menor número de llamadas al sistema.
    
- Menos cambios entre modo usuario y modo kernel.
    
- Eliminación de copias adicionales de memoria.
    
- Integración directa con el sistema de memoria virtual.
    
- Mejor aprovechamiento del page cache.
    
- Posibilidad de optimización automática del acceso a disco.
    

---

### Desventajas

- Mayor complejidad en la gestión de memoria.
    
- Aparición de faltas de página (_page faults_).
    
- Puede desperdiciar memoria si se mapean ficheros muy grandes.
    

---

## Alternativa preferible para el SO

Para accesos frecuentes o ficheros grandes:

```text
mmap() > write()
```

El uso de memoria proyectada suele ser más eficiente para el sistema operativo.

---

# 3. Desde el punto de vista del programador

## Uso de `write()`

### Ventajas

- Modelo sencillo e intuitivo.
    
- Fácil de depurar.
    
- Muy portable.
    
- Adecuado para pequeñas escrituras secuenciales.
    

### Desventajas

- Mayor número de llamadas al sistema.
    
- Necesidad de gestionar offsets manualmente (`lseek()`).
    
- Mayor sobrecarga.
    

---

## Uso de `mmap()`

### Ventajas

- El fichero se manipula como un array en memoria:
    

```c
map[posicion] = valor;
```

- Código más limpio para accesos aleatorios.
    
- No es necesario usar `write()` ni `lseek()`.
    

### Desventajas

- Requiere comprender conceptos de memoria virtual.
    
- Necesidad de gestionar `mmap()`, `msync()` y `munmap()`.
    
- Errores potencialmente más difíciles de detectar.
    

---

## Alternativa preferible para el programador

Depende del contexto:

### Programas sencillos o ficheros pequeños:

```text
write() > mmap()
```

por simplicidad y claridad.

### Aplicaciones con muchos accesos aleatorios o ficheros grandes:

```text
mmap() > write()
```

por eficiencia y facilidad para manipular los datos como memoria convencional.

---

# Conclusiones

- Ambas alternativas utilizan finalmente el **page cache** del kernel para almacenar temporalmente los datos antes de escribirlos en disco.
    
- La solución basada en `write()` realiza un mayor número de llamadas al sistema y más transiciones usuario-kernel.
    
- La solución basada en `mmap()` integra la E/S con el subsistema de memoria virtual, reduciendo el overhead y permitiendo al SO optimizar las escrituras.
    
- Desde el punto de vista del **Sistema Operativo**, `mmap()` suele ser la opción más eficiente.
    
- Desde el punto de vista del **programador**, `write()` es más sencilla, mientras que `mmap()` ofrece mejor rendimiento a costa de una mayor complejidad.

---

#### Eercicio 6.  Considera un sistema operativo con una memoria virtual paginada de un nivel y tamaño de página de 512 palabras. El espacio de direcciones virtuales tiene 512 páginas y la memoria física tiene 10 marcos de página.

- Describe la estructura del espacio de direcciones virtuales y física, y de la tabla de páginas que usaría el sistema operativo. Indica el tamaño de todos los campos.
    
- Si el contenido de la memoria física es el que se muestra a continuación, determina el contenido de la tabla de páginas del proceso. ¿Qué dirección física corresponde con las direcciones virtuales 0x13FF y 0x1403?
    

|   |   |
|---|---|
|0x0000||
|0x0200||
|0x0400||
|0x0600|Página virtual 34|
|0x0800|Página virtual 9|
|0x0a00||
|0x0c00||
|0x0e00|Página virtual 65|
|0x1000||
|0x1200|Página virtual 10|
|Contenido de la memoria|   |

- ¿Qué ocurre si un proceso intenta acceder a la posición 0x80E8?
    
- Supongamos que el marco de página en las direcciones 0x0800-0x09ff se quiere compartir con otro proceso. ¿Debería asignarse al mismo segmento virtual?

Respuesta:

# Ejercicio 6 - Memoria virtual paginada de un nivel

## 1. Estructura del espacio de direcciones virtuales y físicas

### Espacio de direcciones virtuales

Datos del problema:

- Tamaño de página: **512 palabras**
    
- Número de páginas virtuales: **512 páginas**
    

Como:

```text
512 = 2^9
```

se necesitan **9 bits** para identificar la página virtual.

Además:

```text
512 palabras = 2^9 palabras
```

por lo que el desplazamiento dentro de la página necesita **9 bits**.

Por tanto, la dirección virtual tendrá:

|Campo|Bits|
|---|---|
|Número de página virtual (VPN)|9 bits|
|Desplazamiento (offset)|9 bits|

Dirección virtual total:

```text
9 + 9 = 18 bits
```

Estructura:

```text
┌─────────────────┬─────────────────┐
│ VPN (9 bits)    │ Offset (9 bits) │
└─────────────────┴─────────────────┘
```

---

### Espacio de direcciones físicas

La memoria física tiene:

```text
10 marcos de página
```

Para identificar un marco:

```text
⌈log₂(10)⌉ = 4 bits
```

Se necesitan **4 bits** para el número de marco físico.

El desplazamiento sigue siendo el mismo:

```text
512 palabras → 9 bits
```

Por tanto, la dirección física tendrá:

|Campo|Bits|
|---|---|
|Número de marco (PFN)|4 bits|
|Desplazamiento|9 bits|

Dirección física total:

```text
4 + 9 = 13 bits
```

Estructura:

```text
┌─────────────────┬─────────────────┐
│ PFN (4 bits)    │ Offset (9 bits) │
└─────────────────┴─────────────────┘
```

---

## 2. Tabla de páginas

Existen:

```text
512 páginas virtuales
```

Por tanto, la tabla tendrá:

```text
512 entradas
```

Cada entrada debe contener:

|Campo|Bits|
|---|---|
|Número de marco físico|4 bits|
|Bit de validez (V)|1 bit|

Tamaño mínimo por entrada:

```text
5 bits
```

Tamaño mínimo de la tabla:

```text
512 × 5 = 2560 bits
```

equivalente a:

```text
320 bytes
```

---

## 3. Contenido de la tabla de páginas

La memoria física contiene:

|Marco|Dirección física|Página virtual|
|---|---|---|
|0|`0x0000-0x01FF`|—|
|1|`0x0200-0x03FF`|—|
|2|`0x0400-0x05FF`|—|
|3|`0x0600-0x07FF`|34|
|4|`0x0800-0x09FF`|9|
|5|`0x0A00-0x0BFF`|—|
|6|`0x0C00-0x0DFF`|—|
|7|`0x0E00-0x0FFF`|65|
|8|`0x1000-0x11FF`|—|
|9|`0x1200-0x13FF`|10|

Por tanto, la tabla de páginas contiene:

|Página virtual|Marco físico|Válida|
|---|---|---|
|9|4|1|
|10|9|1|
|34|3|1|
|65|7|1|
|Resto|—|0|

---

## 4. Traducción de direcciones virtuales

### Dirección virtual `0x13FF`

#### Obtener VPN y offset

Tamaño de página:

```text
0x200 = 512 palabras
```

VPN:

```text
VPN = 0x13FF / 0x200 = 9
```

Offset:

```text
Offset = 0x13FF mod 0x200 = 0x1FF
```

Consulta de la tabla:

```text
VPN 9 → Marco 4
```

Dirección física:

```text
PF = 4 × 0x200 + 0x1FF
```

```text
PF = 0x800 + 0x1FF
```

Resultado:

```text
0x09FF
```

---

### Dirección virtual `0x1403`

VPN:

```text
VPN = 0x1403 / 0x200 = 10
```

Offset:

```text
Offset = 0x1403 mod 0x200 = 0x003
```

Consulta:

```text
VPN 10 → Marco 9
```

Dirección física:

```text
PF = 9 × 0x200 + 0x003
```

```text
PF = 0x1200 + 0x003
```

Resultado:

```text
0x1203
```

---

## 5. Acceso a la dirección virtual `0x80E8`

VPN:

```text
VPN = 0x80E8 / 0x200 = 64
```

Offset:

```text
Offset = 0x80E8 mod 0x200 = 0x0E8
```

Consultando la tabla:

|VPN|Marco|
|---|---|
|9|4|
|10|9|
|34|3|
|65|7|

La página virtual **64 no está presente**.

Por tanto:

```text
Bit de validez = 0
```

El acceso provoca una:

# **Falta de página (Page Fault)**

El sistema operativo recibirá una excepción y podrá:

1. Buscar la página en memoria secundaria y cargarla en un marco libre.
    
2. Sustituir otra página si no hay marcos disponibles.
    
3. Terminar el proceso con un error de segmentación si el acceso es inválido.
    

---

## 6. Compartición del marco `0x0800-0x09FF`

Este marco corresponde a:

```text
Marco físico 4
Página virtual 9
```

Si se quiere compartir con otro proceso:

## ¿Debe asignarse al mismo segmento virtual?

**No necesariamente.**

La memoria virtual proporciona independencia entre:

- direcciones virtuales,
    
- direcciones físicas.
    

Dos procesos distintos pueden tener:

|Proceso|Página virtual|Marco físico|
|---|---|---|
|A|9|4|
|B|25|4|

Ambos accederían al mismo contenido físico mediante diferentes direcciones virtuales.

### Conclusión

- **No es obligatorio utilizar el mismo segmento virtual.**
    
- Basta con que las entradas de las tablas de páginas de ambos procesos apunten al mismo marco físico.
    
- Utilizar la misma dirección virtual puede simplificar la programación, pero no es un requisito del sistema operativo.
    

---

## Resumen

- Dirección virtual: **18 bits** (9 VPN + 9 offset).
    
- Dirección física: **13 bits** (4 PFN + 9 offset).
    
- Tabla de páginas: **512 entradas**.
    
- `0x13FF → 0x09FF`
    
- `0x1403 → 0x1203`
    
- `0x80E8` produce una **falta de página**.
    
- La memoria compartida puede mapearse en **diferentes direcciones virtuales** en cada proceso.

---

#### 💻Ejercicio 7. Considera el siguiente programa:

``` C
#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

  

int main(int argc, char *argv[]) {

    int factor    = atoi(argv[1]);

    int elementos = factor * 1024;

    printf("Inicializando región de memoria\n");

    int * ptr = (int *) malloc(elementos * sizeof(int));

    for (size_t i = 0; i < elementos; i++){

        ptr[i] = 1;

    }

    sleep(600);

    return 0;

}
```

Realiza dos ejecuciones con el comando strace para identificar cómo se reserva la memoria del array ptr. Consulta el mapa de memoria del proceso en /proc para completar la siguiente tabla para la región de memoria dónde está la memoria del array. Nota: lee la sección NOTES de la página de manual de malloc(3).

- Caso A. strace ./ejercicio7 1
    
- Caso B. strace ./ejercicio7 1024
    

  

|   |   |   |   |   |   |
|---|---|---|---|---|---|
|Caso|Dirección<br><br>ptr|Rango irecciones|Flags|Tipo Segmento|Mecanismo Memoria Dinámica|
|A||||||
|B||||||

Respuesta:

# Ejercicio 7 - Reserva de memoria dinámica mediante `malloc()`

## Objetivo

Analizar cómo `malloc()` reserva memoria dinámica dependiendo del tamaño solicitado, utilizando `strace` y examinando el mapa de memoria del proceso en `/proc/<pid>/maps`.

El programa utilizado es:

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int factor    = atoi(argv[1]);
    int elementos = factor * 1024;

    printf("Inicializando región de memoria\n");

    int * ptr = (int *) malloc(elementos * sizeof(int));

    for (size_t i = 0; i < elementos; i++) {
        ptr[i] = 1;
    }

    sleep(600);

    return 0;
}
```

---

# Funcionamiento de `malloc()`

Según la página de manual de `malloc(3)`, glibc utiliza dos mecanismos distintos para reservar memoria:

1. **`brk()` / `sbrk()`**
    
    - Para asignaciones pequeñas.
        
    - Amplía el **heap** del proceso.
        
2. **`mmap()`**
    
    - Para asignaciones grandes (normalmente mayores de 128 KiB).
        
    - Crea una nueva región anónima independiente.
        

---

# Caso A: `strace ./Ejercicio7.exe 1`

## Tamaño solicitado

```text
factor = 1
elementos = 1 × 1024 = 1024 enteros
```

Cada entero ocupa:

```text
sizeof(int) = 4 bytes
```

Memoria total:

```text
1024 × 4 = 4096 bytes = 4 KiB
```

---

## Evidencia obtenida con `strace`

Se observa:

```text
brk(NULL)                 = 0x564da8f4b000
brk(0x564da8f6c000)       = 0x564da8f6c000
```

No aparece ningún `mmap()` adicional después del mensaje:

```text
Inicializando región de memoria
```

Por tanto, `malloc()` utiliza:

```text
Heap → brk()
```

---

## Identificación en `/proc/<pid>/maps`

La memoria reservada estará dentro del segmento:

```text
[heap]
```

con permisos:

```text
rw-p
```

---

# Caso B: `strace ./Ejercicio7.exe 1024`

## Tamaño solicitado

```text
factor = 1024
elementos = 1024 × 1024 = 1 048 576 enteros
```

Memoria total:

```text
1 048 576 × 4 = 4 194 304 bytes
```

equivalente a:

```text
4 MiB
```

---

## Evidencia obtenida con `strace`

Después del mensaje:

```text
Inicializando región de memoria
```

aparece:

```text
mmap(NULL, 4198400,
     PROT_READ|PROT_WRITE,
     MAP_PRIVATE|MAP_ANONYMOUS,
     -1, 0)
     = 0x7f372f3ff000
```

La memoria solicitada mediante `malloc()` se obtiene usando:

```text
mmap()
```

El tamaño solicitado es ligeramente superior a 4 MiB debido a la información de gestión interna utilizada por glibc.

---

## Identificación en `/proc/<pid>/maps`

La región aparecerá como un segmento anónimo:

```text
7f372f3ff000-XXXXXXXX rw-p 00000000 00:00 0
```

No formará parte del heap.

---

# Tabla resumen

|Caso|Dirección `ptr`|Rango direcciones|Flags|Tipo de segmento|Mecanismo de memoria dinámica|
|---|---|---|---|---|---|
|**A** (`./Ejercicio7 1`)|Dentro del heap|Región `[heap]`|`rw-p`|Heap|`brk()`|
|**B** (`./Ejercicio7 1024`)|`0x7f372f3ff000`|`0x7f372f3ff000 - 0x7f372f800000` (aprox.)|`rw-p`|Segmento anónimo|`mmap()`|

---

# Explicación de los resultados

Para reservas pequeñas, ampliar el heap mediante `brk()` es más eficiente porque:

- reutiliza la región ya existente,
    
- tiene menor sobrecarga administrativa,
    
- reduce el número de segmentos virtuales.
    

Para reservas grandes, `malloc()` utiliza `mmap()` porque:

- permite liberar la memoria completamente mediante `munmap()`,
    
- evita la fragmentación del heap,
    
- facilita la gestión de grandes bloques independientes.
    

---

# Conclusiones

- **Caso A (4 KiB):** `malloc()` utiliza el **heap** mediante la llamada al sistema **`brk()`**.
    
- **Caso B (≈ 4 MiB):** `malloc()` utiliza una **región anónima obtenida con `mmap()`**.
    
- El umbral exacto a partir del cual glibc cambia de estrategia depende de la implementación, aunque típicamente se sitúa alrededor de **128 KiB** (`M_MMAP_THRESHOLD`).
    
- El análisis mediante `strace` y `/proc/<pid>/maps` permite observar directamente el mecanismo utilizado por el gestor de memoria dinámica.


``` bash
~/Documentos/SO/EjerciciosSO/Cuarta-hoja main*
❯ strace ./Ejercicio7.exe 1
execve("./Ejercicio7.exe", ["./Ejercicio7.exe", "1"], 0x7ffc57d25078 /* 74 vars */) = 0
brk(NULL)                               = 0x564da8f4b000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No existe el fichero o el directorio)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=189235, ...}) = 0
mmap(NULL, 189235, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f3011ce8000
close(3)                                = 0
openat(AT_FDCWD, "/usr/lib/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\320~\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 896, 64) = 896
fstat(3, {st_mode=S_IFREG|0755, st_size=2411968, ...}) = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f3011ce6000
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 896, 64) = 896
mmap(NULL, 2435952, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f3011a00000
mmap(0x7f3011a24000, 1744896, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x24000) = 0x7f3011a24000
mmap(0x7f3011bce000, 487424, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1ce000) = 0x7f3011bce000
mmap(0x7f3011c45000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x244000) = 0x7f3011c45000
mmap(0x7f3011c4b000, 31600, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f3011c4b000
close(3)                                = 0
mmap(NULL, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f3011ce3000
arch_prctl(ARCH_SET_FS, 0x7f3011ce3740) = 0
set_tid_address(0x7f3011ce3d68)         = 10591
set_robust_list(0x7f3011ce3a20, 24)     = 0
rseq({cpu_id_start=0, cpu_id=RSEQ_CPU_ID_UNINITIALIZED, rseq_cs=NULL, flags=0, node_id=0, mm_cid=0, slice_ctrl={request=0, granted=0, __reserved=0}, __reserved=0}, 33, 0, 0x53053053) = 0
mprotect(0x7f3011c45000, 16384, PROT_READ) = 0
mprotect(0x564d7928d000, 4096, PROT_READ) = 0
mprotect(0x7f3011d5d000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
getrandom("\x5e\xd1\xf1\x4e\xde\xe8\xd4\xbc", 8, GRND_NONBLOCK) = 8
munmap(0x7f3011ce8000, 189235)          = 0
fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}) = 0
brk(NULL)                               = 0x564da8f4b000
brk(0x564da8f6c000)                     = 0x564da8f6c000
write(1, "Inicializando regi\303\263n de memoria"..., 33Inicializando región de memoria
) = 33
clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=600, tv_nsec=0}

^C, {tv_sec=584, tv_nsec=272541523}) = ? ERESTART_RESTARTBLOCK (Interrupted by signal)
strace: Process 10591 detached
```

``` bash
~/Documentos/SO/EjerciciosSO/Cuarta-hoja main* 15s
❯ strace ./Ejercicio7.exe 1024
execve("./Ejercicio7.exe", ["./Ejercicio7.exe", "1024"], 0x7ffcd204f6e8 /* 74 vars */) = 0
brk(NULL)                               = 0x564139100000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No existe el fichero o el directorio)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=189235, ...}) = 0
mmap(NULL, 189235, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f372fb92000
close(3)                                = 0
openat(AT_FDCWD, "/usr/lib/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\320~\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 896, 64) = 896
fstat(3, {st_mode=S_IFREG|0755, st_size=2411968, ...}) = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f372fb90000
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 896, 64) = 896
mmap(NULL, 2435952, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f372f800000
mmap(0x7f372f824000, 1744896, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x24000) = 0x7f372f824000
mmap(0x7f372f9ce000, 487424, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1ce000) = 0x7f372f9ce000
mmap(0x7f372fa45000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x244000) = 0x7f372fa45000
mmap(0x7f372fa4b000, 31600, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f372fa4b000
close(3)                                = 0
mmap(NULL, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f372fb8d000
arch_prctl(ARCH_SET_FS, 0x7f372fb8d740) = 0
set_tid_address(0x7f372fb8dd68)         = 10709
set_robust_list(0x7f372fb8da20, 24)     = 0
rseq({cpu_id_start=0, cpu_id=RSEQ_CPU_ID_UNINITIALIZED, rseq_cs=NULL, flags=0, node_id=0, mm_cid=0, slice_ctrl={request=0, granted=0, __reserved=0}, __reserved=0}, 33, 0, 0x53053053) = 0
mprotect(0x7f372fa45000, 16384, PROT_READ) = 0
mprotect(0x564121903000, 4096, PROT_READ) = 0
mprotect(0x7f372fc07000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
getrandom("\xcb\x8f\xfd\xe6\xb4\x9e\x65\x06", 8, GRND_NONBLOCK) = 8
munmap(0x7f372fb92000, 189235)          = 0
fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}) = 0
brk(NULL)                               = 0x564139100000
brk(0x564139121000)                     = 0x564139121000
write(1, "Inicializando regi\303\263n de memoria"..., 33Inicializando región de memoria
) = 33
mmap(NULL, 4198400, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f372f3ff000
clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=600, tv_nsec=0}^C, {tv_sec=597, tv_nsec=528697066}) = ? ERESTART_RESTARTBLOCK (Interrupted by signal)
strace: Process 10709 detached
```

---

#### Ejercicio 8.  Un sistema tiene un uso medio de la CPU del 15% (usuario) y 3% (sistema), el área de swap está ocupada al 92%. La salida del comando ps muestra 50 procesos en estado “D” (espera no interrumpible) . ¿Cuál de estas acciones aumentará más la utilización de la CPU?:

- Ampliar la memoria principal
    
- Ejecutar más programas para aumentar el grado de multiprogramación 
    
- Aumentar el área de swap
    
- Añadir más CPU’s

Respuesta:

## Datos proporcionados

El sistema presenta las siguientes características:

- **Uso medio de CPU (modo usuario):** 15%
    
- **Uso medio de CPU (modo sistema):** 3%
    
- **Uso total de CPU:**
    

```text
15% + 3% = 18%
```

Por tanto, la CPU está inactiva aproximadamente:

```text
100% - 18% = 82%
```

- **Área de swap ocupada:** 92%
    
- **Procesos en estado D (espera no interrumpible):** 50 procesos
    

---

## Interpretación de los datos

### Baja utilización de CPU

La CPU solo está ocupada el **18% del tiempo**, lo que indica que **no es el recurso limitante** del sistema.

---

### Alta ocupación del área de swap

Una utilización del **92% de swap** indica una fuerte presión sobre la memoria principal.

Esto suele significar que:

- muchos procesos tienen páginas intercambiadas al disco,
    
- se producen numerosas faltas de página,
    
- el sistema dedica gran parte del tiempo a operaciones de E/S relacionadas con memoria virtual.
    

---

### Procesos en estado "D"

El estado **D (Uninterruptible Sleep)** indica que el proceso está esperando la finalización de una operación de E/S que no puede interrumpirse.

Tener **50 procesos en estado D** sugiere que:

- existe una importante congestión en las operaciones de entrada/salida,
    
- muchos procesos están bloqueados esperando acceso al disco,
    
- el sistema podría estar sufriendo **thrashing**.
    

---

## ¿Qué es el thrashing?

El **thrashing** ocurre cuando el sistema dedica más tiempo a intercambiar páginas entre memoria principal y swap que a ejecutar procesos útiles.

Sus síntomas son precisamente:

- baja utilización de CPU,
    
- elevada utilización del área de swap,
    
- numerosos procesos bloqueados esperando E/S.
    

---

# Evaluación de las alternativas

## 1. Ampliar la memoria principal

### Efectos

- Reduce la necesidad de utilizar swap.
    
- Disminuye el número de faltas de página.
    
- Reduce las operaciones de E/S hacia el disco.
    
- Permite que más procesos permanezcan completamente residentes en memoria.
    

### Resultado

La CPU podrá ejecutar más procesos y pasará menos tiempo esperando operaciones de memoria virtual.

✅ **Incrementa significativamente la utilización de la CPU.**

---

## 2. Ejecutar más programas para aumentar el grado de multiprogramación

### Efectos

- Aumenta el número de procesos compitiendo por memoria.
    
- Incrementa aún más la presión sobre el sistema de paginación.
    
- Puede agravar el thrashing.
    

### Resultado

Probablemente empeorará el rendimiento global.

❌ **No es recomendable.**

---

## 3. Aumentar el área de swap

### Efectos

- Proporciona más espacio para almacenar páginas intercambiadas.
    
- No reduce el número de accesos al disco.
    
- No soluciona la escasez de memoria física.
    

### Resultado

El sistema seguirá realizando muchas operaciones de swapping.

❌ **No mejora significativamente la utilización de CPU.**

---

## 4. Añadir más CPUs

### Efectos

- Incrementa la capacidad de procesamiento.
    
- Sin embargo, la CPU actual ya está infrautilizada (18%).
    

### Resultado

Los nuevos procesadores permanecerán mayoritariamente ociosos mientras los procesos continúen esperando E/S.

❌ **No resuelve el cuello de botella existente.**

---

# Respuesta

La acción que **más aumentará la utilización de la CPU** es:

## ✅ Ampliar la memoria principal.

---

# Justificación final

Los indicadores muestran claramente un escenario de **thrashing**:

- CPU poco utilizada (**18%**).
    
- Uso muy elevado del área de swap (**92%**).
    
- Gran cantidad de procesos bloqueados en E/S (**50 procesos en estado D**).
    

El problema principal es la **falta de memoria física**, no la capacidad de procesamiento ni el tamaño del área de intercambio.

Por tanto, la solución más efectiva consiste en:

> **Aumentar la memoria principal para reducir el swapping y permitir que la CPU permanezca ocupada ejecutando procesos útiles.**


---

#### Ejercicio 9.  En un sistema con memoria virtual paginada indicar las acciones son realizadas por el sistema operativo (especificando a qué estructuras de datos accede y como las modifica) en los siguiente casos: 

- un proceso intenta escribir en una página de solo lectura
    
- un proceso intenta acceder a una dirección correspondiente a una página que no está en memoria.

Respuesta:

# Ejercicio 9 - Acciones del Sistema Operativo en un sistema con memoria virtual paginada

En un sistema con memoria virtual paginada, el hardware de gestión de memoria (MMU) verifica en cada acceso si la operación solicitada es válida según la información almacenada en la tabla de páginas del proceso. Cuando detecta una situación anómala, genera una excepción (_trap_) que es atendida por el sistema operativo.

---

# 1. Un proceso intenta escribir en una página de solo lectura

## Situación inicial

La MMU traduce la dirección virtual consultando la entrada correspondiente en la **tabla de páginas** del proceso.

La entrada contiene, entre otros, los siguientes campos:

- Número de marco físico.
    
- Bit de presencia (Present/Valid).
    
- Bits de protección (lectura, escritura, ejecución).
    
- Bit de acceso (_Referenced_).
    
- Bit de modificación (_Dirty_).
    

Supongamos que la entrada tiene:

```text
Presente = 1
Permiso escritura = 0
```

---

## Acciones realizadas

### 1. La MMU detecta una violación de protección

Al intentar ejecutar una escritura:

```c
pagina[x] = valor;
```

la MMU comprueba que:

```text
Escritura permitida = NO
```

y genera una excepción de **fallo de protección** (_Protection Fault_).

---

### 2. Cambio al modo kernel

El procesador:

- Guarda el contexto del proceso:
    
    - contador de programa (PC),
        
    - registros,
        
    - código de error de la excepción.
        
- Cambia al modo privilegiado.
    
- Invoca el manejador de excepciones del sistema operativo.
    

---

### 3. El SO examina la causa del fallo

El manejador accede a:

- Registro de dirección causante del fallo (CR2 en x86).
    
- Código de error proporcionado por la CPU.
    
- Tabla de páginas del proceso.
    

Comprueba que:

```text
Página presente = Sí
Permiso de escritura = No
```

---

### 4. Actuación del sistema operativo

Existen dos posibilidades:

#### a) Violación real de protección

Si la página es realmente de solo lectura:

- El SO envía una señal al proceso:
    

```text
SIGSEGV (Segmentation Fault)
```

- El proceso termina si no captura la señal.
    

---

#### b) Implementación Copy-On-Write (COW)

Si la página es compartida mediante **Copy-On-Write**:

El SO:

1. Reserva un nuevo marco físico.
    
2. Copia el contenido de la página original.
    
3. Modifica la tabla de páginas:
    
    - nuevo marco físico,
        
    - activa permiso de escritura.
        
4. Actualiza la TLB eliminando la traducción antigua.
    
5. Reanuda la ejecución del proceso.
    

---

## Estructuras modificadas

|Estructura|Modificación|
|---|---|
|Tabla de páginas|Actualización de permisos o marco (COW)|
|TLB|Invalidación de la entrada antigua|
|Lista de marcos libres|Asignación de un nuevo marco (COW)|
|PCB del proceso|Señal pendiente (`SIGSEGV`) si procede|

---

# 2. Un proceso intenta acceder a una página que no está en memoria

## Situación inicial

La MMU consulta la tabla de páginas.

La entrada correspondiente tiene:

```text
Bit de presencia = 0
```

lo que indica que la página virtual no está cargada en memoria principal.

---

## Acciones realizadas

### 1. Generación de un Page Fault

La MMU detecta que:

```text
Present = 0
```

y genera una excepción de:

# **Falta de página (Page Fault)**

---

### 2. Cambio a modo kernel

La CPU:

- Guarda el contexto del proceso.
    
- Almacena la dirección virtual causante del fallo.
    
- Transfiere el control al manejador de faltas de página.
    

---

### 3. Verificación de validez

El sistema operativo consulta:

- Tabla de páginas.
    
- Información del espacio de direcciones del proceso.
    

Determina si la dirección pertenece a:

- código,
    
- datos,
    
- pila,
    
- heap,
    
- memoria mapeada.
    

---

### 4. Si el acceso es inválido

Por ejemplo:

```text
puntero = NULL;
*puntero = 5;
```

El SO:

- genera una señal:
    

```text
SIGSEGV
```

- finaliza el proceso.
    

---

### 5. Si la página es válida pero no residente

El SO debe traerla desde memoria secundaria.

#### a) Buscar un marco libre

Consulta:

- lista de marcos libres,
    
- bitmap de memoria física.
    

---

#### b) Si no hay marcos libres

Se ejecuta el algoritmo de reemplazo:

Ejemplos:

- FIFO.
    
- LRU.
    
- Clock.
    

Se selecciona una página víctima.

---

#### c) Si la página víctima está modificada

Si:

```text
Dirty bit = 1
```

el SO:

1. Escribe la página al área de swap o al fichero correspondiente.
    
2. Espera a que termine la operación de E/S.
    

---

#### d) Leer la nueva página

Se inicia una operación de disco para cargar la página requerida:

```text
Swap/Fichero → Marco físico
```

---

#### e) Actualizar estructuras

El SO modifica:

##### Tabla de páginas

Para la nueva página:

```text
Presente = 1
Marco = nuevo marco
```

Para la página expulsada:

```text
Presente = 0
```

---

##### TLB

Se invalida la entrada antigua.

Posteriormente se cargará automáticamente la nueva traducción.

---

##### Información del marco

Se actualizan:

- propietario del marco,
    
- bits de referencia,
    
- bits de modificación.
    

---

### 6. Reanudar el proceso

Una vez cargada la página:

- se restaura el contexto,
    
- se repite automáticamente la instrucción que causó el fallo.
    

El proceso continúa normalmente.

---

## Estructuras modificadas

|Estructura|Modificación|
|---|---|
|Tabla de páginas|Actualización de bits de presencia y marco|
|TLB|Invalidación/actualización|
|Tabla de marcos|Asignación o reemplazo|
|Lista de marcos libres|Reserva/liberación|
|Área de swap|Escritura/lectura de páginas|
|PCB del proceso|Estado bloqueado mientras espera E/S|

---

# Resumen comparativo

|Situación|Acción principal del SO|
|---|---|
|Escritura sobre página de solo lectura|Generar `SIGSEGV` o realizar **Copy-On-Write**|
|Acceso a página no residente|Gestionar una **falta de página** cargando la página desde memoria secundaria|

La diferencia fundamental es que una **violación de protección** suele indicar un acceso ilegal, mientras que una **falta de página** es un mecanismo normal de funcionamiento de la memoria virtual paginada.

---

Ejercicio 10. Considera el siguiente código e indique razonadamente si cada una de las afirmaciones posteriores son ciertas o falsas. Asuma que todos los segmentos de memoria del proceso asociado (texto, datos, pila,...) ocupa  una página de 4 Kb.

```C
int main() {
    int i;
    int M[128];
    int x,y;
    x = M[0];
    y = 0;
    for (i=1; i < 200; i++) {
        y = x + M[i];
        M[i] = y;
    }
    return 0;
}
```

- El proceso provocará una excepción por violación de segmento (SIGSEV) al ejecutarlo porque accede a elementos de M no reservados.
- La ejecución del proceso  corromperá los datos de la pila.
- La ejecución del proceso puede corromper el segmento de código del programa. 
- Si se modifica el bucle para que recorra 4096 posiciones del array M se producirá una excepción.

Respuesta:

# Ejercicio 10 - Análisis de accesos fuera de límites en la pila

Código proporcionado:

```c
int main() {
    int i;
    int M[128];
    int x, y;

    x = M[0];
    y = 0;

    for (i = 1; i < 200; i++) {
        y = x + M[i];
        M[i] = y;
    }

    return 0;
}
```

Se asume que:

- Cada segmento del proceso ocupa **una página de 4 KiB**.
    
- `sizeof(int) = 4 bytes`.
    

---

## Análisis previo

El array declarado es:

```c
int M[128];
```

Su tamaño es:

```text
128 × 4 bytes = 512 bytes
```

Por tanto, `M` ocupa:

```text
512 bytes < 4096 bytes
```

Todo el array cabe dentro de **una única página de pila**.

Sin embargo, el bucle accede hasta:

```c
M[199]
```

es decir:

```text
200 × 4 = 800 bytes
```

La escritura máxima será:

```text
M[199] → desplazamiento = 199 × 4 = 796 bytes
```

que sigue siendo menor que:

```text
4096 bytes
```

Por tanto, los accesos fuera de los límites del array siguen realizándose dentro de la misma página de pila.

---

## 1. El proceso provocará una excepción por violación de segmento (SIGSEGV) al ejecutarlo porque accede a elementos de M no reservados.

### Respuesta:

❌ **FALSO**

### Justificación:

Aunque el programa accede fuera de los límites lógicos del array:

```c
M[128], M[129], ..., M[199]
```

el hardware de memoria virtual **no conoce el tamaño del array**.

La MMU únicamente verifica que:

- la dirección virtual pertenezca a una página válida,
    
- existan permisos adecuados.
    

Como todas las posiciones accedidas permanecen dentro de la misma página de pila:

```text
0 ≤ desplazamiento ≤ 796 < 4096
```

no se produce ninguna excepción.

El acceso es **comportamiento indefinido en C**, pero no implica necesariamente un `SIGSEGV`.

---

## 2. La ejecución del proceso corromperá los datos de la pila.

### Respuesta:

✅ **VERDADERO**

### Justificación:

Las posiciones:

```c
M[128] ... M[199]
```

no pertenecen al array.

Estas escrituras modificarán otras variables o estructuras almacenadas en la pila, por ejemplo:

- variables locales (`i`, `x`, `y`),
    
- registros salvados,
    
- punteros de marco (`frame pointer`),
    
- dirección de retorno de la función.
    

Por tanto, el programa **puede corromper el contenido de la pila**.

---

## 3. La ejecución del proceso puede corromper el segmento de código del programa.

### Respuesta:

❌ **FALSO**

### Justificación:

El segmento de código (`.text`):

- se encuentra en una región distinta del espacio de direcciones,
    
- normalmente posee permisos:
    

```text
r-x
```

(lectura y ejecución, sin escritura).

Los accesos realizados desde `M` afectan únicamente a direcciones cercanas dentro de la pila.

No existe ninguna posibilidad razonable de alcanzar el segmento de código con un desbordamiento de solo:

```text
800 bytes
```

Por tanto, **no puede corromper el segmento de código**.

---

## 4. Si se modifica el bucle para que recorra 4096 posiciones del array M se producirá una excepción.

Supongamos:

```c
for (i = 1; i < 4096; i++)
```

### Análisis

La última posición accedida sería:

```text
M[4095]
```

Dirección relativa:

```text
4095 × 4 = 16380 bytes
```

Esto equivale aproximadamente a:

```text
16380 / 4096 ≈ 4 páginas
```

El array original solo ocupa:

```text
512 bytes
```

Por tanto, el programa accederá mucho más allá de la página inicialmente asignada para la pila.

Como el enunciado especifica que:

> "todos los segmentos ocupan una página de 4 KiB"

la pila dispone únicamente de una página válida.

Al intentar acceder a una dirección perteneciente a páginas no mapeadas:

- la MMU generará un **Page Fault**,
    
- el SO comprobará que la dirección es inválida,
    
- enviará una señal:
    

```text
SIGSEGV
```

### Respuesta:

✅ **VERDADERO**

---

# Resumen

|Afirmación|Respuesta|Justificación|
|---|---|---|
|El proceso provocará `SIGSEGV` porque accede a elementos no reservados.|❌ Falso|Los accesos permanecen dentro de la misma página de pila.|
|La ejecución corromperá datos de la pila.|✅ Verdadero|Se sobrescriben posiciones fuera del array.|
|Puede corromper el segmento de código.|❌ Falso|El código está en otra región protegida y distante.|
|Si el bucle recorre 4096 posiciones se producirá una excepción.|✅ Verdadero|Se accederá a páginas no asignadas, generando `SIGSEGV`.|

## Conclusión

El sistema operativo protege **páginas de memoria**, no variables individuales. Por ello, un acceso fuera de los límites de un array no provoca necesariamente una excepción; solo se detectará cuando el acceso alcance una región virtual no válida o sin permisos adecuados.
