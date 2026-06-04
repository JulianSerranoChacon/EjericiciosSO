
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

