/** 
@file roguelike.c
*/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cgi.h"
#include "estado.h"
#include "movimentos_inimigo.c"
#include "score.c"
#include <math.h>


int proximo(int x1, int y1, int x2, int y2){/*vê se algo está proximo do jogadore (1 campo)*/

	if (((x1==x2) && (y1==y2-1 || y1 ==y2+1)) || ((y1==y2) && (x1==x2-1 || x1 ==x2+1)) || (((y1==y2-1)|| (y1==y2+1)) && (x1==x2-1 || x1 ==x2+1)))
		return 1;
	else 
		return 0;
}      
 
int proximo2(int x1, int y1, int x2, int y2){/*vê se algo está proximo do jogadore (2 campo)*/

	if (((x1==x2) && (y1==y2-2 || y1 ==y2+2)) || ((y1==y2) && (x1==x2-2 || x1 ==x2+2)))
		return 1;
	else 
		return 0;
} 
int invariante2(int x, int y, ESTADO e)/* Função que indica se a posição do tabuleiro já esta ocupada, retornando 1 caso esteja ocupada e 0 se estiver livre */ 
	{

 	int i=0;

 	while(i < e.num_knifes && (x != e.knifes[i].x || y != e.knifes[i].y) ) {i++;}
 		if(i < e.num_knifes)
 	{return 1;}
 
 
 	i=0;
 	while(i < e.num_bonus && (x != e.bonus[i].x || y != e.bonus[i].y) ) {i++;}
 	if(i < e.num_bonus)
 	{return 1;}

 	i=0;
 	while(i < e.num_mudaEstado && (x != e.mudaEstado[i].x || y != e.mudaEstado[i].y) ) {i++;}
 	if(i < e.num_mudaEstado)
 	{return 1;}

 	return 0;
	}

int tem_obstaculo(ESTADO e, int x,int y){ /** Testa se se tem um obstaculo na posição (x,y)*/
	int i;
	for(i=0;i<e.num_obstaculos;i++)
		if((x == e.obstaculo[i].x) && (y == e.obstaculo[i].y))
			return 1;
	return 0;	
} 

int tem_inimigo(ESTADO e, int x,int y){ /** Testa se se tem um inimigo na posição (x,y)*/
	int i;
	for(i=0;i<e.num_inimigos;i++)
		if((x == e.inimigo[i].x) && (y == e.inimigo[i].y))
			return 1;
	return 0;	
} 
    
void imprime_casa(ESTADO e, int x, int y) { /** imprime tabuleiro */
	if(e.flag==1){ /** tabuleiro com texturas do nivel 1*/
		if ((x+y)%2 == 0)
			IMAGEM(x, y, ESCALA, "sand.jpg");
		else
			IMAGEM(x, y, ESCALA, "relva.jpg");
	}
	else /** tabuleiro com textura dos restantes niveis */
	{
		if(e.M[x][y]==1){
			if ((x+y)%2 == 0)
				IMAGEM(x, y, ESCALA, "texture1.jpg");
			else
			IMAGEM(x, y, ESCALA, "texture2.jpg");
		}
		else 
			IMAGEM(x, y, ESCALA, "texture3.jpg");
	}
}


ESTADO inicializar() { /** inicializa as variaveis da estrutura estado no incio do jogo */
	

	ESTADO e;
	memset(&e, 0, sizeof(ESTADO)); 
	e.flag=0;
	e.flag2 = 0;

return e; 
}
  
ESTADO nivel1(ESTADO e) {  /** inicializa as variaveis da estrutura estado no nivel 1 */
	 
	 int i, w, z;

	e.nivel = 1;
	e.score = 0; 
	e.saida.x = 16;
	e.saida.y = 3;
	e.num_knifes = 0;
	e.my_knifes = 3;
	e.num_bonus = 1;
	e.jog.x = 18; 
	e.vidajog = 5;
	e.jog.y = 12;
	e.num_inimigos=INIMIGOSNIVEL1;
	e.num_obstaculos=OBSTACULOSNIVEL1;
	e.flag=1;

									
    srand(time(NULL));

for(i=0;i<23;i++){       /** coloca todos os campos da matriz a 1 (visiveis) */
	for(w=0;w<23;w++)
		e.M[i][w]=1;
}


for(i=0; i< e.num_inimigos; i++)	/** coloca posiçoes aleatórias não repetidas no array dos inimigos */
{

	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-4);
	while(invariante(w,z,e)==1)
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-4); 
	}
	e.inimigo[i].x = w; e.inimigo[i].y = z;
	
}


for(i=3; i< e.num_obstaculos; i++) /** coloca posiçoes aleatórias não repetidas no array dos inimigos, algumas fixas devido ao tabuleiro ser meio costumizado (montanha e mar) */ 
{	

	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);

	while(invariante(w,z,e)==1 || (w == 16 && z == 4) || (w==16 && z == 5))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.obstaculo[i].x = w; e.obstaculo[i].y = z;
	e.obstaculo[0].x = 17;e.obstaculo[0].y = 4;    
	e.obstaculo[1].x = 18;e.obstaculo[1].y = 4;
	e.obstaculo[2].x = 13;e.obstaculo[2].y = 4;


}

for(i=0; i< e.num_bonus; i++) /** coloca posiçoes aleatórias não repetidas no array das vidas */ 
{ 
	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.bonus[i].x = w;
	e.bonus[i].y =z;
}

for(i=0; i< e.num_knifes; i++)  /** coloca posiçoes aleatórias não repetidas no array das espadas */ 
{

	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e) )
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1); 
	}
	e.knifes[i].x = w; e.knifes[i].y = z; 
	
 
}

return e; 
}

ESTADO novo_nivel2e3(ESTADO e) { /** inicializa as variaveis da estrutura estado no nivel 2 e do nivel 3 */
	
	int i, w, z;

	e.nivel += 1; 
	e.score += 10;
	e.num_knifes = 8;
	e.my_knifes = 3; 
	e.num_bonus = 3;   
	e.jog.x = 17;
	e.jog.y = 12; 
	e.num_inimigos=INIMIGOSNIVEL2e3; 
	e.num_obstaculos=OBSTACULOS;
	e.lanca=1;
	e.num_saltos= 1; 
	e.num_setasRandom=1;
	e.num_mudaEstado=1;
	e.flag += 1;
    srand(time(NULL));


for(i=0;i<23;i++){  /** coloca todos os campos da matriz a 1 (visiveis) */
	for(w=0;w<23;w++)
		e.M[i][w]=1;
}

for(i=1; i< e.num_inimigos; i++) /** coloca posiçoes aleatórias não repetidas no array dos inimigos */ 
{

	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-4);
	while(invariante(w,z,e)==1)
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-4); 
	}
	e.inimigo[i].x = w; e.inimigo[i].y = z;

}	
   
