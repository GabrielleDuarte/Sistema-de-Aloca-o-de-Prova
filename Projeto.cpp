#include<iostream>
#include<fstream>
#include<cstdlib>
#include<time.h>
#define TAM 100


using namespace std;

struct solucao
{
    int alocacao_cadidato[TAM], locais_selecionados[TAM], candidato_alocado_por_local[TAM], custo;// alocacao_cadidato = local que o candidato vai ficar '10'; locais_selecionados = medianas '3'; candidato_alocado_por_local = capacidade de alunos por local '3'; custo = soma do tempo de cada candidato até o local (tempo_local).
};

struct dado
{
   int m, n, p, capacidade_local[TAM], tempo_local[TAM][TAM];// m = 5(Número de locais disponíveis); n = 10(Número de candidatos); p = 3(Númeor de locais a ser selecionados);  vetor 'id_candidato' serve para identificar o candidato e a matriz 'tempo_local' dÃ¡ o tempo que cada candidato leva pra cada local
};

int leitor(char nome_aquivo[TAM], dado & d );
bool teste(int vetor[] ,int i, int aux);
void calculo_custo ( dado d , solucao & s );
solucao pertubar ( solucao s_aux , dado d);
solucao gera_solucao_inicial( dado d, solucao s);
solucao busca_local( dado d , solucao s );
void salvar( dado d , solucao s, char nome_arquivo );




int leitor(char nome_aquivo[TAM], dado & d )
{
    ifstream ler( nome_aquivo );// ler o arquivo
    dado d;


    if( !ler.is_open() )// testa se o arquivo tá aberto
    {
    cout << "Falha ao abrir o arquivo!" <<endl;
    //exit(1);
    }
    else
    {
    cout << "Arquivo aberto com sucesso" << endl;
    }

    ler >> d.m >> d.n >> d.p;

    for ( int c = 0; c < d.m; c++ )// copia os números do arquivo para as variáveis
    {
        ler >> d.capacidade_local[c];
    }
    for ( int c = 0; c < d.n; c++ )
    {
        for ( int i = 0; i < d.m; i++ )
        {
            ler >> d.tempo_local[c][i];
        }
    }
}

bool teste(int vetor[] ,int i, int aux)// testa se o numero  repetido(mediana)
{
    for ( int j = 0; j < i; j++ )
        {
            if(vetor[j] == aux)
            {
             return true;

            }
        }
        return false;

}//verifica a ocorrencia de um numero no vetor

 solucao gera_solucao_inicial( dado d, solucao s)
{

    srand (time(NULL));
    int aux ;//
    int cont=0;

        do{

            for ( int i = 0; i < d.p ; i++ )//p=3
            {
                aux = rand()%d.m;// m=5

                while( teste(s.locais_selecionados ,i,aux ))
                {//5 5 4 5 6.
                aux = rand()%d.m;

                }
                s.locais_selecionados[i] = aux;
                cont = cont + d.capacidade_local[s.locais_selecionados[i]];
            }

        }while( cont < d.n );// d.n = 10;

    int indice_local, codigo_local;

        for( int c = 0; c < d.n; c++)// d.n = 10;
        {

            do{
            indice_local = rand()%d.p;// p = 3; indice do local é qual local
            codigo_local = s.locais_selecionados[indice_local];
            }while( d.capacidade_local[codigo_local] <= s.candidato_alocado_por_local[indice_local]);

        s.alocacao_cadidato[c] = codigo_local;
        s.candidato_alocado_por_local[indice_local]++;

        calculo_custo ( d, s );// função para computar a o custo

return s;
}


}

