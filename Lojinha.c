#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>

typedef struct node{
	int qtde;
	char nome[60];
	float precoCusto;
	float precoVenda;
	struct node *prox;
}Lista;

typedef struct no{
	int qtde;
	float preco;
	char nome[60];
	struct no *prox;
}Venda;

Lista *criarLista();
Venda *criarVenda();
Lista *inserir(Lista *l, int qtde, char *nome, float precoCusto, float precoVenda);
Lista *buscar(Lista *l, char *nome);
Lista *retirar(Lista *l, char *nome);
void salvarEstoque(Lista *l, FILE *arq, FILE *arq1, float lucro);
void relatorioVenda(Venda *l, FILE *arq, float total);
void estoqueEmFalta(Lista *l, FILE *arq);
Venda *insereVenda(Venda *l, int qtde, char *nome, float preco);
Venda *libera(Venda *l);
void menu(float lucro);
void listagem(Lista *l);
void limpatela();

int main(){
	//setlocale(LC_ALL,"Portuguese");
	Lista *mylista = criarLista();
	Venda *myvenda = criarVenda();
	Lista *elem;
	int qtde,op,i=0,quan;
	float precoCusto,precoVenda,lucro,total=0;
    char nome[60],opc;
    FILE *arq = fopen("Produtos.txt","r");
    if(!arq){
        printf("\nImpossivel de se abrir o arquivo de Estoque\n");
    }else{
        while(fscanf(arq,"%d %f %f %[^\n]s",&qtde,&precoVenda,&precoCusto,nome) != EOF){
           mylista = inserir(mylista,qtde,nome,precoCusto,precoVenda);
        }
        fclose(arq);
    }
    FILE *arq1 = fopen("Lucro.txt","r");
    if(!arq){
    	printf("\nImpossivel de se abrir o arquivo de Lucro\n");
	}else{
		fscanf(arq1,"%f",&lucro);
		fclose(arq1);
	}
	do{
		Menu: menu(lucro);
		printf("-> ");
		scanf("%d",&op);
		system("cls");
		switch(op){
			case 1:	printf("Digite o nome do produto: ");
				scanf(" %[^\n]s",nome);
				printf("\nDigite a sua quantidade: ");
				scanf("%d",&qtde);
				printf("\nDigite o preco de Custo: ");
				scanf("%f",&precoCusto);
				printf("\nDigite o preco de Venda: ");
				scanf("%f",&precoVenda);
				mylista = inserir(mylista,qtde,nome,precoCusto,precoVenda);
				printf("\nDados Cadastrados.\n\n");
				salvarEstoque(mylista,arq,arq1,lucro);
				estoqueEmFalta(mylista,arq);
			break;
			case 2:
				printf("Digite o nome do produto: ");
				scanf(" %[^\n]s",nome);
				system("cls");
				elem = buscar(mylista,nome);
				if(elem){
					printf("|---------------------------------\n");  
    				printf("|Nome: %s\n",elem->nome);
    				printf("|Quantidade: %d\n",elem->qtde);
    				printf("|Preco de Custo: R$%.2f\n",elem->precoCusto);
    				printf("|Preco de Venda: R$%.2f\n",elem->precoVenda);
    				printf("|---------------------------------\n");
				}else{
					printf("\nProduto nao encontrado.\n");
				}
				limpatela();
			break;
			case 3: listagem(mylista);
				break;
			case 4: printf("Digite o nome do produto: ");
				fflush(stdin);
				gets(nome);
				elem = buscar(mylista,nome);
				if(elem){
					system("cls");
					printf("|---------------------------------\n");  
    				printf("|Nome: %s\n",elem->nome);
    				printf("|Quantidade: %d\n",elem->qtde);
    				printf("|Preco de Custo: R$%.2f\n",elem->precoCusto);
    				printf("|Preco de Venda: R$%.2f\n",elem->precoVenda);
    				printf("|---------------------------------\n\n");
    				printf("Deseja excluir esse produto 'S/N': ");
    				scanf("%c",&opc);
    				if(opc == 'S' || opc == 's'){
    					mylista = retirar(mylista,nome);
						printf("\nProduto Excluido com sucesso\n\n");	
					}else{
						system("cls");
						printf("Operacao cancelada.\n\n");
					}
				}else{
					printf("\nProduto nao consta no Estoque\n\n");
				}
				salvarEstoque(mylista,arq,arq1,lucro);
				estoqueEmFalta(mylista,arq);
			break;
			case 5:
				printf("Digite a quantidade de produtos: ");
				scanf("%d",&quan);
				while(i != quan){
					printf("\nProduto: ");
					fflush(stdin);
					gets(nome);
					elem = buscar(mylista,nome);
					system("cls");
					if(elem){
						printf("-------------------------------------\n");
						printf("Produto: %s\n",elem->nome);
						printf("\nPreco: R$%.2f\n",elem->precoVenda);
						printf("\nEstoque: %d\n",elem->qtde);
						printf("-------------------------------------\n");
						printf("\nQuantidade: ");
						scanf("%d",&qtde);
						if(elem->qtde - qtde <= 0){
							system("cls");
							printf("Demanda maior que estoque, tente de novo.\n\n");
							system("pause");
							goto Menu;
						}else{
							elem->qtde -= qtde; 
							total = total + elem->precoVenda * qtde;
							system("cls");
							printf("Total Parcial: R$%.2f\n",total);
							myvenda = insereVenda(myvenda,qtde,elem->nome,elem->precoVenda*qtde);
						}
					}else{
						printf("\nProduto nao consta no Estoque\n");
					}
					i++;	
				}
				lucro += total;
				system("cls");
				printf("-------------------------------------\n");
				printf("Total: R$%.2f\n",total);
				printf("-------------------------------------\n");
				relatorioVenda(myvenda,arq,total);
				salvarEstoque(mylista,arq,arq1,lucro);
				estoqueEmFalta(mylista,arq);
				myvenda = libera(myvenda);
			break;
			case 6: 
				printf("Produto: ");
				fflush(stdin);
				gets(nome);
				elem = buscar(mylista,nome);
				if(elem){
					system("cls");
					printf("-------------------------------------\n");
					printf("|Produto: %s\n",elem->nome);
					printf("|Preco de Venda: R$%.2f\n",elem->precoVenda);
					printf("|Preco de Custo: R$%.2f\n",elem->precoCusto);
					printf("|Quantidade: %d\n",elem->qtde);
					printf("-------------------------------------\n");
					printf("1 - Atualizar Nome: \n");
					printf("2 - Atualizar Preco de Venda: \n");
					printf("3 - Atualizar Quantidade: \n");
					printf("4 - Atualizar Preco de Custo: \n");
					printf("5 - Sair: \n\n");
					printf("-> ");
					scanf("%d",&quan);
					switch(quan){
						case 1:
							system("cls");
							printf("Digite o novo nome do produto: ");
							fflush(stdin);
							gets(nome);
							strcpy(elem->nome,nome);
							salvarEstoque(mylista,arq,arq1,lucro);
						break;
						case 2:
							system("cls");
							printf("Digite o novo preco do produto: ");
							scanf("%f",&precoVenda);
							elem->precoVenda = precoVenda;
							salvarEstoque(mylista,arq,arq1,lucro);
						break;	
						case 3:
							system("cls");
							printf("Digite a nova quantidade de produtos: ");
							scanf("%d",&qtde);
							elem->qtde = qtde;
							salvarEstoque(mylista,arq,arq1,lucro);
							estoqueEmFalta(mylista,arq);
						break;			
						case 4:
							printf("Digite o novo preco de custo do produto: ");
							scanf("%f",&precoCusto);
							elem->precoCusto = precoCusto;
							salvarEstoque(mylista,arq,arq1,lucro);
						break;	
						case 5:
							system("cls");
						break;
					}
				}else{
					printf("\nProduto nao encontrado.\n");
					limpatela();
				}
			break;
			case 7:
				system("calc");
			break;
		}
	}while(op != 8);
}

