
#### 💻 Ejercicio 1. ps(1) permite ver los procesos del sistema y su estado. Estudiar la página de manual y determinar las opciones necesarias para:
Mostrar todos los procesos del usuario actual en formato extendido. (Nota: usar la variable de entorno USER).
Mostrar los procesos del sistema, incluyendo el identificador del proceso, el identificador del grupo de procesos, el identificador de sesión, el estado y el comando con todos sus argumentos.
Observar el identificador de proceso, grupo de procesos y sesión de los procesos. ¿Qué identificadores comparten la shell y los programas que se ejecutan en ella (usar la opción -H para identificar fácilmente la relación entre procesos)? 
¿Cuál es el identificador de grupo de procesos cuando se crea un nuevo proceso? Usar el comando sleep para poder verlo fácilmente en la salida del comando ps.

Respuesta:

Para resolver este ejercicio, hay que consultar la página de manual de `ps` (`man ps`) y localizar las opciones adecuadas.

---

## 1. Mostrar todos los procesos del usuario actual en formato extendido

La opción `-u` permite seleccionar procesos de un usuario específico y `-f` muestra el **formato completo (full format)**.

Usando la variable de entorno `USER`:

```bash
ps -fu "$USER"
```

o equivalentemente:

```bash
ps -f -u "$USER"
```

Esto mostrará información como:

- UID (usuario propietario)
    
- PID (identificador del proceso)
    
- PPID (identificador del proceso padre)
    
- Hora de inicio
    
- Terminal asociada
    
- Tiempo de CPU consumido
    
- Comando ejecutado

![](Imagenes/Pasted%20image%2020260607190144.png)

---

## 2. Mostrar los procesos del sistema incluyendo PID, PGID, SID, estado y comando completo

Las opciones necesarias son:

- `-e`: muestra todos los procesos del sistema.
    
- `-o`: permite especificar el formato de salida personalizado.
    

Comando:

```bash
ps -e -o pid,pgid,sid,state,args
```

Campos mostrados:

- `pid`: Process ID (identificador del proceso).
    
- `pgid`: Process Group ID (identificador del grupo de procesos).
    
- `sid`: Session ID (identificador de sesión).
    
- `state`: estado del proceso (`R`, `S`, `T`, `Z`, etc.).
    
- `args`: comando completo con todos sus argumentos.
    

Ejemplo de salida:

```text
  PID  PGID   SID S COMMAND
 1023  1023  1023 S bash
 1050  1050  1023 R ps -e -o pid,pgid,sid,state,args
```

![](Imagenes/Pasted%20image%2020260607190933.png)

---

## 3. ¿Qué identificadores comparten la shell y los programas que se ejecutan en ella?

Para observar fácilmente la relación jerárquica entre procesos, puede usarse la opción `-H`:

```bash
ps -H -o pid,ppid,pgid,sid,cmd
```

![](Imagenes/Pasted%20image%2020260607191034.png)

o, limitándolo a los procesos del usuario:

```bash
ps -H -u "$USER" -o pid,ppid,pgid,sid,cmd
```

![](Imagenes/Pasted%20image%2020260607191112.png)

El usuario tiene muchos más procesos que la shell
### Observaciones típicas

Supongamos la siguiente salida:

```text
PID   PPID  PGID   SID CMD
2100  2050  2100  2100 bash
2150  2100  2100  2100  vim archivo.txt
2200  2100  2100  2100  ls -l
```

Se observa que:

- **La shell y los programas ejecutados desde ella comparten el mismo SID (Session ID)**.
    
- Los procesos que forman parte del mismo trabajo en primer plano suelen **compartir el mismo PGID (Process Group ID)**.
    
- Cada proceso tiene un **PID único**.
    

Por tanto:

> **La shell y los programas que ejecuta comparten el identificador de sesión (SID). Además, los procesos pertenecientes al mismo trabajo suelen compartir el mismo identificador de grupo de procesos (PGID).**

<img src="Imagenes/Pasted%20image%2020260607191339.png" width="697">

procesos de Wayland con el mismo PGID.

---

## 4. ¿Cuál es el identificador de grupo de procesos cuando se crea un nuevo proceso?

Para comprobarlo, ejecuta un proceso sencillo como:

```bash
sleep 60 &
```

Después consulta:

```bash
ps -H -o pid,ppid,pgid,sid,cmd | grep sleep
```

o:

```bash
ps -o pid,ppid,pgid,sid,cmd -C sleep
```

Ejemplo:

```text
PID   PPID  PGID   SID CMD
2500  2100  2500  2100 sleep 60
```

Se observa que:

- El proceso `sleep` tiene `PID = 2500`.
    
- Su `PGID = 2500`.
    

### Conclusión

> Cuando se crea un **nuevo grupo de procesos**, el **identificador del grupo de procesos (PGID) coincide con el PID del proceso líder del grupo**.

En el caso de un comando ejecutado en segundo plano (`sleep 60 &`), normalmente el propio proceso `sleep` se convierte en líder del nuevo grupo, por lo que:

```text
PGID = PID
```

![](Imagenes/Pasted%20image%2020260607193918.png)


---

### Comandos resumen

```bash
# Procesos del usuario actual en formato extendido
ps -fu "$USER"

# Todos los procesos con PID, PGID, SID, estado y comando
ps -e -o pid,pgid,sid,state,args

# Ver relaciones jerárquicas entre procesos
ps -H -u "$USER" -o pid,ppid,pgid,sid,cmd

# Crear un proceso para observar sus identificadores
sleep 60 &

# Consultar sus identificadores
ps -o pid,ppid,pgid,sid,cmd -C sleep
```

