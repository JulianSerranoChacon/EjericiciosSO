
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

![](Pasted%20image%2020260603183055.png)


Respuesta:

- ls -la meustra todos los ficheros de manera detallada

![](Pasted%20image%2020260603184623.png)

- ls -lA /run | tail -n +2 | cut -c1 | sort -u | wc -l muestra el numero de ficheros diferentes que hay.

![](Pasted%20image%2020260603184707.png)

- ls -lA| tail -n +2 | cut -c1 | sort -u| wc -l muestra los tipos de ficheros ordenados

![](Pasted%20image%2020260603184730.png)


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

![](Pasted%20image%2020260603185841.png)

- El inodo del fichero es 1948663

- Los permisos son los siguientes:
	- Propietario (`verxxuxz`): `rw-` → puede leer y escribir.
	- Grupo (`verxxuxz`): `r--` → solo puede leer.
	- Otros usuarios: `r--` → solo pueden leer.
	
- En cualto al uso en disco:
	- Tamaño: **15 bytes**.
	- Bloques ocupados: **8 bloques de 512 bytes** (así los muestra `stat` normalmente en Linux).
- Para la ruta relativa he usado el comando df:

	![](Pasted%20image%2020260603191130.png)

	- del que podemos sacar que la ruta relativa del disco está en /dev/sda1, que está montado en /home y el uso del disco está al 64%.

- Con los siguientes comando vemos las siguientes reacciones:
	- Con touch archivo txt cambian:
		- el tiempo de acceso(atime),
		- el tiempo de modificación(mtime).
		- el tiempo de cambio (ctime).
	  ![](Pasted%20image%2020260603191501.png)
	- Con echo “Otra línea más en el fichero” >> archivo.txt cambian:
		- Tamaño
		- Bloques ocupados (si es necesario)
		- Modificación (mtime)
		- Cambio (ctime)
		
		![](Pasted%20image%2020260603191929.png)
	- Con cat archivo.txt cambian:
		- Acceso (atime) (solo lee el fichero)
		![](Pasted%20image%2020260603192209.png)

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

![](Pasted%20image%2020260605205221.png)

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

- ![](Pasted%20image%2020260604182050.png)

- ![](Pasted%20image%2020260604182233.png) 
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


![](Pasted%20image%2020260604182920.png)
![](Pasted%20image%2020260604191924.png)


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

![](Pasted%20image%2020260604192155.png)

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


![](Pasted%20image%2020260605204458.png)

![](Pasted%20image%2020260605205822.png)

![](Pasted%20image%2020260605211058.png)

![](Pasted%20image%2020260605211305.png)

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

- ![](Pasted%20image%2020260605211819.png)
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

#### 💻Ejercicio 9. Escribir un programa que tenga un comportamiento similar a ls. El programa mils mostrará  el contenido de un directorio cuya ruta se proporciona como argumento. Para ello, el programa:

- Comprobará que el argumento es un directorio y que tiene acceso con la llamada al sistema adecuada.
    
- Recorrerá las entradas del directorio y escribirá su nombre de fichero. Además:
    

- Si es un fichero regular y tiene permiso de ejecución para usuario, grupo u otros, escribirá el carácter * después del nombre.
    
- Si es un directorio escribirá el carácter / después del nombre.
    
- Si es un enlace simbólico, escribirá -> y el nombre del fichero enlazado obtenido con readlink(2).
    

Nota: la variable d_name de las estructuras dirent sólo contienen el nombre para obtener los atributos del archivo es necesario especificar la ruta completa concatenando el nombre del directorio, en el primer argumento. 

