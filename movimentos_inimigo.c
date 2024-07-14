/** 
@file movimentos_inimigo.c
*/
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cgi.h"
#include "estado.h"
#include "estado.c"

#define wizard		2
#define dragon 		2
#define escudeiro 	2
#define ceifeiro  	2

#define MAX_BUFFER		10240
#define TAM				10
#define ESCALA			50
#define OBSTACULOS      30
#define INIMIGOSNIVEL1  ceifeiro
#define OBSTACULOSNIVEL1 20
#define INIMIGOSNIVEL2e3 ceifeiro + escudeiro
#define INIMIGOSNIVEL4 ceifeiro + escudeiro + 1
#define INIMIGOSNIVEL5 wizard + ceifeiro + escudeiro



#define Liminfx		13
#define Limsupx		23
#define Liminfy		3
#define Limsupy     13
/** Função que indica se a posição do tabuleiro já esta ocupada, retornando 1 caso esteja ocupada e 0 se estiver livre */
int invariante(int x, int y, ESTADO e) 
{
	int i=0;
	/** refere-se unicamente ao primeiro nivel do jogo */
	if(e.flag == 1){
		/** Devolve 1 caso esteja a ocupar a posição da porta*/
		if(x==16 && y==3) 
			return 1;
		i=0;
         /** Devolve 1 caso esteja a ocupar a posição do jogador*/
		if (x == e.jog.x && y == e.jog.y) 
			return 1;
         /** Devolve 1 caso esteja  ocupado por inimigos*/
		i = 0;
		while(i < e.num_inimigos && (x != e.inimigo[i].x || y != e.inimigo[i].y) ) {i++;} 
			if(i < e.num_inimigos)
				{return 1;}
	
	/** Devolve 1 caso esteja  ocupado por obstaculos*/
		i=0;
		while(i < e.num_obstaculos && (x != e.obstaculo[i].x || y != e.obstaculo[i].y) ) {i++;} 
			if(i < e.num_obstaculos)
				{return 1;}
		/** Posições ocupadas pela paisagem(mar e montanha)*/
		i=0;
		if((x ==19 && y==12) || (x ==20 && y==12) || (x ==21 && y==12) || (x ==22 && y==12) || (x ==20 && y==11) || (x ==21 && y==11) || (x ==22 && y==11) || (x ==21 && y==10) || (x ==22 && y==10) || (x ==22 && y==9) )
			return 1;
		if((x ==13 && y==3) || (x ==14 && y==3) || (x ==15 && y==3) || (x ==16 && y==3) || (x ==17 && y==3) || (x ==18 && y==3) || (x ==14 && y==4) || (x ==15 && y==4) || (x ==13 && y==4) || (x ==18 && y==4) || (x ==17 && y==4))
			return 1;
		
		return 0;
	}

	else{
/** Devolve 1 caso esteja a ocupar a posição da porta*/
	if(x==16 && y==3)
		return 1;
	i=0;
/** Devolve 1 caso esteja a ocupar a posição do jogador*/
	if (x == e.jog.x && y == e.jog.y) 
		return 1;

	i = 0;
	/** Devolve 1 caso esteja  ocupado por inimigos*/
	while(i < e.num_inimigos && (x != e.inimigo[i].x || y != e.inimigo[i].y) ) {i++;}
	if(i < e.num_inimigos)
	{return 1;}
	
	
	i=0;
	/** Devolve 1 caso esteja  ocupado por obstaculos*/
	while(i < e.num_obstaculos && (x != e.obstaculo[i].x || y != e.obstaculo[i].y) ) {i++;}
	if(i < e.num_obstaculos)
	{return 1;}

	return 0;
	}
}