Estas observaciones permiten entender la relación entre **procesos**, **grupos de procesos** y **sesiones** en sistemas Unix/Linux.

---

💻 Ejercicio 2. Escribir un programa que muestre los identificadores del proceso (PID, PPID, PGID y SID), el identificador del usuario y grupo, y su directorio de trabajo actual.

``` C
#define _POSIX_C_SOURCE 200809L //PARA PATH_MAX multiplataforma
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>


int main() {
    char cwd[PATH_MAX];

    // PID, PPID, PGID, SID
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgid = getpgid(0); // 0 = proceso actual
    pid_t sid = getsid(0);   // 0 = proceso actual

    // UID y GID
    uid_t uid = getuid();
    gid_t gid = getgid();

    // Directorio de trabajo actual
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return 1;
    }

    printf("PID: %d\n", pid);
    printf("PPID: %d\n", ppid);
    printf("PGID: %d\n", pgid);
    printf("SID: %d\n", sid);
    printf("UID: %d\n", uid);
    printf("GID: %d\n", gid);
    printf("Directorio actual: %s\n", cwd);

    return 0;
}
```

![](Imagenes/Pasted%20image%2020260607194639.png)

Para esta prueba he añadido la línea:
```C
sleep(30);
```

al final del script.

Se me ha olvidado mostar UID y GID pero el UID 1000 coresponde al de mi usuario.

---

#### Ejercicio 3. Considera la siguiente orden de la línea de comandos:

```bash
echo "12345" > /proc/$$/fd/1
```

Discuta razonadamente cuál es el resultado esperado de la ejecución.

Respuesta:

Analicemos la orden:

```bash
echo "12345" > /proc/$$/fd/1
```

### 1. ¿Qué significa `$$`?

En una shell, `$$` se sustituye por el **PID de la propia shell** que está ejecutando el comando.

Por ejemplo, si el PID de la shell es `3245`, la orden quedaría:

```bash
echo "12345" > /proc/3245/fd/1
```

---

### 2. ¿Qué es `/proc/<PID>/fd/`?

En Linux, el directorio `/proc/<PID>/fd/` contiene enlaces simbólicos a los **descriptores de fichero abiertos** por el proceso con PID `<PID>`.

Los descriptores estándar son:

- `0` → entrada estándar (**stdin**)
    
- `1` → salida estándar (**stdout**)
    
- `2` → salida de error estándar (**stderr**)
    

Por tanto:

```text
/proc/$$/fd/1
```

representa el **descriptor de salida estándar (stdout) de la shell actual**.

---

### 3. ¿Qué hace la redirección `>`?

La redirección:

```bash
> /proc/$$/fd/1
```

hace que la salida del comando `echo` se escriba en el fichero indicado. Pero ese "fichero" es realmente el mismo descriptor `stdout` de la shell.

Es equivalente a decir:

> "Escribe la salida estándar del comando en la salida estándar de la shell."

---

### Resultado esperado

Si la salida estándar de la shell está conectada al terminal (caso habitual), el resultado será simplemente:

```text
12345
```

mostrado en la pantalla.

Es decir, el comportamiento será prácticamente el mismo que ejecutar:

```bash
echo "12345"
```

---

### Justificación

- `$$` obtiene el PID de la shell.
    
- `/proc/$$/fd/1` referencia el descriptor de salida estándar de esa shell.
    
- La redirección `>` envía la salida de `echo` a ese descriptor.
    
- Como dicho descriptor apunta normalmente al terminal, el texto aparece por pantalla.
    

Por tanto:

> **La ejecución imprimirá `12345` en la salida estándar (normalmente el terminal), ya que la redirección se realiza hacia el propio descriptor de salida estándar de la shell.**

![](Imagenes/Pasted%20image%2020260607195316.png)

En fish el comando no es valido, en bash sí.

La diferencia se debe a que **`fish` no implementa las redirecciones exactamente igual que `bash`**.

## ¿Qué pasa en Bash?

En Bash:

```bash
echo "12345" > /proc/$$/fd/1
```

1. `$$` se expande al PID de la shell.
    
2. Bash abre `/proc/<PID>/fd/1` para escritura.
    
3. Ese archivo es un enlace al descriptor 1 (stdout) de la propia shell.
    
4. El `echo` escribe ahí, que finalmente acaba siendo el terminal.
    

Por eso ves:

```text
12345
```

---

## ¿Qué pasa en Fish?

Fish da este error:

```text
fish: Destino de redirección inválido
```

porque **fish detecta que estás intentando redirigir la salida hacia uno de sus propios descriptores de archivo** (`/proc/self/fd/*` o equivalentes) y lo considera una operación inválida.

Esto es una **decisión de diseño de fish** para evitar comportamientos ambiguos o peligrosos, como:

```bash
cmd > /proc/self/fd/1
cmd > /dev/stdout
cmd > /proc/$$/fd/1
```

donde la shell tendría que gestionar una redirección cuyo destino es precisamente el descriptor que está intentando reemplazar.

Bash simplemente lo permite y el kernel lo resuelve sin problemas.

---

## El caso de `sudo`

```fish
sudo echo "12345" > /proc/$$/fd/1
```

sigue fallando por la misma razón.

Es importante notar que:

```bash
sudo echo hola
```

