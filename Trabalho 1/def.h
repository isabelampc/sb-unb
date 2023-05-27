#define SIZE_MAX 500
#define SIZE_NAME 300

typedef struct table_simb{
    char name[SIZE_NAME];
    int lc; 
    struct table_simb *prox; //para fazer uma lista e facilitar;  
}table_simb;

typedef struct{ //tipo para localizar a lista
    table_simb *inicio;
    int tam;
}lista;

typedef struct instrution{
    int line; 
    char rot[SIZE_NAME];
    int mnemonico;
    char operando_1[SIZE_NAME];
    char operando_2[SIZE_NAME];
    struct instrution *proximo; 
} instrution;

typedef struct{
    instrution *comeco, *fim;
}list;

// Variaveis globais
lista tab_simbolos; // tabela de simbolos
list lista_instrucoes;  //lista de instrucoes
list sec_data;
list mdt; //tabela das macros
list equ; //tabela das igualdades
int tem_equ = 0, linha_equ = 0, erro_if = 0, usa_equ = 0, usa_if = 0; //segunda variavel vai verificar se adiciona no arquivo final o equ
int tem_macro = 0, linhas_macro = 0, usa_macro = 0, macro = 0; //variavel de controle para uso da macro 
char nome_macro[SIZE_MAX]; // apenas uma macro por programa 
int contador_posicao = 0; // contador de posicao 
int erro = 0; // caso 1, possui um erro e não é possivel montar o programa final. 
int comentarios = 0;//verifica se tem comentario colado
int section_data = 0;

void inicializa_tabela(lista *tabela){
    tabela->inicio = NULL;
    tabela->tam = 0;
}

void inicializa_lista(list *lista){
    lista->comeco = NULL;
    lista->fim = NULL;
}

void insere_tabela(lista *tabela, int cont, char* nome){
    table_simb *aux, *new = (table_simb*)malloc(sizeof(table_simb));

    if(new){
        new->lc = cont;
        strcpy(new->name, nome);
        new->prox = NULL;
        if(tabela->inicio == NULL){
            tabela->inicio = new;
        }
        else{
            aux = tabela->inicio;
            while(aux->prox != NULL){
                aux = aux->prox;
            }
            aux->prox = new;    
        }
        tabela->tam++;
    }
    else printf("Não possível!\n"); 
}

void insere_lista(list *lista, int linha, char *rotulo, int instru, char *op1, char *op2){
    instrution *new = (instrution*)malloc(sizeof(instrution));
    if(new){
        new->line = linha;
        new->mnemonico = instru;
        strcpy(new->rot,rotulo);
        strcpy(new->operando_1,op1);
        strcpy(new->operando_2,op2);
        new->proximo = NULL;
        if(lista->comeco == NULL){
            lista->comeco = new;
            lista->fim = new;
        }
        else{
            lista->fim->proximo = new;
            lista->fim = new;
        }
    }
    else printf("Não possível!\n");
}

void imprime_tabela(lista *tabela){
    table_simb *aux = tabela->inicio;
    while(aux != NULL){
        printf("%s %d\n", aux->name, aux->lc);
        aux=aux->prox;
    }
}

void imprime_lista(list *tabela){
    instrution *aux = tabela->comeco;
    while(aux != NULL){
        printf("%d %s %d %s %s\n", aux->line, aux->rot, aux->mnemonico, aux->operando_1, aux->operando_2);
        aux=aux->proximo;
    }
}

table_simb* verifica_rotulo(lista* tabela, char *rotulo){
    table_simb *aux, *rot = NULL;
    aux = tabela->inicio;
    while(aux && (strcmp(aux->name, rotulo) != 0)){
        aux = aux->prox;
    }
    if(aux){
        rot = aux;
    }
    return rot; 
}

char* busca_equ(list *lista, char *valor){
    instrution *aux, *val = NULL;
    aux = lista->comeco;
    while(aux){
        if(strcmp(aux->rot, valor) == 0){
            break;
        }
        aux = aux->proximo;
    }
    if(aux){
        val = aux;
        return val->operando_1; 
    }
    return NULL;
}

