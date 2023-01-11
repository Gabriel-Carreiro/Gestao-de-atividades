#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#define MAX_P 5000
#define MAX_A 200
#define MAX_I 10000
#define MAX_CARACT 50
#define MAX_HORA 24
#define MAX_MIN 60
#define MAX_SEG 60

 typedef struct {
    int dia, mes, ano, hora, minuto, segundo;
} t_data_hora;

typedef struct {
    int id_part, atividades_inscritas;
    long nif, telefone;
    char nome[MAX_CARACT], email[MAX_CARACT], escola[10];
} t_participante;

typedef struct {
    int id_ativ;
    char designacao[MAX_CARACT], tipo_atividade[MAX_CARACT], associacao[MAX_CARACT], local[MAX_CARACT];
    float valor_inscricao;
    t_data_hora data_hora;
} t_atividade;

typedef struct {
    int id_ins, id_ins_part, id_ins_ativ;
    float valor_ins_ativ;
    struct tm data_hora_atual;
} t_inscricao;

 int menu_principal(){
    int resposta;
    //limpar_console();

    printf("\n*****Menu principal*****\n");
    printf("1-Registrar\n");
    printf("2-Consultar\n");
    printf("3-Estatisticas\n");
    printf("0-Sair\n");
    printf("************************\n");

    scanf("%d", &resposta);

    return resposta;
}

int menu_registros(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[],int *ultimo_participante, int *ultima_atividade, int *ultima_inscricao){
    int resposta;
    int x, y;

    do{
        printf("\n*****Menu de registros*****\n");
        printf("1-Registrar participantes\n");
        printf("2-Registrar atividades\n");
        printf("3-Registrar inscricoes\n");
        printf("0-Sair\n");
        printf("***************************\n");

        scanf("%d", &resposta);

        switch(resposta){
            case 1:
                registrar_participante(participante, atividade, ultimo_participante);
                break;
            case 2:
                registrar_atividade(atividade, ultima_atividade, participante);
                break;
            case 3:
                scanf("%d %d", &x, &y);
                registrar_inscricoes(inscricao, atividade, x,y, ultima_inscricao);
                break;
            default:
                if(resposta != 0)
                    printf("Digite o numero correspondente a uma das op�oes do menu\n");
        }
    } while(resposta != 0);

    return resposta+1;// retorna true para voltar ao menu_principal
}

int menu_consultas(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[],int *ultimo_participante, int *ultima_atividade, int *ultima_inscricao){
    int resposta, busca, encontrado, tipo_busca;

    limpar_console();

    do{
        printf("\n*****Menu de consultas*****\n");
        printf("1-Consultar participantes\n");
        printf("2-Consultar atividades\n");
        printf("3-Consultar inscricoes\n");
        printf("0-Sair\n");
        printf("***************************\n");

        scanf("%d", &resposta);

        switch(resposta){
            case 1:
                limpar_console();
                printf("1- Se deseja buscar individualmente o participante\n2- Se deseja uma lista com todos os participantes\n3- Sair\n");
                scanf("%d", &tipo_busca);
                if(tipo_busca == 1){
                    printf("Digite o Id do participante: ");
                    scanf("%d", &busca);
                    encontrado = consultar_participante(busca, &ultimo_participante);
                    if(encontrado == 0)
                        printf("Participante nao encontrado!\n");
                }
                else if(tipo_busca == 2){
                    busca = -1;
                    encontrado = consultar_participante(participante, busca, ultimo_participante);
                }
                break;
            case 2:
                printf("Digite o Id da atividade: ");
                scanf("%d", &busca);
                encontrado = consultar_atividade(atividade, busca, ultima_atividade);
                if(encontrado == 0)
                    printf("Atividade nao encontrada!\n");
                break;
            case 3:
                printf("Digite o Id da inscri��o: ");
                scanf("%d", &busca);
                consultar_inscricoes(inscricao, busca, ultima_inscricao);
                break;
            default:
                if(resposta != 0)
                    printf("Digite o numero correspondente a uma das op�oes do menu\n");
        }
    }while(resposta != 0);
    return resposta+1;// retorna true para voltar ao menu_principal
}

