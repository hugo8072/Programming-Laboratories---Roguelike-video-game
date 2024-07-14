#ifndef ___ESTADO_H___
#define ___ESTADO_H___

/**
@file estado.h
Definição do estado e das funções que convertem estados em strings e vice-versa
*/

/** \brief O nº máximo de inimigos */
#define MAX_INIMIGOS		100

/** \brief O nº máximo de obstáculos */
#define MAX_OBSTACULOS		100


/**
\brief Estrutura que armazena uma posição
*/
typedef struct posicao {
	int x;
	int y;
} POSICAO;

/**
\brief Estrutura que armazena o estado do jogo
*/
typedef struct estado {
	/** A posição do jogador */
	POSICAO jog;
	/** O nº de inimigos */
	char num_inimigos;
	/** O nº de obstáculos */
	char num_obstaculos;
	/** O número de espadas que serão distribuidas pelo tabuleiro*/
	char num_knifes;
	/** O número de setas que serão distribuidas pelo tabuleiro*/
	char num_setasRandom;
	/** O número de saltos disponiveis*/
	char num_saltos;
	/** Array com a posição dos inimigos */
	POSICAO inimigo[MAX_INIMIGOS];
	/** Array com a posição dos obstáculos */
	POSICAO obstaculo[MAX_OBSTACULOS];
	/** Posição da porta para passar de nivel*/
	POSICAO saida;
	/** Array com a posição das vidas*/
	POSICAO bonus[10];
	/** Array com a posição das espadas */
	POSICAO knifes[10];
	/** Array com a posição das setas*/
	POSICAO setasRandom[10];
	/** Array com a posição do baú mágica*/
	POSICAO mudaEstado[10];
	/** O número de espadas que temos em determinado momento*/
	int my_knifes;
	/**  O número de vidas que temos em determinado momento*/
	char vidajog;
	/**  Guarda o score que temos em determinado momento*/
	char score;
	/** O número de vidas que serão distribuidas pelo tabuleiro*/
	char num_bonus;
	/** Guarda o nivel em que estamos*/
	char nivel;
	/** String que guarda o nome do utilizador*/
	char user[1024];
	/** O número de caixas mágicas que serão distribuidas pelo tabuleiro*/
	char num_mudaEstado;
	/** O número de lanças que serão distribuidas pelo tabuleiro*/
	char lanca;
	/** Matriz que guarda os campos que estão ou não visiveis no tabuleiro*/
	int M[23][23];
	/** Guarda e determina em que nivel será usada cada função*/
	char flag;
	/** Usada para criar exceção do nivel 3 na função "movimento_permitido*/
	char flag2;

} ESTADO;

/**
\brief Função que converte um estado numa string
@param e O estado
@returns A string correspondente ao estado e
*/
char *estado2str(ESTADO e);

/**
\brief Função que converte uma string num estado 
@param argumentos Uma string contendo os argumentos passados à CGI
@returns O estado correspondente à string dos argumentos
*/
ESTADO str2estado(char *argumentos);

#endif