Lista *criarLista(){
	return NULL;
}

Venda *criarVenda(){
	return NULL;
}

Lista *inserir(Lista *l, int qtde, char *nome, float precoCusto, float precoVenda){
	if(l == NULL || strcmp(nome,l->nome) < 0){
		Lista *novo = (Lista*)malloc(sizeof(Lista));
		novo->qtde = qtde;
		novo->precoCusto = precoCusto;
		novo->precoVenda = precoVenda;
		strcpy(novo->nome,nome);
		novo->prox = l;
		return novo;
	}else{
		l->prox = inserir(l->prox,qtde,nome,precoCusto,precoVenda);
	}
	return l;
}

Venda *insereVenda(Venda *l, int qtde, char *nome, float preco){
	if(l == NULL || strcmp(nome,l->nome) < 0){
		Venda *novo = (Venda*)malloc(sizeof(Venda));
		novo->qtde = qtde;
		strcpy(novo->nome,nome);
		novo->preco = preco;
		novo->prox = l;
		return novo;
	}else{
		l->prox = insereVenda(l->prox,qtde,nome,preco);
	}
	
}

void limpatela(){
	system("pause");
	system("cls");
}

void listagem(Lista *l){
	Lista *p = l;
	for(p=l;p!=NULL;p=p->prox){
		printf("Nome: %s\n",p->nome);
		printf("Quantidade: %d\n",p->qtde);
		printf("Preco de Custo: R$%.2f\n",p->precoCusto);
		printf("Preco de Venda: R$%.2f\n",p->precoVenda);
		printf("---------------------------------------------------------------------------\n");
	}
	limpatela();
}

