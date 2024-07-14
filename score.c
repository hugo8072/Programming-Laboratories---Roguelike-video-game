/** 
@file score.c
*/

#define RANGE		10240

void melhor_score(ESTADO e){ /** escreve e atualiza o ficheiro que grava o score */

	char linha[RANGE];
	FILE *saida;
	FILE *highscore;
	if((highscore = fopen("/var/www/html/files/highscore", "r")) == NULL) {  /** lê o ficheiro */
		perror("nao consegui abrir o ficheiro para leitura");
		exit(1);
	} 
	if((saida = fopen("/var/www/html/files/saida", "w")) == NULL) { /** guarda o ficheiro nesta localização */
		perror("nao consegui abrir o ficheiro para escrita");
		exit(1);
	} 
	
	while(fgets(linha,RANGE,highscore)!=NULL){
		char nome[RANGE];
		int score;

		sscanf(linha, "%s %d", nome, &score); /** lê o ficheiro score e coloca o nome e o score respetivo na string linha*/
		if (score>e.score){	/** caso o score que estava no ficheiro seja maior do que o que está guardado na estrutura ESTADO ele não grava no ficheiro. Contrariamente, se o score da estrutura for maior do que o do ficheiro ele irá subescrevre o mesmo guardando sempre o maior score*/
			fprintf(saida, "%s", linha);
		}else{
			fprintf(saida, "%s %d\n", e.user, e.score);
			fprintf(saida, "%s", linha);
			e.score=-1;
		}
	}
	fclose(highscore);/** substitui o ficheiro highscore pelo ficheiro saida para assim ficar apenas guardado o ficheiro atualizado */
	fclose(saida);
	rename("/var/www/html/files/saida", "/var/www/html/files/highscore");
}