Para concatenar ambas cadenas, directorio y nombre del archivo, definir un buffer de tamaño PATH_MAX (#include <linux/limits.h>) y la llamada snprintf(3).

Ejemplo de uso:

```C

#include<stdio.h>
#include<dirent.h>
#include<errno.h>
#include<sys/stat.h>
#include<linux/limits.h>
#include<unistd.h>

int main (int argc, char *argv[]){

    if(argc < 2){
        printf("El programa debe recibir al menos un argumento");
        return 1;
    }

    DIR* dir = opendir(argv[1]);
    if(dir == NULL){
        printf("El parametro facilitado no es un directorio o no se puede abrir");
        return errno;
    }

    struct dirent* entry = readdir(dir);

    while ((entry = readdir(dir)) != NULL)
    {

        struct stat entryStat;
        char path[PATH_MAX];

        snprintf(path, PATH_MAX, "%s/%s", argv[1], entry->d_name);

        lstat(path,&entryStat);

        printf("%s",entry->d_name);

        if(S_ISDIR(entryStat.st_mode))
            printf("/");
        else if(S_ISLNK(entryStat.st_mode)){
            char link[PATH_MAX];
            ssize_t len =  readlink(path,link,PATH_MAX-1);
            link[len] = '\0'; 
            printf(" -> %s", link);
        }
        else if(S_ISREG(entryStat.st_mode) && 
                ((entryStat.st_mode & S_IXUSR) || 
                (entryStat.st_mode & S_IXGRP) || 
                (entryStat.st_mode & S_IXOTH)) )
            printf("*");

        printf("\n");
    }

    if(closedir(dir) == -1){
        perror("Error al cerrar el directoriio");
        return 1;
    }

    return 0;
}

```


``` Bash
$ ./mils dir_prueba/

Contenidos del direcotrio dir_prueba/

---------------------------------------

programa1*

subdir1/

archivo1

../

archivo2

link1 -> /etc/passwd

./

#Comprobación comparando la salida con el comando ls

$ ls -l dir_prueba/

total 12

drwxr-xr-x 3 ubuntu ubuntu 4096 Feb 3 22:02 .

drwxr-xr-x 3 ubuntu ubuntu 4096 Feb 3 22:02 ..

-rw-r--r-- 1 ubuntu ubuntu    0 Feb 3 22:02 archivo1

-rw-r--r-- 1 ubuntu ubuntu    0 Feb 3 22:02 archivo2

lrwxrwxrwx 1 ubuntu ubuntu   11 Feb 3 22:02 link1 -> /etc/passwd

-rwxr-xr-x 1 ubuntu ubuntu    0 Feb 3 22:02 programa1

drwxr-xr-x 2 ubuntu ubuntu 4096 Feb 3 22:02 subdir1`
```

![](Pasted%20image%2020260606173021.png)

---

#### Ejercicio 10.  Un dispositivo de memoria flash de 64 MB de capacidad y bloques de 1KB, contiene un sistema de ficheros FAT. Describa la estructura de la tabla y cómo se representa la asignación de bloques a un fichero. ¿Cuántos bytes son necesarios para almacenar la tabla FAT?

Respuesta:

Para resolver el ejercicio, analicemos los datos del sistema de archivos FAT.

### 1. Número de bloques del dispositivo

Capacidad total de la memoria:

[  
64\ \text{MB} = 64 \times 1024 \times 1024 = 67,108,864\ \text{bytes}  
]

Tamaño de cada bloque:

[  
1\ \text{KB} = 1024\ \text{bytes}  
]

Número total de bloques:

[  
\frac{67,108,864}{1024} = 65,536\ \text{bloques}  
]

---

### 2. Estructura de la tabla FAT

La **FAT (File Allocation Table)** es una tabla donde existe **una entrada por cada bloque (o clúster) del disco**.

Cada entrada almacena información sobre ese bloque:

- **0** → bloque libre.
    
- **Valor especial EOF (End Of File)** → último bloque de un fichero.
    
- **Número de otro bloque** → indica cuál es el siguiente bloque del fichero.
    

Como hay **65 536 bloques**, cada entrada debe poder almacenar números entre 0 y 65 535, por lo que se necesitan:

[  
\log_2(65,536)=16\ \text{bits}  
]

Es decir, **2 bytes por entrada**.

---

### 3. Representación de la asignación de bloques a un fichero

La FAT representa un fichero mediante una **lista enlazada implícita**.

Ejemplo:

Supongamos que un fichero comienza en el bloque 10 y ocupa los bloques:

[  
10 \rightarrow 25 \rightarrow 18 \rightarrow 40 \rightarrow EOF  
]

La tabla FAT contendría:

|Bloque|Entrada FAT|
|---|---|
|10|25|
|25|18|
|18|40|
|40|EOF|

El directorio del fichero almacena únicamente el **primer bloque** (en este caso, el bloque 10). Siguiendo los enlaces de la FAT se recuperan todos los bloques del fichero.

---

### 4. Tamaño necesario para almacenar la FAT

Número de entradas:

[  
65,536  
]

Tamaño de cada entrada:

[  
2\ \text{bytes}  
]

Por tanto:

[  
65,536 \times 2 = 131,072\ \text{bytes}  
]

o equivalentemente:

[  
\frac{131,072}{1024} = 128\ \text{KB}  
]

---

## Respuesta final

- La **tabla FAT** contiene una entrada por cada bloque del dispositivo. Cada entrada indica si el bloque está libre, cuál es el siguiente bloque del fichero o si es el último bloque del mismo.
    
- La asignación de bloques de un fichero se representa mediante una **lista enlazada de bloques**, almacenándose en el directorio el número del primer bloque del fichero.
    
- El dispositivo tiene **65 536 bloques**, por lo que cada entrada de la FAT necesita **16 bits (2 bytes)**.
    
- El tamaño total de la FAT es:
    

[  
65,536 \times 2 = 131,072\ \text{bytes} = 128\ \text{KB}  
]

**Por tanto, son necesarios 131 072 bytes (128 KB) para almacenar la tabla FAT.**


---



Ejercicio 11. En la siguiente figura se representa una tabla FAT y el contenido de cierto directorio: que incluye: el nombre del archivo, el tipo (F=archivo, D=directorio) y el número del bloque inicial.


|     | Bloque |     | Bloque |     | Nombre   | Tipo | Bloque |
| --- | ------ | --- | ------ | --- | -------- | ---- | ------ |
| 0   |        | 10  |        |     | DATA.TXT | F    | 3      |
| 1   |        | 11  |        |     |          |      |        |
| 2   |        | 12  |        |     |          |      |        |
| 3   | 15     | 13  |        |     |          |      |        |
| 4   |        | 14  |        |     |          |      |        |
| 5   |        | 15  | [EOF]  |     |          |      |        |
| 6   |        | 16  |        |     |          |      |        |
| 7   |        | 17  |        |     |          |      |        |
| 8   |        | 18  |        |     |          |      |        |
| 9   |        | 19  |        |     |          |      |        |
  
El tamaño de bloque en este sistema de ficheros es de 512 bytes y que el sistema operativo siempre asigna los bloques empezando por el primer bloque libre (número inferior) disponible. Completar el estado final de las tablas tras realizar (en orden) las siguientes operaciones:

1. Creación del fichero DATA1.TXT de tamaño 10 bytes.
    
2. Creación del fichero DATA2.TXT de tamaño 1200 bytes. 
    
3. Se añaden datos al archivo DATA.TXT que requieren 2 bloques más. 
    
4. Creación del directorio LOGS. 
    

Creacion del fichero RESULTS.JPG de tamaño 2 Kbytes

Vamos resolviendo las operaciones **una a una**, recordando que:

- Tamaño de bloque = **512 bytes**.
    
- Los bloques se asignan **empezando por el bloque libre con menor número**.
    
- Estado inicial:
    
    - `DATA.TXT` comienza en el bloque **3**.
        
    - FAT[3] = 15
        
    - FAT[15] = EOF
        

Por tanto:

```text
DATA.TXT: 3 → 15 → EOF
```

Bloques ocupados inicialmente: **3 y 15**.

Bloques libres inicialmente:

```text
0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 18, 19
```

---

## 1) Crear DATA1.TXT de 10 bytes

Necesita:

[  
\lceil 10/512 \rceil = 1 \text{ bloque}  
]

Primer bloque libre: **0**

```text
DATA1.TXT: 0 → EOF
```

FAT:

```text
FAT[0] = EOF
```

---

## 2) Crear DATA2.TXT de 1200 bytes

Necesita:

[  
\lceil 1200/512 \rceil = 3 \text{ bloques}  
]

Primeros bloques libres: **1, 2 y 4**

```text
DATA2.TXT: 1 → 2 → 4 → EOF
```

FAT:

```text
FAT[1] = 2
FAT[2] = 4
FAT[4] = EOF
```

---

## 3) Añadir datos a DATA.TXT que requieren 2 bloques más

Estado actual:

```text
DATA.TXT: 3 → 15 → EOF
```

Último bloque: **15**

Primeros bloques libres: **5 y 6**

La cadena queda:

```text
DATA.TXT: 3 → 15 → 5 → 6 → EOF
```

FAT:

```text
FAT[15] = 5
FAT[5] = 6
FAT[6] = EOF
```

---

## 4) Crear el directorio LOGS

Suponiendo que un directorio vacío ocupa **1 bloque**.

Primer bloque libre: **7**

```text
LOGS: 7 → EOF
```

FAT:

```text
FAT[7] = EOF
```

---

## 5) Crear RESULTS.JPG de 2 KB

2 KB = 2048 bytes.

Bloques necesarios:

[  
2048/512 = 4 \text{ bloques}  
]

Primeros bloques libres: **8, 9, 10 y 11**

```text
RESULTS.JPG: 8 → 9 → 10 → 11 → EOF
```

FAT:

```text
FAT[8] = 9
FAT[9] = 10
FAT[10] = 11
FAT[11] = EOF
```

---

## Estado final de la FAT

|Bloque|FAT|
|---|---|
|0|EOF|
|1|2|
|2|4|
|3|15|
|4|EOF|
|5|6|
|6|EOF|
|7|EOF|
|8|9|
|9|10|
|10|11|
|11|EOF|
|12|Libre|
|13|Libre|
|14|Libre|
|15|5|
|16|Libre|
|17|Libre|
|18|Libre|
|19|Libre|

---

## Estado final del directorio

|Nombre|Tipo|Bloque inicial|
|---|---|---|
|DATA.TXT|F|3|
|DATA1.TXT|F|0|
|DATA2.TXT|F|1|
|LOGS|D|7|
|RESULTS.JPG|F|8|

---

### Cadenas finales de bloques

```text
DATA.TXT     : 3 → 15 → 5 → 6 → EOF
DATA1.TXT    : 0 → EOF
DATA2.TXT    : 1 → 2 → 4 → EOF
LOGS         : 7 → EOF
RESULTS.JPG  : 8 → 9 → 10 → 11 → EOF
```

Este sería el **estado final de las tablas FAT y del directorio** tras ejecutar todas las operaciones en el orden indicado.

|     | Bloque |     | Bloque |     | Nombre      | Tipo | Bloque |
| --- | ------ | --- | ------ | --- | ----------- | ---- | ------ |
| 0   | EOF    | 10  | 11     |     | DATA.TXT    | F    | 3      |
| 1   | 2      | 11  | EOF    |     | DATA1.TXT   | F    | 0      |
| 2   | 4      | 12  |        |     | DATA2.TXT   | F    | 1      |
| 3   | 15     | 13  |        |     | LOGS        | D    | 7      |
| 4   | EOF    | 14  |        |     | RESULTS.JPG | F    | 8      |
| 5   | 6      | 15  | 5      |     |             |      |        |
| 6   | EOF    | 16  |        |     |             |      |        |
| 7   | EOF    | 17  |        |     |             |      |        |
| 8   | 9      | 18  |        |     |             |      |        |
| 9   | 10     | 19  |        |     |             |      |        |

---

#### 💻 Ejercicio 13. Crear un sistema de ficheros ext2 con la siguiente estructura de ficheros y directorios. Nota: los comandos mostrados a continuación se ejecutarán como root (cambiar a ejecutando sudo -i):

  

1. Crear un fichero de 100MB.
    
```bash
# truncate --size 100M ext2.img 
```

2. Crear el sistema de ficheros ext2 en el fichero anterior. Nota: normalmente los sistemas de ficheros se crean en dispositivos en modo bloque. También es posible usar un fichero como soporte.
    
```bash
# mkfs.ext2 ext2.img
```

¿Qué tamaño de bloque se ha usado? ¿Cuántos inodos hay disponibles?

3. Montar el sistema de ficheros (la opción especial loop, permite montar el fichero accediendo mediante un dispositivo en modo bloque especial, loopdevice en /dev/loop[0-9])
    
```bash
# mount -t ext2 -o loop ext2.img /mnt
```

4. En /mnt crear los siguientes contenidos:
    

```bash
# mkdir /mnt/dir

# echo "123456789" > /mnt/dir/small.txt

# dd if=/dev/random of=/mnt/dir/big bs=1024 count=5120

# dd if=/dev/random of=/mnt/dir/sparse bs=1024 count=1 seek=5192
```


Comprobar que se han creado correctamente los ficheros:

```bash
# ls -lhis /mnt/dir/

total 5.1M

15 5.1M -rw-r--r-- 1 root root 5.0M Feb 31 13:31 big

14 4.0K -rw-r--r-- 1 root root   10 Feb 31 13:29 small.txt

16  12K -rw-r--r-- 1 root root 5.1M Feb 31 14:15 sparse
```

5. Usando la herramienta debugfs estudiar la estructura de los inodos ext2 de los tres ficheros (comando stat).  El fichero se referencia con el número de inodo en la forma `<inodo>`:
    
``` bash
$ debugfs -R "stat <14>" /dev/sda1

Inode: 14   Type: regular    Mode:  0644   Flags: 0x0

Generation: 3900547475    Version: 0x00000000:00000001

User:     0   Group:     0   Project:     0   Size: 10

File ACL: 0

Links: 1   Blockcount: 8

Fragment:  Address: 0    Number: 0    Size: 0

 ctime: 0x68b44e22:7ba162a8 -- Sun Aug 31 13:29:06 2025

 atime: 0x68b44e22:7ba162a8 -- Sun Aug 31 13:29:06 2025

 mtime: 0x68b44e22:7ba162a8 -- Sun Aug 31 13:29:06 2025

crtime: 0x68b44e22:7ba162a8 -- Sun Aug 31 13:29:06 2025

Size of extra inode fields: 32

BLOCKS:

(0):24576

TOTAL: 1
``` 
En este caso el tamaño es 10 bytes (correspondiente a los bytes de la cadena  “123456789\n”) que se almacenan en el bloque 24576 que usa el primer puntero directo (0).

Cuestiones

- Estudiar la estructura de bloques del fichero big:
    

- Bloques directos usados
    
- Bloque dónde se guarda el puntero indirecto (IND). Y los bloques usados en el primer nivel de indirecciń (12-1035)
    
- Bloque dónde se guarda el puntero doble indirecto (DIND). ¿Cuántos bloques indirectos (IND) hay en la tabla?. ¿Y los bloques usados para los bloques restantes (1036-1279)?
    
- ¿Cuántos bloques consume en total el archivo? ¿Cuántos bloques se usan para guardar datos útiles?
    
- Dibujar un esquema con las tablas usadas para almacenar el fichero.
    

- Considerar ahora el fichero sparse y repetir la cuestión anterior. Comparar los punteros inicializados en ambos casos.
Respuesta:

![](Pasted%20image%2020260606233450.png)

![](Pasted%20image%2020260606233556.png)

![](Pasted%20image%2020260607000028.png)

![](Pasted%20image%2020260607000945.png)

![](Pasted%20image%2020260607001351.png)

![](Pasted%20image%2020260607001412.png)

Perfecto, ya tienes toda la información necesaria para analizar la estructura de los inodos. De los resultados obtenidos se puede concluir:

### 1. `small.txt` (inodo 9842)

```text
Size: 10
Blockcount: 2
BLOCKS:
(0):41985
TOTAL: 1
```

- Tamaño lógico: **10 bytes**.
    
- Utiliza **1 bloque de datos** (`TOTAL: 1`).
    
- Solo emplea un **puntero directo** (`(0):41985`).
    
- `Blockcount: 2` porque este campo se expresa en sectores de 512 bytes. Como el sistema de ficheros usa bloques de 1 KB:
    

[  
2 \times 512 = 1024 \text{ bytes} = 1 \text{ bloque}  
]

---

### 2. `big` (inodo 9843)

```text
Size: 5242880
Blockcount: 10282
...
(0-11):42497-42508
(IND):41714
...
(DIND):41715
...
TOTAL: 5141
```

- Tamaño lógico: **5 242 880 bytes (5 MB)**.
    
- Ocupa **5141 bloques de 1 KB** (`TOTAL: 5141`).
    
- Utiliza:
    
    - Los **12 punteros directos** (`(0-11)`).
        
    - **Punteros indirectos simples** (`IND`).
        
    - Un **puntero indirecto doble** (`DIND`).
        
- No aparece ningún puntero triple indirecto.
    

El valor:

```text
Blockcount: 10282
```

equivale a:

[  
10282 \times 512 = 5,264,384 \text{ bytes}  
]

que corresponde a los bloques de datos más los bloques de indirección.

---

### 3. `sparse` (inodo 9844)

```text
Size: 5317632
Blockcount: 6
BLOCKS:
(DIND):41735
(IND):41736
(5192):11337
TOTAL: 3
```

- Tamaño lógico: **5 317 632 bytes (~5,1 MB)**.
    
- Solo ocupa **3 bloques físicos** (`TOTAL: 3`).
    
- Utiliza:
    
    - Un bloque de **indirección doble** (`DIND`).
        
    - Un bloque de **indirección simple** (`IND`).
        
    - Un único bloque de datos real (`(5192):11337`).
        

Esto demuestra que `sparse` es un **archivo disperso (sparse file)**: aunque su tamaño lógico es grande, la mayoría de los bloques son huecos que no ocupan espacio físico en disco.

El campo:

```text
Blockcount: 6
```

indica:

[  
6 \times 512 = 3072 \text{ bytes}  
]

que corresponden exactamente a los **3 bloques de 1 KB realmente asignados**.

---

### Conclusión

- **`small.txt`**: fichero pequeño almacenado mediante **punteros directos**.
    
- **`big`**: fichero grande que requiere **punteros directos, indirectos simples e indirectos dobles**.
    
- **`sparse`**: fichero disperso que posee un gran tamaño lógico pero ocupa muy poco espacio físico, utilizando únicamente los bloques estrictamente necesarios para representar los datos reales.


## CUESTIONES:
Vamos a analizarlo paso a paso usando la información obtenida con `debugfs`.

---

# 1. Fichero `big`

```text
Size: 5242880 bytes
TOTAL: 5141 bloques

BLOCKS:
(0-11):42497-42508,
(IND):41714,
(12-15):42509-42512,
(16-63):785-832,
(64-127):865-928,
(128-255):1025-1152,
(256-267):1281-1292,

(DIND):41715,

(IND):41716,(268-523):1293-1548,
(IND):41717,(524-779):1549-1804,
(IND):41718,(780-1023):1805-2048,

(1024-1035):2561-2572,

(IND):41719,(1036-1291):2573-2828,
...
(IND):41734,(4876-5119):6925-7168
```

---

## a) Bloques directos usados

Los primeros 12 bloques del fichero utilizan punteros directos:

|Bloque lógico|Bloque físico|
|---|---|
|0|42497|
|1|42498|
|2|42499|
|...|...|
|11|42508|

**Respuesta:** utiliza los **12 punteros directos** del inodo.

---

## b) Bloque donde se guarda el puntero indirecto (IND)

Aparece:

```text
(IND):41714
```

Este bloque contiene la tabla de punteros indirectos simples.

Como el tamaño de bloque es **1024 B** y cada puntero ocupa **4 bytes**:

[  
\frac{1024}{4}=256\text{ punteros}  
]

Por tanto puede direccionar **256 bloques de datos**.

---

### Bloques usados en el primer nivel de indirección (12–1035)

Los bloques lógicos:

```text
12–267
268–523
524–779
780–1035
```

Total:

[  
1035-12+1=1024\text{ bloques}  
]

Se usan:

- 1 tabla IND → bloque 41714
    
- 4 tablas IND bajo DIND:
    
    - 41716
        
    - 41717
        
    - 41718
        
    - parte de 41719
        

---

## c) Bloque donde se guarda el puntero doble indirecto (DIND)

```text
(DIND):41715
```

El bloque **41715** contiene punteros hacia bloques IND.

---

### ¿Cuántos bloques IND hay en la tabla DIND?

Desde:

```text
41716 hasta 41734
```

Número total:

[  
41734-41716+1=19  
]

**Hay 19 bloques indirectos simples.**

---

### Bloques usados para los bloques restantes (1036–5119)

Desde:

```text
1036–1291 → IND 41719
1292–1547 → IND 41720
...
4876–5119 → IND 41734
```

Total:

[  
5119-1036+1=4084\text{ bloques de datos}  
]

Se necesitan:

[  
\lceil \frac{4084}{256}\rceil=16\text{ bloques IND}  
]

---

## d) ¿Cuántos bloques consume en total?

Debugfs indica:

```text
TOTAL: 5141
```

Por tanto:

### Bloques de datos útiles

El fichero mide:

[  
5242880/1024=5120\text{ bloques}  
]

### Bloques de metadatos

Indirectos:

- 1 IND simple → 41714
    
- 1 DIND → 41715
    
- 19 IND bajo DIND → 41716–41734
    

Total:

[  
1+1+19=21  
]

Total consumido:

[  
5120+21=5141  
]

✓ Coincide con debugfs.

---

## e) Esquema del fichero `big`

```text
INODO
│
├─ Directos (12)
│   ├─ 42497
│   ├─ ...
│   └─ 42508
│
├─ IND → 41714
│        │
│        ├─ Datos 12–267
│        └─ 256 punteros
│
└─ DIND → 41715
         │
         ├─ IND 41716 → Datos 268–523
         ├─ IND 41717 → Datos 524–779
         ├─ IND 41718 → Datos 780–1035
         ├─ IND 41719 → Datos 1036–1291
         ├─ ...
         └─ IND 41734 → Datos 4876–5119
```

---

# 2. Fichero `sparse`

```text
Size: 5317632
Blockcount: 6

BLOCKS:
(DIND):41735
(IND):41736
(5192):11337

TOTAL: 3
```

---

## a) Bloques directos usados

No aparece:

```text
(0):
```

Por tanto:

**No hay punteros directos inicializados.**

---

## b) Bloque DIND

```text
(DIND):41735
```

---

## c) Bloques IND

Solo hay uno:

```text
(IND):41736
```

---

## d) Bloques de datos

Solo existe:

```text
(5192):11337
```

Es decir:

- El bloque lógico **5192** contiene datos.
    
- Todos los bloques anteriores son huecos.
    

---

## e) Bloques consumidos

Debugfs indica:

```text
TOTAL: 3
```

Corresponden a:

|Tipo|Bloques|
|---|---|
|DIND|1|
|IND|1|
|Datos|1|

Total:

[  
1+1+1=3  
]

---

# Comparación `big` vs `sparse`

|Característica|big|sparse|
|---|---|---|
|Tamaño lógico|5 MB|5.1 MB|
|Bloques de datos|5120|1|
|Bloques totales|5141|3|
|Directos usados|Sí (12)|No|
|IND simple|Sí|Sí|
|DIND|Sí|Sí|
|Triple indirecto|No|No|
|Espacios vacíos|No|Sí, casi todo|

---

## Diferencia principal

- **`big`** tiene todos sus bloques realmente asignados.
    
- **`sparse`** solo reserva bloques para los datos escritos; los huecos se representan mediante punteros nulos.
    

Por ello, aunque ambos tienen un tamaño lógico similar (~5 MB), `big` ocupa **5141 bloques**, mientras que `sparse` solo consume **3 bloques físicos**.

---

#### Ejercicio 14. Las entradas de directorio en un sistema de ficheros ext2 incluyen: el número de inodo (32 bits), la longitud del nombre  y el nombre del archivo, que puede tener un máximo de 256 caracteres. ¿Cuántas entradas de directorio se pueden almacenar en un bloque de disco de 4 KB?

Respuesta:

Las entradas de directorio en un sistema de ficheros **ext2** incluyen:

- Número de inodo: **32 bits = 4 bytes**
- Longitud total de la entrada (`rec_len`): **16 bits = 2 bytes**
- Longitud del nombre (`name_len`): **8 bits = 1 byte**
- Tipo de fichero (`file_type`): **8 bits = 1 byte**
- Nombre del archivo: máximo **256 caracteres = 256 bytes**

Por tanto, el tamaño máximo de una entrada de directorio es:

$$
4 + 2 + 1 + 1 + 256 = 264\ \text{bytes}
$$

Un bloque de disco tiene un tamaño de **4 KB**:

$$
4\ \text{KB} = 4096\ \text{bytes}
$$

El número máximo de entradas que pueden almacenarse en un bloque es:

$$
\left\lfloor \frac{4096}{264} \right\rfloor = 15
$$

Comprobación:

$$
15 \times 264 = 3960\ \text{bytes}
$$

$$
16 \times 264 = 4224\ \text{bytes} > 4096\ \text{bytes}
$$

Por tanto, **en un bloque de 4 KB se pueden almacenar como máximo 15 entradas de directorio**, suponiendo que todos los nombres de archivo tienen la longitud máxima de **256 caracteres**.

> **Nota:** En la práctica suelen almacenarse más entradas, ya que la mayoría de los nombres son más cortos y las entradas de directorio en ext2 tienen longitud variable.

---


Ejercicio 15. Considere un sistema de ficheros con los siguientes contenidos de inodos y bloques de disco.

|         |     |     |         |     |     |         |     |     |         |     |     |         |     |
| ------- | --- | --- | ------- | --- | --- | ------- | --- | --- | ------- | --- | --- | ------- | --- |
| inodo   | 2   |     | inodo   | 3   |     | inodo   | 4   |     | inodo   | 5   |     | inodo   | 9   |
| Enlaces |     |     | Enlaces |     |     | Enlaces |     |     | Enlaces |     |     | Enlaces |     |
| Tipo    | D   |     | Tipo    | F   |     | Tipo    | F   |     | Tipo    | D   |     | Tipo    | D   |
| Bloque  |     |     | Bloque  | 6   |     | Bloque  | 12  |     | Bloque  | 0   |     | Bloque  |     |

  
  

|        |     |     |        |     |     |        |     |
| ------ | --- | --- | ------ | --- | --- | ------ | --- |
| Bloque | 0   |     | Bloque | 3   |     | Bloque | 5   |
| .      | 5   |     | .      | 2   |     | .      | 9   |
| ..     | 2   |     | ..     |     |     | ..     | 5   |
| C      | 9   |     | A      | 3   |     |        |     |
| D      | 4   |     | B      | 5   |     |        |     |
|        |     |     | E      | 4   |     |        |     |

1. Rellene los huecos para que el sistema sea consistente.
    
2. Dibuje de forma esquemática  el árbol del directorio empleando.

Respuesta:


|         |     |     |         |     |     |         |     |     |         |     |     |         |     |
| ------- | --- | --- | ------- | --- | --- | ------- | --- | --- | ------- | --- | --- | ------- | --- |
| inodo   | 2   |     | inodo   | 3   |     | inodo   | 4   |     | inodo   | 5   |     | inodo   | 9   |
| Enlaces | 3   |     | Enlaces | 1   |     | Enlaces | 2   |     | Enlaces | 3   |     | Enlaces | 2   |
| Tipo    | D   |     | Tipo    | F   |     | Tipo    | F   |     | Tipo    | D   |     | Tipo    | D   |
| Bloque  | 3   |     | Bloque  | 6   |     | Bloque  | 12  |     | Bloque  | 0   |     | Bloque  | 5   |

  
  

|        |     |     |        |     |     |        |     |
| ------ | --- | --- | ------ | --- | --- | ------ | --- |
| Bloque | 0   |     | Bloque | 3   |     | Bloque | 5   |
| .      | 5   |     | .      | 2   |     | .      | 9   |
| ..     | 2   |     | ..     | 2   |     | ..     | 5   |
| C      | 9   |     | A      | 3   |     |        |     |
| D      | 4   |     | B      | 5   |     |        |     |
|        |     |     | E      | 4   |     |        |     |

## 1. Completar los huecos para que el sistema sea consistente

### Inodos

|Inodo|Enlaces|Tipo|Bloque|
|---|---|---|---|
|2|**3**|D|**3**|
|3|**1**|F|6|
|4|**2**|F|12|
|5|**3**|D|0|
|9|**2**|D|**5**|

---

### Bloques de directorio

#### Bloque 0 (corresponde al inodo 5)

|Entrada|Inodo|
|---|---|
|.|5|
|..|2|
|C|9|
|D|4|

---

#### Bloque 3 (corresponde al inodo 2)

|Entrada|Inodo|
|---|---|
|.|2|
|..|**2**|
|A|3|
|B|5|
|E|4|

Como el inodo 2 es el **directorio raíz**, la entrada `..` apunta a sí mismo.

---

#### Bloque 5 (corresponde al inodo 9)

|Entrada|Inodo|
|---|---|
|.|9|
|..|5|

---

## 2. Cálculo del número de enlaces

### Inodo 2 (directorio raíz)

Contiene el subdirectorio:

- `B → inodo 5`
    

Por tanto:

- `.` → 1 enlace
    
- `..` → 1 enlace (al ser raíz apunta a sí mismo)
    
- Subdirectorio `B` → añade 1 enlace
    

Total:

$$  
2 + 1 = 3  
$$

---

### Inodo 3 (fichero)

Solo aparece en:

```text
A → 3
```

Total:

$$  
1  
$$

---

### Inodo 4 (fichero)

Aparece dos veces:

```text
D → 4
E → 4
```

Es un **hard link**.

Total:

$$  
2  
$$

---

### Inodo 5 (directorio)

Tiene:

- `.` y `..`
    
- Un subdirectorio:
    

```text
C → 9
```

Total:

$$  
2 + 1 = 3  
$$

---

### Inodo 9 (directorio)

No tiene subdirectorios.

Solo:

- `.`
    
- `..`
    

Total:

$$  
2  
$$

---

## 3. Árbol de directorios

El directorio raíz es el **inodo 2**:

```text
/ (inodo 2)
├── A  (inodo 3) [fichero]
├── E  (inodo 4) [fichero]
└── B/ (inodo 5)
    ├── D  (inodo 4) [fichero]
    └── C/ (inodo 9)
```

Observaciones:

- **D** y **E** apuntan al **mismo inodo 4**, por lo que son **enlaces duros (hard links)** al mismo fichero.
    
- El directorio raíz (`inodo 2`) contiene el directorio `B`.
    
- `B` contiene el directorio `C`.
    
- `C` es un directorio vacío (solo tiene `.` y `..`).
    

---

### Estado final consistente

|Inodo|Enlaces|Tipo|Bloque|
|---|---|---|---|
|2|3|D|3|
|3|1|F|6|
|4|2|F|12|
|5|3|D|0|
|9|2|D|5|

y el árbol resultante es:

```text
/
├── A
├── E
└── B
    ├── D
    └── C
```

donde **D y E son dos nombres distintos para el mismo fichero (inodo 4)**.

```
en el bloque 3 como van a ser . y .. el mismo 
```

Sí, **pueden ser el mismo**, pero **solo si el directorio es el directorio raíz (`/`)**.

En cualquier directorio:

- `.` apunta al propio directorio.
    
- `..` apunta al directorio padre.
    

Por ejemplo:

```text
/home/verxxuxz/Documentos
```

tendría:

```text
.  → Documentos
.. → /home/verxxuxz
```

Sin embargo, en el **directorio raíz** no existe un directorio padre superior, por lo que:

```text
/ (raíz)
.  → /
.. → /
```

Es decir, ambos apuntan al mismo inodo.

---

En este ejercicio, el bloque 3 contiene:

|Entrada|Inodo|
|---|---|
|.|2|
|..|?|
|A|3|
|B|5|
|E|4|

Y sabemos que el bloque 0 (inodo 5) tiene:

|Entrada|Inodo|
|---|---|
|.|5|
|..|2|

Esto significa que **el directorio 5 está contenido dentro del inodo 2**, ya que `..` del inodo 5 apunta al 2.

Además, el bloque 5 (inodo 9) tiene:

|Entrada|Inodo|
|---|---|
|.|9|
|..|5|

Por lo que el árbol es:

```text
inodo 2
└── B (inodo 5)
    └── C (inodo 9)
```

Como **ningún directorio apunta al inodo 2 como hijo**, la única posibilidad consistente es que el **inodo 2 sea la raíz**.

Por tanto:

|Entrada|Inodo|
|---|---|
|.|2|
|..|2|

es correcto.

Si el ejercicio indicara que existe otro directorio padre para el inodo 2, entonces `..` debería apuntar a otro inodo distinto. Pero con los datos proporcionados, **el inodo 2 es la raíz y `.` y `..` deben ser ambos 2**.

#### Ejercicio 16.  Un usuario desea dar formato a una partición de disco para almacenar su colección de fotografías. Cada fotografía se almacena en un fichero con un tamaño fijo de 7000 bytes. El sistema de ficheros usado tiene las siguientes características:

- Bloques indexados con 2 enlaces directos y 1 indirecto simple por inodo. 
    
- 4 bytes para identificar un inodo
    
- Tamaño de puntero a bloque de 32 bits. 
    

El sistema de ficheros se puede formatear con  tres tamaños de bloque: 1024 bytes, 2048 bytes y 4096 bytes. Analiza las ventajas de cada tamaño contestando a las siguientes preguntas:

-  ¿Cuál de las tres opciones presenta menos fragmentación interna? Para cada tamaño de bloque, indique el porcentaje de ocupación real de cada uno de los bloques de datos asignados a un fichero. 
    
- ¿Cuál de las tres opciones ofrece un mejor aprovechamiento de los bloques de disco disponibles? Para cada tamaño de bloque, indique qué porcentaje de bloques de disco se utilizarán para almacenar los datos de un determinado fichero.
    
- ¿Cuántos accesos a disco requerirá una lectura secuencial completa de una fotografía en cada caso?
    
- ¿Cómo afectarán a la selección de un tamaño de bloque determinado al tamaño de la tabla de inodos y al mapa de bits de bloques libres?

Respuesta:

## Ejercicio 16

**Datos del problema:**

- Tamaño de cada fotografía: **7000 bytes**
- Cada inodo dispone de:
  - **2 punteros directos**
  - **1 puntero indirecto simple**
- Tamaño del puntero a bloque: **32 bits = 4 bytes**
- Tamaño del identificador de inodo: **4 bytes**
- Tamaños de bloque posibles:
  - 1024 bytes
  - 2048 bytes
  - 4096 bytes

---

## 1. Fragmentación interna

La fragmentación interna corresponde al espacio desperdiciado en el último bloque del fichero.

### Bloques de 1024 bytes

Número de bloques de datos necesarios:

$$
\left\lceil \frac{7000}{1024} \right\rceil = 7 \text{ bloques}
$$

Espacio ocupado:

$$
6 \times 1024 = 6144 \text{ bytes}
$$

Último bloque:

$$
7000 - 6144 = 856 \text{ bytes}
$$

Ocupación del último bloque:

$$
\frac{856}{1024}\times 100 = 83.59\%
$$

Fragmentación interna:

$$
100 - 83.59 = 16.41\%
$$

**Porcentaje de ocupación de cada bloque:**

| Bloque | Ocupación |
|---------|------------|
| 1–6 | 100 % |
| 7 | 83.59 % |

---

### Bloques de 2048 bytes

Número de bloques:

$$
\left\lceil \frac{7000}{2048} \right\rceil = 4 \text{ bloques}
$$

Espacio ocupado:

$$
3 \times 2048 = 6144 \text{ bytes}
$$

Último bloque:

$$
7000 - 6144 = 856 \text{ bytes}
$$

Ocupación del último bloque:

$$
\frac{856}{2048}\times 100 = 41.80\%
$$

Fragmentación interna:

$$
100 - 41.80 = 58.20\%
$$

**Porcentaje de ocupación:**

| Bloque | Ocupación |
|---------|------------|
| 1–3 | 100 % |
| 4 | 41.80 % |

---

### Bloques de 4096 bytes

Número de bloques:

$$
\left\lceil \frac{7000}{4096} \right\rceil = 2 \text{ bloques}
$$

Espacio ocupado en el último bloque:

$$
7000 - 4096 = 2904 \text{ bytes}
$$

Ocupación del último bloque:

$$
\frac{2904}{4096}\times 100 = 70.90\%
$$

Fragmentación interna:

$$
100 - 70.90 = 29.10\%
$$

**Porcentaje de ocupación:**

| Bloque | Ocupación |
|---------|------------|
| 1 | 100 % |
| 2 | 70.90 % |

---

### Conclusión

La opción con **menor fragmentación interna** es:

| Tamaño de bloque | Fragmentación |
|------------------|---------------|
| 1024 B | **16.41 %** |
| 2048 B | 58.20 % |
| 4096 B | 29.10 % |

Por tanto, **1024 bytes presenta la menor fragmentación interna**.

---

## 2. Aprovechamiento de bloques de disco

Se consideran tanto los bloques de datos como los bloques indirectos.

### Bloques de 1024 bytes

Datos:

- 7 bloques de datos.

Los 2 punteros directos cubren:

$$
2 \text{ bloques}
$$

Puntero indirecto simple:

Capacidad:

$$
\frac{1024}{4} = 256 \text{ punteros}
$$

Se necesita un bloque indirecto para los 5 bloques restantes.

Bloques totales consumidos:

$$
7 + 1 = 8
$$

Porcentaje útil:

$$
\frac{7}{8}\times100 = 87.5\%
$$

---

### Bloques de 2048 bytes

Datos:

$$
4 \text{ bloques}
$$

Directos:

$$
2 \text{ bloques}
$$

Indirectos:

Se necesita un bloque indirecto para 2 bloques.

Bloques totales:

$$
4 + 1 = 5
$$

Porcentaje útil:

$$
\frac{4}{5}\times100 = 80\%
$$

---

### Bloques de 4096 bytes

Datos:

$$
2 \text{ bloques}
$$

Los 2 punteros directos son suficientes.

No se necesita indirección.

Bloques totales:

$$
2
$$

Porcentaje útil:

$$
\frac{2}{2}\times100 = 100\%
$$

---

### Conclusión

| Tamaño de bloque | Bloques de datos | Bloques indirectos | Total | Aprovechamiento |
|------------------|------------------|--------------------|--------|------------------|
| 1024 B | 7 | 1 | 8 | 87.5 % |
| 2048 B | 4 | 1 | 5 | 80 % |
| 4096 B | 2 | 0 | 2 | **100 %** |

El mejor aprovechamiento lo ofrece **4096 bytes**.

---

## 3. Accesos a disco para lectura secuencial completa

Suponiendo que es necesario leer el inodo inicialmente.

### Bloques de 1024 bytes

- 1 acceso al inodo
- 1 acceso al bloque indirecto
- 7 accesos a bloques de datos

Total:

$$
1 + 1 + 7 = 9
$$

---

### Bloques de 2048 bytes

- 1 acceso al inodo
- 1 acceso al bloque indirecto
- 4 accesos a datos

Total:

$$
1 + 1 + 4 = 6
$$

---

### Bloques de 4096 bytes

- 1 acceso al inodo
- 2 accesos a datos

Total:

$$
1 + 2 = 3
$$

---

### Conclusión

| Tamaño de bloque | Accesos |
|------------------|----------|
| 1024 B | 9 |
| 2048 B | 6 |
| 4096 B | **3** |

Los bloques de **4096 bytes requieren menos accesos a disco**.

---

## 4. Influencia sobre la tabla de inodos y el mapa de bits

### Tabla de inodos

El tamaño de bloque **no afecta al tamaño individual del inodo**, pero sí al número total de inodos que pueden almacenarse en cada bloque de la tabla.

Con bloques mayores:

- Se almacenan más inodos por bloque.
- Se necesitan menos accesos para consultar la tabla.

---

### Mapa de bits de bloques libres

El tamaño del bitmap depende del número de bloques del sistema de ficheros.

Con bloques más grandes:

- Se necesitan menos bloques para almacenar la misma cantidad de datos.
- El bitmap será más pequeño.

Con bloques más pequeños:

- Habrá más bloques en el disco.
- El bitmap será más grande.

---

## Conclusiones finales

- **Menor fragmentación interna:** bloques de **1024 bytes**.
- **Mejor aprovechamiento de bloques:** bloques de **4096 bytes**.
- **Menor número de accesos a disco:** bloques de **4096 bytes**.
- **Bitmap más pequeño:** bloques de **4096 bytes**.

Por tanto, existe un compromiso entre minimizar la fragmentación interna (**1024 B**) y maximizar el rendimiento y reducir metadatos (**4096 B**).


--- 

#### Ejercicio 17. Un sistema de ficheros UNIX tiene las siguientes características:

- Bloques de 512 bytes y direcciones de bloque de disco de 16 bits.
    
- Bloques indexados con 10 punteros directos a bloque,  1 puntero indirecto simple y 1 puntero indirecto doble.
    

 Conteste de manera razonada a las siguientes cuestiones:

- ¿Cuál es el tamaño máximo de un fichero en este sistema? 
    
- Un programa UNIX crea un fichero en este sistema e inmediatamente después escribe un byte de datos en la posición 1.000 y otro en la posición 10.000. ¿Cuántos bloques del disco



Respuesta:

**Datos del problema:**

- Tamaño de bloque: **512 bytes**
- Direcciones de bloque: **16 bits = 2 bytes**
- Cada inodo contiene:
  - **10 punteros directos**
  - **1 puntero indirecto simple**
  - **1 puntero indirecto doble**

---

## 1. Tamaño máximo de un fichero

### Bloques direccionables mediante punteros directos

El inodo contiene **10 punteros directos**, por lo que puede direccionar:

$$
10 \text{ bloques}
$$

Espacio direccionado:

$$
10 \times 512 = 5120 \text{ bytes}
$$

---

### Bloques direccionables mediante el puntero indirecto simple

Cada bloque indirecto ocupa 512 bytes.

Cada dirección ocupa:

$$
16 \text{ bits} = 2 \text{ bytes}
$$

Número de punteros que caben en un bloque indirecto:

$$
\frac{512}{2} = 256 \text{ punteros}
$$

Por tanto, el puntero indirecto simple puede direccionar:

$$
256 \text{ bloques}
$$

Espacio direccionado:

$$
256 \times 512 = 131072 \text{ bytes}
$$

---

### Bloques direccionables mediante el puntero indirecto doble

El bloque indirecto doble contiene:

$$
256 \text{ punteros}
$$

Cada uno apunta a un bloque indirecto simple con otros:

$$
256 \text{ punteros}
$$

Por tanto, puede direccionar:

$$
256 \times 256 = 65536 \text{ bloques}
$$

Espacio direccionado:

$$
65536 \times 512 = 33554432 \text{ bytes}
$$

---

### Tamaño máximo del fichero

Número total de bloques de datos:

$$
10 + 256 + 65536 = 65802 \text{ bloques}
$$

Tamaño máximo:

$$
65802 \times 512 = 33690624 \text{ bytes}
$$

Equivale aproximadamente a:

$$
\frac{33690624}{1024^2} = 32.13 \text{ MB}
$$

**El tamaño máximo de un fichero es:**

$$
\boxed{33\,690\,624\ \text{bytes} \approx 32.13\ \text{MB}}
$$

---

## 2. Escritura de un byte en las posiciones 1000 y 10000

Se crea un fichero vacío y se escribe:

- Un byte en la posición **1000**.
- Un byte en la posición **10000**.

Debemos determinar cuántos bloques de disco se asignan.

---

### Posición 1000

Bloque lógico correspondiente:

$$
\left\lfloor \frac{1000}{512} \right\rfloor = 1
$$

Es decir, corresponde al **segundo bloque de datos**.

Como el inodo dispone de 10 punteros directos, este bloque se almacena mediante un puntero directo.

Bloques necesarios:

- 1 bloque de datos.

---

### Posición 10000

Bloque lógico correspondiente:

$$
\left\lfloor \frac{10000}{512} \right\rfloor = 19
$$

El bloque lógico 19 corresponde al vigésimo bloque del fichero.

Los bloques:

- 0–9 → punteros directos.
- 10–265 → puntero indirecto simple.

Por tanto, el bloque lógico 19 se direcciona mediante el **puntero indirecto simple**.

Para ello se necesitan:

- 1 bloque indirecto simple.
- 1 bloque de datos.

---

### Bloques totales asignados

#### Bloques de datos

- Bloque lógico 1.
- Bloque lógico 19.

Total:

$$
2 \text{ bloques de datos}
$$

#### Bloques de metadatos

- 1 bloque indirecto simple.

Total:

$$
1 \text{ bloque}
$$

---

### Resultado final

El sistema asignará:

| Tipo de bloque | Cantidad |
|----------------|-----------|
| Bloques de datos | 2 |
| Bloques indirectos | 1 |
| **Total** | **3 bloques** |

Por tanto, el programa provocará la asignación de:

$$
\boxed{3\ \text{bloques de disco}}
$$

---

### Esquema del fichero

```text
INODO
├── Directo[0] → vacío
├── Directo[1] → bloque de datos (posición 1000)
├── Directo[2-9] → vacíos
├── Indirecto simple
│     └── Entrada 9 → bloque de datos (posición 10000)
└── Indirecto doble → vacío
```

Obsérvese que los bloques intermedios no se asignan físicamente, por lo que el fichero resultante es un **fichero disperso (sparse file)**.

--- 

#### Ejercicio 18.  Describe detalladamente qué operaciones relativas al sistema de ficheros realiza el sistema operativo al ejecutar leer los contenidos de /home/ubuntu/.bashrc en un sistema de ficheros tipo ext con tamaño de bloque de 4K. Suponer que el nodo-i del directorio raíz está ya en memoria y que el resto de las cachés del VFS están vacías.


respuesta:

## Ejercicio 18

Se desea leer el contenido del fichero:

```text
/home/ubuntu/.bashrc
```

en un sistema de ficheros **ext** con tamaño de bloque de **4 KB**.

Se supone que:

- El **nodo-i del directorio raíz (`/`) ya está cargado en memoria**.
- **Todas las cachés del VFS (Virtual File System) están vacías**, excepto dicho nodo-i.

---

## 1. Resolución de la ruta (`pathname resolution`)

Para acceder al fichero `/home/ubuntu/.bashrc`, el sistema operativo debe recorrer secuencialmente cada componente de la ruta:

```text
/
└── home
    └── ubuntu
        └── .bashrc
```

El VFS utiliza el nodo-i del directorio actual para localizar la entrada correspondiente al siguiente componente del camino.

---

## 2. Acceso al directorio raíz (`/`)

El nodo-i del directorio raíz **ya se encuentra en memoria**, por lo que no es necesario leerlo desde disco.

Sin embargo, como la caché de dentries está vacía, el sistema debe:

1. Localizar los bloques de datos asociados al directorio raíz utilizando su nodo-i.
2. Leer del disco los bloques que contienen las entradas del directorio.
3. Buscar la entrada:

```text
home
```

4. Obtener el número de nodo-i asociado al directorio `home`.
5. Crear en la caché VFS una entrada (`dentry`) para:

```text
/ → home
```

---

## 3. Carga del nodo-i de `/home`

Como la caché de inodos está vacía, el sistema debe:

1. Determinar en qué grupo de bloques se encuentra el nodo-i de `home`.
2. Acceder a la tabla de inodos correspondiente.
3. Leer desde disco el nodo-i de `home`.
4. Insertarlo en la caché de inodos del VFS.

---

## 4. Búsqueda de `ubuntu` dentro de `/home`

Con el nodo-i de `home` ya cargado:

1. Se leen los bloques de datos del directorio `/home`.
2. Se examinan las entradas del directorio hasta encontrar:

```text
ubuntu
```

3. Se obtiene el número de nodo-i correspondiente.
4. Se almacena la nueva dentry en la caché.

---

## 5. Carga del nodo-i de `/home/ubuntu`

El sistema:

1. Lee desde la tabla de inodos el nodo-i de `ubuntu`.
2. Lo incorpora a la caché de inodos.

---

## 6. Búsqueda del fichero `.bashrc`

Usando el nodo-i del directorio `/home/ubuntu`:

1. Se leen los bloques del directorio.
2. Se busca la entrada:

```text
.bashrc
```

3. Se obtiene el número de nodo-i asociado al fichero.
4. Se crea la correspondiente entrada en la caché de dentries.

---

## 7. Carga del nodo-i de `.bashrc`

El sistema:

1. Localiza el nodo-i del fichero en la tabla de inodos.
2. Lo lee desde disco.
3. Lo almacena en la caché de inodos.

El nodo-i contiene:

- Permisos del fichero.
- Propietario y grupo.
- Tamaño del fichero.
- Fechas de acceso/modificación.
- Direcciones de los bloques de datos.

---

## 8. Comprobación de permisos

Antes de realizar la lectura, el kernel verifica que el proceso tiene permisos suficientes para acceder al fichero:

- Permiso de ejecución (`x`) sobre los directorios:

```text
/
/home
/home/ubuntu
```

- Permiso de lectura (`r`) sobre:

```text
.bashrc
```

Si la comprobación falla, se devuelve un error:

```text
EACCES
```

---

## 9. Apertura del fichero (`open()`)

Si los permisos son correctos:

1. Se crea una estructura `file` en memoria.
2. Se inicializa el desplazamiento (`offset`) a cero.
3. Se asocia dicha estructura al descriptor de fichero devuelto al proceso.

---

## 10. Lectura del contenido (`read()`)

Para satisfacer la llamada a lectura:

1. Se determinan los bloques de datos necesarios a partir del nodo-i.
2. Como la caché de páginas está vacía, los bloques deben leerse desde disco.
3. Cada bloque leído se almacena en la **page cache**.
4. Los datos solicitados se copian desde la page cache al espacio de usuario.

---

## 11. Actualización del tiempo de acceso

Tras la lectura, el sistema puede actualizar el campo:

```text
atime
```

del nodo-i del fichero.

Dependiendo de la política de montaje (`relatime`, `noatime`, etc.), esta actualización puede:

- Realizarse inmediatamente.
- Diferirse.
- Omitirse.

---

## 12. Estado final de las cachés

Después de completar la operación, el VFS contendrá:

### Caché de inodos

- Nodo-i de `/`
- Nodo-i de `/home`
- Nodo-i de `/home/ubuntu`
- Nodo-i de `.bashrc`

### Caché de dentries

```text
/ → home
/home → ubuntu
/home/ubuntu → .bashrc
```

### Page cache

- Los bloques de datos leídos de `.bashrc`.
- Posiblemente algunos bloques de los directorios recorridos.

---

## Resumen de accesos al sistema de ficheros

1. Usar el nodo-i raíz ya presente en memoria.
2. Leer el directorio `/` para localizar `home`.
3. Leer el nodo-i de `home`.
4. Leer el directorio `/home` para localizar `ubuntu`.
5. Leer el nodo-i de `ubuntu`.
6. Leer el directorio `/home/ubuntu` para localizar `.bashrc`.
7. Leer el nodo-i de `.bashrc`.
8. Verificar permisos.
9. Crear la estructura `file`.
10. Leer los bloques de datos del fichero.
11. Actualizar `atime` si procede.
12. Almacenar la información obtenida en las distintas cachés del VFS.

En consecuencia, aunque el nodo-i raíz ya estuviese en memoria, la primera lectura del fichero requiere recorrer toda la jerarquía de directorios y cargar en memoria los inodos, dentries y bloques de datos necesarios.