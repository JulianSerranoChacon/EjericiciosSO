
#### 💻 Ejercicio 1. ls(1) muestra el contenido de directorios y los atributos básicos de los ficheros. Consultar la página de manual y estudiar el uso de las opciones -a -l -d -h -i -R -1 -F.

| Opción   | Descripción                                                                             | Ejemplo        |
| -------- | --------------------------------------------------------------------------------------- | -------------- |
| **`-a`** | Muestra todos los archivos, incluidos los ocultos (los que comienzan por `.`).          | `ls -a`        |
| **`-l`** | Formato largo: muestra permisos, número de enlaces, propietario, grupo, tamaño y fecha. | `ls -l`        |
| **`-d`** | Muestra información sobre el directorio en sí, no sobre su contenido.                   | `ls -ld /home` |
| **`-h`** | Con `-l`, muestra tamaños legibles para humanos (KB, MB, GB...).                        | `ls -lh`       |
| **`-i`** | Muestra el número de inodo de cada archivo.                                             | `ls -i`        |
| **`-R`** | Lista recursivamente todos los subdirectorios.                                          | `ls -R`        |
| **`-1`** | Muestra un archivo por línea.                                                           | `ls -1`        |
| **`-F`** | Añade un indicador al final del nombre según el tipo de archivo.                        | `ls -F`        |
### Indicadores de `-F`

| Símbolo | Significado        |
| ------- | ------------------ |
| `/`     | Directorio         |
| `*`     | Archivo ejecutable |
| `@`     | Enlace simbólico   |
| `       | `                  |
| `=`     | Socket             |

---

#### 💻 Ejercicio 2.  Considerar el directorio /run:

- Escribir un comando que obtenga  las características detalladas de todos los ficheros.
    
- ¿Cuántos tipos de ficheros (directorio, fichero regular, enlace…) diferentes hay?
    
- Escribir una línea de comandos que muestre qué  tipos diferentes hay en ese directorio. Los tipos se indicarán con el caracter (d, -, l...) usado por el comando ls. Ejemplo:

![[Pasted image 20260603183055.png]]


Respuesta:

- ls -la meustra todos los ficheros de manera detallada

![[Pasted image 20260603184623.png]]

- ls -lA /run | tail -n +2 | cut -c1 | sort -u | wc -l muestra el numero de ficheros diferentes que hay.

![[Pasted image 20260603184707.png]]

- ls -lA| tail -n +2 | cut -c1 | sort -u| wc -l muestra los tipos de ficheros ordenados

![[Pasted image 20260603184730.png]]


---

#### 💻 Ejercicio 3.  Crear un fichero en el directorio home del usuario con la orden:

echo “Fichero de prueba” > archivo.txt

Usando el comando stat(1) contestar a las siguientes preguntas:

- ¿Cuál es el inodo del fichero?
    
- ¿Qué usuario puede leer los contenidos del fichero? ¿y modificarlo?
    
- ¿Cuál es el tamaño del fichero? ¿Cuántos bloques ocupa en el sistema de ficheros?
    
- ¿Cuál es la ruta del disco (relativa a /dev) dónde se almacena el archivo?
    
- Determinar qué atributos del inodo del fichero cambian cuando se ejecutan los siguientes comandos:
    

- touch archivo.txt
    
- echo “Otra línea más en el fichero” >> archivo.txt
    

cat archivo.txt


respuesta: 

![[Pasted image 20260603185841.png]]

- El inodo del fichero es 1948663

- Los permisos son los siguientes:
	- Propietario (`verxxuxz`): `rw-` → puede leer y escribir.
	- Grupo (`verxxuxz`): `r--` → solo puede leer.
	- Otros usuarios: `r--` → solo pueden leer.
	
- En cualto al uso en disco:
	- Tamaño: **15 bytes**.
	- Bloques ocupados: **8 bloques de 512 bytes** (así los muestra `stat` normalmente en Linux).
- Para la ruta relativa he usado el comando df:

	![[Pasted image 20260603191130.png]]

	- del que podemos sacar que la ruta relativa del disco está en /dev/sda1, que está montado en /home y el uso del disco está al 64%.