**no ejecuta `echo` con privilegios elevados**, porque `echo` es un builtin de la shell.

Además, **la redirección `>` siempre la procesa la shell actual antes de ejecutar el comando**. En tu caso:

1. Fish intenta abrir `/proc/$$/fd/1`.
    
2. Fish decide que ese destino es inválido.
    
3. Nunca llega a ejecutarse `sudo`.
    

---

## Si quieres hacerlo funcionar en fish

Puedes ejecutar Bash explícitamente:

```fish
bash -c 'echo "12345" > /proc/$$/fd/1'
```

o simplemente:

```fish
echo "12345"
```

---

## Para el ejercicio

La respuesta teórica sigue siendo correcta:

>  En una shell POSIX como Bash, el comando imprime `12345` por pantalla porque `/proc/$$/fd/1` referencia la salida estándar de la shell actual.

Sin embargo, **fish no sigue completamente la semántica POSIX de Bash para este tipo de redirecciones y las rechaza como destinos inválidos**, por lo que el comando produce un error en lugar de ejecutarse.

---

#### 💻 Ejercicio 4. Consulta el tipo de fichero y contenido de los siguientes ficheros del proceso de la shell actual `(/proc/$$)` completa la siguiente tabla:

|         |                             |                                        |
| ------- | --------------------------- | -------------------------------------- |
| Fichero | Tipo<br><br>enlace, dir,... | Descripción<br><br>contenido/propósito |
| cmdline |                             |                                        |
| cwd     |                             |                                        |
| environ |                             |                                        |
| exe     |                             |                                        |
| fd      |                             |                                        |
| limits  |                             |                                        |
| maps    |                             |                                        |
| root    |                             |                                        |
|         |                             |                                        |

![](Imagenes/Pasted%20image%2020260607195908.png)

respuesta:

| Fichero   | Tipo                                   | Descripción / contenido / propósito                                                                                             |
| --------- | -------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------- |
| `cmdline` | Fichero regular virtual (`-r--r--r--`) | Contiene la línea de comandos con la que se ejecutó el proceso. Los argumentos están separados por caracteres nulos (`\0`).     |
| `cwd`     | Enlace simbólico (`lrwxrwxrwx`)        | Apunta al directorio de trabajo actual del proceso. En tu caso: `/home/verxxuxz/Documentos/SO/EjerciciosSO/Tercera-hoja`.       |
| `environ` | Fichero regular virtual (`-r--------`) | Contiene las variables de entorno del proceso (`PATH`, `HOME`, `USER`, etc.), separadas por caracteres nulos (`\0`).            |
| `exe`     | Enlace simbólico (`lrwxrwxrwx`)        | Apunta al ejecutable asociado al proceso. En tu caso: `/usr/bin/bash`.                                                          |
| `fd`      | Directorio (`dr-x------`)              | Contiene enlaces simbólicos a los descriptores de fichero abiertos por el proceso (`0`: stdin, `1`: stdout, `2`: stderr, etc.). |
| `limits`  | Fichero regular virtual (`-r--r--r--`) | Muestra los límites de recursos del proceso (archivos abiertos, tamaño de pila, número máximo de procesos, etc.).               |
| `maps`    | Fichero regular virtual (`-r--r--r--`) | Muestra el mapa de memoria del proceso: regiones de memoria, bibliotecas compartidas cargadas y permisos asociados.             |
| `root`    | Enlace simbólico (`lrwxrwxrwx`)        | Apunta al directorio raíz visto por el proceso. En tu caso debería ser `/`.                                                     |

![](Imagenes/Pasted%20image%2020260607200308.png)

