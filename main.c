#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include<string.h>
#include <windows.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

struct conta{
	int numero_conta;
	int codigo_cliente;
	int estado;
	float saldo ;


};
struct pessoa{
	int codigo;
	char nome[100];
	char bi_ou_nif[20];
	char morada[120];
	char telefone[10];

};

typedef struct pessoa pessoa;
typedef struct conta conta;
 conta contas[1000];
 pessoa clientes[1000];
 int tipo_de_conta;
 int totalClientes;
 int totalContas;
 FILE *bd;

 bool Cadastrar_Clientes(pessoa p){
 	p.codigo = totalClientes + 1;
 	clientes[totalClientes] = p;
 	totalClientes = totalClientes + 1;
 	return true;

 }

 int buscar_codigo(int codigo){
 	 int t;
 	 for(t=0;t<totalContas;t++){
 	 	conta c = contas[t];
 	 	if (c.codigo_cliente == codigo) {
 	 		return t;
		  }
	  }
 	return -1;
 }
  void  Listar_Clientes(){
  	int lista;
  	for (lista=0 ; lista<totalClientes;lista++){
  	pessoa  cliente = clientes[lista];
  		printf("Dados Pessoais \n");
  		printf("C�digo: %d\n", cliente.codigo);
  		printf("Nome: %s \n", cliente.nome);
  		printf("Telefone: %s\n", cliente.telefone);
  		printf("Identifica��o: %s\n", cliente.bi_ou_nif);
  	    printf("Morada: %s\n", cliente.morada );

  	    int posicao = buscar_codigo(cliente.codigo);

  	    if (posicao != -1){
  	    	conta c = contas[posicao];
  	    	printf(" Dados da conta \n");
  	    	printf("N�mero da conta: %d \n ", c.numero_conta);
  	    	printf("Saldo: %2.f \n ", c.saldo);
  	    	printf(" Estado: %d\n ", c.estado);
  	    	printf("C�digo da conta: %d\n ", c.codigo_cliente);
		  }


	  }


   }

    int buscar_cliente(int codigo){
    	int n;
    	for(n=0; n < totalClientes; n++){
    		pessoa p = clientes[n];
    		if (p.codigo == codigo) {
    			return n;
			}
		}

		return -1;
	}

	bool Abertura_de_conta( pessoa p, int tipo_de_conta ) {
		conta c;
	//	int n = 02;
		c.numero_conta = totalContas + 1;

		c.estado = 0;
		c.codigo_cliente = p.codigo;
		contas[totalContas] = c;

		totalContas++;

		return true;
	}

	 int buscar_conta_pelo_numero_conta (int numero_conta){
	int i;
	for (i =0; i < totalContas;i++) {
		conta c= contas[i];
		if (c.numero_conta == numero_conta) {
			return i;
		}
	}

	return -1;
    }

	bool depositar_dinheiro (int numero_conta, float valor){
	int posicao = buscar_conta_pelo_numero_conta (numero_conta);
    if(posicao==-1){

        return false;
    }
    contas[posicao].saldo +=valor;
	contas[posicao].estado=1;
    return true;

	}

bool levantar_dinheiro (int numero_conta, float valor) {
	int posicao =buscar_conta_pelo_numero_conta (numero_conta);

	if (posicao == -1) {
		return false;
	}
	if (contas[posicao].saldo < valor) {
		return false;
	}
	contas[posicao].saldo =contas[posicao].saldo - valor;
	return true;
}
float consultar_saldo (int numero_conta) {
	int posicao =buscar_conta_pelo_numero_conta (numero_conta);
	if (posicao == -1) {
		return -1;
	}
	printf ("O seu saldo é de: %2.f\n",contas[posicao].saldo);

	return 1;
}

bool transferir_dinheiro (int numero_conta_enviar, int numero_conta_receber, float valor){
	int posicao_enviar = buscar_conta_pelo_numero_conta (numero_conta_enviar);

	 if (posicao_enviar == -1) {
	 	return false;
	 }

	int posicao_receber = buscar_conta_pelo_numero_conta (numero_conta_receber);

	 if (posicao_receber == -1) {
	 	return false;
	 }else{
	 	printf("conta encontrada\n");
	}


	if (contas[posicao_enviar].saldo < valor) {
		return false;
	}
	contas[posicao_enviar].saldo  -= valor;
	contas[posicao_receber].saldo += valor;

	return true;
}

 bool atualizar_dados (pessoa p, int codigo_cliente){
	int posicao = buscar_cliente(codigo_cliente);

	if (posicao == -1){
		return false;
	}

	p.codigo = codigo_cliente;
	clientes[posicao] = p;

	return true;
 }

 void criarficheiro(){

bd=fopen("banco_de_dados.txt","a");
if(bd==NULL){
    printf("Base dados nao existe\n");
}
printf("Ficheiro criado! \n");
fclose(bd);
 }