- Con los siguientes comando vemos las siguientes reacciones:
	- Con touch archivo txt cambian:
		- el tiempo de acceso(atime),
		- el tiempo de modificación(mtime).
		- el tiempo de cambio (ctime).
	  ![[Pasted image 20260603191501.png]]
	- Con echo “Otra línea más en el fichero” >> archivo.txt cambian:
		- Tamaño
		- Bloques ocupados (si es necesario)
		- Modificación (mtime)
		- Cambio (ctime)
		
		![[Pasted image 20260603191929.png]]
	- Con cat archivo.txt cambian:
		- Acceso (atime) (solo lee el fichero)
		![[Pasted image 20260603192209.png]]

--- 

#### 💻 Ejercicio 4. Escribir un programa (mistat) que emule el comportamiento de stat(1). El programa aceptará un único argumento que será la ruta del fichero del que se quiere obtener la información. Si el fichero no existe se informará del error. La información del fichero será:

- El número major y minor asociado al dispositivo, ver major(3)/minor(3).
    
- El número de inodo del fichero.
    
- El tipo de fichero (únicamente considerar directorio, enlace simbólico o fichero ordinario).
    
- La hora en la que se accedió el fichero por última vez. ¿Qué diferencia hay entre st_mtime y st_ctime?
    

Ejemplo de salida:

$ ./mistat /no/existe

Error en stat: No such file or directory

$ ./mistat archivo.txt 

inodo: 256395

Número de dispositivo: [8,1]

Tipo: fichero regular

Último cambio de estado: Fri Feb 19 10:51:56 2025

Último acceso: Fri Feb 19 10:53:15 2025

Nota: Si el archivo es un enlace simbólico debe mostrar la información del enlace y no del fichero al que apunta.

Respuesta:

```C
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>
#include <unistd.h> 

int main(int argc, char *argv[]){

    struct stat mstat;
    if(argc < 2){
        fprintf(stderr,"Uso: %s <ruta> \n",argv[0]);
        return 1;
    }

    if(stat(argv[1],&mstat) == -1){
        fprintf(stderr,"Se ha producido un eror con la ruta del archivo: ",argv[1]);
        printf("\n");
        return 1;
    }

    printf("Inode: %d \n",mstat.st_ino);
    printf("Numero de dispositivo: [%d,%d]\n", major(mstat.st_dev),minor(mstat.st_dev));
    printf("Permisos: %o\n",  mstat.st_mode & 0777);

    printf("Tipo: ");
    if(S_ISREG(mstat.st_mode)) printf("fichero regular\n");
    else if(S_ISDIR(mstat.st_mode)) printf("directorio\n");
    else if(S_ISLNK(mstat.st_mode)) printf("enlace simbólico\n");
    else if(S_ISCHR(mstat.st_mode)) printf("dispositivo de caracteres\n");
    else if(S_ISBLK(mstat.st_mode)) printf("dispositivo de bloques\n");
    else if(S_ISFIFO(mstat.st_mode)) printf("tubería (FIFO)\n");
    else if(S_ISSOCK(mstat.st_mode)) printf("socket\n");
    else printf("desconocido\n");

    printf("Último acceso: %s", ctime(&mstat.st_atime));
    printf("Última modificación: %s", ctime(&mstat.st_mtime));
    printf("Último cambio de estado: %s", ctime(&mstat.st_ctime));

    return 0;
}
```

![[Pasted image 20260605205221.png]]

---


#### Ejercicio 5. Los permisos se pueden otorgar de forma selectiva usando la notación octal o la simbólica. Ejemplo, probar las siguientes órdenes (equivalentes):

- chmod 540 fichero
    
- chmod u=rx,g=r,o= fichero
    

Consultar la página de manual chmod(1) para ver otras formas de fijar los permisos (p.ej. los operadores + y -). Contesta las siguientes preguntas:

- ¿Cómo se podrían fijar los permisos rw-r--r-x usando notación octal? 
    
- ¿Qué permisos debería fijar para que el usuario y grupo puedan leer y escribir y el resto de usuario nos tengan acceso?
    
-  Considera la siguiente salida del comando ls:
    
```
$ ls -ld /tmp/ /usr/bin/passwd 

drwxrwxrwt 12 root root  4096 Fre 12 06:48 /tmp/

-rwsr-xr-x  1 root root 59976 Nov 24  2022 /usr/bin/passwd
```

¿Qué permisos tiene el directorio /tmp y el fichero /usr/bin/passwd en notación octal? ¿Cuál es su significado?