int busca_ultima(list *lista, int linha, char *rotulo, int instru, char *op1, char *op2){
    instrution *aux;
    int i = 0;
    aux = lista->fim;
    if(aux){
        //printf("rotulo anterior = %s rotulo atual = %s mne anterior = %d mne atual = %d\n", aux->rot, rotulo, aux->mnemonico, instru);
        if((strcmp(aux->rot, "rotulo") != 0 ) && (strcmp(rotulo, "rotulo") != 0 ) && (aux->mnemonico == 0) ){
            printf("Linha %d: Erro sintatico, mais de um rotulo definido.\n", linha);
            erro = 1;
            i = 3;
        }
        else if((strcmp(aux->rot, "rotulo") != 0 ) && (aux->mnemonico == 0)){
            aux->mnemonico = instru;
            strcpy(aux->operando_1,op1);
            strcpy(aux->operando_2,op2);
            aux->proximo = NULL;
            i = 1;
        }
        
    }
    return i;
}

int instruction(char *ptr){
    if(strcmp(ptr, "ADD") == 0){return 1;}
    else if(strcmp(ptr, "MULT") == 0){ return 3;}
    else if(strcmp(ptr, "SUB") == 0){ return 2;}
    else if(strcmp(ptr, "DIV") == 0){ return 4;}
    else if(strcmp(ptr, "JMP") == 0){ return 5;}
    else if(strcmp(ptr, "JMPN") == 0){ return 6;}
    else if(strcmp(ptr, "JMPP") == 0){ return 7;}
    else if(strcmp(ptr, "JMPZ") == 0){ return 8;}
    else if(strcmp(ptr, "COPY") == 0){ return 9;}
    else if(strcmp(ptr, "LOAD") == 0){ return 10;}
    else if(strcmp(ptr, "STORE") == 0){ return 11;}
    else if(strcmp(ptr, "INPUT") == 0){ return 12;}
    else if(strcmp(ptr, "OUTPUT") == 0){ return 13;}
    else if(strcmp(ptr, "STOP") == 0){ return 14;}
    else return 0;
}

char* aux_ins(int ins){
    if(ins == 1){return "ADD";}
    else if(ins == 3){ return "MULT";}
    else if(ins == 2){ return "SUB";}
    else if(ins == 4){ return "DIV";}
    else if(ins  == 5){ return "JMP";}
    else if(ins  == 6){ return "JMPN";}
    else if(ins  == 7){ return "JMPP";}
    else if(ins == 8){ return "JMPZ";}
    else if(ins  == 9){ return "COPY";}
    else if(ins  == 10){ return "LOAD";}
    else if(ins  == 11){ return "STORE";}
    else if(ins  == 12){ return "INPUT";}
    else if(ins  == 13){ return "OUTPUT";}
    else if(ins  == 14){ return "STOP";}
    else if(ins == 15){ return "SPACE";}
    else if(ins == 16){return "CONST";}
    else if(ins == 17){return "EQU";}
    else if(ins == 18){return "IF";}
    else if(ins == 19){return "MACRO";}
    else if(ins == 20){return "ENDMACRO";}
    else return " ";
}

int diretiva(char *ptr){
    if(strcmp(ptr, "SPACE")==0){ return 15;} //retornar valores depois para facilitar
    else if(strcmp(ptr, "CONST")==0){return 16;}
    else if(strcmp(ptr, "EQU")==0){return 17;}
    else if(strcmp(ptr, "IF") == 0){return 18;}
    else if (strcmp(ptr,"MACRO")==0){return 19;}
    else if(strcmp(ptr, "ENDMACRO") == 0){return 20;}
    else return 0; 
}

int erro_identificador(char ptr[SIZE_MAX], int linha, int opcode){
    int cont = 0;
    int retorno = 0;
    while(ptr[cont] != '\0'){
       if(isalnum(ptr[cont]) == 0){
           if(ptr[cont] == '_' || ptr[cont] == ';' || ptr[cont] == ' '){
           }
           else{
                retorno = 1;
                break;
           }
        }
        cont++;
    }
    if(opcode == 16 && ptr[0] == '-'){
        retorno = 0;
    }
    if(isdigit(ptr[0]) != 0 && opcode != 16){ //caso tenha algum dado comecando com numero, fora o o valor de const retorna 1
        retorno = 1;
    }
    if(retorno == 1){
        printf("Linha %d : Erro lexico encontrado, caractere nao reconhecido ou instrucao/identificador mal digitado\n", linha); 
    }

    return retorno; 
}

void incrementa(int valor){
    if (valor == 1 || valor == 2 || valor == 3 || valor == 4 || valor == 5 || valor == 6 || valor == 7 || valor == 8 || valor == 10 || valor == 11 ||valor == 12 || valor == 13){
        contador_posicao+=2;
    }
    else if (valor == 14|| valor == 15||valor == 16){
        contador_posicao++; 
    }
    else if (valor == 9){
        contador_posicao+=3;
    }
}