``` bash
[verxxuxz@CachyVerXxuxZ Tercera-hoja]$ # Ver el tipo
ls -ld /proc/$$/cmdline
ls -ld /proc/$$/cwd
ls -ld /proc/$$/environ
ls -ld /proc/$$/exe
ls -ld /proc/$$/fd
ls -ld /proc/$$/limits
ls -ld /proc/$$/maps
ls -ld /proc/$$/root
-r--r--r-- 1 verxxuxz verxxuxz 0 jun  7 19:59 /proc/16202/cmdline
lrwxrwxrwx 1 verxxuxz verxxuxz 0 jun  7 20:01 /proc/16202/cwd -> /home/verxxuxz/Documentos/SO/EjerciciosSO/Tercera-hoja
-r-------- 1 verxxuxz verxxuxz 0 jun  7 20:01 /proc/16202/environ
lrwxrwxrwx 1 verxxuxz verxxuxz 0 jun  7 19:59 /proc/16202/exe -> /usr/bin/bash
dr-x------ 2 verxxuxz verxxuxz 9 jun  7 20:01 /proc/16202/fd
-r--r--r-- 1 verxxuxz verxxuxz 0 jun  7 20:01 /proc/16202/limits
-r--r--r-- 1 verxxuxz verxxuxz 0 jun  7 20:01 /proc/16202/maps
lrwxrwxrwx 1 verxxuxz verxxuxz 0 jun  7 20:01 /proc/16202/root -> /


[verxxuxz@CachyVerXxuxZ Tercera-hoja]$ cat /proc/$$/cmdline | tr '\0' ' '; echo ""
bash

[verxxuxz@CachyVerXxuxZ Tercera-hocat /proc/$$/environ | tr '\0' '\n' | head head
UWSM_WAIT_VARNAMES=HYPRLAND_INSTANCE_SIGNATURE
ALACRITTY_LOG=/tmp/Alacritty-14986.log
XDG_SESSION_ID=2
LC_TELEPHONE=es_ES.UTF-8
NOTIFY_SOCKET=/run/user/1000/systemd/notify
XDG_DATA_HOME=/home/verxxuxz/.local/share
UWSM_FINALIZE_VARNAMES=HYPRLAND_INSTANCE_SIGNATURE HYPRLAND_CMD HYPRCURSOR_THEME HYPRCURSOR_SIZE XCURSOR_SIZE XCURSOR_THEME
QT_QPA_PLATFORMTHEME=qt5ct
XDG_MENU_PREFIX=hyprland-
XDG_SESSION_CLASS=user


[verxxuxz@CachyVerXxuxZ Tercera-hoja]$ ls -l /proc/$$/fd
total 0
lrwx------ 1 verxxuxz verxxuxz 64 jun  7 20:03 0 -> /dev/pts/0
lrwx------ 1 verxxuxz verxxuxz 64 jun  7 20:03 1 -> /dev/pts/0
lr-x------ 1 verxxuxz verxxuxz 64 jun  7 20:03 13 -> anon_inode:inotify
lrwx------ 1 verxxuxz verxxuxz 64 jun  7 20:03 2 -> /dev/pts/0
lrwx------ 1 verxxuxz verxxuxz 64 jun  7 20:03 255 -> /dev/pts/0
lr-x------ 1 verxxuxz verxxuxz 64 jun  7 20:03 3 -> 'pipe:[14263]'
l-wx------ 1 verxxuxz verxxuxz 64 jun  7 20:03 4 -> 'pipe:[14263]'
lr-x------ 1 verxxuxz verxxuxz 64 jun  7 20:03 5 -> 'pipe:[14264]'
l-wx------ 1 verxxuxz verxxuxz 64 jun  7 20:03 6 -> 'pipe:[14264]'


[verxxuxz@CachyVerXxuxZ Tercera-hoja]$ head /proc/$$/limits
Limit                     Soft Limit           Hard Limit           Units
Max cpu time              unlimited            unlimited            seconds
Max file size             unlimited            unlimited            bytes
Max data size             unlimited            unlimited            bytes
Max stack size            8388608              unlimited            bytes
Max core file size        unlimited            unlimited            bytes
Max resident set          unlimited            unlimited            bytes
Max processes             30203                30203                processes
Max open files            1024                 1048576              files
Max locked memory         8388608              8388608              bytes


[verxxuxz@CachyVerXxuxZ Tercera-hoja]$ head /proc/$$/maps
55a4912d2000-55a4912ee000 r--p 00000000 00:1c 1070010                    /usr/bin/bash
55a4912ee000-55a4913a6000 r-xp 0001c000 00:1c 1070010                    /usr/bin/bash
55a4913a6000-55a4913db000 r--p 000d4000 00:1c 1070010                    /usr/bin/bash
55a4913db000-55a4913de000 r--p 00109000 00:1c 1070010                    /usr/bin/bash
55a4913de000-55a4913e2000 rw-p 0010c000 00:1c 1070010                    /usr/bin/bash
55a4913e2000-55a4913f1000 rw-p 00000000 00:00 0
55a4a4291000-55a4a43db000 rw-p 00000000 00:00 0                          [heap]
7f27ae5d1000-7f27ae600000 r--p 00000000 00:1c 1070141                    /usr/share/locale/es/LC_MESSAGES/bash.mo
7f27ae600000-7f27aeb65000 r--p 00000000 00:1c 1988632                    /usr/lib/locale/locale-archive
7f27aeb7d000-7f27aeb8a000 r--p 00000000 00:1c 1989068                    /usr/lib/libncursesw.so.6.6


[verxxuxz@CachyVerXxuxZ Tercera-hoja]$ readlink /proc/$$/root
/


[verxxuxz@CachyVerXxuxZ Tercera-hoja]$
```


---

#### 💻Ejercicio 5.  Escribir un programa que cree un proceso hijo con las siguientes características:
- El programa recibirá dos argumentos en la forma: 
``` bash
./ejercicio5 <segundos_padre> <segundos_hijo>
```
- El hijo creará su propia sesión, imprimirá sus identificadores (como en el Ejercicio 2), esperará `<segundos hijo>` (segundo argumento) con la llamada sleep(3); y terminará.
- El padre imprimirá sus identificadores (Ejercicio 2), esperará `<segundos padre>` (primer argumento) con la llamada sleep; y terminará.

Ejemplo de salida:
```bash
$ ./eje5 2 1
[Padre] PID=1616, PPID=1362, PGID=1616, SID=1362. Durmiendo 2s
[Hijo] PID=1617, PPID=1616, PGID=1617, SID=1617. Durmiendo 1s
[Hijo] Terminado.
[Padre] Terminado.
```

Considera las siguientes ejecuciones, observa los procesos con la orden `ps -lHu $USER` y completa la siguiente tabla para los procesos relacionados con el ejercicio:

|               |             |                    |        |
| ------------- | ----------- | ------------------ | ------ |
| Orden         | Proceso PID | PPID, CMD ( padre) | Estado |
| ./eje5 600 1& |             |                    |        |
|               |             |                    |        |
| ./eje5 1 600& |             |                    |        |
|               |             |                    |        |

> NOTA: no todas las filas son necesarias