- ¿Qué significan los permisos de ejecución en un directorio? ¿Qué comando se puede utilizar para dar permisos de ejecución?

Respuesta:

- ![[Pasted image 20260604182050.png]]

- ![[Pasted image 20260604182233.png]] 
- #### Directorio `/tmp`

```
drwxrwxrwt
```

Descomposición:

- Usuario: `rwx` = 7
- Grupo: `rwx` = 7
- Otros: `rwx` = 7
- Bit especial: `t` (sticky bit)

Notación octal:

```
1777
```

**Significado:**

- Todos los usuarios pueden leer, escribir y acceder al directorio.
- El **sticky bit** (`t`) hace que cada usuario solo pueda borrar o renombrar sus propios archivos (o los del administrador), aunque todos tengan permiso de escritura en el directorio.
- Es el comportamiento típico del directorio temporal `/tmp`.

---

#### Fichero `/usr/bin/passwd`

```
-rwsr-xr-x
```

Descomposición:

- Usuario: `rws` = 7 (rwx) + bit especial setuid
- Grupo: `r-x` = 5
- Otros: `r-x` = 5

Notación octal:

```
4755
```

**Significado:**

- El propietario (`root`) tiene lectura, escritura y ejecución.
- Grupo y otros tienen lectura y ejecución.
- El bit **setuid** (`s`) hace que, al ejecutarse el programa, se ejecute con los privilegios del propietario del archivo (en este caso, `root`).
- Esto permite que un usuario normal pueda cambiar su contraseña, ya que el programa necesita modificar archivos del sistema que solo `root` puede escribir.


- En un directorio, el permiso de ejecución (`x`) **no significa ejecutar programas**.

Significa que un usuario puede:

- Entrar en el directorio (`cd directorio`).
- Acceder a los archivos y subdirectorios cuyo nombre conoce.
- Atravesar el directorio durante una ruta.

Sin permiso `x`, aunque se tenga permiso de lectura (`r`), no se puede acceder al contenido de forma efectiva.

Ejemplo:

```
cd directorio
```

requiere permiso de ejecución sobre ese directorio.


- Se utiliza `chmod`.

Por ejemplo, para añadir permiso de ejecución al usuario:

```
chmod u+x fichero
```

Para todos los usuarios:

```
chmod +x fichero
```

Y en notación octal:

```
chmod 755 fichero
```

(donde el propietario tiene `rwx` y grupo/otros tienen `r-x`).


--- 

#### 💻Ejercicio 6. Considera el archivo.txt creado en el ejercicio 3:

- Crea un enlace simbólico (symlink.txt) y otro duro (hardlink.txt) con el comando ln(1).
    
- Completar la siguiente tabla usando el comando ls(1) (opciones -l y -i) o stat(1):
    

|   |   |   |   |
|---|---|---|---|
|Archivo|i-nodo|Número de enlaces|Tamaño|
|archivo.txt||||
|symlink.txt||||
|hardlink.txt||||

- ¿Qué sucede con los enlaces si se borra el archivo.txt, siguen los contenidos disponibles? ¿Cómo cambia el número de enlaces?


![[Pasted image 20260604182920.png]]
![[Pasted image 20260604191924.png]]


Tras ejecutar:

```
ln -s archivo.txt symlink.txtln archivo.txt hardlink.txt
```

|Archivo|i-nodo|Nº de enlaces|Tamaño|
|---|---|---|---|
|archivo.txt|X|2|Igual que el contenido del fichero|
|symlink.txt|Y (distinto)|1|Tamaño de la ruta apuntada (`archivo.txt`)|
|hardlink.txt|X (igual que archivo.txt)|2|Igual que archivo.txt|

Donde:

- **X** es el i-nodo compartido por `archivo.txt` y `hardlink.txt`.
- **Y** es un i-nodo diferente correspondiente al enlace simbólico.

#### ¿Qué sucede si se borra `archivo.txt`?

- **hardlink.txt** sigue funcionando y conserva todos los contenidos, ya que apunta al mismo i-nodo que `archivo.txt`.
- **symlink.txt** queda roto (broken link), porque apunta a un nombre de fichero que ya no existe.

#### ¿Cómo cambia el número de enlaces?

Antes de borrar `archivo.txt`:

```
archivo.txt   → 2 enlaceshardlink.txt  → 2 enlaces
```

Después de borrar `archivo.txt`:

```
hardlink.txt  → 1 enlace
```