int processa_macro(list *origem, list *destino, int linha){
    instrution *aux; 
    aux = origem->comeco;
    
    if(aux){
    aux = aux->proximo;}
    while(aux){
        insere_lista(destino, linha, aux->rot, aux->mnemonico, aux->operando_1, aux->operando_2);
        incrementa(aux->mnemonico);
        aux=aux->proximo;
        linha++;
    }
    
    return linha;
}

void data(list *origem, list *destino){
    instrution *aux, *aux2;
    aux = origem->comeco;
    aux2 = destino->fim;
    int linha = aux2->line;
    linha++;
    while(aux){
        insere_lista(destino, linha, aux->rot, aux->mnemonico, aux->operando_1, aux->operando_2);
        
        if(verifica_rotulo(&tab_simbolos, aux->rot) == NULL){
            insere_tabela(&tab_simbolos, contador_posicao, aux->rot);
        }
        else {
            printf("Linha %d:  Erro semantico. Rotulo/declaracao ja definido anteriormente!\n", linha);
            erro = 1;
        }
        linha++;
        incrementa(aux->mnemonico);
        aux=aux->proximo;
    }
}

int verifica_qtd(int mne, char* op1, char* op2, int linha){
    if (mne == 1 || mne == 2 || mne == 3 || mne == 4 || mne == 5 || mne == 6 || mne == 7 || mne == 8 || mne == 10 || mne == 11 ||mne == 12 || mne == 13 || mne == 16){
        if(strcmp(op1, "operando 1") == 0){
            erro = 1;
            printf("Linha %d: Erro sintatico na quantidade de operandos da instrucao!\n", linha);
            return 1;
        }
        if(strcmp(op2, "operando 2") != 0){
            erro = 1;
            printf("Linha %d: Erro sintatico na quantidade de operandos da instrucao!\n", linha);
            return 1;
        }
    }
    else if (mne == 14|| mne == 15){
        if(strcmp(op1, "operando 1") != 0){
            erro = 1;
            printf("Linha %d: Erro sintatico na quantidade de operandos da instrucao!\n", linha);
            return 1;
        }
        if(strcmp(op2, "operando 2") != 0){
            erro = 1;
            printf("Linha %d: Erro sintatico na quantidade de operandos da instrucao!\n", linha);
            return 1;
        }
    }
    else if (mne == 9){
        if(strcmp(op1, "operando 1") == 0){
            erro = 1;
            printf("Linha %d: Erro sintatico na quantidade de operandos da instrucao!\n", linha);
            return 1;
        }
        if(strcmp(op2, "operando 2") == 0){
            erro = 1;
            printf("Linha %d: Erro sintatico na quantidade de operandos da instrucao!\n", linha);
            return 1;
        }
    }
    return 0; 

}

char *comentario(char *auxiliar){
    char *retorno, *aux2;
    retorno = strstr(auxiliar, ";");
    if(strlen(auxiliar) > 1){
        if(retorno){
            aux2 = strtok(auxiliar, ";");
            comentarios = 1; 
            return aux2; 
        }
    }   
    return auxiliar; 
}

