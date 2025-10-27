// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "pessoa.h"


// ===============================================================================
// UTILIDADES PARA AS FUNCIONALIDADES - ARQUIVO PESSOA
// ===============================================================================

/*
//Implementação da abrirPessoa: Abertura de Arquivo de Pessoa
//Abre o arquivo pessoa de acordo com o modo de abertura desejado.
@param arquivoSaidaBin: Nome do arquivo de pessoa a ser criado.
@param modo: Tipo de abertura do arquivo a ser usado.
@return: Endereço de abertura do arquivo.
*/
FILE *abrirPessoa(char *arquivoSaidaBin, char *modo) {
    FILE *pessoa_bin_file = fopen(arquivoSaidaBin, modo);
    if (pessoa_bin_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }

    return pessoa_bin_file;
}


/*
Função complementar para printar registros na tela no formato exigido.
@param pessoa_record: Estrutura PessoaRecord contendo os dados do registro a ser impresso.
*/
void printNaTela(PessoaRecord pessoa_record) {
		printf("Dados da pessoa de codigo %d\n", pessoa_record.idPessoa);
		
		// Imprime nomePessoa ou '-' se for nulo
		if (pessoa_record.tamanhoNomePessoa > 0) {
				printf("Nome: %s\n", pessoa_record.nomePessoa);
		} else {
				printf("Nome: -\n");
		}
		
		// Imprime idadePessoa ou '-' se for -1
		if (pessoa_record.idadePessoa != -1) {
				printf("Idade: %d\n", pessoa_record.idadePessoa);
		} else {
				printf("Idade: -\n");
		}
		
		// Imprime nomeUsuario ou '-' se for nulo
		if (pessoa_record.tamanhoNomeUsuario > 0) {
				printf("Usuario: %s\n", pessoa_record.nomeUsuario);
		} else {
				printf("Usuario: -\n");
		}
}


/*
//Implementação de initCabecPessoa: Inicializar o Cabeçalho do Pessoa
//Atualiza os valores iniciais para o arquivo de pessoas
@param pessoa_header: A estrutura do cabeçalho do arquivo do pessoa.
*/
void initCabecPessoa(FILE *pessoa_bin_file, PessoaHeader *pessoa_header) {
    // Variáveis de inicialização
    pessoa_header->status = '0'; 
    pessoa_header->quantidadePessoas = 0;
    pessoa_header->quantidadeRemovidos = 0;
    pessoa_header->proxByteOffset = PESSOA_HEADER_SIZE; 

    // Escreve cabeçalho inicial no arquivo de dados
    fwrite(&pessoa_header->status, sizeof(char), 1, pessoa_bin_file);
    fwrite(&pessoa_header->quantidadePessoas, sizeof(int), 1, pessoa_bin_file);
    fwrite(&pessoa_header->quantidadeRemovidos, sizeof(int), 1, pessoa_bin_file);
    fwrite(&pessoa_header->proxByteOffset, sizeof(long int), 1, pessoa_bin_file);
}


/*
Implementação de atualizaCabecPessoa: Atualiza o Cabeçalho do Pessoa
Atualiza o cabeçalho do arquivo de pessoa.
@param pessoa_bin_file: O endereço do arquivo do pessoa.
@param pessoa_header: A estrutura do cabeçalho do arquivo do pessoa.
*/
void atualizaCabecPessoa(FILE *pessoa_bin_file, PessoaHeader *pessoa_header) {
    fseek(pessoa_bin_file, 0, SEEK_END);
    pessoa_header->proxByteOffset = ftell(pessoa_bin_file); // Tamanho total do arquivo
    fseek(pessoa_bin_file, 0, SEEK_SET);
    pessoa_header->status = '1'; // Consistente
    fwrite(&pessoa_header->status, sizeof(char), 1, pessoa_bin_file);
    fwrite(&pessoa_header->quantidadePessoas, sizeof(int), 1, pessoa_bin_file);
    fwrite(&pessoa_header->quantidadeRemovidos, sizeof(int), 1, pessoa_bin_file);
    fwrite(&pessoa_header->proxByteOffset, sizeof(long int), 1, pessoa_bin_file);
}


/*
Implementação de verificaStatusPessoa: Verifica o Status de Consistência do Arquivo
Verifica o status do arquivo de pessoa.
@param pessoa_bin_file: O endereço do arquivo do pessoa.
@param pessoa_header: A estrutura do cabeçalho do arquivo do pessoa.
@return: Booleano da consistência do arquivo pessoa.
*/
bool verificaStatusPessoa(FILE *pessoa_bin_file, PessoaHeader *pessoa_header) {
    fseek(pessoa_bin_file, 0, SEEK_SET);
    fread(&pessoa_header->status, sizeof(char), 1, pessoa_bin_file);
    if (pessoa_header->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(pessoa_bin_file);
        return 0;
    }

    return 1;
}