Explica razonadamente el estado de los procesos en el primer caso (el hijo termina antes) y el PPID de los procesos en el segundo caso (el padre termina antes), identifica el proceso padre con la ayuda del comando ps(1).
Ejecuta el programa con un tiempo de espera largo (ej. ./eje5 60 60), antes de que terminen las llamadas a sleep(3) presiona Ctrl-C en el terminal. ¿Qué ocurre? ¿Mueren ambos procesos? ¿Por qué?

Respuesta:
``` C
#define _POSIX_C_SOURCE 200809L //PARA PATH_MAX multiplataforma
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>


void imprimir_ids(const char *quien) {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return;
    }

    printf("[%s] PID=%d PPID=%d PGID=%d SID=%d UID=%d GID=%d\n",
           quien,
           getpid(),
           getppid(),
           getpgid(0),
           getsid(0),
           getuid(),
           getgid());

    printf("[%s] Directorio actual: %s\n", quien, cwd);
}

int main(int argc, char * argv[]){

    if(argc < 3){
        fprintf(stderr, "Uso %s <segundos_padre> <segundos_hijo>", argv[0]);
        return 1;
    }

    
    int seg_padre, seg_hijo;
    seg_padre = atoi(argv[1]);
    seg_hijo = atoi(argv[2]);

    if(seg_padre < 0 || seg_hijo < 0){
        fprintf(stderr, "Los argumentos deben ser números enteros mayores o iguales a 0\n");
        return 1;
    }


    pid_t pidChild;
    pidChild = fork();

    if (pidChild < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }else if (pidChild == 0) {      // HIJO
        setsid();             // crea nueva sesión

        imprimir_ids("Hijo");       // código del ejercicio 2
        sleep(seg_hijo);
        printf("[Hijo] Terminado.\n");
        exit(EXIT_SUCCESS);
    }
    else {                    // PADRE
        imprimir_ids("Padre");
        sleep(seg_padre);
        printf("[Padre] Terminado.\n");
        exit(EXIT_SUCCESS);
    }
    return 0;
}
```

![](Imagenes/Pasted%20image%2020260607211207.png)

![](Imagenes/Pasted%20image%2020260607211655.png)

![](Imagenes/Pasted%20image%2020260607211727.png)

- En el primer caso el hijo termina y queda como Zombie esperando a que termine el proceso padre y poder morir.
### Explicación del primer caso (`./Ejercicio5.exe 600 1 &`)

En este caso:

- El **padre** duerme durante 600 segundos.
- El **hijo** duerme durante 1 segundo y termina rápidamente.

Al terminar el hijo, el padre sigue ejecutándose y **no llama a `wait()` ni `waitpid()`** para recoger el estado de finalización del hijo. Por ello, el proceso hijo permanece en la tabla de procesos como un **proceso zombi**, lo que se observa en `ps` mediante el estado **`Z`** y el sufijo `<defunct>`:

```
20332 20330 Z ... Ejercicio5.exe <defunct>
```

El kernel mantiene la entrada del hijo hasta que el padre recoja su estado o termine.



![](Imagenes/Pasted%20image%2020260607212346.png)
![](Imagenes/Pasted%20image%2020260607212312.png)

![](Imagenes/Pasted%20image%2020260607212241.png)

- En el segundo caso tras la muerte del procso padre el hijo queda huerfano y es adoptado por el proceso de systemd (840) a la espera de que termine como podemos ver en la segunda imagen (en la primera el padre aún vive).

--- 

Perfecto, ahora sí se ve claramente el comportamiento esperado.

Tu observación es correcta: **cuando el padre termina antes, el hijo es adoptado por `systemd` con PID 840** en tu sistema.

Por tanto, la tabla definitiva sería:

|Orden|Proceso PID|PPID, CMD (padre)|Estado|
|---|---|---|---|
|`./Ejericicio5.exe 600 1 &`|20330|14995 (`fish`)|`S`|
||20332|20330 (`Ejericicio5.exe`)|`Z` (`<defunct>`)|
|`./Ejericicio5.exe 1 600 &`|20778|840 (`systemd`)|`S`|

Y la explicación podría quedar así:

> **Primer caso (`./Ejericicio5.exe 600 1 &`):** el hijo termina antes que el padre. Como el padre no ejecuta `wait()` ni `waitpid()` para recoger el estado de finalización del hijo, éste permanece en la tabla de procesos en estado **Zombie (`Z`)**, apareciendo como `<defunct>`, hasta que el padre termine.

> **Segundo caso (`./Ejericicio5.exe 1 600 &`):** el padre termina antes que el hijo. El hijo se convierte en un **proceso huérfano** y el kernel lo reasigna automáticamente a un nuevo proceso padre. En este sistema, dicho proceso es **`systemd` con PID 840**, por lo que el **PPID del hijo cambia de 20776 a 840**, permaneciendo en estado **Sleeping (`S`)** hasta finalizar su ejecución.

Además, algo interesante que has comprobado es que **no siempre el nuevo PPID es 1**. En sistemas modernos con `systemd`, los procesos huérfanos pueden ser adoptados por el **`systemd --user`** de la sesión del usuario (PID 840 en tu caso), en lugar del `systemd` principal (PID 1).

Por último, la pregunta del **Ctrl+C** se respondería así:

> Al pulsar Ctrl+C, el terminal envía la señal **SIGINT** al grupo de procesos en primer plano. Sin embargo, el hijo ejecuta `setsid()`, creando una nueva sesión y un nuevo grupo de procesos independientes del terminal. Como consecuencia, **el padre recibe SIGINT y termina, mientras que el hijo no recibe la señal y continúa ejecutándose** hasta completar su `sleep()`.