int primeira_passagem(char instrucao[SIZE_MAX], int contador_linha){
    int i = 0, contador = 0, opcode, is_data = 0;
    char *pt; 
    char *aux, *aux2; // aux 2 serve para cmentarios colados na palavra
    char vetor_aux[][SIZE_NAME]= {"rotulo","instrucao","operando 1","operando 2"};
    char aux_instru[SIZE_MAX];
    strcpy(aux_instru, instrucao);
    pt = strtok(aux_instru, ":");
    while(pt){ //verifica se tem rótulos na mesma linha
        if (pt != NULL){
            i++;
         }
         pt = strtok(NULL, ":");
    }
    if(i == 1){ //não tem rótulo
        contador = 1; //primeiro espaço do vetor é para rótulos 
        char aux_case1[SIZE_MAX];
        strcpy(aux_case1,instrucao);
        aux = strtok(aux_case1, " ");
        if(aux[0] == ';'){
            contador = 5;
        }
        opcode = instruction(aux);
        char chi = aux[strlen(aux) - 1];
        if(chi == ':'){
            contador = 0;
            aux[strlen(aux)-1] = '\0';
        }
        aux2 = comentario(aux);
        if(aux2){
            if(strcmp(aux2, nome_macro) == 0){
                usa_macro = 1;
                contador=0;
            }
            strcpy(vetor_aux[contador], aux2);
        } 
        if(comentarios == 1){
            contador = 4; 
            comentarios = 0;
        }
        contador++;
        while(aux && contador < 4){
            if(opcode == 9){
                aux = strtok(NULL, " ,");
            }
            else aux = strtok(NULL, " ");
            if (aux != NULL){
                if(aux[0] != ';'){
                    aux2 = comentario(aux);
                    strcpy(vetor_aux[contador], aux2);
                    if(comentarios == 1){
                        contador = 4; 
                        comentarios = 0;
                    }
                }
                else contador = 4;
            }
            contador++;
        }
    }
    else if(i == 2){ // tem um rótulo
        char aux_case2[SIZE_MAX];
        strcpy(aux_case2, instrucao);
        aux = strtok(aux_case2, ":");
        strcpy(vetor_aux[contador], aux);
        if(aux[0] == ';'){
            contador = 5;
        }
        contador++; 
        while(aux && contador < 4){
            opcode = instruction(aux);
            if(opcode == 9){
                aux = strtok(NULL, " ,");
            }
            else aux = strtok(NULL, " ");

            if (aux != NULL){
                if(aux[0] != ';'){
                    aux2 = comentario(aux);
                    strcpy(vetor_aux[contador], aux2);
                    if(comentarios == 1){
                        contador = 4; 
                        comentarios = 0;
                    }
                }
            }
            contador++;
        }
    }
    else if(i >= 3){ // tem mais de um rótulo 
        printf("Linha %d: Erro sintatico, mais de um rotulo definido.\n", contador_linha); 
        erro = 1;
    }
    

    opcode = instruction(vetor_aux[1]);  
    if (opcode == 0){ //não é instrucao
        opcode = diretiva(vetor_aux[1]);
    }
    int tem1 = erro_identificador(vetor_aux[0], contador_linha, opcode); // verifica erro lexico na palavra
    int tem4 = erro_identificador(vetor_aux[1], contador_linha, opcode);
    int tem2 = erro_identificador(vetor_aux[2], contador_linha,opcode);
    int tem3 = erro_identificador(vetor_aux[3], contador_linha,opcode);
    if(tem1 == 1||tem2 ==1||tem3 == 1||tem4 == 1){
        erro = 1; 
    }
    /*int j;
    for (j  = 0; j < 4; j++)
    {
        printf("vetor[%d] = %s ", j, vetor_aux[j]);
    }
    printf("\n");*/
    
    if(opcode == 17){ 
        insere_lista(&equ, contador_posicao, vetor_aux[0], opcode, vetor_aux[2], vetor_aux[3]);
        tem_equ = 1; 
        linha_equ = 1; 
    }
    
    if(tem_equ == 1 && opcode != 17 && opcode != 18){
        if(busca_equ(&equ, vetor_aux[2]) != NULL){
            usa_equ = 1; 
            strcpy(vetor_aux[2], busca_equ(&equ, vetor_aux[2]));
        }
        if(busca_equ(&equ, vetor_aux[3]) != NULL){
            usa_equ = 1; 
            strcpy(vetor_aux[3], busca_equ(&equ, vetor_aux[3]));
        }
    }

    int busca = busca_ultima(&lista_instrucoes, contador_linha, vetor_aux[0], opcode, vetor_aux[2], vetor_aux[3]);

    if((opcode < 1 || opcode > 20) && busca == 1){ //instrucao nao valida
        printf("Linha %d: Erro sintatico, instrucao nao reconhecida.\n");
        erro = 1;
    }

    //processo de if 
    if(opcode == 18){
        if(tem_equ == 0){
            erro_if = 1;
            printf("Linha %d: Erro semantico! IF utilizado sem a diretiva EQU\n", contador_linha);
        }
        else{
           if(busca_equ(&equ, vetor_aux[2])== NULL){
               erro_if = 1;
               printf("Linha %d: Erro semantico! IF utilizado sem a diretiva EQU\n", contador_linha);
           }else{
               usa_if = 2; 
               if(strcmp(busca_equ(&equ, vetor_aux[2]), "0") == 0){
                   usa_if = 3; 
               }
           }
        }
    }

    if(opcode != 18 && usa_if == 2){
        usa_if = 0;
    }

    //processo de macro
    if(opcode == 19){ 
        macro = 1; 
        tem_macro = 1;
        strcpy(nome_macro, vetor_aux[0]);
    }
    if(tem_macro == 1){
        if(opcode == 20){ //fim da macro
            tem_macro = 0;
        }
        else {
            insere_lista(&mdt, contador_linha, vetor_aux[0], opcode, vetor_aux[2], vetor_aux[3]);
            linhas_macro++;
            }
    }

    int name_macro = strcmp(nome_macro, vetor_aux[0]); 
    if(name_macro == 0 && opcode != 19){
        int pula_linhas = processa_macro(&mdt,&lista_instrucoes,contador_linha);
        contador_linha = pula_linhas;
    }

    if(opcode == 14){
        section_data = 1;
    }
    if((opcode == 15 || opcode == 16) && section_data == 0){ // caso dado nao final do codigo
        is_data = 1;
        insere_lista(&sec_data, contador_linha, vetor_aux[0], opcode, vetor_aux[2], vetor_aux[3]);
    }

    if(strcmp(vetor_aux[0], "rotulo") != 0 && busca == 0){ //acrescentar rótulo na lista
        if(verifica_rotulo(&tab_simbolos, vetor_aux[0]) == NULL && is_data == 0){
            insere_tabela(&tab_simbolos, contador_posicao, vetor_aux[0]);
        }
        else {
            if(opcode == 15 || opcode == 16){}
            else if(linhas_macro == 0 ){
            printf("Linha %d:  Erro sintatico. Rotulo/declaracao ja definido anteriormente!\n", contador_linha);
            erro = 1;}
        }
    } 
    if(busca == 0 && name_macro !=0 && opcode != 19 && opcode != 20 && tem_macro == 0 && contador < 6 && linha_equ == 0 && usa_if == 0 && is_data == 0){
        insere_lista(&lista_instrucoes, contador_linha, vetor_aux[0], opcode, vetor_aux[2], vetor_aux[3]);
        contador_linha++;
    } // esse if é para verificar se tem um rótulo flutante
    else {
        linha_equ = 0;  }
    
    if(name_macro !=0 && opcode != 19 && opcode != 20 && tem_macro == 0 && contador < 6 && linha_equ == 0 && usa_if == 0 && is_data == 0){
        incrementa(opcode);
    }else if(opcode != 18 && usa_if == 3){usa_if = 0;}


 
    return contador_linha;
}