int menu_estatisticas(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[],int *ultimo_participante, int *ultima_atividade, int *ultima_inscricao){
    int resposta;
    limpar_console();

    do{
        printf("\n*****Menu de estatisticas*****\n");
        printf("1-N�mero de atividades realizadas por cada associacao\n");
        printf("2-Percentagem de inscri��es por escola\n");
        printf("3-Valor total das inscri��es entre duas datas (horizonte temporal) por tipo de atividade\n");
        printf("0-Sair\n");
        printf("******************************\n");

        scanf("%d", &resposta);

        switch(resposta){
            case 1:
                atividade_por_associacao(atividade, ultima_atividade);
                break;
            case 2:
                inscricao_por_escola(participante, ultimo_participante, ultima_inscricao);
                break;
            case 3:
                printf("valor total entre bla bla\n");
                break;
            default:
                if(resposta != 0)
                    printf("Digite o numero correspondente a uma das op�oes do menu\n");
        }

    }while(resposta != 0);
    return resposta+1;// retorna true para voltar ao menu_principal
}


void limpar_console(){
    system("cls");
}

int converter_entrada_inteiro(char *entrada){
    int numero;

    numero = atoi(entrada);
    printf("%d    %s .",numero, entrada);
    return numero;
}

int validar_email(char *email) {
    int tam = strlen(email), i, arroba, ponto, antesPonto, depoisPonto;
    arroba = ponto = antesPonto = depoisPonto = 0;

    for (i = 0; i < tam; i++) {
        char c = email[i];
        if (c == '@') {  // Verifica se tem arroba
            if (arroba)
                break;
            arroba = 1;
            if (i < 3)  // Verifica se tem 3 letras antes do arroba;
                break;
        }
        else if (arroba) {
            if (ponto)
                depoisPonto++;
            else if (c == '.') {
                ponto = 1;
                if (antesPonto < 2)  //Verifica se tem 2 letras antes do ponto
                    break;
            }
            else
                antesPonto++;

        }
    }

    if (i == tam && depoisPonto > 1)
        return 1;
    else
        return 0;
}

int validar_data(t_data_hora data_hora){
    if (data_hora.ano >= 1900 && data_hora.ano <= 2050){
        if (data_hora.mes >= 1 && data_hora.mes <= 12){
            if ((data_hora.dia >= 1 && data_hora.dia <= 31) && (data_hora.mes == 1 || data_hora.mes == 3 || data_hora.mes == 5 || data_hora.mes == 7 || data_hora.mes == 8 || data_hora.mes == 10 || data_hora.mes == 12))
                return 1;
            else if ((data_hora.dia >= 1 && data_hora.dia <= 30) && (data_hora.mes == 4 || data_hora.mes == 6 || data_hora.mes == 9 || data_hora.mes == 11))
                return 1;
            else if ((data_hora.dia >= 1 && data_hora.dia <= 29) && (data_hora.mes == 2))
                return 1;
        }
    }
    return 0;// Retorna 0 caso algum dos datos nao seja valido
}

int validar_hora(t_data_hora data_hora){

    printf("%d:%d",data_hora.hora, data_hora.minuto);
    if (data_hora.hora < 0 || data_hora.hora > 23)
        return 0;
    if (data_hora.minuto < 0 || data_hora.minuto > 59)
        return 0;
    if (data_hora.segundo < 0 || data_hora.segundo > 59)
        return 0;
    return 1;
}

void imprimir_data(t_data_hora data_hora){
    printf("%d/%d/%d\n", data_hora.dia, data_hora.mes, data_hora.ano);
}

void imprimir_hora(t_data_hora data_hora){
    printf("%d:%d\n", data_hora.hora, data_hora.minuto);
}

