#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct Produto{ // define uma struct 'Produto'
    char nome[101]; // foi considerado que a string 'nome' de cada produto tem no máximo 100 elementos
    int quantidade;
    double precos;
} Produto;

void criaArrayProduto(Produto **produtos, int *numProdutos, FILE *fp, int j); // protótipo da função que cria um vetor de structs produto
void inserirProduto(Produto **produtos, int *numProdutos); // protótipo da função que insere produtos no estoque
void consultarEstoque(Produto **produtos, int numProdutos); // protótipo da função que consulta o estoque
void venderProdutos(Produto **produtos, double *saldo); // protótipo da função que vende os produtos armazenados no estoque
void consultarSaldo(double saldo); // protótipo da função que consulta o saldo do caixa
void aumentarEstoque(Produto **produtos); // protótipo da função que aumenta o estoque via alocação dinâmica
void mudarPreco(Produto **produtos); // protótipo da função que muda o preço do produto acessando a classe preço da respectiva struct
FILE *inicializaArquivo(const char *nomeArquivo); // protótipo da função que inicializa o arquivo quando se abre o programa pela primeira vez

FILE *inicializaArquivo(const char *nomeArquivo){
    FILE *arquivo = fopen(nomeArquivo, "r+"); // abre o arquivo no modo leitura e escrita
    if (arquivo == NULL){ // verifica se ocorreu algum erro na abertura do arquivo
        arquivo = fopen(nomeArquivo, "w"); // se o arquivo não existir, cria-se um novo arquivo
        if(arquivo == NULL){
            printf("Erro na criação do arquivo"); // caso ocorra algum erro na criação do arquivo, imprime uma mensagem de erro e encerra o programa
            exit(1);
        }
    }
    return(arquivo); // retorna o ponteiro para o arquivo
}

void mudarPreco(Produto **produtos){
    int i;
    double novoPreco;
    scanf("%d %lf", &i, &novoPreco); // lê os valores do índice do produto e do novo preço do produto
    (*produtos)[i].precos = novoPreco; // atualiza o preço do produto no vetor de produtos
}

void aumentarEstoque(Produto **produtos){
    int i, quant;
    scanf("%d %d", &i, &quant); // lê os valores do índice do produto e da quantidade a ser adicionada ao estoque
    (*produtos)[i].quantidade += quant; // aumenta a quantidade em estoque do produto no vetor de produtos
}

void consultarSaldo(double saldo){
    printf("Saldo: %.2lf\n", saldo); // imprime o saldo do caixa com duas casas decimais
    printf("--------------------------------------------------\n");
}

void venderProdutos(Produto **produtos, double *saldo){
    int venda;
    double total = 0;
    do{
        scanf("%d", &venda); // lê o valor do índice do produto a ser vendido
        if (venda != -1){ // verifica se o usuário deseja encerrar a venda
            if ((*produtos)[venda].quantidade > 0){ // verifica se há produtos em estoque para serem vendidos
                (*produtos)[venda].quantidade--; // reduz a quantidade em estoque do produto vendido
                total += (*produtos)[venda].precos; // atualiza o valor total da venda
                printf("%s %.2lf\n", (*produtos)[venda].nome, (*produtos)[venda].precos); // imprime o nome e o preço do produto vendido com duas casas decimais de precisão
            }
        }
    } while (venda != -1); // continua a venda até que o usuário digite -1 para encerrar a venda
    *saldo += total; // atualiza o saldo do caixa com o valor total da venda
    printf("Total: %.2f\n", total); // imprime o valor total da venda com duas casas decimais
    printf("--------------------------------------------------\n");
}

void consultarEstoque(Produto **produtos, int numProdutos){
    int i = 0;
    for (i=0; i<numProdutos; i++){
        printf("%d %s %d\n", i, (*produtos)[i].nome, (*produtos)[i].quantidade); // imprime o índice, o nome e a quantidade em estoque de cada produto
    }
    printf("--------------------------------------------------\n");
}

