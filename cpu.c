//Tomar los datos de cpu del fichero de texto y meterlo en una estructura

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char *proceso;
    int valor1;
    //int valor2;
    //int valor3;
    //int valor4;
    //int valor5;
    //int valor6;
    //int valor7;
    //int valor8;
    //int valor9;
    //int valor10;

}cpuInfo;

cpuInfo *cpu;

void vaciar(char temp[]);
void copiar(char temp[], int i);

int main(){
    int i,j;
    char aux;
    char temp[50];
    int cont = 0;

    FILE *f;
    f = fopen("cpuInfo.txt", "r");
    if (f == NULL) {
        printf("No se ha podido abrir el fichero. \n");
        exit(1);
    }

    while (!feof(f)){
        fgets(temp,50,f);
        cont++;
    }

    rewind(f);

    cpu = (cpuInfo*)malloc(cont*sizeof(cpuInfo));
    if(cpu == NULL){
        printf("No se ha podido reservar la memoria. \n");
        exit(1);
    }

    for (i = 0; !feof(f); i++){
        vaciar(temp);
        aux = '0';
        for (j = 0; aux != ' '; j++){
            aux = fgetc(f);
            if (aux != ' '){
                temp[j] = aux;
            }
        }
        copiar(temp, i);

        fgets(temp, 50, f);
        cpu[i].valor1 = atoi(temp);

        printf("Proceso: %s Valor: %i.\n", cpu[i].proceso,cpu[i].valor1);
    }

    system("pause");
    return 0;
}

void vaciar(char temp[]){
    int i;

    for (i = 0; i < 50; i++){
        temp[i] = '\0';
    }
}

void copiar(char temp[], int i){
    int N = strlen(temp) + 1;
    cpu[i].proceso = (char*)malloc(N*sizeof(char));
    if (cpu[i].proceso == NULL){
        printf("No se ha podido reservar memoria. \n");
        exit(1);
    }
    strcpy(cpu[i].proceso, temp);
}