for(i=0; i< e.num_obstaculos; i++) /** coloca posiçoes aleatórias não repetidas no array dos obstáculos */ 
{	
	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1)
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.obstaculo[i].x = w; e.obstaculo[i].y = z; 

}

for(i=0; i< e.num_bonus; i++)  /** coloca posiçoes aleatórias não repetidas no array das vidas */ 
{
	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.bonus[i].x = w;
	e.bonus[i].y =z;
}


for(i=0; i< e.num_knifes; i++) /** coloca posiçoes aleatórias não repetidas no array das espadas */ 
{

	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.knifes[i].x = w; e.knifes[i].y = z;

}

for(i=0; i< e.num_setasRandom; i++) /** coloca posiçoes aleatórias não repetidas no array das setas */ 
{

	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.setasRandom[i].x = w; 
	e.setasRandom[i].y = z;
	
}

for(i=0; i< e.num_mudaEstado; i++) /** coloca posiçoes aleatórias não repetidas no array das caixas mágicas */ 
{ 
	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.mudaEstado[i].x = w;
	e.mudaEstado[i].y =z;
}

return e; 
}

ESTADO novo_nivel4(ESTADO e) { /** inicializa as variaveis da estrutura estado no nivel 4 */
	
	int i, w, z;

	e.nivel += 1; 
	e.score += 10;
	e.num_knifes = 8;
	e.my_knifes = 3; 
	e.num_bonus = 3;    
	e.jog.x = 17;
	e.jog.y = 13; 
	e.num_inimigos=INIMIGOSNIVEL4; 
	e.num_obstaculos=OBSTACULOS;
	e.lanca=1;
	e.num_setasRandom=1;
	e.num_saltos= 1;
	e.num_mudaEstado=2;
	e.flag = 4 ;
	e.flag2 = 0;
    srand(time(NULL));


for(i=0;i<23;i++){    /** coloca todos os campos da matriz a 0 (invisiveis) */
	for(w=0;w<23;w++)
		e.M[i][w]=0;
}

for(i=0; i< e.num_inimigos; i++) /** coloca posiçoes aleatórias não repetidas no array dos inimigos */ 
{

	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-4);
	while(invariante(w,z,e)==1)
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-4); 
	}
	e.inimigo[i].x = w; e.inimigo[i].y = z;

}	
  
for(i=0; i< e.num_obstaculos; i++) /** coloca posiçoes aleatórias não repetidas no array dos objetos */ 
{	
	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || (w == 17 && z == 12))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.obstaculo[i].x = w; e.obstaculo[i].y = z; 

}

for(i=0; i< e.num_bonus; i++) /** coloca posiçoes aleatórias não repetidas no array das vidas */ 
{
	w = RandRange(Liminfx,Limsupx-1); 
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.bonus[i].x = w;
	e.bonus[i].y =z;
}


for(i=0; i< e.num_knifes; i++) /** coloca posiçoes aleatórias não repetidas no array das espadas */ 
{

	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.knifes[i].x = w; e.knifes[i].y = z;
	
 
}

for(i=0; i< e.num_setasRandom; i++) /** coloca posiçoes aleatórias não repetidas no array das setas */ 
{

	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.setasRandom[i].x = w; 
	e.setasRandom[i].y = z;
	
}

for(i=0; i< e.num_mudaEstado; i++) /** coloca posiçoes aleatórias não repetidas no array das caixas mágicas */ 
{ 
	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.mudaEstado[i].x = w;
	e.mudaEstado[i].y =z;
}

return e; 
}

ESTADO novo_nivel5(ESTADO e) { /** inicializa as variaveis da estrutura estado no nivel 4 */
	
	int i, w, z;

	e.nivel += 1; 
	e.score += 10;
	e.num_knifes = 8;
	e.my_knifes = 3; 
	e.num_bonus = 3;    
	e.jog.x = 17;
	e.jog.y = 12; 
	if(e.num_inimigos<8)
	e.num_inimigos=e.num_inimigos+1; 
	e.num_setasRandom=2;
	e.num_obstaculos=OBSTACULOS;
	e.lanca=1;
	e.num_saltos= 1; 
	e.num_mudaEstado=3;
	e.flag = 5;
    srand(time(NULL));


for(i=0;i<23;i++){   /** coloca todos os campos da matriz a 0 (invisiveis) */
	for(w=0;w<23;w++)
		e.M[i][w]=0;
}
	{
		e.M[16][12]=1;  /** coloca os 6 quadrados visiveis (ponto de inicio de nivel)  */
		e.M[18][12]=1;
		e.M[16][11]=1;
		e.M[18][11]=1;
		e.M[17][11]=1;
		e.M[17][12]=1;
	}

for(i=0; i< e.num_inimigos; i++)  /** coloca posiçoes aleatórias não repetidas no array das caixas mágicas */
{

	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-6);
	while(invariante(w,z,e)==1)
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-6); 
	}
	e.inimigo[i].x = w; e.inimigo[i].y = z;

}

if (e.num_inimigos==7) /** Zona de spawn de Dragões perto da porta de passagem de nivel */
	{
	
	w = RandRange(Liminfx,Limsupx-1);
	z =RandRange(3,4);
	while(invariante(w,z,e)==1)
	{
		w = RandRange(Liminfx,Limsupx-1);
		z =RandRange(3,4);
	}
		e.inimigo[6].x=w;
		e.inimigo[6].y=z;
	}

if (e.num_inimigos==8)
{
	w = RandRange(Liminfx,Limsupx-1);
	z =RandRange(3,4);
	while(invariante(w,z,e)==1)
	{
		w = RandRange(Liminfx,Limsupx-1);
		z =RandRange(3,4);
	}
	e.inimigo[7].x=w;
	e.inimigo[7].y=z;

	w = RandRange(Liminfx,Limsupx-1);
	z =RandRange(3,4);
	while(invariante(w,z,e)==1)
	{
		w = RandRange(Liminfx,Limsupx-1);
		z =RandRange(3,4);
	}
	e.inimigo[6].x=w;
	e.inimigo[6].y=z;


}
  
for(i=0; i< e.num_obstaculos; i++) /** coloca posiçoes aleatórias não repetidas no array dos obstáculos */
{	
	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1)
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.obstaculo[i].x = w; e.obstaculo[i].y = z; 

}

for(i=0; i< e.num_bonus; i++) /** coloca posiçoes aleatórias não repetidas no array das vidas */
{
	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.bonus[i].x = w;
	e.bonus[i].y =z;
}