int pre_processa_macro(char instrucao[SIZE_MAX], int contador_linha){
    int i = 0, contador = 0, opcode;
    char *pt; 
    char *aux, *aux2;
    char vetor_aux[][SIZE_NAME]= {"rotulo","instrucao","operando 1","operando 2"};
    char aux_instru[SIZE_MAX];
    strcpy(aux_instru, instrucao);
    pt = strtok(aux_instru, ":");
    while(pt){ //verifica se tem rótulos na mesma linha
        if (pt != NULL){
            i++;
         }
         pt = strtok(NULL, ":");
    }
    if(i == 1){ //não tem rótulo
        contador = 1; //primeiro espaço do vetor é para rótulos 
        char aux_case1[SIZE_MAX];
        strcpy(aux_case1,instrucao);
        aux = strtok(aux_case1, " ");
        if(aux[0] == ';'){
            contador = 5;
        }
        opcode = instruction(aux);
        char chi = aux[strlen(aux) - 1];
        if(chi == ':'){
            contador = 0;
            aux[strlen(aux)-1] = '\0';
        }
        aux2 = comentario(aux);
        if(aux2){
            if(strcmp(aux2, nome_macro) == 0){
                usa_macro = 1;
                contador=0;
            }
            strcpy(vetor_aux[contador], aux2);
        } 
        if(comentarios == 1){
            contador = 4; 
            comentarios = 0;
        }
        contador++;
        while(aux && contador < 4){
            if(opcode == 9){
                aux = strtok(NULL, " ,");
            }
            else aux = strtok(NULL, " ");
            if (aux != NULL){
                if(aux[0] != ';'){
                    aux2 = comentario(aux);
                    strcpy(vetor_aux[contador], aux2);
                    if(comentarios == 1){
                        contador = 4; 
                        comentarios = 0;
                    }
                }
                else contador = 4;
            }
            contador++;
        }
    }
    else if(i == 2){ // tem um rótulo
        char aux_case2[SIZE_MAX];
        strcpy(aux_case2, instrucao);
        aux = strtok(aux_case2, ":");
        strcpy(vetor_aux[contador], aux);
        if(aux[0] == ';'){
            contador = 5;
        }
        contador++; 
        while(aux && contador < 4){
            opcode = instruction(aux);
            if(opcode == 9){
                aux = strtok(NULL, " ,");
            }
            else aux = strtok(NULL, " ");

            if (aux != NULL){
                if(aux[0] != ';'){
                    aux2 = comentario(aux);
                    strcpy(vetor_aux[contador], aux2);
                    if(comentarios == 1){
                        contador = 4; 
                        comentarios = 0;
                    }
                }
            }
            contador++;
        }
    }
    else if(i >= 3){ // tem mais de um rótulo 
        printf("Linha %d: Erro sintatico, mais de um rotulo definido.\n", contador_linha); 
        erro = 1;
    }
    opcode = instruction(vetor_aux[1]);  
    if (opcode == 0){ //não é instrucao
        opcode = diretiva(vetor_aux[1]);
    }
    //processo de macro
    if(opcode == 19){ 
        macro = 1; 
        tem_macro = 1;
        strcpy(nome_macro, vetor_aux[0]);
    }
    if(tem_macro == 1){
        if(opcode == 20){ //fim da macro
            tem_macro = 0;
        }
        else {
            insere_lista(&mdt, contador_linha, vetor_aux[0], opcode, vetor_aux[2], vetor_aux[3]);
            linhas_macro++;
            }
    }
    int name_macro = strcmp(nome_macro, vetor_aux[0]); 
    if(name_macro == 0 && opcode != 19){
        int pula_linhas = processa_macro(&mdt,&lista_instrucoes,contador_linha);
        contador_linha = pula_linhas;
    }
    
    int busca = busca_ultima(&lista_instrucoes, contador_linha, vetor_aux[0], opcode, vetor_aux[2], vetor_aux[3]);

    if(busca == 0 && name_macro !=0 && opcode != 19 && opcode != 20 && tem_macro == 0 && contador < 6){
        insere_lista(&lista_instrucoes, contador_linha, vetor_aux[0], opcode, vetor_aux[2], vetor_aux[3]);
        contador_linha++;
    } 
    return contador_linha;
}