El contenido del fichero permanece accesible a través de `hardlink.txt` hasta que desaparezca el último enlace duro.

![[Pasted image 20260604192155.png]]

---

#### Ejercicio 7. Considera la siguiente salida del comando stat(1). ¿Qué tipo de fichero se trata, cuánto tamaño ocupa en disco?:

```
$ stat ejercicio7.file

  File: ejercicio7.file

  Size: 105906176    Blocks: 2048       IO Block: 4096   regular file

  Device: 801h/2049d Inode: 256409      Links: 1

...
```


Respuesta:

Se trata de un **fichero regular** (indicado por `regular file`).

Datos relevantes:

- **Tamaño lógico del fichero:** `105906176` bytes (≈ 101 MB).
    
- **Bloques asignados:** `2048`.
    
- En `stat`, el campo **Blocks** suele expresarse en bloques de **512 bytes**.
    

Por tanto, el espacio realmente ocupado en disco es:

[  
2048 \times 512 = 1048576 \text{ bytes}  
]

≈ **1 MiB**.

### Interpretación

El fichero parece ser un **archivo disperso (sparse file)**: aunque su tamaño lógico es de unos **101 MB**, solo ocupa **1 MiB en disco** porque gran parte de su contenido corresponde a zonas vacías (huecos) que el sistema de ficheros no almacena físicamente.

**Respuesta:**

- Tipo de fichero: **fichero regular**.
    
- Tamaño lógico: **105.906.176 bytes (~101 MB)**.
    
- Espacio ocupado en disco: **1.048.576 bytes (1 MiB)**.


---

#### 💻Ejercicio 8. Implementar una versión simplificada del comando dd. El programa recibirá 5 argumentos posicionales :

- input_file: El archivo de entrada del que se leerá. Si el archivo es ‘-’ se leerá de la entrada estándar.
    
- output_file: El archivo de salida en el que se escribirá. Si el archivo es ‘-’ se escribirá en la salida estándar.
    
- block_size: El número de bytes que se leerán o escribirán en cada llamada al sistema. (entero, int) 
    
- block_count: El número de bloques que se copiarán (entero, int)
    
- seek: Número de bloques que se saltarán de la salida antes de escribir.
    

Notas sobre la implementación:

- Tratar los errores en las llamadas al sistema adecuadamente. Asumir que el programa siempre se ejecuta con 5 argumentos.
    
- La función atoi(3) puede usarse para convertir una cadena de caracteres a un entero.
    
- El fichero de salida se creará (con permisos rw- rw- r--) si no existe, en caso contrario se truncará el contenido.
    
- El programa usará un buffer estático de 8192 bytes. Si el tamaño de bloque indicado es mayor se usará el tamaño máximo del buffer como tamaño de bloque.
    
- Tratar adecuadamente el valor que devuelven las llamadas read(2) y write(2). Además considerar que la llamada read()/write() pueden no leer un bloque completo en la llamada.
    

Ejemplo de salida del comando:
```
$ ./midd /dev/random ./prueba1 512 10 0

Copiados 10 bloques de 512 bytes

$ ls -l prueba1

-rw-r----- 1 ubuntu ubuntu 5120 Feb 3 20:57 prueba1
```
  

Cuestiones

- Comprobar el funcionamiento de lectura de bloques incompletos mostrado a continuación ( ¿Qué hace la orden de comandos indicada?). Para el ejemplo el programa debería escribir con dos llamadas write de 8 y 2 bytes, comprobarlo con la orden strace (Tema 1) :


```
$ ((echo "0123"; sleep 1; echo "4567")&) |  ./midd - prueba2 8 2 0

Copiados 2 bloques de 8 bytes

$ ls -l prueba2

-rw-r----- 1 ubuntu ubuntu 10 Feb 3 21:00 prueba2

$ cat prueba2

0123

4567
```

  

- Finalmente comprobar si el comando puede generar un archivo sparse de 100MB. Elegir los argumentos adecuados para midd y verificar que el fichero es sparse con los comandos: ls -lh, du -h y stat.


Respuesta:

``
``
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_BUFFER 8192

