#include <stdio.h>
#include <stdlib.h>

#include "sqlite3.h"
#include "contato.h"

void menu_contato(){

    int opcao = 0;

    while(opcao != 5){

        system("chcp 65001 >nul 2>&1");
        printf("\n1 - Inserir cadastro de contato");
        printf("\n2 - Pesquisar cadastro de contato");
        printf("\n3 - Excluir cadastro de contato");
        printf("\n4 - Atualizar cadastro de contato");
        printf("\n5 - Sair");
        printf("\n\nOpção [1-5]: ");

        scanf("%d", &opcao);

        if(!(opcao >= 1 && opcao <= 5)){
            printf("\nOpção inválida!\n");
        }

        if(opcao == 1) {
            inserir_contato();
        }

        if(opcao == 2) {
            selecionar_cadastro_contato();
        }

        if(opcao == 3) {
            excluir_cadastro_contato();
        }

        if(opcao == 4) {
            atualizar_cadastro_contato();
        }

    }
}

static int callback(void *data, int argc, char **argv, char **azColName) {
    struct contato *contato = (struct contato*)data;

    for (int i = 0; i < argc; i++) {
        if (strcmp(azColName[i], "id") == 0) {
            contato->id = atoi(argv[i]);
        } else if (strcmp(azColName[i], "nome") == 0) {
            strncpy(contato->nome, argv[i], sizeof(contato->nome));
        } else if (strcmp(azColName[i], "cpf_cnpj") == 0) {
            strncpy(contato->cpf_cnpj, argv[i], sizeof(contato->cpf_cnpj));
        } else if (strcmp(azColName[i], "telefone") == 0) {
            strncpy(contato->telefone, argv[i], sizeof(contato->telefone));
        } else if (strcmp(azColName[i], "email") == 0) {
            strncpy(contato->email, argv[i], sizeof(contato->email));
        }
    }

    return 0;
}

void inserir_contato() {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("agenda.db", &db);

    if (rc) {
        fprintf(stderr, "\nNão é possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS contato (id INTEGER PRIMARY KEY AUTOINCREMENT, nome TEXT, cpf_cnpj TEXT, telefone TEXT, email TEXT);";

    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printf("\nTabela criada com sucesso!\n");
    }

    struct contato contato;

    getchar();

    printf("\nNome: ");
    fgets(contato.nome, sizeof(contato.nome), stdin);
    contato.nome[strcspn(contato.nome, "\n")] = 0;

    printf("\nCPF | CNPJ: ");
    fgets(contato.cpf_cnpj, sizeof(contato.cpf_cnpj), stdin);
    contato.cpf_cnpj[strcspn(contato.cpf_cnpj, "\n")] = 0;

    printf("\nTelefone: ");
    fgets(contato.telefone, sizeof(contato.telefone), stdin);
    contato.telefone[strcspn(contato.telefone, "\n")] = 0;


    printf("\nE-Mail: ");
    fgets(contato.email, sizeof(contato.email), stdin);
    contato.email[strcspn(contato.email, "\n")] = 0;

    char sql_insert[150];
    sprintf(sql_insert, "INSERT INTO contato (nome, cpf_cnpj, telefone, email) VALUES ('%s', '%s', '%s', '%s');", contato.nome, contato.cpf_cnpj, contato.telefone, contato.email);

    rc = sqlite3_exec(db, sql_insert, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printf("Dados inseridos com sucesso!\n");
    }

    sqlite3_close(db);
}

void atualizar_cadastro_contato() {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("agenda.db", &db);

    if (rc) {
        fprintf(stderr, "Não é possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    struct contato contato;

    int id;

    printf("\nIdentificador: ");
    scanf("%d", &contato.id);

    getchar();

    printf("\nNome: ");
    fgets(contato.nome, sizeof(contato.nome), stdin);
    contato.nome[strcspn(contato.nome, "\n")] = 0;

    printf("\nCPF | CNPJ: ");
    fgets(contato.cpf_cnpj, sizeof(contato.cpf_cnpj), stdin);
    contato.cpf_cnpj[strcspn(contato.cpf_cnpj, "\n")] = 0;

    printf("\nTelefone: ");
    fgets(contato.telefone, sizeof(contato.telefone), stdin);
    contato.telefone[strcspn(contato.telefone, "\n")] = 0;

    printf("\nE-Mail: ");
    fgets(contato.email, sizeof(contato.email), stdin);
    contato.email[strcspn(contato.email, "\n")] = 0;

    char sql_update[100];
    sprintf(sql_update, "UPDATE contato SET nome='%s', telefone='%s', email='%s', cpf_cnpj='%s' WHERE id='%d';", contato.nome, contato.telefone, contato.email, contato.cpf_cnpj, contato.id);

    rc = sqlite3_exec(db, sql_update, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printf("\nDados atualizados com sucesso!\n");
    }

    sqlite3_close(db);
}


void selecionar_cadastro_contato() {
    int idDesejado;
    printf("Digite o Identificador desejado: ");
    scanf("%d", &idDesejado);


    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("agenda.db", &db);

    if (rc) {
        fprintf(stderr, "Não é possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return;
    }

    const char *sql_select = "SELECT * FROM contato WHERE id = ?;";
    struct contato contato;

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Preparação da consulta falhou: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, idDesejado);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        contato.id = sqlite3_column_int(stmt, 0);
        strncpy(contato.nome, (const char *)sqlite3_column_text(stmt, 1), sizeof(contato.nome));
        strncpy(contato.cpf_cnpj, (const char *)sqlite3_column_text(stmt, 2), sizeof(contato.cpf_cnpj));
        strncpy(contato.telefone, (const char *)sqlite3_column_text(stmt, 3), sizeof(contato.telefone));
        strncpy(contato.email, (const char *)sqlite3_column_text(stmt, 4), sizeof(contato.email));

        printf("\nIdentificador: %d\n", contato.id);
        printf("Nome: %s\n", contato.nome);
        printf("CPF | CNPJ: %s\n", contato.cpf_cnpj);
        printf("Telefone: %s\n", contato.telefone);
        printf("E-Mail: %s\n", contato.email);
    } else {
        printf("\nNenhum resultado encontrado para o Identificador %d.\n", idDesejado);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


void excluir_cadastro_contato(){

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("agenda.db", &db);

    if (rc) {
        fprintf(stderr, "Não é possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    int idParaDeletar;
    char sql_delete[100];


    printf("\nIdentificador: ");
    scanf("%d", &idParaDeletar);

    sprintf(sql_delete, "DELETE FROM contato WHERE id=%d;", idParaDeletar);

    rc = sqlite3_exec(db, sql_delete, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printf("\nDados excluídos com sucesso!\n");
    }

    sqlite3_close(db);
}