void registrar_participante(t_participante participante[], t_atividade atividade[], int *ultimo_participante){
    int resposta_escola, email_valido, i;
    getchar();
    limpar_console();
    i = *ultimo_participante;

    participante[i].id_part = i;
    printf("Nome:");
    gets(participante[i].nome);
    printf("Email: ");
    gets(participante[i].email);
    email_valido = validar_email(participante[i].email);
    while (email_valido == 0){
        printf("email invalido, tente novamente\n Email: ");
        gets(participante[i].email);
        email_valido = validar_email(participante[i].email);
    };
    fflush(stdin);
    do{
        printf("Digite o numero correspontende a Escola\n1-ESTG 2-ESECS 3-ESSLEI 4-ESAD 5-ESTM: ");
        scanf("%d", &resposta_escola);
        identificador_escola(participante, atividade, resposta_escola, i, 0);
    } while(resposta_escola < 1 || resposta_escola > 5);
    printf("NIF: ");
    scanf("%ld", &participante[i].nif);
    printf("Telefone: ");
    scanf("%ld", &participante[i].telefone);
    printf("Id: %d Nome: %s \n",participante[i].id_part, participante[i].nome);
    printf("%d", i);
    *ultimo_participante = i +1;

}

//Podemos incluir as atividades em que o aluno esta matriculado
int consultar_participante(t_participante participante[], int busca, int *ultimo_participante){
    int encontrado, i;
    limpar_console();

    for(i = 0; i < *ultimo_participante; i++){
        if(busca == -1){
            printf("Id: %d Nome: %s NIF: %ld  Escola: %s Email: %s Telefone: %ld\n",participante[i].id_part, participante[i].nome, participante[i].nif, participante[i].escola, participante[i].email, participante[i].telefone);
            encontrado = 1;
        }
        else if(participante[i].id_part == busca) {
            encontrado = 1;//verdadeiro
            printf("Encontrado\n");
            printf("Id: %d Nome: %s NIF: %ld\n",participante[i].id_part, participante[i].nome, participante[i].nif);
            printf("Escola: %s Email: %s Telefone: %ld\n", participante[i].escola, participante[i].email, participante[i].telefone);
        }
    }
    return encontrado;
}

void registrar_atividade(t_atividade atividade[], int *ultima_atividade, t_participante participante[]){
    int resposta_escola, data_valida, hora_valida, i = *ultima_atividade;

    limpar_console();
    getchar();
    atividade[i].id_ativ = i;
    printf("Id: %d\n", atividade[i].id_ativ);
    printf("Designação: ");
    gets(atividade[i].designacao);
    printf("Local: ");
    gets(atividade[i].local);
    printf("Tipo de atividade: ");
    gets(atividade[i].tipo_atividade);
    printf("Associação de estudantes organizadora: ");
    fflush(stdin);
    printf("Digite o numero correspontende a associação de estudantes organizadora\n1- AE-ESTG 2- AE-ESECS 3- AE-ESSLEI 4- AE-ESAD 5- AE-ESTM: ");
    scanf("%d", &resposta_escola);
    identificador_escola(participante, atividade, resposta_escola, i, 1);
    printf("Valor da inscrição: ");
    scanf("%f", &atividade[i].valor_inscricao);

    do {
        printf("Data (DD/MM/YYYY): ");
        scanf("%d/%d/%d", &atividade[i].data_hora.dia, &atividade[i].data_hora.mes, &atividade[i].data_hora.ano);
        data_valida = validar_data(atividade[i].data_hora); //Retorna 0 em caso de erro e 1 em caso de sucesso
        if (data_valida == 0)
            printf("Erro, a data não é valida\n");
    }while (data_valida == 0);
    do{
        printf("Hora (HH:MM): ");
        scanf("%d:%d", &atividade[i].data_hora.hora, &atividade[i].data_hora.minuto);
        hora_valida = validar_hora(atividade[i].data_hora);
        if (hora_valida == 0)
            printf("Erro, horario invalido\n");
    }while (hora_valida == 0);

    *ultima_atividade = i + 1;
}

