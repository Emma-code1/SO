#include <stdio.h>

int main(){

char nombre[15];
int cantidad;

printf("Bienvenido, escribe tu nombre\n");
scanf("%s", nombre);
printf("Cuantas veces deseas imprimirlo?\n");
scanf("%i", &cantidad);

for(int i = 1; i <= cantidad; i++){
printf("%s\n", nombre);
}
}


