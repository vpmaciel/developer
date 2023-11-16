#ifndef CONTATO_H_INCLUDED
#define CONTATO_H_INCLUDED

struct contato {
    int id;
    char nome[50];
    char cpf_cnpj[25];
    char telefone [20];
    char email[100];
};

void inserir_contato();

void atualizar_cadastro_contato();

void selecionar_cadastro_contato();

void excluir_cadastro_contato();

#endif // CONTATO_H_INCLUDED