int consultar_atividade(t_atividade atividade[], int id_atividade, int *ultima_atividade){

    int i, encontrado = 0;
    limpar_console();

    for(i = 0; i < *ultima_atividade; i++){
        if(atividade[i].id_ativ == id_atividade){
            printf("Atividade encontrada\n");
            encontrado = 1;
            printf("Id: %d\t Designação: %s\t Local: %s\t Tipo de atividade: %s\n",atividade[i].id_ativ, atividade[i].designacao, atividade[i].local, atividade[i].tipo_atividade);
            printf("Associação organizadora: %s\t Valor: %.2f\t\n", atividade[i].associacao, atividade[i].valor_inscricao);
            imprimir_data(atividade[i].data_hora);
            imprimir_hora(atividade[i].data_hora);
        }
    }
    return encontrado;
}

void registrar_inscricoes(t_inscricao inscricao[], t_atividade atividade[], int id_part, int id_ativ, int *ultima_inscricao){
    time_t segundos;
    int i = *ultima_inscricao;
    time(&segundos);


    inscricao[i].id_ins = i;
    inscricao[i].id_ins_part = id_part;
    inscricao[i].id_ins_ativ = id_ativ;
    inscricao[i].valor_ins_ativ = atividade[id_ativ].valor_inscricao;
    //participante[id_part].atividades_inscritas++;
    inscricao[i].data_hora_atual = *localtime(&segundos);
    printf("Data e hora da inscrição: %d/%d/%d   %d:%d\n", inscricao[i].data_hora_atual.tm_mday, inscricao[i].data_hora_atual.tm_mon+1, inscricao[i].data_hora_atual.tm_year+1900, inscricao[i].data_hora_atual.tm_hour, inscricao[i].data_hora_atual.tm_min);
    printf("Id participante = %d, Id atividade = %d, valor = %.2f", inscricao[i].id_ins_part, inscricao[i].id_ins_ativ, inscricao[i].valor_ins_ativ);
    *ultima_inscricao = i + 1;
}

void consultar_inscricoes(t_inscricao inscricao[], int id_ins, int *ultima_inscricao){
    int i, encontrado;

    for(i = 0; i < *ultima_inscricao; i++){
        if(inscricao[i].id_ins == id_ins){
            printf("Id inscrição: %d\n", inscricao[i].id_ins);
            printf("Id participante: %d\n", inscricao[i].id_ins_part);
            printf("Id atividade: %d\n", inscricao[i].id_ins_ativ);
            printf("Valor pago: %f\n", inscricao[i].valor_ins_ativ);
            printf("Data: %d/%d/%d Hora: %d:%d:%d\n", inscricao[i].data_hora_atual.tm_mday, inscricao[i].data_hora_atual.tm_mon+1, inscricao[i].data_hora_atual.tm_year+1900, inscricao[i].data_hora_atual.tm_hour, inscricao[i].data_hora_atual.tm_min, inscricao[i].data_hora_atual.tm_sec);
            encontrado = 1;
        }
    }
    if (encontrado != 1)
        printf("Inscrição nao encontrada\n");
}

void atividade_por_associacao(t_atividade atividade[], int *ultima_atividade){
    int i, ESTG, ESECS, ESSLEI, ESAD, ESTM;
    ESTG = ESECS = ESSLEI = ESAD = ESTM = 0;

    for(i = 0; i < *ultima_atividade; i++){
            if(strcmp(atividade[i].associacao, "AE-ESTG") == 0){
                ESTG++;
            }
            else if(strcmp(atividade[i].associacao, "AE-ESECS") == 0){
                ESECS++;
            }
            else if(strcmp(atividade[i].associacao, "AE-ESSLEI") == 0){
                ESSLEI++;
            }
            else if(strcmp(atividade[i].associacao, "AE-ESAD") == 0){
                ESAD++;
            }
            else if(strcmp(atividade[i].associacao, "AE-ESTM") == 0){
                ESTM++;
            }
            else
                printf("ERRO %s NAO EXISTE\n", atividade[i].associacao);
    }
    printf("AE-ESTG: %d\nAE-ESECS: %d\nAE-ESSLEI: %d\nAE-ESAD: %d\nAE-ESTM: %d\n", ESTG, ESECS, ESSLEI, ESAD, ESTM);
}