for(i=0; i< e.num_knifes; i++) /** coloca posiçoes aleatórias não repetidas no array das espadas */
{

	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.knifes[i].x = w; e.knifes[i].y = z;
	
  
}

for(i=0; i< e.num_setasRandom; i++) /** coloca posiçoes aleatórias não repetidas no array das setas */
{

	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.setasRandom[i].x = w; 
	e.setasRandom[i].y = z;
	
}

for(i=0; i< e.num_mudaEstado; i++) /** coloca posiçoes aleatórias não repetidas no array das caixas mágicas */
{ 
	w = RandRange(Liminfx,Limsupx-1);
	z = RandRange(Liminfy,Limsupy-1);
	while(invariante(w,z,e)==1 || invariante2 (w,z,e))
	{
		w = RandRange(Liminfx,Limsupx-1);
		z = RandRange(Liminfy,Limsupy-1);
	}
	e.mudaEstado[i].x = w;
	e.mudaEstado[i].y =z;
}

return e; 
}

void imprime_saida (ESTADO e){    /** Imprime imagem da porta de mudança de nivel */
	if(e.M[e.saida.x][e.saida.y]==1)
	IMAGEM(e.saida.x, e.saida.y, ESCALA, "trapdoor1.png");
}

void imprime_movimento(ESTADO e, int dx, int dy) { /** Move o jogador*/
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	char link[MAX_BUFFER];


	if(!posicao_valida(x, y))
		return;

	if(e.flag==1){
		if(x == e.saida.x && y == e.saida.y)  /** abre o link para a saida do nivel 1*/
		{	
			sprintf(link, "http://localhost/cgi-bin/roguelike?%s,%d,%d", e.user, x, y); 
			ABRIR_LINK(link);
			FECHAR_LINK;
		}
		else
			if(invariante(x,y,e) == 1)
				return;
	}

	else
	{

	if(x == e.saida.x && y == e.saida.y)  /** abre o link para a porta de saida dos restantes niveis*/
	{	
		sprintf(link, "http://localhost/cgi-bin/roguelike?%s,%d,%d", e.user, x, y); 
		ABRIR_LINK(link);
		imprime_saida(e);
		FECHAR_LINK;}
		else
		{
			if(invariante(x,y,e) == 1)
				return;
		}

	}

	sprintf(link,"http://localhost/cgi-bin/roguelike?%s,%d,%d",e.user,x,y);
	ABRIR_LINK(link);
	imprime_jogavel(x ,y);
	FECHAR_LINK;
}



void imprime_inimigos(ESTADO e) { /** imprime imagem de todos os inimigos visiveis */
int i ;
for (i=0; i<ceifeiro;i++) 
if(e.M[e.inimigo[i].x][e.inimigo[i].y]==1)
IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "faqueiro.gif");

for (i=ceifeiro; i<escudeiro+ceifeiro;i++) 
if(e.M[e.inimigo[i].x][e.inimigo[i].y]==1)
IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "escudas.gif");
  
for (i=escudeiro+ceifeiro; i<wizard+escudeiro+ceifeiro;i++)
if(e.M[e.inimigo[i].x][e.inimigo[i].y]==1)
IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "magico.gif");
 
 for (i=wizard+escudeiro+ceifeiro; i<dragon+wizard+escudeiro+ceifeiro;i++)
if(e.M[e.inimigo[i].x][e.inimigo[i].y]==1)
IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "dragonn.gif"); 

}

int matou_inimigo(ESTADO e, int x, int y){ /** verifica se matamos algum inimigo */
 	int i;
 	for (i=0;i<e.num_inimigos;i++)
 		if(x == e.inimigo[i].x && y == e.inimigo[i].y)
 			return 1;

 	return 0;
 }


ESTADO mata_inimigos (ESTADO e,int i){  /**  Mata um inimigo e aumenta o score consoante o tipo de inimigo */
			
			e.inimigo[i].x = 100;
			e.inimigo[i].y = 100;
			e.my_knifes--;		

	if (i >= (wizard+escudeiro+ceifeiro))
		e.score+= 5;

	else if (i >= (escudeiro+ceifeiro))
		e.score += 3;

	else if (i >= ceifeiro)
		e.score += 2;

	return e;

}  

void linka_inimigo (ESTADO e){  /** torna os inimigos clicáveis e indica quais deles podemos atacar */
	char link[MAX_BUFFER];
	int i;
	for (i=0; i<ceifeiro;i++) 
	{
		if(e.M[e.inimigo[i].x][e.inimigo[i].y]==1)
		{
			if ((proximo(e.jog.x,e.jog.y,e.inimigo[i].x,e.inimigo[i].y) && e.my_knifes>0) ||( proximo2(e.jog.x,e.jog.y,e.inimigo[i].x,e.inimigo[i].y) && e.lanca>0))
				QUADRADO(e.inimigo[i].x,e.inimigo[i].y,ESCALA, "#610B0B");
			sprintf(link,"http://localhost/cgi-bin/roguelike?%s,%d,%d",e.user,e.inimigo[i].x,e.inimigo[i].y);
			ABRIR_LINK(link);
			IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "faqueiro.gif");
			FECHAR_LINK;

		} 
	}  
	for (i=ceifeiro; i<escudeiro+ceifeiro;i++)  
	{ 
		if(e.M[e.inimigo[i].x][e.inimigo[i].y]==1)
		{
			if ((proximo(e.jog.x,e.jog.y,e.inimigo[i].x,e.inimigo[i].y) && e.my_knifes>0) ||( proximo2(e.jog.x,e.jog.y,e.inimigo[i].x,e.inimigo[i].y) && e.lanca>0))
				QUADRADO(e.inimigo[i].x,e.inimigo[i].y,ESCALA, "#610B0B");
			sprintf(link,"http://localhost/cgi-bin/roguelike?%s,%d,%d",e.user,e.inimigo[i].x,e.inimigo[i].y);
			ABRIR_LINK(link);
			IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "escudas.gif");
			FECHAR_LINK; 

		}
	}
	for (i=escudeiro+ceifeiro; i<wizard+escudeiro+ceifeiro;i++)
	{
		if(e.M[e.inimigo[i].x][e.inimigo[i].y]==1)
		{
			if ((proximo(e.jog.x,e.jog.y,e.inimigo[i].x,e.inimigo[i].y) && e.my_knifes>0) ||( proximo2(e.jog.x,e.jog.y,e.inimigo[i].x,e.inimigo[i].y) && e.lanca>0))
				QUADRADO(e.inimigo[i].x,e.inimigo[i].y,ESCALA, "#610B0B");
			sprintf(link,"http://localhost/cgi-bin/roguelike?%s,%d,%d",e.user,e.inimigo[i].x,e.inimigo[i].y);
			ABRIR_LINK(link);
			IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "magico.gif");
			FECHAR_LINK;
		}
		
	}
	for (i=wizard+escudeiro+ceifeiro; i<dragon+wizard+escudeiro+ceifeiro;i++)
			{
		if(e.M[e.inimigo[i].x][e.inimigo[i].y]==1)
		{
			if ((proximo(e.jog.x,e.jog.y,e.inimigo[i].x,e.inimigo[i].y) && e.my_knifes>0) ||( proximo2(e.jog.x,e.jog.y,e.inimigo[i].x,e.inimigo[i].y) && e.lanca>0))
				QUADRADO(e.inimigo[i].x,e.inimigo[i].y,ESCALA, "#610B0B");
			sprintf(link,"http://localhost/cgi-bin/roguelike?%s,%d,%d",e.user,e.inimigo[i].x,e.inimigo[i].y);
			ABRIR_LINK(link);
			IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "dragonn.gif");
			FECHAR_LINK;
			

		}
	}
}