int pre_processa_equ(char instrucao[SIZE_MAX], int contador_linha){
    int i = 0, contador = 0, opcode;
    char *pt; 
    char *aux, *aux2;
    char vetor_aux[][SIZE_NAME]= {"rotulo","instrucao","operando 1","operando 2"};
    char aux_instru[SIZE_MAX];
    strcpy(aux_instru, instrucao);
    pt = strtok(aux_instru, ":");
    while(pt){ //verifica se tem rótulos na mesma linha
        if (pt != NULL){
            i++;
         }
         pt = strtok(NULL, ":");
    }
    if(i == 1){ //não tem rótulo
        contador = 1; //primeiro espaço do vetor é para rótulos 
        char aux_case1[SIZE_MAX];
        strcpy(aux_case1,instrucao);
        aux = strtok(aux_case1, " ");
        if(aux[0] == ';'){
            contador = 5;
        }
        opcode = instruction(aux);
        char chi = aux[strlen(aux) - 1];
        if(chi == ':'){
            contador = 0;
            aux[strlen(aux)-1] = '\0';
        }
        aux2 = comentario(aux);
        if(aux2){
            if(strcmp(aux2, nome_macro) == 0){
                usa_macro = 1;
                contador=0;
            }
            strcpy(vetor_aux[contador], aux2);
        } 
        //strcpy(vetor_aux[contador], aux2);
        if(comentarios == 1){
            contador = 4; 
            comentarios = 0;
        }
        contador++;
        while(aux && contador < 4){
            //erro = erro_identificador(aux, contador_linha);
            if(opcode == 9){
                aux = strtok(NULL, " ,");
            }
            else aux = strtok(NULL, " ");
            if (aux != NULL){
                if(aux[0] != ';'){
                    aux2 = comentario(aux);
                    strcpy(vetor_aux[contador], aux2);
                    if(comentarios == 1){
                        contador = 4; 
                        comentarios = 0;
                    }
                }
                else contador = 4;
            }
            contador++;
        }
    }
    else if(i == 2){ // tem um rótulo
        char aux_case2[SIZE_MAX];
        strcpy(aux_case2, instrucao);
        aux = strtok(aux_case2, ":");
        strcpy(vetor_aux[contador], aux);
        if(aux[0] == ';'){
            contador = 5;
        }
        contador++; 
        while(aux && contador < 4){
            opcode = instruction(aux);
            if(opcode == 9){
                aux = strtok(NULL, " ,");
            }
            else aux = strtok(NULL, " ");

            if (aux != NULL){
                if(aux[0] != ';'){
                    aux2 = comentario(aux);
                    strcpy(vetor_aux[contador], aux2);
                    if(comentarios == 1){
                        contador = 4; 
                        comentarios = 0;
                    }
                }
            }
            contador++;
        }
    }
    else if(i >= 3){ // tem mais de um rótulo 
        printf("Linha %d: Erro sintatico, mais de um rotulo definido.\n", contador_linha); 
        erro = 1;
    }

    opcode = instruction(vetor_aux[1]);  
    if (opcode == 0){ //não é instrucao
        opcode = diretiva(vetor_aux[1]);
    }
    if(opcode == 19){ 
        strcpy(nome_macro, vetor_aux[0]);
    }
    //processo de equ
    
    if(opcode == 17){ 
        insere_lista(&equ, contador_posicao, vetor_aux[0], opcode, vetor_aux[2], vetor_aux[3]);
        tem_equ = 1; 
        linha_equ = 1; 
    }
    
    if(tem_equ == 1 && opcode != 17 && opcode != 18){
        if(busca_equ(&equ, vetor_aux[2]) != NULL){
            usa_equ = 1; 
            strcpy(vetor_aux[2], busca_equ(&equ, vetor_aux[2]));
        }
        if(busca_equ(&equ, vetor_aux[3]) != NULL){
            usa_equ = 1; 
            strcpy(vetor_aux[3], busca_equ(&equ, vetor_aux[3]));
        }
    }


    if(opcode == 18){
        if(tem_equ == 0){
            erro_if = 1;
            printf("Linha %d: Erro semantico! IF utilizado sem a diretiva EQU\n", contador_linha);
        }
        else{
           if(busca_equ(&equ, vetor_aux[2])== NULL){
               erro_if = 1;
               printf("Linha %d: Erro semantico! IF utilizado sem a diretiva EQU\n", contador_linha);
           }else{
               //strcpy(vetor_aux[2],busca_equ(&equ, vetor_aux[2]));
               usa_if = 2; 
               if(strcmp(busca_equ(&equ, vetor_aux[2]), "0") == 0){
                   usa_if = 3; 
               }
           }
        }
    }

    if(opcode != 18 && usa_if == 2){
        usa_if = 0;
    }
    
    if(contador < 6 && linha_equ == 0 && usa_if == 0){
        insere_lista(&lista_instrucoes, contador_linha, vetor_aux[0], opcode, vetor_aux[2], vetor_aux[3]);
        contador_linha++;
    }else if(opcode != 18 && usa_if == 3){usa_if = 0;}
    else{
        linha_equ = 0;
    }
    //imprime_lista(&lista_instrucoes);
    return contador_linha;
}