Lista *retirar(Lista *l, char *nome){
	int str;
	str = strlen(nome);
	Lista *ant = NULL;
	Lista *p = l;
	while(p!=NULL && strnicmp(p->nome,nome,str) != 0){
		ant = p;
		p = p->prox;
	}
	if(p == NULL){
		return l;
	}
	if(ant == NULL){
		l = p->prox;
	}else{
		ant->prox = p->prox;
	}
	free(p);
	return l;

}

Venda *libera(Venda *l){
    Venda *p = l;
    while (p != NULL) {
        p = l->prox;
        free(l);
        l = p;
    }
    return NULL;
}

Lista *buscar(Lista *l, char *nome){
	int tam = strlen(nome);
    if(l == NULL){
        return NULL;
    }else if(strnicmp(l->nome,nome,tam) == 0){
    	return l;
    }
    return buscar(l->prox,nome);
}

void menu(float lucro){
	printf("|------------------------------|\n");
	printf("|-------Comercial K & V--------|\n");
	printf("|------------------------------|\n");
	printf("| 1 - Inserir Produto:         |\n");
	printf("|------------------------------|\n");
	printf("| 2 - Procurar Produto:        |\n");
	printf("|------------------------------|\n");
	printf("| 3 - Exibir Estoque:          |\n");
	printf("|------------------------------|\n");
	printf("| 4 - Excluir Produto:         |\n");
	printf("|------------------------------|\n");
	printf("| 5 - Realizar Venda:          |\n");
	printf("|------------------------------|\n");
	printf("| 6 - Atualizar Produto:       |\n");
	printf("|------------------------------|\n");
	printf("| 7 - Calculadora:             |\n");
	printf("|------------------------------|\n");
	printf("| 8 - Sair:                    |\n");
	printf("|------------------------------|\n");
	printf(" Caixa: R$%.2f\n\n",lucro);
}

void salvarEstoque(Lista *l, FILE *arq, FILE *arq1, float lucro){
	arq = fopen("Produtos.txt","w");
	Lista *elem = l;
	for(elem=l;elem!=NULL;elem=elem->prox){
		fprintf(arq,"%d %.2f %.2f %s\n",elem->qtde,elem->precoVenda,elem->precoCusto,elem->nome);
	}
	fclose(arq);
	arq1 = fopen("Lucro.txt","w");
	fprintf(arq1,"%.2f",lucro);
	fclose(arq1);
	limpatela();
}

void relatorioVenda(Venda *l, FILE *arq,float total){
	Venda *p = l;
	arq = fopen("RelatorioDeVendas.txt","a");
	if(!arq){
		printf("Impossivel de se abrir o relatorio de vendas.");
	}else{
		fprintf(arq,"\n\n-> Relatorio de Vendas\n");
		fprintf(arq,"--------------------------------------------------------------------\n");
		for(p=l;p!=NULL;p=p->prox){
			fprintf(arq,"Total: R$%.2f \tQuantidade: %d \tProduto: %s\n",p->preco,p->qtde,p->nome);	
		}
		fprintf(arq,"--------------------------------------------------------------------\n");
		fprintf(arq,"Total: R$%.2f  DATA: %s  HORA: %s",total,__DATE__,__TIME__);
	}
	fclose(arq);	
}

void estoqueEmFalta(Lista *l, FILE *arq){
	Lista *p = l;
	arq = fopen("EstoqueEmFalta.txt","w");
	if(!arq){
		printf("Impossivel de se abrir o arquivo de estoque em falta.");
	}else{
		fprintf(arq,"-> Estoque em Falta:\n");
		fprintf(arq,"--------------------------------------------------------------------\n");
		for(p=l;p!=NULL;p=p->prox){
			if(p->qtde <= 5){
				fprintf(arq,"Quantidade: %d \tProduto: %s\n",p->qtde,p->nome);			
			}
		}
		fprintf(arq,"--------------------------------------------------------------------\n");		
	}
	fclose(arq);
	
}
