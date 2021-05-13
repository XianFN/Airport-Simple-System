#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h> 
#include <time.h> 
#include <errno.h>
int pidJuezPista,pid;//variable global

void juezDePista(int senhal);
void resultadoPista(int senhal);
void carrera(int senhal);

int main(int argc, char *argv[]){

    if (argc <2){
   	  printf("Error, tienes que introducir como argumento el numero de corredores que desea. \n");
   	  return 0;
    }
    char *p;
    int corredores= strtol(argv[1],&p,10);//entrada como argumento
    int *numerocorredor = (int*) malloc (corredores *sizeof (int));//punteros de datos
    int *pidcorredores = (int*) malloc (corredores*2 * sizeof (int));
    int *tiempo = (int*) malloc (corredores*3 *sizeof (int));
    pid_t pid;
    int pistaLibre=-1,edad,edadhijo,estadoJuezPista,pidPadre;
    pidPadre=getpid();

    printf("Creo los Procesos...\n");
    for (int i=1; i<=corredores+1;i++){
    		 	pid=fork();
          srand(getpid());
    		 	if (pid==-1){
  			  	perror("Error en la llamada a fork()");
  			 }else if (pid==0){
             if (getppid()+1==getpid()){//solo entra el primer proceso, el que será el juez
              srand(time(NULL));
              printf("Se creó el juez de pista con pid: %d\n", getpid());
              pidJuezPista=getpid();
              do{//se reprte eseperando la señal hasta que acabe que es cuando la ista está en beun estado
               signal(SIGUSR1,juezDePista);
               pause();
              }while(1==1);
              break;
            }else{
      		 		printf("Se creó el corredor n: %d con pid: %d\n",i-1, getpid() );
              signal(SIGUSR1,carrera);//creo los corredores y esperan la señal
              pause();
              exit(0);
    		  	}
        }else{

    		 	pidcorredores[i]=pid;//gardo el pid en un vector
    		 }
    }

    		 if (getpid()==pidPadre){//todo el codigo hasta el fina solo lo ejecuta el padre
      		sleep(3);
          printf("----Procesos creados-----\n");
      		printf("LLamo al juez de pista para comprobar el estado\n" );
      		printf("Envio la señal al juez de pista con pid: %d\n",pidcorredores[1]);
      		kill(*(pidcorredores+1),SIGUSR1);//manda la señal al juez
          signal(SIGUSR1,resultadoPista);
          signal(SIGUSR2,resultadoPista);//espera la señal
          pause();
          wait(&estadoJuezPista);
          //aqui tengo un error, que cuando el estado de la pista es erroneo, los procesos siguen ejecutandose, por muchos waitpid que ponga sigue siendo igual.
          sleep(3);
          for (int i = 2; i < corredores+2; ++i){//cada corredor

              int pidCorredor=pidcorredores[i];
              printf("Envio la señal al corredor n :%d con el pid :%d  \n",i-1,pidCorredor );
              kill(pidCorredor,SIGUSR1);
              waitpid(pidCorredor,&edadhijo,WCONTINUED);//espero a que acabe el anterior
              edad=WEXITSTATUS(edadhijo);//la salida son los ms, la edad
              *(numerocorredor+i)=i-1;//guardo en los punteros
              *(tiempo+i)=edad;
              kill(pidCorredor,SIGKILL);//me aseguro que el proceso se cierra, para evitar problemas
              sleep(1);//se puede quitar, es para hacerlo mas visual
            }

      			//imprime
      		
      			printf("===============================================================================================\n");
      			printf("=============================RESULTADO DE LA CARRERA===========================================\n");
      			printf("===============================================================================================\n\n");
      			for (int j=2; j<=corredores+1;j++){
      				printf("EL corredor numero: %d acabó en un tiempo de :  %d y su pid era: %d  \n",numerocorredor[j],tiempo[j],pidcorredores[j] );
      			}
      			free(numerocorredor);//libero punteros
      			free(tiempo);
      			free(pidcorredores);
      			printf("\n===============================================================================================\n");
      			return 0;
      		
         }
        
      }
        void carrera(int senhal){
          sleep(1);
          printf("Soy el Corredor con pid %d voy a correr\n",getpid() );
          srand (time(NULL));
          int ms= rand()%4 +1;
          
          for (int i = 0; i < ms; ++i){
            printf("Me quedan %d segundos..\n", ms-i);
            sleep(1);
          }
          printf("\n");
          int edadhijo=ms;
          exit(edadhijo);

}
void juezDePista(int senhal){
if(senhal==SIGUSR1){

		printf("LLamada recibida, comprobando el estado de la pista\n");
    sleep(5);
  			int salida;
        srand(time(NULL));
  			int estadopista=rand()%2;
  			if (estadopista==0){
  				printf("El estado de la pista es MALO, no se puede empezar aún\n");
  				kill(getppid(),SIGUSR1);
  			}else{

  			printf("El estado de la pista es BUENO, empieza la carrera\n");
  			kill(getppid(),SIGUSR2);
        exit(0);
  			
  		}
 }

}
void resultadoPista(int senhal){
  if(senhal==SIGUSR1){

  	printf("El juez principal recibio la señal SIGUSR1\n");
  	printf("El estado de la pista no es valido\n");
  	printf("El juez principal duerme 3 segundos\n");
  	sleep(3);
    kill(pidJuezPista,SIGUSR1);
    pause();
  	
  	


  }else if(senhal==SIGUSR2){
   	printf("El juez principal recibio la señal SIGUSR2\n");
  	printf("El estado de la pista es valido\n");
    printf("----------------------------------------------------\n");

  }
}