void inserirProduto(Produto **produtos, int *numProdutos){
    *produtos = (Produto *)realloc(*produtos, (*numProdutos + 1)*sizeof(Produto)); // aumenta o tamanho do vetor de produtos dinamicamente
    scanf("%s %d %lf", (*produtos)[*numProdutos].nome, &(*produtos)[*numProdutos].quantidade, &(*produtos)[*numProdutos].precos); // Lê o nome, a quantidade e o preço do produto a ser inserido
    (*numProdutos)++; // incrementa o número de produtos
}

void criaArrayProduto(Produto **produtos, int *numProdutos, FILE *fp, int j){
    *produtos = (Produto *)malloc(j * sizeof(Produto)); // aloca memória para o vetor de produtos com base no número de linhas no arquivo
    char linha[100];
    int i = 0;
    rewind(fp); // retorna ao início do arquivo
    while (fgets(linha, sizeof(linha), fp) != NULL){ // lê cada linha do arquivo
        char *token = strtok(linha, " "); // divide a linha em tokens separados por espaços
        int k = 0;
        while (token != NULL){
            switch (k){
            case 0:
                strcpy((*produtos)[i].nome, token); // copia o token para o nome do produto
                break;
            case 1:
                (*produtos)[i].quantidade = atoi(token); // converte o token em um número inteiro e atribui à quantidade do produto
                break;
            case 2:
                (*produtos)[i].precos = atof(token); // converte o token em um número de ponto flutuante e atribui ao preço do produto
                break;
            }
            token = strtok(NULL, " "); // avança para o próximo token
            k++;
        }
        i++;
    }
    *numProdutos = j; // define o número de produtos com base no número de linhas lidas do arquivo
}

int main(void){
    FILE *fp;
    int j = 0, n, numProdutos = 0;
    double saldo;
    char comando[3];
    Produto *produtos = NULL;
    char linha[100];
    scanf("%d", &n); // lê o valor de n
    scanf("%lf", &saldo); // lê o valor do saldo inicial do caixa
    fp = inicializaArquivo("projeto_icc.txt"); // inicializa o arquivo
    while(fgets(linha, sizeof(linha), fp) != NULL){
        j++; // conta o número de linhas no arquivo
    }
    if(j!=0){
        criaArrayProduto(&produtos, &numProdutos, fp, j); // cria o vetor de produtos com base nas informações do arquivo
    }
    int i = 0;
    while((scanf("%s", comando)) == 1){ // lê o comando digitado pelo usuário
        if((strcmp(comando, "IP")) == 0){
            inserirProduto(&produtos, &numProdutos); // chama a função para inserir um novo produto no estoque
        }
        else if((strcmp(comando, "CE")) == 0){
            consultarEstoque(&produtos, numProdutos); // chama a função para consultar o estoque
        }
        else if((strcmp(comando, "VE")) == 0){
            venderProdutos(&produtos, &saldo); // chama a função para vender produtos
        }
        else if((strcmp(comando, "CS")) == 0){
            consultarSaldo(saldo); // chama a função para consultar o saldo do caixa
        }
        else if((strcmp(comando, "AE")) == 0){
            aumentarEstoque(&produtos); // chama a função para aumentar o estoque de um produto
        }
        else if((strcmp(comando, "MP")) == 0){
            mudarPreco(&produtos); // chama a função para mudar o preço de um produto
        }
        else if((strcmp(comando, "FE")) == 0){
            break; // encerra o loop se o comando for "FE"
        }
    }
    i = 0;
    rewind(fp); // retorna ao início do arquivo
    for (i=0; i<numProdutos; i++){
        fprintf(fp, "%s %d %.2lf\n", produtos[i].nome, produtos[i].quantidade, produtos[i].precos); // Escreve as informações dos produtos atualizadas no arquivo
    }
    free(produtos); // libera a memória alocada para o vetor de produtos
    fclose(fp); // fecha o arquivo
    return(0);
}