solucao pertubar ( solucao s_aux , dado d)// locais selecionados
{
    int novo_local;// novo local sorteado;
    int local_retirado;//
    int local_antigo;//variável para guardar o antigo local que foi substituido;
   // solucao s_aux;

    local_retirado = rand()%d.p;// indice da variável local_antigo

    srand(time(NULL));
    novo_local = rand()%d.m;
    for( int c = 0; c < d.p; c++ )
    {
        if ( novo_local == s_aux.locais_selecionados[c])
        {
         novo_local = rand()%d.m;
         c = 0;
        }
    }
    local_antigo = s_aux.locais_selecionados[local_retirado];

    s_aux.locais_selecionados[local_retirado] = novo_local;

    for( int c = 0; c < d.n; c++ )
    {
        int j;
            if( s_aux.alocacao_cadidato[c] == local_antigo )
            {
            s_aux.alocacao_cadidato[c] = novo_local;

            if(d.capacidade_local[novo_local] > s_aux.candidato_alocado_por_local[novo_local])
            {
            s_aux.candidato_alocado_por_local[novo_local]++;
            }
            else
            {
                do{// sorteia outro local e testa se ele é igual ao local antigo ou se a capacidade dele foi excedida e se sim outro local é sorteado.

                    j = rand()%d.p;

                  }while( j == local_retirado || s_aux.candidato_alocado_por_local[s_aux.locais_selecionados[j]] == d.capacidade_local[s_aux.locais_selecionados[j]]);

                 s_aux.alocacao_cadidato[c] = s_aux.locais_selecionados[j];//
            }

    }
    }

    s_aux.locais_selecionados[rand()%d.p]= novo_local;

return s_aux;

}

solucao busca_local( dado d , solucao s )//
{
    solucao s_aux;

    s_aux = s;// s_aux recebe s para no fim comparar os custos
    int local_anterior ;

    for( int i = 0; i < d.n; i++ )
    {
       for(int c = i + 1;  c < d.n; c++ )
        {

        local_anterior = s_aux.alocacao_cadidato[i];
        s_aux.alocacao_cadidato[i] = s_aux.alocacao_cadidato[c];
        s_aux.alocacao_cadidato[c] = local_anterior ;

        calculo_custo( d , s_aux );

        if( s_aux.custo < s.custo )
    {
        s = s_aux ;
    }

        }
    }

  return s;
}


void calculo_custo ( dado d, solucao & s)
{
    s.custo=0;

       for( int c = 0; c < d.n; c++)// d.n = 10;
        {
        // tempo_local[d.n][s.alocacao_cadidato[d.n];
        s.custo = s.custo +  d.tempo_local[c][s.alocacao_cadidato[c]];
        }
}

void copiar( solucao s, solucao & s_aux, dado d )
{
s_aux = s;
}
void salvar( dado d , solucao s, char nome_arquivo )
{
    ofstream escreve("nome_arquivo");

    if( !escreve.is_open() )
    {
    cout <<"Falha ao abrir o arquivo!" <<endl;
    }
    escrita << "Custo: " << s.custo << endl;
    escrita << "Locais de Prova: " << s.locais_selecionados << endl;
    escrita << " Alocacao:" << endl;
    for( int c = 0; c < d.n; c++)
    {
        escrita << "Candidato " << c++ << " faz prova em " << s.alocacao_cadidato[c] <<endl;
    }
}

int main ()
{
    clock_t tInicial,tFinal;




    char nome_arquivo[TAM]; // é necessário criar uma variável para mandar o arquivo para as outras funções? essa variavel vai ser um int ou um char ?
    dado d;
    solucao s, s_aux;
    float tempo;


    cout << "Insira o nome do aquivo que voce deseja abrir" << endl;
    cin >> nome_arquivo;

    cout << "Insira o tempo em segundos:  ";
    cin >> tempo;
     tInicial = clock();
     leitor( nome_arquivo , d );// função que ler o arquivo;

     s = gera_solucao_inicial ( d, s ); //s = gera_solucao_inicial( d , s );

     s_aux = busca_local( d, s ); //
     tFinal = clock();
    // somatorio = (tFinal - tInicial)/CLOCKS_PER_SEC;
     while( (tFinal - tInicial)/CLOCKS_PER_SEC <= tempo)
     {
      cout << (tFinal - tInicial)/CLOCKS_PER_SEC << endl;

     s = pertubar( s_aux , d );
     s = busca_local( d, s );
     calculo_custo( d, s);
     calculo_custo(d , s_aux);

     if( s.custo < s_aux.custo )
     {

        s_aux = s;

     }
     tFinal = clock();
     // (tFinal - tInicial)/CLOCKS_PER_SEC;
     }
     salvar( d , s , nome_arquivo );

     cout << "Custo: " << s_aux.custo << endl << s.custo;


     }

