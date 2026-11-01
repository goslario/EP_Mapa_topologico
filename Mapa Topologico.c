/*
    Feito por Gustavo Santos Sá 2026
    Usp Leste
    gcc -o prog mapa_topologico.c
    ./prog entrada.txt
    inserir arquivo entrada na execução
    compilador pelo wls/ubuntu
*/

#include <stdio.h>
#include <stdlib.h>
// por algum motivo preciso digitar -lm para usar math.h, ent vou criar as funções dela q uso no codigo msm
#include <stdbool.h>
#include <string.h>

int Spow(int base, int expoente)
{
    int resultado = 1;

    for (int i = 0; i < expoente; i++)
    {
        resultado *= base;
    }

    return resultado;
}

int CEIL(float valor)
{
    int inteiro = (int)valor;

    if (valor > inteiro)
        return inteiro + 1;

    return inteiro;
}

struct matriz
{
    float **m;
    int seed;
    float roughness;
    float maior;
    float menor;
};

void funcao(struct matriz *mat, int x, int y, int size, bool Topo, bool Esquerda)
    {  

        if(size == 2) //tamanho 2, quantidade de colunas
        {
            return;
        }
        
        //y é altura/ linha
	float AB, AC;
	float X = (2.0 * mat->roughness) * ((float)rand() / (float)RAND_MAX) - mat->roughness + (mat->m[y][x] + mat->m[y][x + size - 1] + mat->m[y + size - 1][x] + mat->m[y + size - 1][x + size - 1])/4.0;

	//AB = (2.0 * mat->roughness) * ((float)rand() / (float)RAND_MAX) - mat->roughness + (mat->m[y][x] + mat->m[y][x + size - 1] + X)/3.0;    
        if(Topo)
        {
	    AB = (2.0 * mat->roughness) * ((float)rand() / (float)RAND_MAX) - mat->roughness + (mat->m[y][x] + mat->m[y][x + size - 1] + X)/3.0;
     
	    mat->m[y][x + size/2] = AB;
            if(mat->maior < AB)
            {
                mat->maior = AB;
            }

            if(mat->menor > AB)
            {
                mat->menor = AB;
            }
        }

	float BD = (2.0 * mat->roughness) * ((float)rand() / (float)RAND_MAX) - mat->roughness + (mat->m[y][x + size - 1] + mat->m[y + size - 1][x + size - 1] + X)/3.0;
        float CD = (2.0 * mat->roughness) * ((float)rand() / (float)RAND_MAX) - mat->roughness + (mat->m[y + size - 1][x] + mat->m[y + size - 1][x + size - 1] + X)/3.0;
	
	//AC = (2.0 * mat->roughness) * ((float)rand() / (float)RAND_MAX) - mat->roughness + (mat->m[y][x] + mat->m[y + size - 1][x] + X)/3.0;
        if(Esquerda)
        {
	    AC = (2.0 * mat->roughness) * ((float)rand() / (float)RAND_MAX) - mat->roughness + (mat->m[y][x] + mat->m[y + size - 1][x] + X)/3.0;
         
	    mat->m[y + size/2][x] = AC;
            if(mat->maior < AC)
            {
                mat->maior = AC;
            }

            if(mat->menor > AC)
            {
                mat->menor = AC;
            }
        }

	

        if(mat->maior < X) mat->maior = X;
        if(mat->menor > X) mat->menor = X;

        if(mat->maior < BD)
        {
            mat->maior = BD;
        }
        if(mat->maior < CD)
        {
            mat->maior = CD;
        }

        if(mat->menor > BD)
        {
            mat->menor = BD;
        }
        if(mat->menor > CD)
        {
            mat->menor = CD;
        }
	mat->m[y + size/2][x + size/2] = X;
        mat->m[y + size/2][x + size - 1] = BD;
        mat->m[y + size - 1][x + size/2] = CD;

	//puts("");
	//printf("x: %d, y: %d, size: %d X: %f, AB: %f, BD: %f, CD: %f, AC; %f", x, y, size, X, AB, BD, CD, AC);
	//puts("");
	funcao(mat, x, y, size/2 + 1, Topo, Esquerda);
        funcao(mat, x + size/2, y, size/2 + 1, Topo, false);
        funcao(mat, x, y + size/2, size/2 + 1, false, Esquerda);
        funcao(mat, x + size/2, y + size/2, size/2 + 1, false, false);
        return;
    }