void identificador_escola(t_participante participante[], t_atividade atividade[], int resposta, int final_lista, int associacao_estudante){

    if(associacao_estudante == 0){
        switch(resposta){
            case 1:
                strcpy(participante[final_lista].escola, "ESTG");
                break;
            case 2:
                strcpy(participante[final_lista].escola, "ESECS");
                break;
            case 3:
                strcpy(participante[final_lista].escola, "ESSLEI");
                break;
            case 4:
                strcpy(participante[final_lista].escola, "ESAD");
                break;
            case 5:
                strcpy(participante[final_lista].escola, "ESTM");
                break;
        }
    }
    if(associacao_estudante == 1){
                switch(resposta){
            case 1:
                strcpy(atividade[final_lista].associacao, "AE-ESTG");
                break;
            case 2:
                strcpy(atividade[final_lista].associacao, "AE-ESECS");
                break;
            case 3:
                strcpy(atividade[final_lista].associacao, "AE-ESSLEI");
                break;
            case 4:
                strcpy(atividade[final_lista].associacao, "AE-ESAD");
                break;
            case 5:
                strcpy(atividade[final_lista].associacao, "AE-ESTM");
                break;
        }
    }
}

void inscricao_por_escola(t_participante participante[], int *ultimo_participante, int *ultima_inscricao){
    int i, total_inscricao = *ultima_inscricao;
    float ESTG, ESECS, ESSLEI, ESAD, ESTM;
    ESTG = ESECS = ESSLEI = ESAD = ESTM = 0;

    for(i = 0; i < *ultima_inscricao; i++){
            if(strcmp(participante[i].escola, "ESTG") == 0){
                ESTG++;
            }
            else if(strcmp(participante[i].escola, "ESECS") == 0){
                ESECS++;
            }
            else if(strcmp(participante[i].escola, "ESSLEI") == 0){
                ESSLEI++;
            }
            else if(strcmp(participante[i].escola, "ESAD") == 0){
                ESAD++;
            }
            else if(strcmp(participante[i].escola, "ESTM") == 0){
                ESTM++;
            }
    }
    if(ESTG || ESECS || ESSLEI || ESAD || ESTM){
        ESTG = (ESTG * 100)/ total_inscricao;
        ESECS = (ESECS * 100)/ total_inscricao;
        ESSLEI = (ESSLEI * 100)/ total_inscricao;
        ESAD = (ESAD * 100) / total_inscricao;
        ESTM =(ESTM * 100) / total_inscricao;
        printf("ESTG: %.2f%%\n", ESTG);
        printf("ESECS: %.2f%%\n", ESECS);
        printf("ESSLEI: %.2f%%\n", ESSLEI);
        printf("ESAD: %.2f%%\n", ESAD);
        printf("ESTM: %.2f%%\n", ESTM);
    }
    else
        printf("Nenhuma inscrição encontrada.\n");
}

