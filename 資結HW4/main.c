#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int map[12][12]={0};
int horiz[8]={1,2,2,1,-1,-2,-2,-1};		//horizontal direction
int vert[8]={-2,-1,1,2,2,1,-1,-2};		//vertical direction
int step3=0;							//compute step in mode 3
typedef struct {
	int row,col,dir,pre;				//'pre' stores previous node
} element;

element queue[100];
int rear=-1,front=-1;

void addq(element item){
	queue[++rear].row=item.row;
	queue[rear].col=item.col;
	queue[rear].dir=item.dir;
	queue[rear].pre=item.pre;
	return;
}
element deleteq(){
	element delspot;
	delspot.col=queue[++front].col;
	delspot.row=queue[front].row;
	delspot.dir=queue[front].dir;
	delspot.pre=queue[front].pre;
	return delspot;
}

int cleanMap(){					//map initialization
	int i,j;
	for(i=0;i<12;i++){
		for(j=0;j<12;j++){
			if(i==0||i==1||i==10||i==11||j==0||j==1||j==10||j==11){
				map[i][j]=1;	//prevent finding path out the 8x8 map
			}
			else{
				map[i][j]=0;
			}
		}
	}
}

void bfs(int xstart,int ystart,int xend,int yend,int mode){
	int nextRow,nextCol,find=0,row,col,dir,i,end,pre;
	
	element position;
	position.col=xstart+1;	//set starting point
	position.row=ystart+1;
	position.dir=0;
	position.pre=0;
	addq(position);
	map[ystart+1][xstart+1]=1;	//set to visited=1, '0' means unvisited
	
	while(front!=rear && find==0){
		
		position=deleteq();
		row = position.row;
		col = position.col;
		dir = 0;
		pre = front;
		//printf("deq = %d,%d\n",col-1,row-1);
		while(dir<8 && find==0){
			nextRow = row + vert[dir];				
			nextCol = col + horiz[dir];	
			if(nextRow == yend+1 && nextCol == xend+1){			
				//printf("ended:%d,%d\n",nextCol-1,nextRow-1);
				find=1;											//if find target
				end=pre;
				//printf("pre = %d\n",pre);
			}	
			else if(map[nextRow][nextCol]==0){	
				//printf("%d,%d\n",nextCol-1,nextRow-1);		
				map[nextRow][nextCol] = 1;										
				position.row = nextRow;
				position.col = nextCol;
				position.dir = ++dir;
				position.pre = pre;
				//printf("pre = %d\n",position.pre);
				addq(position);
			}
			else{
				++dir;	
			}
						
		}
	}
	if(find==1){
		//for (i=0;i<100;i++)
		//	printf("%d,%d\n",queue[i].col-1,queue[i].row-1);
		int j=2,step=1;
		int path[10000];										//record answer path
		path[0]=yend;
		path[1]=xend;
		while(end!=queue[end].pre){								//backtracking from end to the start
		//	end=front;
		//	printf("%d,%d\n",queue[end].col-1,queue[end].row-1);
			path[j]=queue[end].row-1;
			path[j+1]=queue[end].col-1;
			j=j+2;
			step++;
			//printf("end=%d\n",end);
			//printf("pre=%d\n",queue[end].pre);
			end=queue[end].pre;
		}
		path[j]=ystart;
		path[j+1]=xstart;
		if(mode!=3){								//print mode1 and mode2 path
			printf("Path: ");
			for(i=j+1;i>0;i=i-2){
				printf("(%d,%d)",path[i],path[i-1]);
			}
			printf("\nStep: %d\n",step);
		}
		else{
			step3=step3+step;						//compute steps in mode 3
		}
	}
	else{
		printf("NULL\n");	//no path return NULL
	}
	front = -1;
	rear = -1;
	cleanMap();
	while(front!=rear){		//clear queue
		deleteq();
	}
}

