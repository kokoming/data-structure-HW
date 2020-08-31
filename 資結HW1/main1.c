#include <stdio.h>
#include <stdlib.h>
/*資工四 4104056034 柯冠名 資料結構 HW.1*/
typedef struct {
	short int vert,horiz;
} offsets;

typedef struct {
	short int row,col,dir;
} element;

element stack[300];
short int top = 0;
//stack function: push, pop
void push(element s){			
	stack[++top].row=s.row;
	stack[top].col=s.col;
	stack[top].dir=s.dir;
	return;
}

element pop(){
	element delspot;
	delspot.dir=stack[top].dir;
	delspot.col=stack[top].col;
	delspot.row=stack[top--].row;
	return delspot;
}

int main(int argc, char *argv[]) {
	
	int i,j;
	int mark[17][17] = {0};		//if there was a point been visited, set to '1', else set to '0'
	char maze[17][17];			//used to store input file's information
	element position;
	
	FILE *fptr;
	fptr = fopen("input.txt","r");	//read input file
	
	if(fptr==NULL){
		printf("File can't open!");
		return 0;
	}
	else{		
		for(i=1;i<16;i++){
			for(j=1;j<17;j++){
				fscanf(fptr,"%c",&maze[i][j]);	//read input 
			}
		}
		fclose(fptr);
	}
	
	for(i=0;i<17;i++){							//prevent finding path out of 15*15 maze
		for(j=0;j<17;j++){
			if(i==0 || j==0 || i==16 || j==16){		
				maze[i][j] = '0';
				mark[i][j] = 1;
			}
		}
	}

	printf("Original Lake:\n");
	for(i=1;i<16;i++){
		for(j=1;j<16;j++){
			printf("%2c",maze[i][j]);	//print the original lake
			if(maze[i][j] == '0'){
				mark[i][j] = 1;			//set point '0' to "visited"
			}
		}
		printf("\n");
	}
	
	printf("\nResult Lake:\n");
	
	//array of moves for each direction
	offsets move[4];								
	//N
	move[0].horiz = 0;
	move[0].vert = -1;
	//E
	move[1].horiz = 1;
	move[1].vert = 0;
	//S
	move[2].horiz = 0;
	move[2].vert = 1;
	//W
	move[3].horiz = -1;
	move[3].vert = 0;
	
	short int nextCol,nextRow;
	int row,col,dir,maxArea=0,area=0,num=2,maxNum=0;

	for(i=1;i<16;i++){
		for(j=1;j<16;j++){
			stack[0].col = j;		//starting position
			stack[0].row = i;
			stack[0].dir = 0;
			while(top>-1){			//if stack not empty
				element position=pop();
				row = position.row;
				col = position.col;
				dir = position.dir;
				while(dir<4){
					nextRow = row + move[dir].vert;
					nextCol = col + move[dir].horiz;
					
					if(mark[nextRow][nextCol] == 0){	//finding next point which is not '0'
						position.row = row;
						position.col = col;
						position.dir = ++dir;
						push(position);
						row = nextRow;
						col = nextCol;
						dir = 0;
						mark[nextRow][nextCol] = num;			//set different mark number to different lake
						if(maze[nextRow][nextCol]=='1'){		//calculate depth of the lake
							area = area+1;
						}
						if(maze[nextRow][nextCol]=='2'){
							area = area+2;
						}
						if(maze[nextRow][nextCol]=='3'){
							area = area+3;
						}
						if(maze[nextRow][nextCol]=='4'){
							area = area+4;
						}
					}
					else{
						++dir;			//change direction
					}
				}
			}
			if(area > maxArea){			//after counting the depth of all lake, find the biggest one
				maxArea = area;			//store the total number of the biggest lake
				maxNum = num;			//store the mark of the biggest lake
			}
			area = 0;
			top = 0;
			num++;
		}
	}
	
	for(i=1;i<16;i++){
		for(j=1;j<16;j++){
			if(mark[i][j]==maxNum){			//printf the biggest lake
				printf("%2c",maze[i][j]);
			}	
			else{
				printf("%2c",'*');
			}
		}
		printf("\n");
	}
	
	printf("\nThe coordinates of the biggest lake:\n");
	for(i=1;i<16;i++){
		for(j=1;j<16;j++){
			if(mark[i][j]==maxNum){
				printf("(%d,%d)\n",i-1,j-1);
			}
		}
	}
	
	printf("\nThe total number of the water that is the biggest lake: ");
	printf("%d\n",maxArea);
	
	
	return 0;
}
