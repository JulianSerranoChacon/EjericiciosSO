#!/bin/bash
function hola(){
  echo “Hola $1!”
}


hola javier
A=`hola mundo`
echo “el valor de A es ${A}”