/** Função que delimita o tabuleiro*/
int posicao_valida(int x, int y) 
{
	return x >= Liminfx && y >= Liminfy && x < Limsupx && y < Limsupy;
}
/** Função que delimita o tabuleiro com exceção*/
int posicao_valida2(ESTADO e ,int x, int y) 
{
	if(e.flag2!=1)
	return x >= Liminfx && y >= Liminfy && x < Limsupx && y < Limsupy;
	else return 1;
}
/**Mostra ao jogador os seus movimentos possiveis*/
void imprime_jogavel(int x, int y) 
{
	QUADRADO2(x, y,ESCALA, "#000000", 0.3);
}
/**Cria um numero aleatório entre dois limites */
int RandRange(int Min, int Max)
{
	int w;

	w = random() % ((Max-Min)+1)+Min;
	return w;
}



ESTADO movimentos_inimigo (ESTADO e)
{
	int i;

	int aux1,aux2,aux3,aux4;

	

	aux1=e.inimigo[ceifeiro+ escudeiro+ wizard].x; /** utilizado posteriormente para impedir os dragões de se moverem até que o jogador atinja determinada coordenada */


	aux2=e.inimigo[ceifeiro+ escudeiro+ wizard].y;	
	aux3=e.inimigo[ceifeiro+ escudeiro+ wizard+1].x;
	aux4=e.inimigo[ceifeiro+ escudeiro+ wizard+1].y;


	for (i=0;i<e.num_inimigos;i++)
	{
		/**caso o jogador esteja na dioganal inferior direita  do inimigo*/
			if (e.jog.x > e.inimigo[i].x && e.jog.y > e.inimigo[i].y)
		{
			/** Se a posição na diagonal inferior direita estiver livre e for uma  posição dentro do tabuleiro, o inimigo vai se mexer para lá, senão for possivel vai tentar as proximas posiçoes de forma a proximar do jogador,mexendo prioritariamente para baixo,direita, diagonal inferior esquerda, esquerda,diagonal superior esquerda, cima,diagonal superior direita */
			if ((invariante (e.inimigo[i].x+1,e.inimigo[i].y+1,e)==0)&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y+1)==1)  
			{                                                                 
			e.inimigo[i].x= e.inimigo[i].x+1;                                     
			e.inimigo[i].y= e.inimigo[i].y+1;      
			}                                                              

			

			else 
			
			if(invariante (e.inimigo[i].x,e.inimigo[i].y+1,e)==0&& posicao_valida (e.inimigo[i].x,e.inimigo[i].y+1)==1)
			{
			e.inimigo[i].y= e.inimigo[i].y+1;
			e.inimigo[i].x= e.inimigo[i].x;
			}	


			 else 
			
			if (invariante (e.inimigo[i].x+1,e.inimigo[i].y,e)==0 && posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x+1;
			e.inimigo[i].y= e.inimigo[i].y;
			}  
			
			

			else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y+1,e)==0&& posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y+1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x-1;
			e.inimigo[i].y= e.inimigo[i].y+1;
			}	


			else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x-1;
			e.inimigo[i].y= e.inimigo[i].y;
			}   

			 else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y-1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x-1;   
			e.inimigo[i].y= e.inimigo[i].y-1;
			}	


			 else 
			
			if(invariante (e.inimigo[i].x,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x,e.inimigo[i].y-1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x;
			e.inimigo[i].y= e.inimigo[i].y-1;
			}

			 else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y+1,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y+1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x+1;
			e.inimigo[i].y= e.inimigo[i].y-1;
			}			
		 }




	else 
		/*caso o jogador esteja na dioganal superior direita  do inimigo*/
		if (e.jog.x > e.inimigo[i].x && e.jog.y < e.inimigo[i].y)
		{
			/** Se a posição na diagonal superior direita estiver livre e for uma posição dentro do tabuleiro, o inimigo vai se mexer para lá, senão for possivel vai tentar as proximas posiçoes de forma a proximar do jogador, mexendo prioritariamente para cima,direita, diagonal superior esquerda,diagonal inferior direita,abaixo, diagonal inferior esquerda,esquerda*/
			if (invariante (e.inimigo[i].x+1,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y-1)==1)  
			{                                                               
			e.inimigo[i].x= e.inimigo[i].x+1;                               
			e.inimigo[i].y= e.inimigo[i].y-1;
			}


			else 
			
			if (invariante (e.inimigo[i].x,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x,e.inimigo[i].y-1)==1)
			{
			e.inimigo[i].y= e.inimigo[i].y-1;
			e.inimigo[i].x= e.inimigo[i].x;
			}
			

			else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x+1;
			e.inimigo[i].y=e.inimigo[i].y;
			}	
			

			else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y-1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y-1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x-1;
			e.inimigo[i].y= e.inimigo[i].y-1;
			}	

			else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y+1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x+1;
			e.inimigo[i].y= e.inimigo[i].y+1;
			}	

			 else 
			
			if(invariante (e.inimigo[i].x,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x,e.inimigo[i].y+1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x;
			e.inimigo[i].y= e.inimigo[i].y+1;
			}
			
			 else
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y+1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x-1;
			e.inimigo[i].y= e.inimigo[i].y+1;
			}	

			 else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x-1;
			e.inimigo[i].y=e.inimigo[i].y;
			}
		}


	else



	/**caso o jogador esteja na dioganal superior esquerda do inimigo*/
		if (e.jog.x < e.inimigo[i].x && e.jog.y < e.inimigo[i].y)  
		{
			/** Se a posição na diagonal superior esquerda estiver livre e for uma posição dentro do tabuleiro, o inimigo vai se mexer para lá, senão for possivel vai tentar as proximas posiçoes de forma a proximar do jogador,mexendo prioritariamente para cima,esquerda, diagonal inferior esquerda,diagonal superior direita,direita, baixo,diagonal inferior direita*/
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y-1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y-1)==1)
			{                                                           
			e.inimigo[i].x= e.inimigo[i].x-1;                           
			e.inimigo[i].y= e.inimigo[i].y-1;
			}	

			else 
			
			if(invariante (e.inimigo[i].x,e.inimigo[i].y-1,e)==0 && posicao_valida (e.inimigo[i].x,e.inimigo[i].y-1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x;
			e.inimigo[i].y= e.inimigo[i].y-1;
			}	

			 else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x-1;
			e.inimigo[i].y=e.inimigo[i].y;
			}	

			else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y+1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x-1;
			e.inimigo[i].y= e.inimigo[i].y+1;
			}	

			else

			if (invariante (e.inimigo[i].x+1,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y-1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x+1;
			e.inimigo[i].y= e.inimigo[i].y-1;
			}

			else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x+1;
			e.inimigo[i].y=e.inimigo[i].y;
			}	

			 else 
			
			if(invariante (e.inimigo[i].x,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x,e.inimigo[i].y+1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x;
			 e.inimigo[i].y= e.inimigo[i].y+1;
			} 


			else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y+1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x+1;
			 e.inimigo[i].y= e.inimigo[i].y+1;
			}	


		}


	else

