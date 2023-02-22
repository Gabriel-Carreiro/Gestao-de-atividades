#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#define MAX_P 5000
#define MAX_A 200
#define MAX_I 10000
#define MAX_CARACT 50

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
    struct tm data_hora_atual; //Armazena data e hora atual do PC
} t_inscricao;

int menu_principal();
int menu_registros(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[],int *ultimo_participante, int *ultima_atividade, int *ultima_inscricao);
int menu_estatisticas(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[],int ultimo_participante, int ultima_atividade, int ultima_inscricao);
int menu_consultas(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[],int ultimo_participante, int ultima_atividade, int ultima_inscricao);
void limpar_console();
long validar_numero();
int validar_email(char *email);
int validar_data(t_data_hora data_hora);
int validar_hora(t_data_hora data_hora);
int validar_nif(t_participante participante[], int ultimo_participante);
void imprimir_data(t_data_hora data_hora);
void registrar_participante(t_participante participante[], t_atividade atividade[], int *ultimo_participante);
int consultar_participante(t_participante participante[], int busca, int ultimo_participante);
void registrar_atividade(t_atividade atividade[], int *ultima_atividade, t_participante participante[]);
int consultar_atividade(t_atividade atividade[], char busca_designacao[], int ultima_atividade);
void registrar_inscricoes(t_inscricao inscricao[], t_atividade atividade[], t_participante participante[], int id_part, int id_ativ, int *ultima_inscricao);
void consultar_inscricoes(t_inscricao inscricao[], int id_ins, int ultima_inscricao);
void atividade_por_associacao(t_atividade atividade[], int ultima_atividade);
void horizonte_temporal_inscricao(t_inscricao inscricao[], t_atividade atividade[], t_data_hora data1, t_data_hora data2, int ultima_inscricao);
void identificador_escola(t_participante participante[], t_atividade atividade[], int resposta, int final_lista, int associacao_estudante);
void identificador_atividade(t_atividade atividade[], int resposta, int final_lista);
void inscricao_por_escola(t_participante participante[], int ultimo_participante, int ultima_inscricao);
void ler_ficheiros(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[], int *ultimo_participante, int *ultima_inscricao, int *ultima_atividade);
void gravar_ficheiros(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[], int ultimo_participante, int ultima_inscricao, int ultima_atividade);

