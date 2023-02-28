#include<iostream>
#include<stdio.h> 
#include<stdlib.h> 
#include<math.h>
#include<time.h>
#include<cstdlib>
#include<ctime>

using namespace std;

const int rows = 6;
const int cols = 7;
const int maxdepth = 6;
//min and max are used for alpha beta pruning
const int maxPrune = 1;
const int minPrune = -1;

void printB(char b[][cols]);
void move(char c, char b[][cols]);
void makeMove(char b[][cols], int m, char c);
int evalB(char b[][cols]);
int chooseMove(char b[][cols]);
int mini(char b[][cols], int c, int depth);
int maxi(char b[][cols], int c, int depth);

int main(){
  // const int rows = 6;
  // const int cols = 7;
  char board[6][7];

  //then we initialize the board
  int i,j;
  for(i=0; i<rows; i++){
    for(j=0; j<cols; j++){
      board[i][j] = 'O';
    }
  }

  int w;
  int mv;//mv is the move of AI
  while(true){

    printB(board);
    //    move('A', board);
    mv = chooseMove(board);
    makeMove(board, mv, 'A');

    w = evalB(board);
     if( w == 1){
       printf("AI's move:\n");
       printB(board);
      printf("AI wins!\n");
      return 0;
     }  
 
     printf("AI's move:\n");
    printB(board);

    move('P', board);
    w = evalB(board);
    if( w==-1){
      printB(board);
      printf("Player wins!\n");
      return 0;
    }


  }

}

void printB(char b[][cols]){

  int i, j;
  for(i=0; i<cols; i++){
    printf("%d|", i);
  }
  printf("\n");
  for(i=0; i< rows; i++){
    for(j=0; j< cols; j++){
      printf("%c|", b[i][j]);
    }
    printf("\n");
  }
  printf("\n");

}

void move(char c,char b[][cols]){

  int m;
  do{
    printf("Please enter a move:");
    scanf("%d", &m);
    printf("\n");
  }while( m>cols || m<0 || b[0][m] != 'O');
  makeMove(b, m, c);

}

void makeMove(char b[][cols], int m, char c){

  int r;
  for(r=6; r>=0; r--){
    if(b[r][m] == 'O'){
      b[r][m] = c;
      return;
    }
  }

}

int evalB(char b[][cols]){

  int counta=0;
  int countp=0;

  int i,j;
  //checks for horizontal
  for(i=0; i<rows; i++){
    for(j=0; j< cols; j++){
      if(b[i][j]=='A'){
	counta++;
	countp=0;
      }
      else if(b[i][j] == 'P'){
	counta=0;
	countp++;
      }
      else{
	counta=countp=0;
      }

      if(counta == 4)
	return 1;
      if(countp == 4)
	return -1;
    }
    counta=countp=0;
  }

  counta=countp=0;
  //checks for vertical
  for(j=0; j<cols; j++){
    for(i=0; i<rows; i++){
      if(b[i][j]=='A'){
        counta++;
        countp=0;
      }
      else if(b[i][j] == 'P'){
        counta=0;
        countp++;
      }
      else{
        counta=countp=0;
      }

      if(counta == 4)
        return 1;
      if(countp == 4)
        return -1;
    }
    counta=countp=0;
  }

  //checks for diagonal
  //diagonal up

  int ii, jj,loop;
  for(i=5; i>=3; i--){
    for(j=0; j<=3; j++){

      for(ii=i, jj=j,loop=0;loop<4; loop++, ii--, jj++){

	if(b[ii][jj]=='A'){
	  counta++;
	  countp=0;
	}
	else if(b[ii][jj] == 'P'){
	  counta=0;
	  countp++;
	}
	else{
	  counta=countp=0;
	}

	if(counta == 4)
	  return 1;
	if(countp == 4)
	  return -1;
      }
      counta=countp=0;
    }

  }
  //diagonal down
  for(i=0; i<=2; i++){
    for(j=0; j<=3; j++){

      for(ii=i, jj=j,loop=0;loop<4; loop++, ii++, jj++){

        if(b[ii][jj]=='A'){
          counta++;
          countp=0;
        }
        else if(b[ii][jj] == 'P'){
          counta=0;
          countp++;
        }
        else{
          counta=countp=0;
        }

        if(counta == 4)
          return 1;
        if(countp == 4)
          return -1;
      }
      counta=countp=0;
    }

  }
  //Then we check if it's a tie
  for(j=0; j<cols; j++){
    if(b[0][j] == 'O')
      return 0;
  }

  printf("It's a tie!\n");
  exit(0);

}

int chooseMove(char b[][cols]){
  int bestMove, v;
  int bestValue = -99;

  int j,i;
  int randM[7];
  for(j=0,i=0; j<cols; j++){

    if(b[0][j] == 'O'){//make sure column is not full
      v = mini (b, j, 1);
      printf("%d value %d\n", j, v);
      if(v > bestValue){
	bestValue = v;
	bestMove = j;
      }
      if(v==0){
	randM[i]=j;
	i++;
      }

    }
  }
  //randomize our moves
  if(bestValue == 0 ){
    srand((unsigned)time(NULL));
    bestMove = randM[rand()%(i)];
  }

  return bestMove;
}

int mini(char b[][cols], int c, int depth){
  //firstly we make a copy of the board
  char test[rows][cols];
  int i,j;
  int v;//evaluation
  for(i=0; i<rows; i++){
    for(j=0; j<cols; j++){
      test[i][j] = b[i][j];
    }
  }

  makeMove(test, c, 'A');
  v = evalB(test);
  if(v == 1)
    return v;
  if(v == -1)
    return v;
  if(depth == maxdepth)
    return v;
  
  int worstValue = 99;
  for(j=0; j<cols; j++){
    if(b[0][j] == 'O'){
      v = maxi(test, j, depth+1);
      if( v < worstValue)
	 worstValue = v;

      //alpha beta pruning
      if( v <= minPrune)
	return minPrune;

    }

  }

  return worstValue;

}
int maxi(char b[][cols], int c, int depth){
  //firstly we make a copy of the board                                                                                                                                                                     
  char test[rows][cols];
  int i,j;
  int v;//evaluation                                                                                                                                                                                        
  for(i=0; i<rows; i++){
    for(j=0; j<cols; j++){
      test[i][j] = b[i][j];
    }
  }

  makeMove(test, c, 'P');
  v = evalB(test);
  if(v == 1)
    return v;
  if(v == -1)
    return v;
  if(depth == maxdepth)
    return v;

  int bestValue = -99;
  for(j=0; j<cols; j++){
    if(b[0][j] == 'O'){
      v = mini(test, j, depth+1);
      if( v > bestValue)
        bestValue = v;

      //alpha beta pruning
      if( v >= maxPrune)
	return maxPrune;

    }
  }

  return bestValue;

}