/**caso o jogador esteja na dioganal inferior esquerda do inimigo*/
		if (e.jog.x < e.inimigo[i].x && e.jog.y > e.inimigo[i].y) 
			{
				/** Se a posição na diagonal inferior esquerda estiver livre e for uma posição dentro do tabuleiro, o inimigo vai se mexer para lá, senão for possivel vai tentar as proximas posiçoes de forma a proximar do jogador,mexendo prioritariamente para baixo,esquerda, diagonal superior esquerda,diagonal inferior direita,cima, direita,diagonal superior direita*/
			   	if(invariante (e.inimigo[i].x-1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y+1)==1) 
				 {                                                            
				 e.inimigo[i].x= e.inimigo[i].x-1;                            
				 e.inimigo[i].y= e.inimigo[i].y+1;
				}	

			else 
			
			if(invariante (e.inimigo[i].x,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x,e.inimigo[i].y+1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x;
			 e.inimigo[i].y= e.inimigo[i].y+1;
			}	
			
 			else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x-1;
			 e.inimigo[i].y=e.inimigo[i].y;
			}	



			else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y-1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y-1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x-1;
			 e.inimigo[i].y= e.inimigo[i].y-1;
			}	


			else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y+1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x+1;
			 e.inimigo[i].y= e.inimigo[i].y+1;
			}	

			else 
			
			if (invariante (e.inimigo[i].x,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x,e.inimigo[i].y-1)==1)
			 {
			 e.inimigo[i].y= e.inimigo[i].y-1;
			 e.inimigo[i].x= e.inimigo[i].x;
			}
			

			else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x+1;
			 e.inimigo[i].y=e.inimigo[i].y;
			}	
			else

             if (invariante (e.inimigo[i].x+1,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y-1)==1)
             {
			 e.inimigo[i].x= e.inimigo[i].x+1;
			 e.inimigo[i].y= e.inimigo[i].y-1;
			}

		}




    else

		
		/**caso o jogador esteja acima do inimigo*/ 
		if (e.jog.x == e.inimigo[i].x && e.jog.y < e.inimigo[i].y)
		{
			/** Se a posição superior ao inimigo estiver livre e for uma posição dentro do tabuleiro, o inimigo vai se mexer para lá, senão for possivel vai tentar as proximas posiçoes de forma a proximar do jogador,mexendo prioritariamente a diagonal superior esquerda,diagonal superior direita, direita,esquerda,diagonal inferior direita, diagonal inferior esquerda,baixo*/
			if (invariante (e.inimigo[i].x,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x,e.inimigo[i].y-1)==1) 
			 {                                                           
			 e.inimigo[i].y= e.inimigo[i].y-1;                          
			 e.inimigo[i].x= e.inimigo[i].x;
			 }
		

		 else 
			
		      if(invariante (e.inimigo[i].x-1,e.inimigo[i].y-1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y-1)==1)
		      {
		      e.inimigo[i].x= e.inimigo[i].x-1;
		 	  e.inimigo[i].y= e.inimigo[i].y-1;
		 	  }	
		

		 else

		     if (invariante (e.inimigo[i].x+1,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y-1)==1)
		     {
			 e.inimigo[i].x= e.inimigo[i].x+1;
			 e.inimigo[i].y= e.inimigo[i].y-1;}

      	else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x+1;
			 e.inimigo[i].y=e.inimigo[i].y;
			 }

 		else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x-1;
			 e.inimigo[i].y=e.inimigo[i].y;
			 }


   
		else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y+1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x+1;
			 e.inimigo[i].y= e.inimigo[i].y+1;
			 }

 		else
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y+1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x-1;
			 e.inimigo[i].y= e.inimigo[i].y+1;
			 }



		else 
			
			if(invariante (e.inimigo[i].x,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x,e.inimigo[i].y+1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x;
			 e.inimigo[i].y= e.inimigo[i].y+1;
			 }


		}






    else



		/**caso o jogador esteja abaixo do inimigo*/ 
		if (e.jog.x == e.inimigo[i].x && e.jog.y > e.inimigo[i].y)
		{
			/**Se a posição inferior ao inimigo estiver livre e for uma posição dentro do tabuleiro, o inimigo vai se mexer para lá, senão for possivel vai tentar as proximas posiçoes de forma a proximar do jogador,mexendo prioritariamente a diagonal inferior direita,diagonal inferior esquerda, direita,esquerda,diagonal superior direita, diagonal superior esquerda,cima*/
			if(invariante (e.inimigo[i].x,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x,e.inimigo[i].y+1)==1) 
			 {                                                         
			 e.inimigo[i].x= e.inimigo[i].x;                           
			 e.inimigo[i].y= e.inimigo[i].y+1;
			 }	

			else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y+1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x+1;
			 e.inimigo[i].y= e.inimigo[i].y+1;
			 }

			 else 
 
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y+1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x-1;
			 e.inimigo[i].y= e.inimigo[i].y+1;
			 }	

			 else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x+1;
			 e.inimigo[i].y=e.inimigo[i].y;
			 }	
			
			 else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x-1;
			 e.inimigo[i].y=e.inimigo[i].y;
			 }	

			 else
			  
			  if (invariante (e.inimigo[i].x+1,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y-1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x+1;
			 e.inimigo[i].y= e.inimigo[i].y-1;
			 }


             else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y-1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y-1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x-1;
			 e.inimigo[i].y= e.inimigo[i].y-1;
			 }	

			 else 
			
			if (invariante (e.inimigo[i].x,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x,e.inimigo[i].y-1)==1)
			 {
			 e.inimigo[i].y= e.inimigo[i].y-1;
			 e.inimigo[i].x= e.inimigo[i].x;
			 }

		}



    else




    	/**caso o jogador esteja à direita do inimigo*/
		if (e.jog.x > e.inimigo[i].x && e.jog.y == e.inimigo[i].y)
		{
			/** Se a posição à direita do inimigo estiver livre e for uma posição dentro do tabuleiro, o inimigo vai se mexer para lá, senão for possivel vai tentar as proximas posiçoes de forma a proximar do jogador,mexendo prioritariamente a diagonal inferior direita,diagonal superior direita,cima,baixo,diagonal superior esquerda, diagonal inferior esquerda,esquerda*/
   			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y)==1)  
			 {                                                        
			 e.inimigo[i].x= e.inimigo[i].x+1;                        
			 e.inimigo[i].y=e.inimigo[i].y;
			 }
		
			else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y+1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x+1;
			 e.inimigo[i].y= e.inimigo[i].y+1;
			 }	

			else

             if (invariante (e.inimigo[i].x+1,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y-1)==1)
             {
			 e.inimigo[i].x= e.inimigo[i].x+1;
			 e.inimigo[i].y= e.inimigo[i].y-1;
			 }
			 else 
			
			if (invariante (e.inimigo[i].x,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x,e.inimigo[i].y-1)==1)
			 {
			 e.inimigo[i].y= e.inimigo[i].y-1;
			 e.inimigo[i].x= e.inimigo[i].x;
			 }
             
             else 
			
			if(invariante (e.inimigo[i].x,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x,e.inimigo[i].y+1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x;
			 e.inimigo[i].y= e.inimigo[i].y+1;
			 }

			else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y-1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y-1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x-1;
			 e.inimigo[i].y= e.inimigo[i].y-1;
			 }

 			 else

			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y+1)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x-1;
			 e.inimigo[i].y= e.inimigo[i].y+1;
			 }
			

		 	  else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y)==1)
			 {
			 e.inimigo[i].x= e.inimigo[i].x-1;
			 e.inimigo[i].y=e.inimigo[i].y;
			 }
		}






   else





		
		/**caso o jogador esteja à esquerda do inimigo*/
		if (e.jog.x < e.inimigo[i].x && e.jog.y == e.inimigo[i].y)
		{
		/** Se a posição à esquerda do inimigo estiver livre e for uma posição dentro do tabuleiro, o inimigo vai se mexer para lá, senão for possivel vai tentar as proximas posiçoes de forma a proximar do jogador,mexendo prioritariamente a diagonal superior esquerda,diagonal inferior esquerda, cima,baixo,diagonal superior direita, diagonal inferior direita,direita*/
		  	if(invariante (e.inimigo[i].x-1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y)==1) 
			{                                                           
			 	e.inimigo[i].x= e.inimigo[i].x-1;                       
			 	e.inimigo[i].y=e.inimigo[i].y;
			}	



			else 
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y-1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y-1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x-1;
			e.inimigo[i].y= e.inimigo[i].y-1;
			}	
			
			else
			
			if(invariante (e.inimigo[i].x-1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x-1,e.inimigo[i].y+1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x-1;
			e.inimigo[i].y= e.inimigo[i].y+1;
			}	



			else 
			
			if (invariante (e.inimigo[i].x,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x,e.inimigo[i].y-1)==1)
			{
			e.inimigo[i].y= e.inimigo[i].y-1;
			e.inimigo[i].x= e.inimigo[i].x;
			}
 			

 			else 
			
			if(invariante (e.inimigo[i].x,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x,e.inimigo[i].y+1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x;
			e.inimigo[i].y= e.inimigo[i].y+1;
			}	 


			 else

             if (invariante (e.inimigo[i].x+1,e.inimigo[i].y-1,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y-1)==1)
            {
			e.inimigo[i].x= e.inimigo[i].x+1;
			e.inimigo[i].y= e.inimigo[i].y-1;
			}

			 else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y+1,e)==0 && posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y+1)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x+1;
			e.inimigo[i].y= e.inimigo[i].y+1;
			}	

			else 
			
			if(invariante (e.inimigo[i].x+1,e.inimigo[i].y,e)==0&& posicao_valida (e.inimigo[i].x+1,e.inimigo[i].y)==1)
			{
			e.inimigo[i].x= e.inimigo[i].x+1;
			e.inimigo[i].y=e.inimigo[i].y;
			}	
			 


		}
	}
		
		if (e.jog.y>6) /** caso o jogador ultrapasse a 3 coluna do tabuleiro, os dragões começam a se mover... "função" que anula os movimentos dos dragões até que o jogador chegue à 3ª coluna */
			{
		e.inimigo[ceifeiro+escudeiro+wizard].x=aux1;
		e.inimigo[ceifeiro+escudeiro+wizard].y=aux2;
		e.inimigo[ceifeiro+escudeiro+wizard+1].x=aux3;
		e.inimigo[ceifeiro+escudeiro+wizard+1].y=aux4;
              }
