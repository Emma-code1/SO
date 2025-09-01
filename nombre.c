#include <stdio.h>

int main(){

int impresion;
char nombre[15];

printf("Bienvenido, escribe tu nombre\n");
scanf("%s", nombre);
printf("Cuantas veces deseas imprimirlo?\n");
scanf("%i", &impresion);

for(int i = 1; i <= impresion; i++){
printf("%s\n", nombre);
}
}
