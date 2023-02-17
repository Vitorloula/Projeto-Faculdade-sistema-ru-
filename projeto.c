#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
    int quantia;
    int matricula;
    struct tm* data;
} Movimentacao;
// Formato no arquivo
// quantia matricula hora minuto dia mes ano

typedef struct {
    char nome[100];
    int matricula;
    int numero_cartao;
    int saldo;
} Aluno;
// Formato no arquivo
// nome matricula numero saldo

void mostrar_menu() {
    printf("*  MENU  *\n\n");
    printf("[1] Cadastrar aluno\n");
    printf("[2] Listar alunos\n");
    printf("[3] Extrato de um aluno\n");
    printf("[4] Adicionar creditos\n");
    printf("[5] Usar credito\n");
    printf("[6] Salvar\n");
    printf("[7] Sair\n");
    printf("\nEscolha uma opcao: ");
}

Aluno* busca_aluno(Aluno* vet_alunos, int n, int matricula) {
    for(int i = 0; i < n; i++) {
        if(vet_alunos[i].matricula == matricula) {
            return &vet_alunos[i];
        }
    }
    return 0;
}

Aluno* cadastrar_aluno(Aluno* vet_alunos, int* quantidade_alunos) {
    char nome[100];
    int matricula;
    printf("Digite o nome: ");
    scanf(" %[^\n]", nome);
    printf("Digite a matricula: ");
    scanf("%d", &matricula);

    if(busca_aluno(vet_alunos, *quantidade_alunos, matricula) != 0) {
        printf("Aluno ja cadastrado\n");
        return vet_alunos;
    }

    vet_alunos = (Aluno*) realloc(vet_alunos, sizeof(Aluno) * (*quantidade_alunos + 1));

    strcpy(vet_alunos[*quantidade_alunos].nome, nome);
    vet_alunos[*quantidade_alunos].matricula = matricula;
    vet_alunos[*quantidade_alunos].numero_cartao = rand() % 1000000000;
    vet_alunos[*quantidade_alunos].saldo = 0;

    *quantidade_alunos += 1;
    printf("* Cadastro concluido *\n");

    return vet_alunos;
};

void listar_alunos(Aluno* vet_alunos, int n) {
    for(int i = 0; i < n; i++) {
        printf("Nome: %s\n", vet_alunos[i].nome);
        printf("Numero do cartao: %d\n", vet_alunos[i].numero_cartao);
        printf("Matricula: %d\n\n", vet_alunos[i].matricula);
    }
}

void extrato(Movimentacao* vet_movimentacao, int quantidade_mov) {
    int matricula;
    printf("Digite uma matricula: ");
    scanf("%d", &matricula);
    
    for(int i = 0; i < quantidade_mov; i++) {
        if (vet_movimentacao[i].matricula == matricula){
            printf("Quantia: %d | ", vet_movimentacao[i].quantia);
            printf("Hora: %d:%d | ", vet_movimentacao[i].data->tm_hour - 3 , vet_movimentacao[i].data->tm_min);
            printf("Data: %d/%d/%d", vet_movimentacao[i].data->tm_mday, vet_movimentacao[i].data->tm_mon + 1, vet_movimentacao[i].data->tm_year + 1900);
            printf("\n");
        }
    }   
}

Movimentacao* adicionar_credito(Aluno* vet_alunos, int quantidade_alunos, Movimentacao* vet_mov, int* quantidade_mov) {
    int matricula, creditos;
    printf("Digite uma matricula: ");
    scanf("%d", &matricula);
    printf("Digite o valor: ");
    scanf("%d", &creditos);
    Aluno* aluno = busca_aluno(vet_alunos, quantidade_alunos, matricula);
    if(aluno == 0) {
        printf("Aluno nao encontrado\n");
        return vet_mov;
    }
    aluno->saldo += creditos;
    vet_mov = (Movimentacao*) realloc(vet_mov, sizeof(Movimentacao) * (*quantidade_mov + 1));
    vet_mov[*quantidade_mov].quantia = aluno->saldo;
    vet_mov[*quantidade_mov].matricula = aluno->matricula;

    time_t atual = time(0);
    ctime(&atual);
    vet_mov[*quantidade_mov].data = gmtime(&atual);

    *quantidade_mov += 1;
    printf("Recarga feita\n");
    return vet_mov;
}

Movimentacao* usar_credito(Aluno* vet_alunos, int quantidade_alunos, Movimentacao* vet_mov, int* quantidade_mov) {
    int matricula;
    printf("Digite uma matricula: ");
    scanf("%d", &matricula);
    Aluno* aluno = busca_aluno(vet_alunos, quantidade_alunos, matricula);
    if(aluno == 0) {
        printf("Aluno nao encontrado\n");
        return vet_mov;
    }
    if(aluno->saldo == 0) {
        printf("Saldo insuficiente\n");
        return vet_mov;
    }
    aluno->saldo -= 1;
    vet_mov = (Movimentacao*) realloc(vet_mov, sizeof(Movimentacao) * (*quantidade_mov + 1));
    vet_mov[*quantidade_mov].quantia = aluno->saldo;
    vet_mov[*quantidade_mov].matricula = aluno->matricula;
    time_t atual = time(0);
    ctime(&atual);
    vet_mov[*quantidade_mov].data = gmtime(&atual);
    *quantidade_mov += 1;
    printf("Credito usado\n");
    return vet_mov;
}

