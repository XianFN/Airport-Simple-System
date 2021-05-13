#!/bin/bash

function catfuncion(){

	cat programa.c
}

function gccfuncion(){

	gcc programa.c -o programa
	echo -e "\n Se acaba de compilar el programa perfectamente\n"
}

function ejecfuncion(){
	if (test -x programa==1)
	then
	echo -e "El archivo no tenia permisos de ejecucion, se acaban de otorgar." 
	chmod 777 programa
	fi		
	if test -e programa  
	then 
		read corredores
		
		./programa $corredores
	else
	echo -e "Error, el archivo no está compilado o no existe \n"
	fi




}


#red='\e[0.31m'
#endColor=' \e[0m' 



	while true
	do
		echo -e "\n\n\e[0;32m------------MENU-------------\n\e[0m"
		echo -e "\e[0;31m Seleccion una opción del menú \n"
		echo -e "1.  Consultar el código del programa \n"
		echo -e "2.  Compilar el programa \n"
		echo -e "3.  Ejecutar el programa \n"
		echo -e "4.  Salir \n\e[0m"
		
		read option
		case $option in
			1)verprog=`catfuncion`
				echo -e "\n El códico del programa es: \n\n $verprog \n"	
			;;
			2)compilar=`gccfuncion`
		        
			echo -e "\n $compilar \n"
				
			;;
			3)
			echo -e "\n Introduzca el número de corredores que participan en la carrera \n"
			if (test -x programa==1)
			then
			echo -e "El archivo no tenia permisos de ejecucion, se acaban de otorgar." 
			chmod 777 programa
			fi		
			if test -e programa  
			then 
				read corredores
				
				./programa $corredores
			else
			echo -e "Error, el archivo no está compilado o no existe \n"
			fi
			;;
			4)break
		
			;;
			*) echo -e "Opcion errónea, introduzca un numero del 1-4 \n"
			
			;;
		esac
	done