![](Imagenes/Pasted%20image%2020260607213242.png)

![](Imagenes/Pasted%20image%2020260607213226.png)

![](Imagenes/Pasted%20image%2020260607213305.png)

> Nota: Equivalente a Ctrl+C.

![](Imagenes/Pasted%20image%2020260607213342.png)


- Al igual que en el caso 2 el proceso hijo queda huerfano y es adoptado por el proceso de systemd (840).


--- 

#### 💻Ejercicio 6. kill(1) permite enviar señales a un proceso o grupo de procesos por su identificador (pkill(1) permite hacerlo por nombre de proceso). Estudiar la página de manual y las señales que se pueden enviar a un proceso.

Para este ejercicio debes revisar `man kill` y `man 7 signal`. Aquí tienes un resumen de lo más importante.

## `kill(1)`

El comando `kill` permite enviar una señal a uno o varios procesos especificados por su **PID**.

Sintaxis:

```bash
kill [opciones] <PID>
kill -SEÑAL <PID>
kill -número <PID>
```

Ejemplos:

```bash
kill 1234
```

Envía la señal por defecto (`SIGTERM`) al proceso 1234.

```bash
kill -SIGKILL 1234
kill -9 1234
```

Fuerza la terminación inmediata del proceso.

```bash
kill -SIGSTOP 1234
```

Suspende el proceso.

```bash
kill -SIGCONT 1234
```

Reanuda un proceso suspendido.

### Mostrar señales disponibles

```bash
kill -l
```

Ejemplo de salida:

```text
1) SIGHUP     2) SIGINT     3) SIGQUIT    4) SIGILL
2) SIGTRAP    6) SIGABRT    7) SIGBUS     8) SIGFPE
3) SIGKILL   10) SIGUSR1   11) SIGSEGV   12) SIGUSR2
4) SIGPIPE  14) SIGALRM   15) SIGTERM   ...
```

También puedes consultar:

```bash
man 7 signal
```

---

## `pkill(1)`

Permite enviar señales utilizando el **nombre del proceso** en lugar del PID.

Ejemplos:

```bash
pkill firefox
```

Envía `SIGTERM` a todos los procesos llamados `firefox`.

```bash
pkill -SIGKILL firefox
```

Finaliza forzosamente todos los procesos `firefox`.

```bash
pkill -u "$USER" bash
```

Envía la señal a todos los procesos `bash` pertenecientes al usuario actual.

---

## Señales más importantes