bool fazerMatriz(struct matriz *mat, float A, float B, float C, float D, int size)
{
    if(size <= 2)
    {
        return false;
    }
    mat->m[0][0] = A;
    mat->m[0][size - 1] = B;
    mat->m[size - 1][0] = C;
    mat->m[size - 1][size - 1] = D;

    mat->maior = A;
    mat->menor = A;

    if(mat->maior < B)
    {
        mat->maior = B;
    }
    if(mat->maior < C)
    {
        mat->maior = C;
    }
    if(mat->maior < D)
    {
        mat->maior = D;
    }
    if(mat->menor > B)
    {
        mat->menor = B;
    }
    if(mat->menor > C)
    {
        mat->menor = C;
    }
    if(mat->menor > D)
    {
        mat->menor = D;
    }


    funcao(mat, 0.0, 0.0, size, true, true);
    
    return true;
}


int main(int argc, char **argv)
{
    //printf("rand max: %d", RAND_MAX);
	//puts("");
    struct matriz Matriz;
    int N;
    float A, B, C, D;

    FILE* fpin = fopen(argv[1], "r"); //ler o arquivo de entrada
    if(!fpin)
    {
        printf("Erro ao abrir o arquivo de entrada");
        return 1;
    }
    
    int i = 1;

    while(fscanf(fpin, "%d %d %f %f %f %f %f", &N, &Matriz.seed, &Matriz.roughness, &A, &B, &C, &D) == 7)
    {
        int Npow = Spow(2, N); 
        srand(Matriz.seed);
        Matriz.m = (float **)malloc((Npow + 1) * sizeof(float *));
        for (int i = 0; i < Npow + 1; i++)
        {
            Matriz.m[i] = (float *)malloc((Npow + 1) * sizeof(float));
        }
        fazerMatriz(&Matriz, A, B, C, D, Npow + 1);
        
        char nomeSaida[50]; //para gerar saidas com nomes esperados
        if(i <= 9)
        {
            strcpy(nomeSaida, "saida1.txt");
            nomeSaida[5] = i + '0';
        }
        else if(i <= 99)
        {
            strcpy(nomeSaida, "saida10.txt");
            nomeSaida[5] = (i / 10) + '0';
            nomeSaida[6] = (i % 10) + '0';
        }
        else
        {
            strcpy(nomeSaida, "saida100.txt");
            nomeSaida[5] = (i / 100) + '0';
            nomeSaida[6] = ((i / 10) % 10) + '0';
            nomeSaida[7] = (i % 10) + '0';
        }
           
        FILE *fpOUT = fopen(nomeSaida, "w");
        int normalizado(float v)
        {
            int X;
                if (Matriz.maior == Matriz.menor)
                {   
                    return CEIL((Matriz.maior - v) * 255);
                }
               
                return CEIL(((Matriz.maior - v)/(Matriz.maior - Matriz.menor)) * 255);
        }
        
        fprintf(fpOUT, "P2\n%d %d\n255\n", Npow + 1, Npow + 1);
   
	for(int j = 0; j < Npow + 1; j++)
        {
            for(int k = 0; k < Npow + 1; k++)
            {
                //fprintf(fpOUT, "%f ", Matriz.m[j][k]);
                fprintf(fpOUT, "%d ", normalizado(Matriz.m[j][k]));
            }
            fprintf(fpOUT, "\n");
        }   

        fclose(fpOUT);
            
        puts("Arquivo de saída gerado com sucesso.");
        for (int i = 0; i < Spow(2, N) + 1; i++)
        {
            free(Matriz.m[i]);
        }
        free(Matriz.m);
        i++;
    }
    fclose(fpin);
    return 0;
}