void imprime_knifes (ESTADO e){ /** Imprime as espadas e torna as que tiverem próximas clicáveis para poderem ser apanhadas */
	int i;
	char link[MAX_BUFFER];

	for (i =0; i < e.num_knifes; i++)
	{
		if(e.M[e.knifes[i].x][e.knifes[i].y]==1){
			if (proximo(e.jog.x,e.jog.y,e.knifes[i].x,e.knifes[i].y))
			{
				sprintf(link,"http://localhost/cgi-bin/roguelike?%s,%d,%d",e.user,e.knifes[i].x,e.knifes[i].y);
				ABRIR_LINK(link);
				IMAGEM(e.knifes[i].x, e.knifes[i].y, ESCALA, "espada2.gif");
				FECHAR_LINK;
			}
			else
				if(e.M[e.knifes[i].x][e.knifes[i].y]==1)
					IMAGEM(e.knifes[i].x, e.knifes[i].y, ESCALA, "espada2.gif");
		}
	}
}    



ESTADO aumenta_naifas(ESTADO e){ /** Colecta espadas do tabuleiro editando o STATUS*/
	int i;
	for (i=0; i< e.num_knifes;i++)
	{
		if ( e.jog.x == e.knifes[i].x && e.jog.y == e.knifes[i].y && e.my_knifes <5)
		{
			e.my_knifes ++; 
			e.knifes[i].x=100;
				}
		}
	return e;
}


ESTADO aumenta_coracoes(ESTADO e){ /** Colecta Vidas do tabuleiro editando o STATUS*/
	int i;
	for (i=0; i< e.num_bonus;i++)
	{
		if ( e.jog.x == e.bonus[i].x && e.jog.y == e.bonus[i].y && e.vidajog< 5)
		{
			e.vidajog++;
			e.bonus[i].x =100; 
		}  
	}
	return e;

}

void imprime_bonus (ESTADO e){ /** Imprime vidas no tabuleiro tornando as próximas clicáveis */
	int i;
	char link[MAX_BUFFER];
	for(i=0;i<e.num_bonus;i++)	
		{
		if(e.M[e.bonus[i].x][e.bonus[i].y]==1){
			if (proximo(e.jog.x,e.jog.y,e.bonus[i].x,e.bonus[i].y))
			{
				sprintf(link,"http://localhost/cgi-bin/roguelike?%s,%d,%d",e.user,e.bonus[i].x, e.bonus[i].y);
				ABRIR_LINK(link);
				IMAGEM(e.bonus[i].x, e.bonus[i].y, ESCALA, "vvida.gif");
				FECHAR_LINK;
			}
			else
				if(e.M[e.bonus[i].x][e.bonus[i].y]==1)
					IMAGEM(e.bonus[i].x, e.bonus[i].y, ESCALA, "vvida.gif");
		}  
	} 
}

ESTADO setasRandom(ESTADO e){ /** Setas especiais que, quando colectadas no tabuleiro, matam até 2 inimigos aleatórios */
	 
	 int i,w,z, c=0, n;

	for (i=0; i< e.num_setasRandom;i++)
	{
		if ( e.jog.x == e.setasRandom[i].x && e.jog.y == e.setasRandom[i].y )
		{
					for (n=0;n<e.num_inimigos;n++)
			{
					if (posicao_valida(e.inimigo[n].x,e.inimigo[n].y)==1) /** calcula número de inimigos no tabuleiro */
						c++;
			}

			if (c>0)
			{

				z = RandRange(0,e.num_inimigos - 1);
				w = RandRange(0,e.num_inimigos - 1); /** seleciona inimigos aleatórios */

			while(posicao_valida(e.inimigo[z].x,e.inimigo[z].y)==0 || posicao_valida(e.inimigo[w].x,e.inimigo[w].y)==0)
			{	z = RandRange(0,e.num_inimigos - 1);
				w = RandRange(0,e.num_inimigos - 1);
			}

			e.inimigo[z].x = 100;    /** remove inimigo do tabueliro */
			e.inimigo[w].x=100;
			}
	e.setasRandom[i].x=100;			/** remove setas do tabulerio */    
		}  
	}
	
		return e;
}

ESTADO mudaEstado2(ESTADO e){ /** Caixa mágica que reorganiza as posições dos inimigos e dos jogadores podendo diminuir ou aumentar o número de inimigos */
	int i,w,z,x,y;
	for (i=0; i< e.num_mudaEstado;i++)
	{
		if ( e.jog.x == e.mudaEstado[i].x && e.jog.y == e.mudaEstado[i].y )
		{
		for (z=0;z<e.num_inimigos;z++)
			{
				e.inimigo[z].y = 200;   /** Retira todos os inimigos do tabuleiro */
			}	
			 srand(time(NULL));
			if(e.flag == 1 || e.flag == 2) /* Cria um número aleatório entre 0 e o número de inimigos de determinado nivel */
				w=RandRange(0,3);
			else if(e.flag == 3)
				w=RandRange(0,4);
			else if(e.flag == 4)
					w=RandRange(0,5);
			else 
				w=RandRange(0,8);

			for (z=0;z<w;z++)
			 {
				x = RandRange(Liminfx,Limsupx-1);   /** Reorganiza as posições dos inimigos aleatóriamente para uma posição livre */
				y =RandRange(Liminfy,Limsupy-1); 
					while(invariante(x,y,e)==1)
						{
							x = RandRange(Liminfx,Limsupx-1);
							y = RandRange(Liminfy,Limsupy-1); 
						}
				e.inimigo[z].x = x; 
				e.inimigo[z].y = y;	
		    } 

	w = RandRange(Liminfx,Limsupx-1);     /** Reorganiza a posição do jogador aleatóriamente para uma posição livre*/
	z = RandRange(Liminfy,Limsupy-1);
			while(invariante(w,z,e)==1)
			{
				w = RandRange(Liminfx,Limsupx-1);
				z = RandRange(Liminfy,Limsupy-1);
			}
		   
		e.jog.x=w;
		e.jog.y=z; 
		e.mudaEstado[i].y=200;	/** Retira a caixa mágica do tabuleiro */
		}  
	}


return e;
}