/*
Implementação de buscaPessoa: Busca Generalizada Para Enocntrar um Registro Específico
Busca um registro de pessoa de acordo com o campo e o valor definido.
@param pessoa_bin_file: O endereço do arquivo do pessoa.
@param campo: O campo definido para busca.
@param valor_final: O valor definido para busca.
@return: Lista com os registros
*/
Lista *buscaPessoa(FILE *pessoa_bin_file, char *campo, char *valor_final) {
    // Lista para os possíveis resultados
	Lista *lista = criarLista();
	if (lista == NULL) return NULL;
	
	// Sem campo e valor_final -> todos
	if (campo == NULL && valor_final == NULL) {
		// Ler do arquivo de dados e salvar na lista
		while (1) {
			PessoaRecord pessoa_record = lerRegistroPessoa(pessoa_bin_file);
			if (pessoa_record == NULL) {
				// Acabou
				break;
			}
			
			addLista(lista, pessoa_record);
		}
		
		return lista;
	}
	
 	if(strcmp(campo, "idPessoa") == 0) {
        // Busca no indice
        long int offset = setProcuradoOffset(index, atoi(valor_final));

        if (offset != -1) {
            // Achar
            fseek(pessoa_bin_file, 0, SEEK_SET);
            fseek(pessoa_bin_file, offset, SEEK_SET);

            // Colocar na lista
            PessoaRecord pessoa_record = lerRegistroPessoa(pessoa_bin_file);					
			addLista(lista, pessoa_record);				
        } 
    } else {
        while(1){ //ou seja, enquanto for possivel ler os registros de dados
        	PessoaRecord pessoa_record = lerRegistroPessoa(pessoa_bin_file);
			if (pessoa_record == NULL) {
				// Acabou
				break;
			}
			
	        if(pessoa_record->status == NAO_REMOVIDO_CHAR && filtroCampoPessoa(pessoa_record, campo, valor_final)){ // se os parametros foram encontrados no registro, printar o dado					
				addLista(lista, pessoa_record);	
         	}
        }
    }

	return lista;
}


/*
Implementação de lerRegistroPessoa: Lê um Registro de Pessoa do Arquivo
Lê um registro de pessoa do arquivo binário.
@param: Ponteiro do arquivo de dados.
@return: Endereço do registro de dados lido.
*/
PessoaRecord lerRegistroPessoa(FILE *pessoa_bin_file) {
    atual_byte_offset = ftell(pessoa_bin_file);

    PessoaRecord pessoa_record;
    pessoa_record.nomePessoa = NULL;
    pessoa_record.nomeUsuario = NULL;
    
    // Tenta ler o campo 'removido'
    if(fread(&pessoa_record.removido, sizeof(char), 1, pessoa_bin_file) != 1) break;

    if (pessoa_record.removido == NAO_REMOVIDO_CHAR ) {
        // Registro não removido: lê e processa
        fread(&pessoa_record.tamanhoRegistro, sizeof(int), 1, pessoa_bin_file);
        fread(&pessoa_record.idPessoa, sizeof(int), 1, pessoa_bin_file);
        fread(&pessoa_record.idadePessoa, sizeof(int), 1, pessoa_bin_file);
        
        // Leitura de nomePessoa (tamanho + dado)
        fread(&pessoa_record.tamanhoNomePessoa, sizeof(int), 1, pessoa_bin_file);
        if (pessoa_record.tamanhoNomePessoa > 0) {
            pessoa_record.nomePessoa = (char *) malloc(pessoa_record.tamanhoNomePessoa + 1);
            fread(pessoa_record.nomePessoa, sizeof(char), pessoa_record.tamanhoNomePessoa, pessoa_bin_file);
            pessoa_record.nomePessoa[pessoa_record.tamanhoNomePessoa] = '\0';
        }

        // Leitura de nomeUsuario (tamanho + dado)
        fread(&pessoa_record.tamanhoNomeUsuario, sizeof(int), 1, pessoa_bin_file);
        if (pessoa_record.tamanhoNomeUsuario > 0) {
            pessoa_record.nomeUsuario = (char *) malloc(pessoa_record.tamanhoNomeUsuario + 1);
            fread(pessoa_record.nomeUsuario, sizeof(char), pessoa_record.tamanhoNomeUsuario, pessoa_bin_file);
            pessoa_record.nomeUsuario[pessoa_record.tamanhoNomeUsuario] = '\0';
        }
    } else {
        // Registro removido, pula
        int tamanhoRegistro;
        fread(&tamanhoRegistro, sizeof(int), 1, pessoa_bin_file);
        // Avança o ponteiro de arquivo
        fseek(pessoa_bin_file, atual_byte_offset + tamanhoRegistro + sizeof(char) + sizeof(int), SEEK_SET);
    }

    return pessoa_record;
}


/*
Implementação de setProcuradoOffset: Define o Offset Atual no Arquivo de Índice
Define o offset atual no arquivo de índice em memória para um ID específico.
@param indice_em_memoria: Registros de índice em memória.
@param idProcurado: Valor inteiro do id da pessoa procurada.
@return: Valor do byte offset referente ao id da pessoa.
*/
long int setProcuradoOffset(ARV *indice_em_memoria, int idProcurado) {
    int idProcurado = atoi(valor_final);
    NO* noEncontrado = buscarNo(arvoreIndice, idProcurado);

    return noEncontrado->byteOffset;
}

/*
Implementação de filtroCampoPessoa: Filtra um Registro de Pessoa com Base em um Campo e Valor
Filtra um registro de pessoa com base em um campo específico e um valor fornecido.
@param pessoa_record:
@param campo:
@param valor_final:
@return:
*/
int filtroCampoPessoa(PessoaRecord pessoa_record, char *campo, char *valor_final) {
    // Compara o campo
    int match = 0;
    if (strcmp(campo, "idadePessoa") == 0) {
        int idadeProcurada = atoi(valor_final);
        if (record.idadePessoa != -1 && record.idadePessoa == idadeProcurada) {
            match = 1;
        }
    } else if (strcmp(campo, "nomePessoa") == 0) {
        if (record.nomePessoa != NULL && strcmp(record.nomePessoa, valor_final) == 0) {
            match = 1;
        }
    } else if (strcmp(campo, "nomeUsuario") == 0) {
        if (record.nomeUsuario != NULL && strcmp(record.nomeUsuario, valor_final) == 0) {
            match = 1;
        }
    }

    return match;
}