void escrever_ficheiro(){
    bd= fopen("banco_de_dados.txt","w");
      int n;
    if(bd==NULL){
        printf("Impossivel escrever\n");
    }

    for(n=0;n<totalClientes;n++){


        fprintf(bd,"%d;%s;%s;%s;%s;",clientes[n].codigo,clientes[n].nome,clientes[n].telefone,clientes[n].bi_ou_nif,clientes[n].morada);
		int i = buscar_codigo(clientes[n].codigo);
		if (i != -1){
			fprintf(bd,"%i;%f;%i;%i" , contas[i].numero_conta, contas[i].saldo, contas[i].estado, contas[i].codigo_cliente);

		}
		fprintf(bd, "\n");
    }
    fclose(bd);
}
void carregar_ficheiro(){
    pessoa p;
	conta c;
     bd = fopen("banco_de_dados.txt", "r");
	 char linhas[1000];
    if (bd == NULL){
        printf("Houve um erro ao abrir o ficheiro.");
    }else if (bd != NULL){
    	printf("Ficheiro aberto!");
	}

		while(fscanf(bd, "%[^\n]\n", &linhas) == 1){

			char *quebras=strtok(linhas, ";");
			char *linha_quebrada[10];


			int search = 0;
			while (quebras != NULL) {
				linha_quebrada[search] = quebras;
				quebras = strtok(NULL, ";");

				search++;
			}

		    sscanf(linha_quebrada[0], "%d", &p.codigo);
			strcpy(p.nome, linha_quebrada[1]);
			strcpy(p.telefone, linha_quebrada[2]);
			strcpy(p.bi_ou_nif, linha_quebrada[3]);
			strcpy(p.morada, linha_quebrada[4]);
			clientes[totalClientes] = p;
			totalClientes++;

			if (search == 5){
				continue;
			}else{

				sscanf(linha_quebrada[5], "%d", &c.numero_conta);
				sscanf(linha_quebrada[6], "%f", &c.saldo);
				sscanf(linha_quebrada[7], "%d", &c.estado);
				sscanf(linha_quebrada[8], "%d", &c.codigo_cliente);
				contas[totalContas] = c;
				totalContas++;
			}

		}

		fclose(bd);
}



int main() {
	pessoa p;
	int codigo;
	int numero_conta, numero_conta_enviar,numero_conta_receber, numero_conta1;
	float valor1, valor;
	setlocale(LC_ALL,"Portuguese");
	criarficheiro();
	carregar_ficheiro();
	printf("\n ------------- Bem-Vindo ao Sistema Bancário BAI -------- \n ");
	int tecla;

		while ( tecla != 9 ) {
		printf("\n 1- Cadastrar Clientes \n");
		printf(" 2- Listar Clientes \n");
		printf(" 3- Abertura de conta \n");
		printf(" 4- Depositar dinheiro \n");
		printf(" 5- Levantar dinheiro \n");
		printf(" 6- Consultar saldo  \n");
		printf(" 7- Transferir Dinheiro  \n");
		printf(" 8- Actualizar Dados do cliente  \n");
		printf(" 9- Terminar  \n");
		printf( " Op��o:" );
		scanf("%d", &tecla);
		switch(tecla) {
			case 1:

				printf("Digite o seu nome: ");
				scanf("%s", p.nome );
				printf("Digite o seu o n�mero de telefone: ");
				scanf("%s", p.telefone);
				printf("Bilhete de identidade/Nif: ");
				scanf("%s", p.bi_ou_nif);
				printf("Digite a sua morada: ");
				scanf("%s", p.morada);

				Cadastrar_Clientes(p);

				break;
			case 2:
				Listar_Clientes();
				break;


			case 3:
				printf("Digite o seu c�digo pessoal : ");
				scanf("%d", &codigo);

				int posicao = buscar_cliente(codigo);

				if (posicao != -1){

					int tipo_de_conta;
					printf ("Insira o tipo de conta (0 para particular ou 1 para empres�rial  ):");
					scanf("%d", &tipo_de_conta);

					Abertura_de_conta(clientes[posicao], tipo_de_conta);

				}

				break;
			case 4:

			printf ("Digite o numero da conta: ");
			scanf ("%d",&numero_conta1);
            int verificando=buscar_conta_pelo_numero_conta(numero_conta1);
            if(verificando==-1){
                printf("Impossivel\n");
            }
            else{
                printf("Pessoa encontrada\n");
                printf("Digite valor\n");
                scanf("%f",&valor1);
                bool verificandod=depositar_dinheiro(numero_conta1,valor1);
                if(verificandod==false){
                    printf("Nao depositado\n");
                }else{
                    printf("Deposito realizado\n");
                }

            }


				break;
			case 5:

printf ("Digite o numero da conta: ");
			scanf ("%d",&numero_conta1);
            int verificandod =buscar_conta_pelo_numero_conta(numero_conta1);
            if(verificandod==-1){
                printf("Impossivel\n");
            }
            else{
                printf("Pessoa encontrada\n");
                printf("Digite valor a levantar\n");
                scanf("%f",&valor1);
                bool verificandod=levantar_dinheiro(numero_conta1,valor1);
                if(verificandod==false){
                    printf("Erro!\n");
                }else{
                    printf("Levantamento realizado\n");
                }

            }
			 break;

		 case 6:

		printf("Digite o numero da conta");
		scanf("%d",&numero_conta);
		consultar_saldo(numero_conta);

				break;
			case 7:

			  printf("Digite a conta enviar");
			  scanf("%d",&numero_conta_enviar);
			  printf("Conta recber");
			  scanf("%d",&numero_conta_receber);
			  printf("Valor");
			  scanf("%f",&valor);
			  transferir_dinheiro (numero_conta_enviar,numero_conta_receber, valor);
				break;
			case 8:


			printf ("Insira o seu codigo pessoal: ");
			scanf ("%d",&codigo);

			 posicao = buscar_cliente(codigo);

			pessoa p;

			printf ("Digite o seu nome: ");
			scanf ("%s",p.nome);
			printf ("Digite o seu telefone: ");

			scanf ("%s", p.telefone);
			printf ("Digite o seu bilhete: ");
			scanf ("%s", p.bi_ou_nif);
			printf ("Digite a sua morada: ");
			scanf ("%s", p.morada);

			 atualizar_dados(p, codigo);
				break;


			case 9:
				break;
		}

		escrever_ficheiro();


	}
	return 0;
}