void imprime_mudaEstado (ESTADO e){ /** Imprime as caixas mágicas no tabuleiro tornando-as clicáveis caso estejam próximas */
	int i;
	char link[MAX_BUFFER];
	for(i=0;i<e.num_mudaEstado;i++)	
		{
			if (proximo(e.jog.x,e.jog.y,e.mudaEstado[i].x,e.mudaEstado[i].y))
			{
			sprintf(link,"http://localhost/cgi-bin/roguelike?%s,%d,%d",e.user,e.mudaEstado[i].x, e.mudaEstado[i].y);
			ABRIR_LINK(link);
			IMAGEM(e.mudaEstado[i].x, e.mudaEstado[i].y, ESCALA, "chest.gif");
			FECHAR_LINK;
			}
			else
				IMAGEM(e.mudaEstado[i].x, e.mudaEstado[i].y, ESCALA, "chest.gif");
		}
	} 


void imprime_setasRandom (ESTADO e){ /** Imprime as setas especiais no tabuleiro tornando-as clicáveis caso estejam próximas */
	int i;
	char link[MAX_BUFFER];
	for(i=0;i<e.num_setasRandom;i++)	
		{
			if (proximo(e.jog.x,e.jog.y,e.setasRandom[i].x,e.setasRandom[i].y))
			{
			sprintf(link,"http://localhost/cgi-bin/roguelike?%s,%d,%d",e.user,e.setasRandom[i].x, e.setasRandom[i].y);
			ABRIR_LINK(link);
			IMAGEM(e.setasRandom[i].x, e.setasRandom[i].y, ESCALA, "setas.gif");
			FECHAR_LINK;
			}
			else
				IMAGEM(e.setasRandom[i].x, e.setasRandom[i].y, ESCALA, "setas.gif");
		}
	} 


void imprime_movimentos(ESTADO e) { /** Define os movimentos possiveis do jogador */
	if (e.num_saltos < 1)

	{
	imprime_movimento(e,  0, -1); /** Movimentos caso não tenha em posse a habilidade "salto" */
	imprime_movimento(e,  0, +1);
	imprime_movimento(e, -1,  0);
	imprime_movimento(e, +1,  0);
	imprime_movimento(e, +1, +1);
	imprime_movimento(e, -1, -1);
	imprime_movimento(e, -1,  +1);
	imprime_movimento(e, +1,  -1);	
	}

	else 
	{
	imprime_movimento(e,  0, -1); /** Movimentos caso tenha em posse a habilidade "salto" */
	imprime_movimento(e,  0, +1);
	imprime_movimento(e, -1,  0);
	imprime_movimento(e, +1,  0);
	imprime_movimento(e, +1, +1);
	imprime_movimento(e, -1, -1);
	imprime_movimento(e, -1,  +1);
	imprime_movimento(e, +1,  -1);	
	imprime_movimento(e,  0, -2);
	imprime_movimento(e,  0, +2);
	imprime_movimento(e, -2,  0);
	imprime_movimento(e, +2,  0);
	}
	
}


void imprime_jogador(ESTADO e) { /** Imprime jogador */
	IMAGEM(e.jog.x, e.jog.y, ESCALA, "horoina.gif");
	imprime_movimentos(e);
}


void imprime_lanca(ESTADO e){ /** Imprime no menu STATUS lanca caso ainda não tenha sido usada */
	if (e.lanca>0) 
	IMAGEM(12 ,16, 38, "spear.gif");
}

void imprime_saltos(ESTADO e){ /** Imprime no menu STATUS salto caso ainda não tenha sido usado */
	if (e.num_saltos>0)
	IMAGEM(13 ,16, 38, "salto.gif");
}

void imprime_mar(){ /** Imprime mar no nivel 1 */

	IMAGEM(19,12, ESCALA, "seaa.gif"); 
	IMAGEM(20,12, ESCALA, "seaa.gif");
	IMAGEM(21,12, ESCALA, "seaa.gif");
	IMAGEM(22,12, ESCALA, "seaa.gif");
	IMAGEM(20,11, ESCALA, "seaa.gif");
	IMAGEM(21,11, ESCALA, "seaa.gif");
	IMAGEM(22,11, ESCALA, "seaa.gif");
	IMAGEM(21,10, ESCALA, "seaa.gif");
	IMAGEM(22,10, ESCALA, "seaa.gif");
	IMAGEM(22,9, ESCALA, "seaa.gif");
}

void imprime_obstaculos(ESTADO e,int x,int y){ /** Imprime os diferentes obstaculos pelos diferentes niveis caso estejam visiveis */
	if(e.flag==1){
		if(e.M[x][y]==1)
			IMAGEM(x,y, ESCALA, "treee.png");

	}
	else if(e.flag==2 || e.flag == 3){
			IMAGEM(x,y, ESCALA, "obstaculo.png");
	}
	else
	{
			if(e.M[x][y]==1)
				IMAGEM(x,y, ESCALA, "textura3.jpg");
	}
}

void imprime_arma(ESTADO e){ /** Imprime o número espadas disponiveis no menu STATUS */
	int i ;
	for (i=9;i<e.my_knifes+9;i++)
        IMAGEM(i + 1 ,15, 38, "espada2.gif");

}


void imprime_vida(ESTADO e){ /** Imprime o número de vidas disponiveis no menu STATUS */
	int i ;

	for (i=8;i<e.vidajog+8;i++) 
        IMAGEM(i + 1 ,13, 40, "vvida.gif");

}




void escrever_ficheiro (char *user, ESTADO e){ /** Grava o estado num ficheiro */
	FILE *escrever;  /** cria um ficheiro */
	char ficheiro[10240];

	sprintf(ficheiro, "/var/www/html/files/%s",user); /** cria um ficheiro nesta localizaçao */
	if ((escrever = fopen(ficheiro, "w"))==NULL)
	{
		perror("não foi possivel escrever o ficheiro"); 
		exit(1);
	}
	else
	{
		fprintf(escrever, "%s", estado2str(e)); /** converte o estado em string e grava-o no ficheiro */
		fclose(escrever);
	}
}