int cerrarArchivos(int *fIn, int *fOut)
{
    int error = 0;

    if (*fIn != STDIN_FILENO && *fIn >= 0)
    {
        if (close(*fIn) == -1)
        {
            perror("close entrada");
            error = 1;
        }
    }

    if (*fOut != STDOUT_FILENO && *fOut >= 0)
    {
        if (close(*fOut) == -1)
        {
            perror("close salida");
            error = 1;
        }
    }

    *fIn = -1;
    *fOut = -1;

    return error;
}

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        fprintf(stderr,
                "Uso: %s <input_file> <output_file> <block_size> <block_count> <seek>\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    size_t block_size = (size_t)atoi(argv[3]);
    int block_count = atoi(argv[4]);
    int seek_blocks = atoi(argv[5]);

    if (block_size <= 0 || block_count <= 0 || seek_blocks < 0)
    {
        fprintf(stderr,
                "Error: block_size y block_count deben ser > 0 y seek >= 0\n");
        return EXIT_FAILURE;
    }

    if (block_size > MAX_BUFFER)
        block_size = MAX_BUFFER;

    char buffer[MAX_BUFFER];

    int fdIn = -1;
    int fdOut = -1;

    /* Abrir entrada */
    if (strcmp(argv[1], "-") == 0)
    {
        fdIn = STDIN_FILENO;
    }
    else
    {
        fdIn = open(argv[1], O_RDONLY);

        if (fdIn == -1)
        {
            perror("open entrada");
            return EXIT_FAILURE;
        }
    }

    /* Abrir salida */
    if (strcmp(argv[2], "-") == 0)
    {
        fdOut = STDOUT_FILENO;
    }
    else
    {
        fdOut = open(argv[2],
                     O_WRONLY | O_CREAT | O_TRUNC,
                     0664);

        if (fdOut == -1)
        {
            perror("open salida");
            cerrarArchivos(&fdIn, &fdOut);
            return EXIT_FAILURE;
        }
    }

    /* Aplicar seek si la salida es un fichero */
    if (fdOut != STDOUT_FILENO && seek_blocks > 0)
    {
        off_t desplazamiento = (off_t)seek_blocks * block_size;

        if (lseek(fdOut, desplazamiento, SEEK_SET) == (off_t)-1)
        {
            perror("lseek");
            cerrarArchivos(&fdIn, &fdOut);
            return EXIT_FAILURE;
        }
    }

    int bloquesCopiados = 0;

    for (int i = 0; i < block_count; i++)
    {
        size_t totalLeidos = 0;

        /* Intentar completar un bloque */
        while (totalLeidos < block_size)
        {
            ssize_t n = read(fdIn,
                             buffer + totalLeidos,
                             block_size - totalLeidos);

            if (n == -1)
            {
                perror("read");
                cerrarArchivos(&fdIn, &fdOut);
                return EXIT_FAILURE;
            }

            if (n == 0) /* EOF */
                break;

            totalLeidos += n;
        }

        /* EOF sin leer nada */
        if (totalLeidos == 0)
            break;

        /* Escribir todo lo leído */
        size_t totalEscritos = 0;

        while (totalEscritos < totalLeidos)
        {
            ssize_t n = write(fdOut,
                              buffer + totalEscritos,
                              totalLeidos - totalEscritos);

            if (n == -1)
            {
                perror("write");
                cerrarArchivos(&fdIn, &fdOut);
                return EXIT_FAILURE;
            }

            totalEscritos += n;
        }

        bloquesCopiados++;

        /* EOF durante este bloque */
        if (totalLeidos < block_size)
            break;
    }

    printf("Copiados %d bloques de %zu bytes\n",
           bloquesCopiados,
           block_size);

    return cerrarArchivos(&fdIn, &fdOut);
}
```


![[Pasted image 20260605204458.png]]

![[Pasted image 20260605205822.png]]

![[Pasted image 20260605211058.png]]

![[Pasted image 20260605211305.png]]

- ¡Perfecto! 🎉

Esa salida de `strace` es exactamente la que esperábamos para una implementación correcta.

Analicémosla:

```text
read(0, "0123\n", 8) = 5
```

El programa intenta leer 8 bytes, pero la tubería solo tiene disponibles 5:

```text
0123\n
```

---

```text
read(0, "456", 3) = 3
```

Como todavía faltan 3 bytes para completar el bloque de 8, vuelve a llamar a `read()` y obtiene:

```text
456
```

Ahora tiene:

```text
0123\n456
```

que son exactamente 8 bytes.

---

```text
write(7, "0123\n456", 8) = 8
```

Escribe el primer bloque completo de 8 bytes.

✅ Correcto.

---

```text
read(0, "7\n", 8) = 2
```

Empieza el segundo bloque y solo quedan:

```text
7\n
```

(2 bytes).

---

```text
read(0, "", 6) = 0
```

Intenta completar los 6 bytes restantes del bloque, pero recibe EOF.

✅ Correcto.

---

```text
write(7, "7\n", 2) = 2
```

Escribe únicamente los 2 bytes realmente leídos.

✅ Esto era precisamente lo que pedía el ejercicio.

### Conclusión para la memoria o cuestiones

Podrías responder algo como:

> La orden
> 
> ```bash
> ((echo "0123"; sleep 1; echo "4567") &) | ./midd - prueba2 8 2 0
> ```
> 
> genera una tubería cuya entrada consta de los caracteres `0123\n4567\n` (10 bytes en total). El programa intenta leer bloques de 8 bytes. La primera llamada a `read()` devuelve 5 bytes (`0123\n`) y una segunda llamada devuelve los 3 bytes restantes necesarios para completar el bloque (`456`). Posteriormente se realiza una escritura de 8 bytes. Para el segundo bloque, `read()` devuelve únicamente 2 bytes (`7\n`) y la siguiente llamada devuelve EOF. Finalmente se realiza una escritura de 2 bytes. El fichero generado tiene un tamaño total de 10 bytes.
> 
> Salida observada con `strace`:
> 
> ```text
> read(0, "0123\n", 8) = 5
> read(0, "456", 3) = 3
> write(..., 8) = 8
> 
> read(0, "7\n", 8) = 2
> read(0, "", 6) = 0
> write(..., 2) = 2
> ```
> 
> Esto demuestra que la implementación gestiona correctamente lecturas y escrituras parciales.

Con ese `strace`, la parte más delicada de la práctica está correctamente resuelta.

- ![[Pasted image 20260605211819.png]]
---

## 📄 Explicación: generación y verificación de un fichero sparse

Para comprobar el funcionamiento de creación de ficheros _sparse_ con el programa `ddpropio.exe`, se ejecutó el siguiente comando:

```bash
./ddpropio.exe - sparse.bin 4096 1 25600
```

### ⚙️ Significado de los parámetros

- `-` → la entrada estándar no se usa (no se leen datos reales de fichero)
    
- `sparse.bin` → fichero de salida
    
- `4096` → tamaño de bloque de escritura (4 KB)
    
- `1` → número de bloques a escribir
    
- `25600` → número de bloques que se saltan antes de escribir (`seek`)
    

---

### 🧠 Qué hace el programa internamente

El programa realiza primero un desplazamiento en el fichero de salida:

```
25600 bloques × 4096 bytes = 104857600 bytes (≈ 100 MB)
```

Esto provoca que el puntero de escritura se sitúe en la posición 100 MB del fichero sin escribir datos intermedios.

Después se escribe un único bloque de 4096 bytes.

---

### 📊 Resultados obtenidos

#### 🔹 Tamaño lógico del fichero (`ls -lh`)

```bash
-rw-r--r-- 1 verxxuxz verxxuxz 101M sparse.bin
```

Esto muestra el tamaño total del fichero incluyendo los huecos creados por el `seek`.

---

#### 🔹 Tamaño real en disco (`du -h`)

```bash
4,0K sparse.bin
```

Aquí se observa el espacio realmente ocupado en disco, que es mínimo (solo el bloque escrito).

---

#### 🔹 Información del sistema (`stat`)

```text
Tamaño: 104857601
Bloques: 8
```

- **Size (104857601 bytes)** → tamaño lógico del fichero (≈ 100 MB + 1 byte / metadata)
    
- **Blocks (8)** → bloques realmente ocupados en disco, muy pequeño comparado con el tamaño lógico
    

---

### 🧾 Conclusión

El resultado demuestra que el fichero generado es un **fichero sparse**, ya que:

- Tiene un tamaño lógico grande (~100 MB) visible con `ls -lh`
    
- Pero ocupa muy poco espacio real en disco (`du -h`)
    
- El sistema de ficheros no ha reservado espacio para los huecos creados por `seek`
    

Esto confirma que el programa implementa correctamente el comportamiento de creación de ficheros dispersos (_sparse files_), donde las zonas no escritas no consumen espacio físico en disco.

---