int main(int argc, char *argv[]) {
	int i=0,j=0;
	char input[100000];
	int Testnum=0,setTest=0,Testcount=0;		//record test number
	int mode=0,count=0;
	int xstart,ystart,xend,yend;				//starting point and ending point
	int blocknum,knightnum;						//block number and knight number
	int knight[200]={0};						//store knight positions
	
	for(i=0;i<12;i++){
		for(j=0;j<12;j++){
			if(i==0||i==1||i==10||i==11||j==0||j==1||j==10||j==11){
				map[i][j]=1;
			}
		}
	}
	
	FILE *fptr;
	fptr = fopen("input.txt","r");	//read input file
	
	if(fptr==NULL){
		printf("File can't open!");
		return 0;
	}
	else{
		while(fgets(input,100,fptr)!=NULL){
			if(setTest==0){
				Testnum=input[0]-48;
			//	printf("%d",Testnum); 
				setTest=1;
			}
			else{
				if(input[0]=='1' && count==1){
					mode=1;
					printf("Mode 1\n"); 
				}
				else if(input[0]=='2' && count==1){
					mode=2;
					printf("Mode 2\n"); 
				}
				else if(input[0]=='3' && count==1){
					mode=3;
					printf("Mode 3\n"); 
				}
			}
			
			if(mode==1 && count!=1){
				xstart=input[0]-48;
				ystart=input[2]-48;
				xend=input[4]-48;
				yend=input[6]-48;
				if(xstart==0 && ystart==0 && xend==0 && yend==0){	//end of mode1
					mode=0;
					count=0;
					Testcount++;
					printf("\n");
				}else{
				    bfs(xstart,ystart,xend,yend,1);
				}	
			}
			else if(mode==2 && count!=1){
				if(count==2){
					blocknum=input[0]-48;
				//	printf("blocknum: %d\n",blocknum);
				}
				else if(count==3){
					for(i=0;i<blocknum*2;i=i+2){
					//	printf("%d,%d\n",input[i*2]-48+1,input[(i+1)*2]-48+1);
						map[input[(i+1)*2]-48+1][input[i*2]-48+1]=1;
					}
				}
				else{
					xstart=input[0]-48;
					ystart=input[2]-48;
					xend=input[4]-48;
					yend=input[6]-48;
					if(xstart==0 && ystart==0 && xend==0 && yend==0){	//end of mode2
						mode=0;
						count=0;
						Testcount++;
						printf("\n");
					}else{
						bfs(xstart,ystart,xend,yend,2);
					}
				}
			}
			else if(mode==3 && count!=1){
				if(count==2){
					knightnum=input[0]-48;
				//	printf("knightnum: %d\n",knightnum);
				}
				else if(count==3){
					for(i=0;i<knightnum*2;i++){
						knight[i]=input[i*2]-48;
					//	printf("knight: %d ",knight[i]);
					}
				}
				else{
					xend=input[0]-48;
					yend=input[2]-48;
					int k=0,distance=10000,idx,idy;
					while(k<knightnum){
						for(i=0;i<knightnum*2;i=i+2){		
							if(knight[i]!=0){
								map[knight[i+1]+1][knight[i]+1]=1;				//continue setting knight's positions to visited
							//	printf("block:%d,%d\n",knight[i],knight[i+1]);
							}
							if((pow((knight[i]-xend),2)+pow((knight[i+1]-yend),2))<=distance){	//find the knight who nearest to the end
								distance=(pow((knight[i]-xend),2)+pow((knight[i+1]-yend),2));
								xstart=knight[i];
								ystart=knight[i+1];
								idx=i;
								idy=i+1;
							}
						}
						bfs(xstart,ystart,xend,yend,3);
						knight[idx]=0;
						knight[idy]=0;
						distance=10000;
						k++;
					}
					printf("Step: %d\n",step3);
					printf("\n");
					step3=0;
					mode=0;
					count=0;
					Testcount++;
				}
			}
			
			if(Testcount==Testnum){		//if achieve test numbers
				break;
			}
			
			while(input[j]!='\0'){
				input[j]='\0';
				j++;
			}
			j=0;
			count++;
		}
		fclose(fptr);
	}

	return 0;
}
