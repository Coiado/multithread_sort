/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#define TAMANHO 1000000

#define WORKERS 3


int resposta = 0;
pthread_mutex_t trava;
int numeros[TAMANHO];
int part= 0;
int tam = 0;
pthread_mutex_t trava1,trava2;


void sort(int ini, int fim){
    for (int i=ini;i<=fim ; i++) {
        for (int j=ini; j<=fim;j++) {
            if (numeros[j] > numeros[i])
            {
                int tmp = numeros[i];
                pthread_mutex_lock(&trava1);
                numeros[i] = numeros[j];
                numeros[j] = tmp;
                pthread_mutex_unlock(&trava1);
            }
        }
    }
}

void* merge_esq(void* arg)
{
    int inicio = 0;
    int fim = tam/2 -1;
    if(fim > inicio){
        sort(inicio, fim);
    }
}

void* merge_dir(void* arg)
{
    int inicio = tam/2;
    int fim = tam -1;
    if(fim>inicio){
        sort(inicio, fim);
    }
}

void* merge(void* arg)
{
    int inicio = 0;
    int fim = tam -1;
    if(fim>inicio){
        sort(inicio, fim);
    }
}


int main() {
    pthread_t workers[WORKERS];
    char text[TAMANHO];
    
    /* Pegando input */
    scanf("%[^\n]",text);
    
    char *token = strtok(text, " ");
    
    
    while(token != NULL)
    {
        numeros[tam] = atoi(token);
        token = strtok(NULL, " ");
        tam++;
        
    }
    /* Disparanto threads */
    pthread_create(&(workers[0]), NULL, merge_dir, (void*) NULL);
    pthread_create(&(workers[1]), NULL, merge_esq, (void*) NULL);
    
    
    
    /* Esperando threads */
    for (int i=0; i<WORKERS-1; i++) {
        pthread_join(workers[i], NULL);
    }
    
    pthread_create(&(workers[2]), NULL, merge, (void*) NULL);
    
    
    pthread_join(workers[2], NULL);
    /* Escrevendo resposta */
    for (int k= 0 ; k<tam-1;k++){
       printf("%d ", numeros[k]);
    }
    printf("%d\n", numeros[tam-1]);
    
    return 0;
}