int ler_ficheiro(char *user, ESTADO *e){ /** Lê e atualiza o ficheiro que tem a estrutura ESTADO guardada */
	 
	FILE *ler;
	char ficheiro[10240];

	sprintf(ficheiro, "/var/www/html/files/%s", user); /** lê um ficheiro nesta localizaçao */
	if ((ler = fopen(ficheiro, "r")) != NULL){

		char estado_str[MAX_BUFFER];

		if( fscanf(ler, "%s", estado_str) == 1) /** caso consiga ler o ficheiro converte a string do ficheiro para a estrutura ESTADO */

		fclose(ler);

		*e = str2estado(estado_str);
		
		return 1;
	}

	return 0;
}

void imprime_backgraound(ESTADO e,int x, int y){ /** imprime imagem de fundo dependendo do nivel */
	if(e.flag == 1||e.flag==0)
		IMAGEM2(0.0,0.0, ESCALA*40, "ilha.jpg");
	else
		printf("<rect x = \"%d\" y = \"%d\" height = \"10000\" width = \"10000\" stroke = \"black\" style = \"fill:#000000\"/></a>\n",x,y);
}

void verifica_melhorscore (ESTADO e){ /** Cria um botão que nos redireciona para a lista dos melhores scores */
		char link[MAX_BUFFER];
		sprintf(link, "http://localhost/files/highscore?"); 
		ABRIR_LINK(link);
        if(e.flag == 1)
        	printf("<text x=\"240\" y=\"630\" fill=\"blue\" opacity =\"0.7\" font-size=\"20\"> Score: </text>\n");
    	else
    		printf("<text x=\"240\" y=\"630\" fill=\"grey\" font-size=\"20\"> Score: </text>\n");
		FECHAR_LINK;}

int movimento_permitido(ESTADO e, int x, int y){ /** Testa se a distância entre 2 casa é menor do que 5 para assim ser um movimento válido */ 
	if (e.flag2!=1)
return ((e.jog.x-x)*(e.jog.x-x) + (e.jog.y-y)*(e.jog.y-y))<=5;
else return 1;
}  


void imprime_menus(ESTADO e){ /** Imprime menus exteriores ao tabuleiro */
		if(e.flag == 1){/** Menus nivel 1*/

        	printf("<rect x = \"0\" y = \"0\" height = \"60\" width = \"100\" stroke = \"black\" style = \"fill:#2E2E2E\"/></a>\n");
        	printf("<text x=\"15\" y=\"40\" fill=\"red\"  opacity =\"0.7\" font-weight=\"bold\" font-size=\"40\"> LA </text>\n");
        	printf("<rect x = \"220\" y = \"510\" height = \"140\" opacity =\"0.7\" width = \"340\" stroke = \"black\" style = \"fill:#2E2E2E\"/></a>\n");
        	printf("<text x=\"240\" y=\"550\" fill=\"blue\"  opacity =\"0.7\"  font-size=\"25\"> Vidas: </text>\n");
        	printf("<text x=\"300\" y=\"500\" fill=\"blue\" opacity =\"0.7\"  font-weight=\"bold\" font-size=\"40\"> STATUS </text>\n");
        	printf("<text x=\"240\" y=\"630\" fill=\"blue\"  opacity =\"0.7\" font-size=\"20\"> Score: </text>\n");
        	printf("<text x=\"320\" y=\"630\" fill=\"blue\"  opacity =\"0.7\" font-size=\"20\"> %d </text>\n", e.score);
        	printf("<text x=\"240\" y=\"595\" fill=\"blue\"  opacity =\"0.7\" font-size=\"25\"> Armas: </text>\n");
        	printf("<text x=\"820\" y=\"140\" fill=\"blue\"  opacity =\"0.7\" font-weight=\"bold\" font-size=\"40\"> Nivel: %d </text>\n",e.nivel);
    	}
    	else /** Menus para os restantes niveis */
    	{
			printf("<rect x = \"0\" y = \"0\" height = \"60\" width = \"100\" stroke = \"black\" style = \"fill:#2E2E2E\"/></a>\n");
        	printf("<text x=\"15\" y=\"40\" fill=\"red\"  opacity =\"0.7\" font-weight=\"bold\" font-size=\"40\"> LA </text>\n");
        	printf("<rect x = \"220\" y = \"510\" height = \"140\" width = \"340\" stroke = \"black\" style = \"fill:#2E2E2E\"/></a>\n");
        	printf("<text x=\"240\" y=\"550\" fill=\"grey\"  font-size=\"25\"> Vidas: </text>\n");
        	printf("<text x=\"300\" y=\"500\" fill=\"grey\" font-weight=\"bold\" font-size=\"40\"> STATUS </text>\n");
        	printf("<text x=\"240\" y=\"630\" fill=\"grey\" font-size=\"20\"> Score: </text>\n");
        	printf("<text x=\"320\" y=\"630\" fill=\"grey\" font-size=\"20\"> %d </text>\n", e.score);
        	printf("<text x=\"240\" y=\"595\" fill=\"grey\" font-size=\"25\"> Armas: </text>\n");
        	printf("<text x=\"820\" y=\"140\" fill=\"grey\" font-weight=\"bold\" font-size=\"40\"> Nivel: %d </text>\n",e.nivel);
		}
}
void game_over (ESTADO e){ /** Cria os botões TRY Again que reinicia o jogo e  Highscore para visualizar os melhores scores */
	char link[MAX_BUFFER];

	IMAGEM2(0.70,0.0, ESCALA*15, "over.png");

	sprintf(link, "http://localhost/cgi-bin/roguelike?%s", e.user);
	ABRIR_LINK(link);
  	printf("<rect x = \"730\" y = \"480\" height = \"50\" width = \"340\" stroke = \"black\" style = \"fill:#2E2E2E\"/></a>\n");
	FECHAR_LINK;
	ABRIR_LINK(link);
    printf("<text x=\"765\" y=\"520\" fill=\"red\" font-weight=\"bold\" font-size=\"40\"> TRY AGAIN! </text>\n");
	FECHAR_LINK;

	sprintf(link, "http://localhost/files/highscore?");
	
	ABRIR_LINK(link);
  	printf("<rect x = \"730\" y = \"560\" height = \"50\" width = \"340\" stroke = \"black\" style = \"fill:#2E2E2E\"/></a>\n");
	FECHAR_LINK;


	ABRIR_LINK(link);
    printf("<text x=\"765\" y=\"600\" fill=\"red\" font-weight=\"bold\" font-size=\"40\"> High Score </text>\n");
	FECHAR_LINK;


}