void open_file(char *arquivo, char *tipo){
    FILE *file;
    int cont_line = 1;
    char instrucao[SIZE_MAX][SIZE_MAX]; 

    file = fopen(arquivo,"rt");
    if(file == NULL){
        printf("Erro ao abrir arquivo.\n"); //teste se conseguiu abrir
    }
    else{
        char *line = malloc(sizeof(arquivo));
        char *aux = malloc(SIZE_MAX);
        while(fscanf(file, "%[^\n] ", line) != EOF){
            strcpy(instrucao[cont_line -1], strupr(line)); //utilizando uper para resolver caso sensitivo 
            if(strcmp(tipo, "-m") == 0){
                cont_line = pre_processa_macro(instrucao[cont_line-1], cont_line);
            }
            else if (strcmp(tipo, "-o") == 0){
                cont_line = primeira_passagem(instrucao[cont_line-1], cont_line);
            }
            else if(strcmp(tipo, "-p") == 0){
                cont_line = pre_processa_equ(instrucao[cont_line-1], cont_line);
            }
            //cont_line = primeira_passagem(instrucao[cont_line-1], cont_line);
        }
        fclose(file);
    }
}

void segunda_passagem(list* lista_inst, lista *tab_simb, char *nome){
    FILE *arquivo;
    instrution *aux;
    table_simb *tab, *tab2;
    int aux_erro;
    int instrucao; 
    aux = lista_inst->comeco;
    arquivo = fopen(nome, "w");
    if(arquivo){
        while(aux){
            instrucao = aux->mnemonico; 
            aux_erro = verifica_qtd(instrucao, aux->operando_1, aux->operando_2, aux->line);
            if(aux_erro == 1){
                erro = 1;
            }
            if(erro == 1){
                break;
            }
            if(instrucao == 0){
                erro = 1;
                printf("Linha %d: Erro sintatico, instrucao nao reconhecida\n", aux->line);
                break;
            }
            if(instrucao == 1 || instrucao == 2 || instrucao == 3 || instrucao == 4 || instrucao == 5 || instrucao == 6 || instrucao == 7 || instrucao == 8 || instrucao == 10 || instrucao == 11 ||instrucao == 12 || instrucao == 13){
                tab = verifica_rotulo(&tab_simbolos, aux->operando_1);
                if(tab){
                    fprintf(arquivo, "%d %d ", instrucao, tab->lc);
                } else {printf("Linha %d: Erro semantico, rotulo/declaracao nao declarado!\n", aux->line);
                        erro = 1;
                        }
            }
            else if(instrucao == 9){
                tab = verifica_rotulo(&tab_simbolos, aux->operando_1);
                tab2 = verifica_rotulo(&tab_simbolos, aux->operando_2);
                if(tab && tab2){
                    fprintf(arquivo, "%d %d %d ", instrucao, tab->lc, tab2->lc);
                } else{ printf("Linha %d: Erro semantico, rotulo/declaracao nao declarado!\n", aux->line);
                        erro = 1;
                        }
            }
            else if(instrucao == 15){
                fprintf(arquivo, "0 ");
            }
            else if(instrucao == 14){
                fprintf(arquivo, "%d ", instrucao);
            }
            else if(instrucao == 16){
                fprintf(arquivo, "%s ", aux->operando_1);
            }
            aux=aux->proximo; 
        }
        if(erro == 0){
            printf("Codigo objeto gerado com sucesso!\n");
        }
    }
}