int main(){
    int resposta, ultimo_participante, ultima_atividade, ultima_inscricao;

    t_participante participante[MAX_P];
    t_atividade atividade[MAX_A];
    t_inscricao inscricao[MAX_I];
    ler_ficheiros(participante,atividade,inscricao,&ultimo_participante,&ultima_inscricao,&ultima_atividade);

    do{
        resposta = menu_principal();

        switch(resposta){
            case 1:
                resposta = menu_registros(participante, atividade, inscricao, &ultimo_participante, &ultima_atividade, &ultima_inscricao);
                break;
            case 2:
                resposta = menu_consultas(participante, atividade, inscricao, ultimo_participante, ultima_atividade, ultima_inscricao);
                break;
            case 3:
                resposta = menu_estatisticas(participante, atividade, inscricao, ultimo_participante, ultima_atividade, ultima_inscricao);
                break;
            case 4:
                gravar_ficheiros(participante,atividade,inscricao,ultimo_participante,ultima_inscricao,ultima_atividade);
                printf("Arquivos salvos\n");
                break;
        }
    } while(resposta != 0);
    getchar();
    // Salvar as alterações nos ficheiros antes de fechar o programa.
    gravar_ficheiros(participante,atividade,inscricao,ultimo_participante,ultima_inscricao,ultima_atividade);

}
int menu_principal(){
    int resposta;

    printf("*****Menu principal*****\n\n");
    printf("1-Registrar\n");
    printf("2-Consultar\n");
    printf("3-Estatisticas\n");
    printf("4-Salvar\n");
    printf("0-Sair\n\n");
    printf("************************\n");

    resposta = validar_numero();
    limpar_console();

    return resposta;
}
int menu_registros(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[],int *ultimo_participante, int *ultima_atividade, int *ultima_inscricao){
    int resposta;
    int id_part, id_ativ;

    do{
        printf("\n*****Menu de registros*****\n1-Registrar participantes\n2-Registrar atividades\n3-Registrar inscricoes\n0-Sair\n***************************\n");
        resposta = validar_numero();
        switch(resposta){
            case 1:
                registrar_participante(participante, atividade, ultimo_participante);
                break;
            case 2:
                registrar_atividade(atividade, ultima_atividade, participante);
                break;
            case 3:
                if(ultimo_participante > 0 && ultima_atividade > 0){
                    printf("Digite o Id do participante: ");
                    id_part = validar_numero();
                    printf("Digite o Id da atividade: ");
                    id_ativ = validar_numero();
                    registrar_inscricoes(inscricao, atividade,participante, id_part, id_ativ, ultima_inscricao);
                }
                else
                    printf("Para realizar uma inscrição é necessario ter ao menos 1 participante e 1 atividade cadastrados\n");
                break;
            default:
                if(resposta != 0)
                    printf("Digite o numero correspondente a uma das opcoes do menu\n");
        }
    } while(resposta != 0);
    limpar_console();
    return resposta+1;// retorna true para voltar ao menu_principal
}
int menu_consultas(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[],int ultimo_participante, int ultima_atividade, int ultima_inscricao){
    int resposta, busca, encontrado, tipo_busca;
    char busca_designacao[MAX_CARACT];
    limpar_console();
    do{
        printf("*****Menu de consultas*****\n\n1-Consultar participantes\n2-Consultar atividades\n3-Consultar inscricoes\n0-Sair\n\n***************************\n");
        resposta = validar_numero();
        switch(resposta){
            case 1:
                limpar_console();
                printf("1- Se deseja buscar individualmente pelo NIF do participante\n2- Se deseja uma lista com todos os participantes\n3- Sair\n");
                tipo_busca = validar_numero();
                if(tipo_busca == 1){
                    printf("Digite o NIF do participante: ");
                    busca = validar_numero();
                }
                else if(tipo_busca == 2)//buscar todos os participantes da lista
                    busca = -1;
                encontrado = consultar_participante(participante, busca, ultimo_participante);
                if(encontrado == 0)
                        printf("Participante nao encontrado!\n");
                break;
            case 2:
                printf("Digite a designação da atividade: ");
                getchar();
                gets(busca_designacao);
                encontrado = consultar_atividade(atividade, busca_designacao, ultima_atividade);
                if(encontrado == 0)
                    printf("Atividade nao encontrada!\n");
                break;
            case 3:
                printf("Digite o Id da inscricao: ");
                busca = validar_numero();
                consultar_inscricoes(inscricao, busca, ultima_inscricao);
                break;
            default:
                if(resposta != 0)
                    printf("Digite o numero correspondente a uma das opcoes do menu\n");
        }
    }while(resposta != 0);
    limpar_console();
    return resposta+1;// retorna true para voltar ao menu_principal
}
int menu_estatisticas(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[],int ultimo_participante, int ultima_atividade, int ultima_inscricao){
    int resposta;
    t_data_hora data[2];
    limpar_console();

    do{
        printf("\n*****Menu de estatisticas*****\n\n");
        printf("1-Numero de atividades realizadas por cada associacao\n");
        printf("2-Percentagem de inscricoes por escola\n");
        printf("3-Valor total das inscricoes entre duas datas (horizonte temporal) por tipo de atividade\n");
        printf("0-Sair\n\n");
        printf("******************************\n");

        resposta = validar_numero();

        switch(resposta){
            case 1:
                atividade_por_associacao(atividade, ultima_atividade);
                break;
            case 2:
                inscricao_por_escola(participante, ultimo_participante, ultima_inscricao);
                break;
            case 3:
                printf("Digite as datas (DD/MM/AA):\n");
                scanf("%d/%d/%d", &data[0].dia, &data[0].mes, &data[0].ano);
                scanf("%d/%d/%d", &data[1].dia, &data[1].mes, &data[1].ano);
                horizonte_temporal_inscricao(inscricao, atividade, data[0], data[1], ultima_inscricao);
                break;
            default:
                if(resposta != 0)
                    printf("Digite o numero correspondente a uma das opcoes do menu\n");
        }

    }while(resposta != 0);
    limpar_console();
    return resposta+1;// retorna true para voltar ao menu_principal
}
void limpar_console(){
    system("cls");
}
long validar_numero(){
    long numero, val;
    do{
        fflush(stdin);
        val = scanf("%ld", &numero);
        if(val < 1){
            printf("Insira um valor numerico\n");
        }
    } while(val < 1);
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
int validar_nif(t_participante participante[], int ultimo_participante){
    int i;
    for(i = 0; i < ultimo_participante; i++)
    if(participante[i].nif == participante[ultimo_participante].nif){
        printf("ERRO: NIF já existente\n");
        return 1;
    }
    if(participante[ultimo_participante].nif < 100000000){
        printf("O NIF deve conter 9 digitos\n");
        return 2;
    }
    if(participante[ultimo_participante].nif > 500000000){
        printf("NIF tem de ser de pessoa singular\n");
        return 3;
    }
    return 0;
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
    participante[i].atividades_inscritas = 0;
    printf("Id: %d \nNome:", participante[i].id_part);
    gets(participante[i].nome);
    printf("Email: ");
    gets(participante[i].email);
    email_valido = validar_email(participante[i].email);
    while (email_valido == 0){
        printf("email invalido, tente novamente\nEmail: ");
        gets(participante[i].email);
        email_valido = validar_email(participante[i].email);
    };
    fflush(stdin);
    do{
        printf("Digite o numero correspontende a Escola\n1-ESTG 2-ESECS 3-ESSLEI 4-ESAD 5-ESTM: ");
        resposta_escola = validar_numero();
        identificador_escola(participante, atividade, resposta_escola, i, 0);
    } while(resposta_escola < 1 || resposta_escola > 5);
    do{
        printf("NIF: ");
        participante[i].nif = validar_numero();
    } while(validar_nif(participante, i) != 0);
    printf("Telefone: ");
    participante[i].telefone = validar_numero();
    printf("Registo realizado, Id: %d Nome: %s \n",participante[i].id_part, participante[i].nome);
    limpar_console();
    *ultimo_participante = i +1;
}
int consultar_participante(t_participante participante[], int busca, int ultimo_participante){
    int encontrado, i;
    limpar_console();

    if(ultimo_participante < 1){
        printf("Nao existem  participantes salvos\n");
        encontrado = 2; //Nao há participantes na lista
    }
    else{
        for(i = 0; i < ultimo_participante; i++){
            if(busca == -1){
                printf("Id: %d Nome: %s NIF: %ld  Escola: %s Email: %s Telefone: %ld\n",participante[i].id_part, participante[i].nome, participante[i].nif, participante[i].escola, participante[i].email, participante[i].telefone);
                encontrado = 1;
            }
            else if(participante[i].nif == busca) {
                    encontrado = 1;//Participante encontrado
                    printf("Id: %d Nome: %s NIF: %ld\n",participante[i].id_part, participante[i].nome, participante[i].nif);
                    printf("Escola: %s Email: %s Telefone: %ld\n", participante[i].escola, participante[i].email, participante[i].telefone);
            }
        }
    }
    return encontrado;
}
void registrar_atividade(t_atividade atividade[], int *ultima_atividade, t_participante participante[]){
    int resposta_escola,tipo_atividade, data_valida, hora_valida, i = *ultima_atividade;

    limpar_console();
    getchar();
    atividade[i].id_ativ = i;
    printf("Id: %d\n", atividade[i].id_ativ);
    printf("Designacao: ");
    gets(atividade[i].designacao);
    printf("Local: ");
    gets(atividade[i].local);
    printf("Tipo de atividade:1-Academica 2-Lazer 3-Cultura 4-Desporto 5-Formacao 6-Outra\nInsira o numero correspondente ao tipo de atividade: ");
    tipo_atividade = validar_numero();
    identificador_atividade(atividade, tipo_atividade, i);
    fflush(stdin);
    printf("Digite o numero correspontende a associação de estudantes organizadora\n1- AE-ESTG 2- AE-ESECS 3- AE-ESSLEI 4- AE-ESAD 5- AE-ESTM: ");
    resposta_escola = validar_numero();
    identificador_escola(participante, atividade, resposta_escola, i, 1);
    printf("Valor da inscricao: ");
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
int consultar_atividade(t_atividade atividade[], char busca_designacao[], int ultima_atividade){

    int i, resposta, encontrado = 0;
    limpar_console();

    if(ultima_atividade < 1){
        printf("Não há atividade salva\n");
        encontrado = 2; //Nao há atividade na lista
    }
    else{
        printf("1- Deseja ver todas\n2-Buscar por designacao\n");
        resposta = validar_numero();
        if(resposta == 1){
            for(i= 0; i < ultima_atividade; i++){
                encontrado = 1;
                printf("Id: %d\t Designacao: %s\t Local: %s\t Tipo de atividade: %s\n",atividade[i].id_ativ, atividade[i].designacao, atividade[i].local, atividade[i].tipo_atividade);
                printf("Associacao organizadora: %s\t Valor: %.2f\t\n", atividade[i].associacao, atividade[i].valor_inscricao);
                imprimir_data(atividade[i].data_hora);
                imprimir_hora(atividade[i].data_hora);
            }
        }
        else if(resposta == 2){
            for(i = 0; i < ultima_atividade; i++){
                if(strcmp(atividade[i].designacao, busca_designacao) == 0) {
                    encontrado = 1;
                    printf("Id: %d\t Designacao: %s\t Local: %s\t Tipo de atividade: %s\n",atividade[i].id_ativ, atividade[i].designacao, atividade[i].local, atividade[i].tipo_atividade);
                    printf("Associacao organizadora: %s\t Valor: %.2f\t\n", atividade[i].associacao, atividade[i].valor_inscricao);
                    imprimir_data(atividade[i].data_hora);
                    imprimir_hora(atividade[i].data_hora);
                }
            }
        }
    }
    return encontrado;
}
void registrar_inscricoes(t_inscricao inscricao[], t_atividade atividade[],t_participante participante[], int id_part, int id_ativ, int *ultima_inscricao){
    time_t segundos;
    int i = *ultima_inscricao;
    time(&segundos);

    inscricao[i].id_ins = i;
    inscricao[i].id_ins_part = id_part;
    inscricao[i].id_ins_ativ = id_ativ;
    participante[id_part].atividades_inscritas++;
    inscricao[i].valor_ins_ativ = atividade[id_ativ].valor_inscricao;
    inscricao[i].data_hora_atual = *localtime(&segundos);
    printf("Data e hora da inscricao: %d/%d/%d   %d:%d\n", inscricao[i].data_hora_atual.tm_mday, inscricao[i].data_hora_atual.tm_mon+1, inscricao[i].data_hora_atual.tm_year+1900, inscricao[i].data_hora_atual.tm_hour, inscricao[i].data_hora_atual.tm_min);
    printf("Id participante = %d, Id atividade = %d, valor = %.2f\n", inscricao[i].id_ins_part, inscricao[i].id_ins_ativ, inscricao[i].valor_ins_ativ);
    *ultima_inscricao = i + 1;
}
void consultar_inscricoes(t_inscricao inscricao[], int id_ins, int ultima_inscricao){
    int i, encontrado;

    if(ultima_inscricao < 1){
        printf("Não existem inscrição salva\n");
        encontrado = 2; //Nao há inscrição na lista
    }
    else{
        for(i = 0; i < ultima_inscricao; i++){
            if(inscricao[i].id_ins == id_ins){
                printf("Id inscricao: %d\n", inscricao[i].id_ins);
                printf("Id participante: %d\n", inscricao[i].id_ins_part);
                printf("Id atividade: %d\n", inscricao[i].id_ins_ativ);
                printf("Valor pago: %.2f\n", inscricao[i].valor_ins_ativ);
                printf("Data: %d/%d/%d Hora: %d:%d:%d\n", inscricao[i].data_hora_atual.tm_mday, inscricao[i].data_hora_atual.tm_mon+1, inscricao[i].data_hora_atual.tm_year+1900, inscricao[i].data_hora_atual.tm_hour, inscricao[i].data_hora_atual.tm_min, inscricao[i].data_hora_atual.tm_sec);
                encontrado = 1;
            }
        }
        if (encontrado != 1)
            printf("Inscricao nao encontrada\n");
    }
}
void atividade_por_associacao(t_atividade atividade[], int ultima_atividade){
    int i, ESTG, ESECS, ESSLEI, ESAD, ESTM;
    ESTG = ESECS = ESSLEI = ESAD = ESTM = 0;
    limpar_console();
    if(ultima_atividade < 1)
        printf("Não existem atividades salvas no arquivo\n");
    else{
        for(i = 0; i < ultima_atividade; i++){
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
}
void horizonte_temporal_inscricao(t_inscricao inscricao[], t_atividade atividade[], t_data_hora data1, t_data_hora data2, int ultima_inscricao){
    long i, soma_data1, soma_data2, soma_data_inscricao[MAX_I], academica, lazer, cultura, desporto, formacao, outra;
    academica = lazer = cultura = desporto = formacao = outra = 0;
    soma_data1 = data1.ano*365+data1.mes*31+data1.dia;
    soma_data2 = data2.ano*365+data2.mes*31+data2.dia;

    if(ultima_inscricao < 1)
        printf("Não existem inscricoes no arquivo\n");
    else{
        for(i = 0; i < ultima_inscricao; i++){
            soma_data_inscricao[i] = (inscricao[i].data_hora_atual.tm_year+1900)*365+(inscricao[i].data_hora_atual.tm_mon+1)*31+inscricao[i].data_hora_atual.tm_mday;
            if(soma_data_inscricao[i] > soma_data1 && soma_data_inscricao[i] < soma_data2){
                if(strcmp(atividade[inscricao[i].id_ins_ativ].tipo_atividade, "Academica") == 0)
                    academica++;
                else if(strcmp(atividade[inscricao[i].id_ins_ativ].tipo_atividade, "Lazer") == 0)
                    lazer++;
                else if(strcmp(atividade[inscricao[i].id_ins_ativ].tipo_atividade, "Cultura") == 0)
                    cultura++;
                else if(strcmp(atividade[inscricao[i].id_ins_ativ].tipo_atividade, "Desporto") == 0)
                    desporto++;
                else if(strcmp(atividade[inscricao[i].id_ins_ativ].tipo_atividade, "Formação") == 0)
                    formacao++;
                else if(strcmp(atividade[inscricao[i].id_ins_ativ].tipo_atividade, "Outra") == 0)
                    outra++;
            }
        }
        printf("Academica: %ld\nLazer: %ld\nCultura: %ld\nDesporto: %ld\nFormação: %ld\nOutra: %ld", academica, lazer, cultura, desporto, formacao, outra);
    }
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
void identificador_atividade(t_atividade atividade[], int resposta, int final_lista){

    switch(resposta){
        case 1:
            strcpy(atividade[final_lista].tipo_atividade, "Academica");
            break;
        case 2:
            strcpy(atividade[final_lista].tipo_atividade, "Lazer");
            break;
        case 3:
            strcpy(atividade[final_lista].tipo_atividade, "Cultura");
            break;
        case 4:
            strcpy(atividade[final_lista].tipo_atividade, "Desporto");
            break;
        case 5:
            strcpy(atividade[final_lista].tipo_atividade, "Formação");
            break;
        case 6:
            strcpy(atividade[final_lista].tipo_atividade, "Outra");
            break;
    }
}
void inscricao_por_escola(t_participante participante[], int ultimo_participante, int ultima_inscricao){
    int i, total_inscricao = ultima_inscricao;
    float ESTG, ESECS, ESSLEI, ESAD, ESTM;
    ESTG = ESECS = ESSLEI = ESAD = ESTM = 0;
    limpar_console();
    if(ultima_inscricao < 1)
        printf("Não existem inscricoes salvos\n");
    else{
        for(i = 0; i < ultima_inscricao; i++){
            if(participante[i].atividades_inscritas > 0){
                if(strcmp(participante[i].escola, "ESTG") == 0)
                    ESTG += participante[i].atividades_inscritas;
                else if(strcmp(participante[i].escola, "ESECS") == 0)
                    ESECS += participante[i].atividades_inscritas;
                else if(strcmp(participante[i].escola, "ESSLEI") == 0)
                    ESSLEI += participante[i].atividades_inscritas;
                else if(strcmp(participante[i].escola, "ESAD") == 0)
                    ESAD += participante[i].atividades_inscritas;
                else if(strcmp(participante[i].escola, "ESTM") == 0)
                    ESTM += participante[i].atividades_inscritas;
            }
        }
        printf("Total de %d inscricoes\n", total_inscricao);
        if(ESTG || ESECS || ESSLEI || ESAD || ESTM){
            ESTG = (ESTG * 100)/ (total_inscricao);
            ESECS = (ESECS * 100)/ (total_inscricao);
            ESSLEI = (ESSLEI * 100)/ (total_inscricao);
            ESAD = (ESAD * 100) / (total_inscricao);
            ESTM =(ESTM * 100) / (total_inscricao);
            printf("ESTG: %.2f%%\nESECS: %.2f%%\nESSLEI: %.2f%%\nESAD: %.2f%%\nESTM: %.2f%%\n",ESTG, ESECS, ESSLEI, ESAD, ESTM);
        }
    }
}
void ler_ficheiros(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[], int *ultimo_participante, int *ultima_inscricao, int *ultima_atividade){
    size_t fSize;
    int i;
    FILE *participantes;
    FILE *atividades;
    FILE *inscricoes;
    participantes = fopen("participantes.dat","rb");
    if(participantes){
        fseek(participantes, 0L, SEEK_END);
        fSize = ftell(participantes);
        i = fSize/sizeof(t_participante);
        fseek(participantes, 0L, SEEK_SET);
        fread(participante, sizeof(t_participante), i, participantes);
        *ultimo_participante = i;
    }
    else
        printf("ERRO: nao foi possivel abrir o arquivo de participantes. \n\n");

    atividades = fopen("atividades.dat","rb");
    if(atividades){
        fseek(atividades, 0L, SEEK_END);
        fSize = ftell(atividades);
        *ultima_atividade = fSize/sizeof(t_atividade);
        fseek(atividades, 0L, SEEK_SET);
        fread(atividade, sizeof(t_atividade), *ultima_atividade, atividades);
    }
    else
        printf("ERRO: nao foi possivel abrir o arquivo de atividades. \n\n");

    inscricoes = fopen("inscricoes.dat","rb");
    if(inscricoes){
        fseek(inscricoes, 0L, SEEK_END);
        fSize = ftell(inscricoes);
        *ultima_inscricao = fSize/sizeof(t_inscricao);
        fseek(inscricoes, 0L, SEEK_SET);
        fread(inscricao, sizeof(t_inscricao), *ultima_inscricao, inscricoes);
    }
    else
        printf("ERRO: nao foi possivel abrir o arquivo de inscrições. \n\n");

    fclose(participantes);
    fclose(atividades);
    fclose(inscricoes);
}
void gravar_ficheiros(t_participante participante[], t_atividade atividade[], t_inscricao inscricao[], int ultimo_participante, int ultima_inscricao, int ultima_atividade){
    FILE *participantes;
    FILE *atividades;
    FILE *inscricoes;

    participantes = fopen("participantes.dat","wb");
    if(participantes)
        fwrite(participante, sizeof(t_participante), ultimo_participante, participantes);
    else
        printf("ERRO: nao foi possivel gravar o arquivo de participantes. \n\n");

    atividades = fopen("atividades.dat","wb");
    if(atividades)
        fwrite(atividade, sizeof(t_atividade), ultima_atividade, atividades);
    else
        printf("ERRO: nao foi possivel gravar o arquivo de atividades. \n\n");

    inscricoes = fopen("inscricoes.dat","wb");
    if(inscricoes)
        fwrite(inscricao, sizeof(t_inscricao), ultima_inscricao, inscricoes);
    else
        printf("ERRO: nao foi possivel gravar o arquivo de inscrições. \n\n");

    fclose(participantes);
    fclose(atividades);
    fclose(inscricoes);

}

