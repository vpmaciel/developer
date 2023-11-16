#include <stdio.h>
#include <stdlib.h>

void exibir_nome_do_sistema();

void menu();

int main()
{
    // system("export LC_ALL=en_US.UTF-8");
    system("chcp 65001 >nul 2>&1");
    exibir_nome_do_sistema();
    menu();


    return 0;
}

void exibir_nome_do_sistema(){
    printf("Sistema Bit Despachante\n");
}

void menu(){

    int opcao = 0;

    while(opcao != 2){
        system("chcp 65001 >nul 2>&1");

        printf("\n1 - Clientes");
        printf("\n2 - Encerrar o programa");
        printf("\n\nOpção [1-2]: ");

        scanf("%d", &opcao);

        if(!(opcao >= 1 && opcao <= 2)){
            printf("\nOpção inválida!\n");
        }

        if(opcao == 1) {
            menu_contato();
        }
    }
}