/**Ciclo que determina como o ceifeiro ataca, e retira uma vida. */
	for (i=0; i<ceifeiro ;i++) 
	{
		/** Ceifeiro ataca para cima e para baixo*/
		if (e.inimigo[i].x==e.jog.x && ((e.inimigo[i].y==e.jog.y-1)||(e.inimigo[i].y==e.jog.y+1))) 
		{
			e.vidajog--;
			if (e.score > 0)
			e.score -= 1;
		}
	}
/**Ciclo que determina como o escudeiro ataca, e retira uma vida. */
	for (i=ceifeiro; i<escudeiro+ceifeiro && i>0 ;i++) 
	{
		/** Escudeiro ataca para a esquerda e para a direita*/
		if (e.inimigo[i].y==e.jog.y && ((e.inimigo[i].x==e.jog.x-1)||(e.inimigo[i].x==e.jog.x+1)))
		{
			e.vidajog--;
			if (e.score > 0)
				e.score -= 1;
		}
	}
	/**Ciclo que determina como o Wizard ataca, e retira uma vida. */
	for (i=escudeiro+ceifeiro; i<escudeiro+ceifeiro+wizard && i>0 ;i++) 
	{
		/** Wizard ataca para as 4 diagonais*/
		if ( (e.inimigo[i].x==e.jog.x-1 && ((e.inimigo[i].y==e.jog.y-1)||(e.inimigo[i].y==e.jog.y+1))) || (e.inimigo[i].x==e.jog.x+1 && ((e.inimigo[i].y==e.jog.y-1)||(e.inimigo[i].y==e.jog.y+1))) )
		{
			e.vidajog--;	
			if (e.score > 0)
				e.score -= 1;
		}
	}
/**Ciclo que determina como o Dragão ataca, e retira uma vida. */
	for (i=escudeiro+ceifeiro+wizard; i<escudeiro+ceifeiro+wizard+dragon && i>0 ;i++) 
	{
		/** Dragão ataca em todas as direções (tal como o jogador)*/
		if ( (e.inimigo[i].x==e.jog.x-1 && ((e.inimigo[i].y==e.jog.y-1)||(e.inimigo[i].y==e.jog.y+1))) || (e.inimigo[i].x==e.jog.x+1 && ((e.inimigo[i].y==e.jog.y-1)||(e.inimigo[i].y==e.jog.y+1))) || (e.inimigo[i].x==e.jog.x && ((e.inimigo[i].y==e.jog.y-1)||(e.inimigo[i].y==e.jog.y+1))) ||  (e.inimigo[i].y==e.jog.y && ((e.inimigo[i].x==e.jog.x-1)||(e.inimigo[i].x==e.jog.x+1))) )
		{
			e.vidajog--;	
			if (e.score > 0)
				e.score -= 1;
		}
	}
return e;
}