void reset (ESTADO e){ /** Cria botão para reiniciar o jogo em qualquer ponto do mesmo */

	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/roguelike?%s", e.user);
	ABRIR_LINK(link);
  	printf("<rect x = \"1235\" y = \"600\" height = \"50\" width = \"230\" stroke = \"black\" style = \"fill:#2E2E2E\"/></a>\n");
	FECHAR_LINK;


	ABRIR_LINK(link);
    printf("<text x=\"1270\" y=\"640\" fill=\"grey\" font-weight=\"bold\" font-size=\"40\"> Restart </text>\n");
	FECHAR_LINK;
}
void imprime_tocha(ESTADO e){ /** Imprime tocha para, no nivel 3, voltar a ter visão*/

	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/roguelike?%s,%d,%d", e.user, 17,12);
	ABRIR_LINK(link);
 	IMAGEM2(8.0,2.0, ESCALA*3, "tocha.gif");
	FECHAR_LINK;
}

ESTADO line_sight(ESTADO e, int px, int py){ /** Imprime campo de visão do jogador */

	float alfa;
	int r;
	int x,y;  
	for(alfa=0; alfa<360 ;alfa++){ /** para um angulo de 0 a 360*/

		for(r=0; r<3; r++) /** para um raio de 0 a 3 */
			{
				x = r * cos(alfa) + px; 
				y = r * sin(alfa) + py;
				if (tem_obstaculo(e,x,y)) /** caso não tenha objecto guarda essa posição como sendo visivel*/
					break;
				e.M[x][y] = 1;
			}
	}
	return e;
}
void imprime_bloqueio( int x, int y){ /** Bloqueia todo o jogo até se ativar a tocha */
	QUADRADO(x, y, ESCALA, "#000000");
}

void ilumina(ESTADO e,int x,int y){ /** Mostra posições para as quais os inimigos podem atacar */ 
	int i;
for (i=0; i<ceifeiro;i++)
	if (x == e.inimigo[i].x && y == e.inimigo[i].y){

		if(posicao_valida(x,y+1) && e.M[x][y+1] && tem_inimigo(e,x,y+1) == 0) /** ilumina as casas na vertical do ceifeito */
			QUADRADO2(x, (y+1),ESCALA, "#610B0B", 0.5);
		
		if(posicao_valida(x,y-1) && e.M[x][y-1] && tem_inimigo(e,x,y-1) == 0) 
			QUADRADO2(x, (y-1),ESCALA, "#610B0B", 0.5);
	}

for (i=ceifeiro; i<escudeiro+ceifeiro;i++)
	if (x == e.inimigo[i].x && y == e.inimigo[i].y){

		if(posicao_valida(x+1,y) && e.M[x+1][y] && tem_inimigo(e,x+1,y) == 0)	/** ilumina as casas na horizontal do escudeiro */
			QUADRADO2((x+1), y,ESCALA, "#610B0B", 0.5);
		
		if(posicao_valida(x-1,y) && e.M[x-1][y] && tem_inimigo(e,x-1,y) == 0)
			QUADRADO2((x-1), y,ESCALA, "#610B0B", 0.5);
	}
  
for (i=escudeiro+ceifeiro; i<wizard+escudeiro+ceifeiro;i++)
 	if (x == e.inimigo[i].x && y == e.inimigo[i].y){

		if(posicao_valida(x+1,y+1) && e.M[x+1][y+1] && tem_inimigo(e,x+1,y+1) == 0) /** ilumina as casas na diagonal do wizard */
			QUADRADO2((x+1), (y+1),ESCALA, "#610B0B", 0.5);
		
		if(posicao_valida(x+1,y-1) && e.M[x+1][y-1] && tem_inimigo(e,x+1,y-1) == 0)
			QUADRADO2((x+1), (y-1),ESCALA, "#610B0B", 0.5);
		
		if(posicao_valida(x-1,y+1) && e.M[x-1][y+1] && tem_inimigo(e,x-1,y+1) == 0)
			QUADRADO2((x-1), (y+1),ESCALA, "#610B0B", 0.5);
				
		if(posicao_valida(x-1,y-1) && e.M[x-1][y-1] && tem_inimigo(e,x-1,y-1) == 0)
			QUADRADO2((x-1), (y-1),ESCALA, "#610B0B", 0.5);
	}
for (i=wizard+escudeiro+ceifeiro; i<dragon+wizard+escudeiro+ceifeiro;i++)
	if (x == e.inimigo[i].x && y == e.inimigo[i].y){

		
		if(posicao_valida(x,y+1) && e.M[x][y+1] && tem_inimigo(e,x,y+1) == 0) /** ilumina todas as casas à volta do dragão */
			QUADRADO2(x, (y+1),ESCALA, "#610B0B", 0.5);
		
		if(posicao_valida(x,y-1) && e.M[x][y-1] && tem_inimigo(e,x,y-1) == 0)
			QUADRADO2(x, (y-1),ESCALA, "#610B0B", 0.5);

		if(posicao_valida(x+1,y) && e.M[x+1][y] && tem_inimigo(e,x+1,y) == 0)	
			QUADRADO2((x+1), y,ESCALA, "#610B0B", 0.5);
		
		if(posicao_valida(x-1,y) && e.M[x-1][y] && tem_inimigo(e,x-1,y) == 0)
			QUADRADO2((x-1), y,ESCALA, "#610B0B", 0.5);
  
		if(posicao_valida(x+1,y+1) && e.M[x+1][y+1] && tem_inimigo(e,x+1,y+1) == 0)
			QUADRADO2((x+1), (y+1),ESCALA, "#610B0B", 0.5);
		
		if(posicao_valida(x+1,y-1) && e.M[x+1][y-1] && tem_inimigo(e,x+1,y-1) == 0)
			QUADRADO2((x+1), (y-1),ESCALA, "#610B0B", 0.5);
		
		if(posicao_valida(x-1,y+1) && e.M[x-1][y+1] && tem_inimigo(e,x-1,y+1) == 0)
			QUADRADO2((x-1), (y+1),ESCALA, "#610B0B", 0.5);
				
		if(posicao_valida(x-1,y-1) && e.M[x-1][y-1] && tem_inimigo(e,x-1,y-1) == 0)
			QUADRADO2((x-1), (y-1),ESCALA, "#610B0B", 0.5);
	}
}


void terramoto(ESTADO e){ /** Imprime terramoto na passagem do nivel 3 para 4 */
	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/roguelike?%s,%d,%d", e.user, 100,100);
	ABRIR_LINK(link);
 	IMAGEM2(1.3,0.3, ESCALA*10, "vibrar.gif");
	FECHAR_LINK;
}

void imprime_montanha(){ /** Imprime montanha no nivel 1 */
	 IMAGEM2(2.16,0.16, ESCALA*6, "mountain1.png");
}