Aluno* importar_alunos(Aluno* vet_alunos, int* quantidade_alunos) {
    FILE *alunos;
    
    alunos = fopen("alunos.txt", "r");

    char nome[100], separador;
    int matricula;
    int numero_cartao;
    int saldo;
    
    while(fscanf(alunos, " %[^;]%c %d %d %d\n", nome, &separador, &matricula, &numero_cartao, &saldo) == 5) {
        vet_alunos = (Aluno*) realloc(vet_alunos, sizeof(Aluno) * (*quantidade_alunos + 1));

        strcpy(vet_alunos[*quantidade_alunos].nome, nome);
        vet_alunos[*quantidade_alunos].matricula = matricula;
        vet_alunos[*quantidade_alunos].numero_cartao = numero_cartao;
        vet_alunos[*quantidade_alunos].saldo = saldo;

        *quantidade_alunos += 1;
    }

    fclose(alunos);
    return vet_alunos;
}

Movimentacao* importar_movimentacoes(Movimentacao* vet_movimentacoes, int* quantidade_mov) {
    FILE *movimentacoes;
    movimentacoes = fopen("movimentacoes.txt", "r");
    
    int matricula;
    int quantia;
    int hora, min;
    int dia, mes, ano;
    
    while(fscanf(movimentacoes, "%d %d %d %d %d %d %d\n", &matricula, &quantia, &hora, &min, &dia, &mes, &ano) == 7) {
        vet_movimentacoes = (Movimentacao*) realloc(vet_movimentacoes, sizeof(Movimentacao) * (*quantidade_mov + 1));

        vet_movimentacoes[*quantidade_mov].matricula = matricula;
        vet_movimentacoes[*quantidade_mov].quantia = quantia;

        time_t atual = time(0);
        ctime(&atual);
        struct tm* data = gmtime(&atual);
        data->tm_hour = hora;
        data->tm_min = min;
        data->tm_mday = dia;
        data->tm_mon = mes;
        data->tm_year = ano;
        vet_movimentacoes[*quantidade_mov].data = data;

        *quantidade_mov += 1;
    }

    fclose(movimentacoes);
    return vet_movimentacoes;
}

void salvar_arquivo(Aluno* vet_alunos, int quantidade_alunos, Movimentacao* vet_movimentacoes, int quantidade_mov) {
    FILE *alunos;
    FILE *movimentacoes;
    alunos = fopen("alunos.txt", "w");
    movimentacoes = fopen("movimentacoes.txt", "w");

    for(int i = 0; i < quantidade_alunos; i++) {
        fprintf(alunos, "%s; ", vet_alunos[i].nome);
        fprintf(alunos, "%d ", vet_alunos[i].matricula);
        fprintf(alunos, "%d ", vet_alunos[i].numero_cartao);
        fprintf(alunos, "%d\n", vet_alunos[i].saldo);
    }

    for(int i = 0; i < quantidade_mov; i++) {
        fprintf(movimentacoes, "%d ", vet_movimentacoes[i].matricula);
        fprintf(movimentacoes, "%d ", vet_movimentacoes[i].quantia);
        fprintf(movimentacoes, "%d ", vet_movimentacoes[i].data->tm_hour);
        fprintf(movimentacoes, "%d ", vet_movimentacoes[i].data->tm_min);
        fprintf(movimentacoes, "%d ", vet_movimentacoes[i].data->tm_mday);
        fprintf(movimentacoes, "%d ", vet_movimentacoes[i].data->tm_mon);
        fprintf(movimentacoes, "%d\n", vet_movimentacoes[i].data->tm_year);
    }

    printf("Dados salvos\n");

    fclose(alunos);
    fclose(movimentacoes);
}

int main(){
    Aluno* vet_alunos = 0;
    int quantidade_alunos = 0;
    Movimentacao* vet_movimentacoes = 0;
    int quantidade_mov = 0;
    time_t t;
    srand((int) time(&t));

    vet_alunos = importar_alunos(vet_alunos, &quantidade_alunos);
    vet_movimentacoes = importar_movimentacoes(vet_movimentacoes, &quantidade_mov);

    while(1) {
        int opcao;
        system("cls");
        mostrar_menu();
        scanf("%d", &opcao);

        if(opcao == 1) {
            vet_alunos = cadastrar_aluno(vet_alunos, &quantidade_alunos);
            system("pause");
        } else if(opcao == 2) {
            listar_alunos(vet_alunos, quantidade_alunos);
            system("pause");
        } else if(opcao == 3) {
            extrato(vet_movimentacoes, quantidade_mov);
            system("pause");
        } else if(opcao == 4) {
            vet_movimentacoes = adicionar_credito(vet_alunos, quantidade_alunos, vet_movimentacoes, &quantidade_mov);
            system("pause");
        } else if(opcao == 5) {
            vet_movimentacoes = usar_credito(vet_alunos, quantidade_alunos, vet_movimentacoes, &quantidade_mov);
            system("pause");
        } else if(opcao == 6) {
            salvar_arquivo(vet_alunos, quantidade_alunos, vet_movimentacoes, quantidade_mov);
            system("pause");
        } else if(opcao == 7) {
            break;
        } else {
            printf("Comando invalido\n");
            system("pause");
            break;
        }
    }
    
    free(vet_alunos);
    free(vet_movimentacoes);
   
    return 0;
}