void ler_ficheiros(FILE *participantes, FILE *atividades, FILE *inscricoes, t_participante participante[], t_atividade atividade[], t_inscricao inscricao[], int *ultimo_participante, int *ultima_inscricao, int *ultima_atividade){
    size_t fSize;
    int i;
    participantes = fopen("participantes.dat","rb");
    if(participantes){
        fseek(participantes, 0L, SEEK_END);
        fSize = ftell(participantes);
        i = fSize/sizeof(t_participante);
        fseek(participantes, 0L, SEEK_SET);
        fread(participante, sizeof(t_participante), i, participantes);
        fclose(participantes);
        *ultimo_participante = i;
    }
    else{
        printf("ERRO: nao foi possivel abrir o arquivo de participantes. \n\n");
        }

    atividades = fopen("atividades.dat","rb");
    if(atividades){
        fseek(atividades, 0L, SEEK_END);
        fSize = ftell(atividades);
        *ultima_atividade = fSize/sizeof(t_atividade);
        fseek(atividades, 0L, SEEK_SET);
        fread(atividade, sizeof(t_atividade), *ultima_atividade, atividades);
        fclose(atividades);
    }
    else{
        printf("ERRO: nao foi possivel abrir o arquivo de atividades. \n\n");
        }

    inscricoes = fopen("inscricoes.dat","rb");
    if(inscricoes){
        fseek(inscricoes, 0L, SEEK_END);
        fSize = ftell(inscricoes);
        *ultima_inscricao = fSize/sizeof(t_inscricao);
        fseek(inscricoes, 0L, SEEK_SET);
        fread(inscricao, sizeof(t_inscricao), *ultima_inscricao, inscricoes);
        fclose(inscricoes);
    }
    else{
        printf("ERRO: nao foi possivel abrir o arquivo de inscrições. \n\n");
        }
}
void gravar_ficheiros(FILE *participantes, FILE *atividades, FILE *inscricoes, t_participante participante[], t_atividade atividade[], t_inscricao inscricao[], int ultimo_participante, int ultima_inscricao, int ultima_atividade){
    int i = ultimo_participante;
    participantes = fopen("participantes.dat","wb");
    if(participantes){
        fwrite(participante, sizeof(t_participante), i, participantes);
        fclose(participantes);
    }
    else{
        printf("ERRO: nao foi possivel gravar o arquivo de participantes. \n\n");
        }

    atividades = fopen("atividades.dat","wb");
    if(atividades){
        fwrite(atividade, sizeof(t_atividade), i, atividades);
        fclose(atividades);
    }
    else{
        printf("ERRO: nao foi possivel gravar o arquivo de atividades. \n\n");
        }

    inscricoes = fopen("inscricoes.dat","wb");
    if(inscricoes){
        fwrite(inscricao, sizeof(t_inscricao), i, inscricoes);
        fclose(inscricoes);
    }
    else{
        printf("ERRO: nao foi possivel gravar o arquivo de inscrições. \n\n");
        }
}


int main(){
    int resposta, ultimo_participante, ultima_atividade, ultima_inscricao, x, y;
    struct tm *data_hora_atual;
    char ficheiro = "participantes.dat";
    FILE *participantes;
    FILE *atividades;
    FILE *inscricoes;

    t_participante participante[MAX_P];
    t_atividade atividade[MAX_A];
    t_inscricao inscricao[MAX_I];

    ler_ficheiros(participantes, atividades, inscricoes, participante,atividade,inscricao,&ultimo_participante,&ultima_atividade,&ultima_inscricao);

    setlocale(LC_ALL, "Portuguese");

//    //participante teste
//    participante[0].id_part = 0;
//    strcpy(participante[0].nome , "Gabriel");
//    strcpy(participante[0].email, "gabri@fassa");
//    participante[0].nif = 123231132;
//    participante[0].telefone = 26241557;
//    strcpy(participante[0].escola, "ESTG");
//    //atividade teste
//    atividade[0].id_ativ = 0;
//    strcpy(atividade[0].designacao, "briga");
//    strcpy(atividade[0].tipo_atividade, "luta");
//    strcpy(atividade[0].associacao, "AE-ESECS");
//    strcpy(atividade[0].local, "leiria");
//    atividade[0].valor_inscricao = 20,20;
//    atividade[0].data_hora.dia = 10;
//    atividade[0].data_hora.mes =10;
//    atividade[0].data_hora.ano = 2022;
//    atividade[0].data_hora.hora =20;
//    atividade[0].data_hora.minuto = 20;

    do{
        resposta = menu_principal();

        switch(resposta){
            case 1:
                resposta = menu_registros(participante, atividade, inscricao, &ultimo_participante, &ultima_atividade, &ultima_inscricao);
                break;
            case 2:
                resposta = menu_consultas(participante, atividade, inscricao, &ultimo_participante, &ultima_atividade, &ultima_inscricao);
                break;
            case 3:
                resposta = menu_estatisticas(participante, atividade, inscricao, &ultimo_participante, &ultima_atividade, &ultima_inscricao);
                break;
            case 4:
                gravar_ficheiros(participantes, atividades, inscricoes, participante,atividade,inscricao,ultimo_participante,ultima_atividade,ultima_inscricao);
                printf("Arquivos salvos\n");
                break;
        }
    } while(resposta != 0);
    getchar();
}