|Señal|Número|Acción por defecto|Descripción|
|---|---|---|---|
|`SIGHUP`|1|Terminar|Indica desconexión del terminal. Muchos demonios la usan para recargar configuración.|
|`SIGINT`|2|Terminar|Interrupción desde teclado (`Ctrl+C`).|
|`SIGQUIT`|3|Terminar + volcado de memoria|Interrupción con volcado (`Ctrl+\`).|
|`SIGKILL`|9|Terminar|Finalización inmediata. **No puede capturarse ni ignorarse**.|
|`SIGTERM`|15|Terminar|Solicitud de terminación ordenada. Es la señal enviada por defecto por `kill`.|
|`SIGSTOP`|19|Detener|Suspende el proceso. **No puede capturarse ni ignorarse**.|
|`SIGTSTP`|20|Detener|Suspensión interactiva (`Ctrl+Z`).|
|`SIGCONT`|18|Continuar|Reanuda un proceso detenido.|
|`SIGUSR1`|10|Terminar|Señal definida por el usuario 1.|
|`SIGUSR2`|12|Terminar|Señal definida por el usuario 2.|
|`SIGCHLD`|17|Ignorar|Se envía al padre cuando un hijo termina o cambia de estado.|
|`SIGALRM`|14|Terminar|Generada por temporizadores (`alarm()`).|
|`SIGSEGV`|11|Terminar + volcado|Error de segmentación (acceso inválido a memoria).|
|`SIGPIPE`|13|Terminar|Escritura en una tubería sin lectores.|

---

## Señales capturables y no capturables

Las señales pueden manejarse mediante `signal()` o `sigaction()`, pero existen dos excepciones:

- **`SIGKILL`**: no puede capturarse, bloquearse ni ignorarse.
    
- **`SIGSTOP`**: no puede capturarse, bloquearse ni ignorarse.
    

Esto garantiza que el sistema siempre pueda detener o suspender un proceso.

---

### Órdenes útiles

```bash
# Mostrar todas las señales
kill -l

# Ver información detallada sobre señales
man 7 signal

# Terminar un proceso correctamente
kill PID

# Forzar terminación
kill -9 PID

# Suspender un proceso
kill -STOP PID

# Reanudar un proceso
kill -CONT PID

# Terminar procesos por nombre
pkill nombre_proceso
```

Para este ejercicio, es importante recordar que **`SIGTERM` es la forma recomendada de finalizar un proceso**, mientras que **`SIGKILL` debe usarse solo cuando el proceso no responde**, ya que no permite realizar limpieza ni liberar recursos antes de terminar.

---

#### 💻Ejercicio 7. En un terminal, arrancar un proceso de larga duración (ej. sleep 600). En otra terminal, enviar diferentes señales al proceso (terminar, interrumpir, parar, continuar) y comprobar el comportamiento.

![](Imagenes/Pasted%20image%2020260607213652.png)

![](Imagenes/Pasted%20image%2020260607213746.png)

![](Imagenes/Pasted%20image%2020260607213914.png)

![](Imagenes/Pasted%20image%2020260607214041.png)

![](Imagenes/Pasted%20image%2020260607214138.png)


---

#### Ejercicio 8. Determina las opciones adecuadas para el comando kill(1) para terminar (SIGINT) los dos procesos que se crean en la ejecución del programa del ejercicio 5.

Respuesta: 

En el **Ejercicio 5**, el proceso hijo ejecuta:

```c
setsid();
```

por lo que:

- El **padre** mantiene el **PGID** heredado de la shell.
    
- El **hijo** crea una **nueva sesión** y un **nuevo grupo de procesos**, por lo que tiene un **PGID distinto** (normalmente igual a su PID).
    

Por tanto, **no puedes terminar ambos procesos con un único `kill` dirigido a un grupo de procesos**, porque pertenecen a grupos diferentes.

Tienes varias opciones:

### 1. Enviar `SIGINT` a cada PID individualmente

Primero obtén los PIDs:

```bash
ps -lHu "$USER"
```

Supongamos:

```text
Padre: PID=20330
Hijo:  PID=20332
```

Entonces:

```bash
kill -SIGINT 20330 20332
```

o equivalentemente:

```bash
kill -2 20330 20332
```

---

### 2. Usar `pkill`

Si solo existe una instancia del programa:

```bash
pkill -SIGINT Ejericicio5.exe
```

o:

```bash
pkill -2 Ejericicio5.exe
```

Esto enviará `SIGINT` a todos los procesos cuyo nombre sea `Ejericicio5.exe`, incluyendo padre e hijo.

---

### 3. Usar grupos de procesos (menos práctico aquí)

Podrías enviar la señal a cada PGID:

```bash
kill -SIGINT -<PGID_padre>
kill -SIGINT -<PGID_hijo>
```

Observa el signo `-` delante del PGID. Por ejemplo:

```bash
kill -SIGINT -20330
kill -SIGINT -20332
```

Sin embargo, como cada grupo contiene solo un proceso, esto equivale prácticamente a usar los PID.

---

### Respuesta para el ejercicio

> Debido a que el proceso hijo ejecuta `setsid()`, crea una nueva sesión y un nuevo grupo de procesos. Por ello, padre e hijo pertenecen a grupos de procesos distintos y no pueden finalizarse con una única señal dirigida a un grupo común. Para terminar ambos con `SIGINT` es necesario enviar la señal a cada PID individualmente:


```bash
kill -SIGINT <PID_padre> <PID_hijo>
```
 
 o utilizar:
 
```bash
pkill -SIGINT Ejericicio5.exe
```
 
 para enviar la señal a todos los procesos con ese nombre.


![](Imagenes/Pasted%20image%2020260607215158.png)

---

#### 💻Ejercicio 9 Escribir un programa que ejecute otro programa (ejecutable y argumentos) que se pasará como argumento. El programa creará un proceso hijo que ejecutará el programa dado en el argumento con la función execvp(3). El proceso padre esperará que termine el hijo e imprimirá su código de salida. Ejemplos de ejecución:

``` bash
$ ./eje9 cat /etc/passwd

root:x:0:0:root:/root:/bin/bash

daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin

…

El proceso hijo terminó con código de salida 0

$ ./eje9 cat /etc/shadow

cat: /etc/shadow: Permission denied

El proceso hijo terminó con código de salida 1

  

$ (./eje9 sleep 3600&) ; sleep 1 ; pkill -SIGKILL sleep

El proceso hijo terminó por señal 9
```

>Nota: Considerar cómo deben pasarse los argumentos en cada caso para que sea sencilla la implementación. Por ejemplo: ¿qué diferencia hay entre: ./eje9 ps -el y ./eje9 "ps -el"?

Respuesta:

``` C
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]){

     if (argc < 2) {
        fprintf(stderr, "Uso: %s <programa> [argumentos...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){
        // PROCESO HIJO
        execvp(argv[1], &argv[1]);

        // Si execvp falla
        perror("execvp");
        _exit(EXIT_FAILURE);

        /*OTRA POSIBLE SOLUCION
                //===== PROCESO HIJO =====

        /*Creamos un nuevo vector de argumentos:
           - descartamos argv[0]
           - argv[1] pasa a ser args[0]/*
        
        int n = argc - 1;
        char *args[n + 1];

        for (int i = 0; i < n; i++) {
            args[i] = argv[i + 1];
        }
        args[n] = NULL;

        execvp(args[0], args);

        /* Solo se llega aquí si execvp falla
        perror("execvp");
        exit(EXIT_FAILURE);*/
    }
    else{
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("El proceso hijo terminó con código de salida %d\n",
                   WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("El proceso hijo terminó por señal %d\n",
                   WTERMSIG(status));
        }
    }
    return 0;
}
```


Se debe ejecutar el programa como:

```
./eje9 programa [argumentos...]
```

porque `execvp(argv[1], &argv[1])` reutiliza directamente el vector de argumentos del programa actual a partir de `argv[1]`.

Por ejemplo:

```
./eje9 ps -el
```

produce:

```
execvp("ps", {"ps", "-el", NULL});
```

mientras que:

```
./eje9 "ps -el"
```

genera:

```
execvp("ps -el", {"ps -el", NULL});
```

intentando ejecutar un programa llamado `"ps -el"`, lo que provoca un error al no existir dicho ejecutable.


ADEMAS

Exacto. **Si `execvp()` tiene éxito, nunca se ejecutan las líneas que vienen después.**

Esto es porque `execvp()` **reemplaza completamente la imagen del proceso actual** por el nuevo programa.

Por ejemplo:

```c
pid_t pid = fork();

if (pid == 0) {  // Hijo
    printf("Antes del exec\n");

    execvp(argv[1], &argv[1]);

    perror("execvp");
    _exit(EXIT_FAILURE);

    printf("Esto nunca se ejecuta\n");
}
```

### Caso 1: `execvp()` tiene éxito

Si ejecutas:

```bash
./eje9 ls
```

el flujo es:

```text
Hijo
 ↓
printf("Antes del exec")
 ↓
execvp("ls", ...)
 ↓
El proceso se convierte en "ls"
 ↓
Se ejecuta ls
 ↓
ls termina
```

Las líneas:

```c
perror("execvp");
_exit(EXIT_FAILURE);
printf("Esto nunca se ejecuta");
```

**NO se ejecutan nunca**.

---

### Caso 2: `execvp()` falla

Por ejemplo:

```bash
./eje9 programa_que_no_existe
```

Flujo:

```text
Hijo
 ↓
printf("Antes del exec")
 ↓
execvp(...) falla
 ↓
retorna -1
 ↓
perror("execvp")
 ↓
_exit(EXIT_FAILURE)
```

Aquí sí se ejecuta el código posterior porque `execvp()` **solo retorna cuando ocurre un error**.

---

Por eso es tan común ver este patrón:

```c
execvp(argv[1], &argv[1]);

perror("execvp");  // Solo si execvp falla
_exit(EXIT_FAILURE);
```

Se interpreta como:

> "Intenta ejecutar este programa. Si vuelves aquí, es que ha habido un error."

De hecho, el manual (`man 3 exec`) dice:

> The `exec()` functions return only if an error has occurred.

Es decir:

- ✅ Éxito → **no vuelve nunca**.
    
- ❌ Error → devuelve `-1` y establece `errno`.

![](Imagenes/Pasted%20image%2020260607234624.png)

![](Imagenes/Pasted%20image%2020260607234644.png)

![](Imagenes/Pasted%20image%2020260607234702.png)

---

#### Ejercicio 10  Dibuja el esquema jerárquico de los procesos creados en la ejecución del siguiente programa con argc=3:

``` C
void main(int argc, char *argv[])

{

    int i;

    for(i=1; i<=argc; i++)

    {

        pid_t pid = fork();

        ...

    }

    return 0;

}
```


Respuesta:

``` C
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int i;

    for(i=1; i<=argc; i++)
    {
        pid_t pid = fork();
    }


    printf("[padre]: %i     [pid]:%i \n",getppid(),getpid());

    return 0;
}
}
```

Como has ejecutado:

```bash
./Ejercicio10.exe 1 2 3
```

entonces:

```text
argc = 4
```

porque:

- `argv[0] = "./Ejercicio10.exe"`
    
- `argv[1] = "1"`
    
- `argv[2] = "2"`
    
- `argv[3] = "3"`
    

El bucle:

```c
for(i = 1; i <= argc; i++)
```

se ejecuta **4 veces** (`i = 1, 2, 3, 4`).

Cada llamada a `fork()` duplica el número de procesos existentes, por lo que el número total de procesos creados es:

```text
1 → 2 → 4 → 8 → 16
```

Por ello, la salida muestra **16 procesos distintos**, que es exactamente lo esperado.

El esquema jerárquico de los procesos generados sería:

```text
P0
├─ P1
│  ├─ P3
│  │  ├─ P7
│  │  │  └─ P15
│  │  └─ P11
│  ├─ P5
│  │  └─ P13
│  └─ P9
├─ P2
│  ├─ P6
│  │  └─ P14
│  ├─ P10
│  └─ P12
├─ P4
│  └─ P8
└─ P16
```

Usando los PID obtenidos en tu ejecución, el árbol aproximado es:

```text
6194
├─ 6195
│  ├─ 6198
│  │  ├─ 6204
│  │  │  └─ 6209
│  │  └─ 6207
│  ├─ 6201
│  └─ 6206
├─ 6196
│  ├─ 6199
│  ├─ 6203
│  └─ 6208
├─ 6197
│  └─ 6200
├─ 6202
└─ 6205
```

Los procesos cuyo **PPID es 828**:

```text
6202
6205
6208
```

aparecen porque sus padres terminaron antes que ellos y fueron **adoptados por `systemd`** (PID 828 en tu sistema), convirtiéndose temporalmente en **procesos huérfanos**.

### Respuesta para entregar

> Al ejecutar el programa con:
> 
> ```bash
> ./Ejercicio10.exe 1 2 3
> ```
> 
> el valor de `argc` es 4, ya que incluye el nombre del programa (`argv[0]`). Por tanto, el bucle `for(i=1; i<=argc; i++)` realiza 4 llamadas a `fork()`.
> 
> Cada llamada a `fork()` duplica el número de procesos existentes, por lo que el número total de procesos generados es:
> 
> ```text
> 2⁴ = 16 procesos
> ```
> 
> El esquema jerárquico de creación de procesos corresponde a un árbol binario de profundidad 4. Algunos procesos muestran como padre al proceso `systemd` (PID 828) debido a que sus procesos padres finalizaron antes de que ellos imprimieran su información, siendo adoptados temporalmente por `systemd`.

![](Imagenes/Pasted%20image%2020260608000644.png)

---