void escreve_m(list *lista, char *nome, char *tipo){
    FILE *arquivo;
    char *ins, *rotulo;
    instrution *aux;
    aux = lista->comeco;
    arquivo = fopen(nome, "w");
    if(arquivo){
        while(aux){
            ins = aux_ins(aux->mnemonico); 
            if(strcmp("rotulo", aux->rot) == 0){
                strcpy(aux->rot, "");
            }else if(strcmp(ins, " ") == 0){
            }
            else strcat(aux->rot, ": ");
            if(strcmp(aux->operando_2, "operando 2") == 0){
                strcpy( aux->operando_2, ""); 
            }
            if(strcmp(aux->operando_1, "operando 1") == 0){
                strcpy( aux->operando_1, ""); 
            }
            fprintf(arquivo, "%s%s %s %s\n", aux->rot, ins, aux->operando_1, aux->operando_2);
            aux=aux->proximo;
        }
        printf("Pre-processamento feito com sucesso!\n");
    }
    else printf("Nao possivel abrir arquivo!\n");

    fclose(arquivo);
}

void roda(char *file1, char *file2, char *tipo){
    int cont_pos = 0;
    inicializa_tabela(&tab_simbolos);
    inicializa_lista(&lista_instrucoes);
    inicializa_lista(&mdt);
    open_file(file1, tipo);
    if(strcmp(tipo, "-o") == 0){
        data(&sec_data, &lista_instrucoes);
        if(erro == 1 || erro_if == 1){
            printf("Nao foi possivel realizar a montagem, seu programa contem erros\n");
        }
        else if(tem_macro == 1){
            printf("Erro semantico: Falta de ENDMACRO \n");
        }
        else if(tem_equ == 1 && usa_equ == 0){
            printf("Erro semantico: EQU definida porem nao utilizada!"); 
        } 
        else if(usa_macro == 0 && macro == 1){
            printf("Erro semantico: Macro nao utilizada!\n");
        }
        else segunda_passagem(&lista_instrucoes, &tab_simbolos, file2);
    }
    else if(strcmp(tipo, "-m") == 0){
            if(tem_macro == 1){
               printf("Erro semantico: Falta de ENDMACRO \n");
               printf("Nao possivel pre-processar arquivo com macros!\n");
            } else if (usa_macro == 0 && macro == 1){
            printf("Erro semantico: Macro nao utilizada!\n");
            printf("Nao possivel pre-processar arquivo com macros!\n");
            }
            else escreve_m(&lista_instrucoes, file2, tipo);
    }
    else if (strcmp(tipo, "-p") == 0){
        if(tem_equ == 1 && usa_equ == 0){
            printf("Erro semantico: EQU definida porem nao utilizada!"); 
        } 
        else if(erro_if == 1){
        }
        else escreve_m(&lista_instrucoes, file2, tipo);
    }
    //imprime_tabela(&tab_simbolos);
    //imprime_lista(&lista_instrucoes);
    //imprime_lista(&mdt);
}