void imprime_inicio(ESTADO e){ /** Imprime chegada à ilha */
		char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/roguelike?%s,%d,%d", e.user, 19,12);
	ABRIR_LINK(link);
 	IMAGEM2(0.7,0.0, ESCALA*15, "inicio1.gif");
	FECHAR_LINK;
}

ESTADO testa_lanca(ESTADO e, int x, int y){ /** Testa se gastamos a lança e se gastamos o salto */
	
	if (((e.jog.x ==x && (e.jog.y== (y +2) || e.jog.y==(y -2) )) || (e.jog.y ==y && (e.jog.x== (x +2) || e.jog.x==(x -2 )))) && !(matou_inimigo(e,x,y))) /** parte referente ao salto */
		e.num_saltos=0;

	if (((e.jog.x ==x && (e.jog.y== (y +2) || e.jog.y==(y -2) )) || (e.jog.y ==y && (e.jog.x== (x +2) || e.jog.x==(x -2 )))) && matou_inimigo(e,x,y)) /** parte referente à lança */
	{
 		e.lanca--;
 		e.my_knifes ++; /** como matar tira uma espada, comensamos 1 espada quando matamos com a lança */
 	}
 	return e;
}


int main() {
	int x, y;
	char user[1024];
	ESTADO e;
	int args;
	int i;
	int px; 
	int py; 
	
	args = sscanf(getenv("QUERY_STRING"),"%[^,],%d,%d",user, &x, &y);
	
	px = x; /** reserva o valor de x */
	py = y; /** reserva o valor de y */

	COMECAR_HTML;  
	ABRIR_SVG(6300, 2700);
	 
	if(ler_ficheiro(user, &e) && args == 3)    
 	{	if(e.flag==3 && x==16 && y == 3) /** abre exceção para ser possivel alterar as coordenadas sem as funções "moviment_permitido" e "posição_valida2" */
 			e.flag2 = 1;

 		if(e.flag == 0){ /** coloca a posição do jogador na posição inicial do jogo */
 			e.jog.x = 19; 
 			e.jog.y = 12;
 		} 
 		if(movimento_permitido(e,x,y) && posicao_valida2(e,x,y))
 		{
				e=testa_lanca(e,x,y);


 			if(!(matou_inimigo(e,x,y)))
 			{   
 				e.jog.x = x; /** muda as coordenadas do jogador para x e y */
 				e.jog.y = y; 
 			 	e=line_sight(e,x,y);
 			 	e=movimentos_inimigo(e);
 			}
 		e=aumenta_naifas(e);
 		e=aumenta_coracoes(e);
 		e=mudaEstado2(e);
		e=setasRandom(e);

			for (i=0;i<e.num_inimigos;i++){
					if ((proximo(e.jog.x,e.jog.y,e.inimigo[i].x,e.inimigo[i].y) && e.my_knifes>0) || ( proximo2(e.jog.x,e.jog.y,e.inimigo[i].x,e.inimigo[i].y) && e.lanca>=0))
					if (x == e.inimigo[i].x && y == e.inimigo[i].y){ /** Caso cliquemos na posição que esteja próximo de um inimgo matamo-lo*/
						e = mata_inimigos(e,i);
					if(e.flag >=4) /** apartir de nivel 4 os inimigos movem-se quando os atacamos */ 
						e=movimentos_inimigo(e);
						break;		
				}
			}


		
		if(e.flag==0)
			e = nivel1(e); /** caso o valor da flag seja 0 muda para nivel 1*/

		if (e.jog.x == e.saida.x && e.jog.y==e.saida.y && (e.flag == 1 || e.flag == 2)) /** caso o jogador esteja na posição da porta de saida de nivel e o valor da flag seja 1 e 2 muda para os niveis 2 e 3 respetivamente*/
			e = novo_nivel2e3 (e);

		else if((x==100 && y==100) && e.flag == 3) /** caso a x e y tenham o valor 100 e a flag seja 3 avança para o nivel 4 */
 				e = novo_nivel4 (e);

		else if (e.jog.x==e.saida.x && e.jog.y==e.saida.y&& e.flag>=4) /** caso o jagador se encontre na porta de saida de nivel e a flag assuma o valor 4, muda para nivel 5 e assim sussecivamente */
			e = novo_nivel5 (e);
		}
	}

	else 
		e = inicializar(); /** caso o número de argumentos seja diferente de 3, ou seja, no inicio do jogo, a estrutura ESTADO assume o valor de da função inicializar */


	strcpy(e.user,user); /** Atualiza a estrutura ESTADO com o novo nome de utilizador */
	escrever_ficheiro(user,e);
	 	

	if(e.flag==0) /** se o jogo estiver a iniciar apeans imprime o gif do inicio e o background */
	 {
	 	imprime_backgraound(e,0,0);
		imprime_inicio(e);
	}
	else
	{   
		imprime_backgraound(e,0,0); 
		
		for(y = Liminfy; y < Limsupy; y++)  /** Imprime tabuleiro */
			for(x = Liminfx; x < Limsupx; x++)
				imprime_casa(e,x,y);
	
	imprime_saida(e);
	imprime_bonus(e);  
	imprime_knifes(e);
	imprime_setasRandom(e);
	imprime_mudaEstado(e);
	imprime_inimigos(e);
	linka_inimigo(e);

	for (i=0;i<e.num_obstaculos;i++)
		imprime_obstaculos(e,e.obstaculo[i].x,e.obstaculo[i].y);

	imprime_menus(e);
	imprime_vida(e); 
	ilumina(e,px,py);
	imprime_arma(e);
	reset(e);
	imprime_lanca(e);
	imprime_saltos(e);

	if(e.vidajog==0) 
		melhor_score(e);

		if(e.flag == 4 && (e.jog.x ==17 && e.jog.y==13)){ /** caso estejamos no inicio do nivel 4, imprime o bloqueio para obrigar a usar a tocha*/
			imprime_tocha(e);
			for(y = Liminfy; y < Limsupy ; y++)
				for(x = Liminfx; x < Limsupx ; x++)
					imprime_bloqueio(x,y);
		}

	if (e.jog.x == e.saida.x && e.jog.y == e.saida.y && e.flag==3) /** caso estejamos no nivel 3 na posição de saída, imprime um terramoto */
		terramoto(e);

	if(e.flag==1){ /** imprime o mar e a montanha no nivel 1 */
		imprime_mar();
		imprime_montanha();
	}

	imprime_jogador(e);
	verifica_melhorscore(e);
	
	if (e.vidajog<=0) /** caso o numero de vidas chegue a 0, acaba o jogo */
		game_over(e);

	}

	return 0;
}
