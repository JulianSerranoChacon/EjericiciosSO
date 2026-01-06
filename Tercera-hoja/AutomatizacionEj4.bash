#!/bin/bash

PROC="/proc/$$"

echo "Fichero       | Tipo           | Contenido / Resumen"
echo "-----------------------------------------------------------"

# cmdline
echo -n "cmdline       | "
[ -f "$PROC/cmdline" ] && echo -n "regular file   | "
tr '\0' ' ' < "$PROC/cmdline"
echo

# cwd
echo -n "cwd           | "
[ -L "$PROC/cwd" ] && echo -n "symlink        | "
readlink "$PROC/cwd"
echo

# environ
echo -n "environ       | "
[ -f "$PROC/environ" ] && echo -n "regular file   | "
tr '\0' '\n' < "$PROC/environ" | head -n 5
echo "..."  # solo mostramos primeras 5 variables

# exe
echo -n "exe           | "
[ -L "$PROC/exe" ] && echo -n "symlink        | "
readlink "$PROC/exe"
echo

# fd
echo -n "fd            | "
[ -d "$PROC/fd" ] && echo -n "directory     | "
ls -l "$PROC/fd" | head -n 5
echo "..."  # mostramos solo algunos descriptores

# limits
echo -n "limits        | "
[ -f "$PROC/limits" ] && echo -n "regular file   | "
head -n 5 "$PROC/limits"
echo "..."

# maps
echo -n "maps          | "
[ -f "$PROC/maps" ] && echo -n "regular file   | "
head -n 5 "$PROC/maps"
echo "..."

# root
echo -n "root          | "
[ -L "$PROC/root" ] && echo -n "symlink        | "
readlink "$PROC/root"
echo
