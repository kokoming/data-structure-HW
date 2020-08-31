#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct listNode *listPointer;
struct listNode{
	int vertex;			//list index
	listPointer next;	//next vertex in a list
	char name[11];		//city's name
	int weight;			//city's throughput
};

listPointer city[50];
listPointer x[50];
listPointer y[50];

typedef struct{
	int vertex;
	listPointer next;
	int pre;	//這裡
}element;

element queue[100];
int front=-1,rear=-1;

void addq(element item){
	queue[++rear].vertex=item.vertex;
	queue[rear].pre=item.pre;	//這裡
	return;
}
element deq(){
	element point;
	point.vertex=queue[++front].vertex;
	point.pre = queue[front].pre;	//這裡
	return point;
}
void bfs(char *start,char *target){
	printf("All the paths from city %s to city %s :\n",start,target);
	int index_a,index_b,i,nextVertex,vertex,find=0;
	for(i=0;i<50;i++){
		if(strcmp(city[i]->name,start)==0){
			index_a=i;
		}
		else if(strcmp(city[i]->name,target)==0){
			index_b=i;
		}
	}
	
	element position;
	listPointer tmp;	//這裡
	position.vertex=index_a;
	position.pre = 0;	//這裡
	addq(position);
	
	while(front!=rear){
		position=deq();
		int pre_o = front;	//這裡
		vertex=position.vertex;
		tmp = city[vertex];	//這裡
	//	printf("de:%s\n",city[position.vertex]->name);

		while(tmp->next!=NULL){	//這裡
			nextVertex=tmp->next->vertex;	//這裡
		//	printf("%d\n",nextVertex);
			if(nextVertex==index_b){
				find=1;
				/*這裡*/
			//	printf("end\n");
				printf("path:%s,",city[nextVertex]->name);
				int t=pre_o;
				do
				{
					printf("%s,",city[queue[t].vertex]->name);
					t = queue[t].pre;
				}while(t!=0);
				printf("%s",city[0]->name);
				printf("\n");
				break;
			}
			else{	
				position.vertex=nextVertex;
				position.pre = pre_o;	
				tmp = tmp->next;	//這裡
				addq(position);
			}
		}
	} 
	if(find==0){
		printf("NULL\n");
	}
}

void addedge(char *a,char *b){
	int index_a,index_b,i,again=0;
	for(i=0;i<50;i++){
		if(strcmp(city[i]->name,a)==0){
			index_a=i;
		}
		else if(strcmp(city[i]->name,b)==0){
			index_b=i;
		}
	}
	listPointer nt = city[index_a]->next;
	while(nt!=NULL)
	{
		if(strcmp(nt->name,b)==0){
			again=1;
		}
		nt = nt->next;	
	}
	if(again==0){
		listPointer newNode=(listPointer)malloc(sizeof(struct listNode));
		newNode->vertex=index_b;
		newNode->next=city[index_a]->next;
		strcpy(newNode->name,city[index_b]->name);	//這裡
		city[index_a]->next=newNode;
	}		
}

void printMap(){
	printf("Map:\n");
	int j=0,i,a,b,min;
	char tmp[10];
	for(i=0;i<50;i++){
		if(strcmp(city[i]->name,"")==0)
		break;
				
		listPointer nt = city[i]->next;
		while(nt!=NULL)
		{	
			strcpy(x[j]->name,city[i]->name);		//每個list的開頭城市 
			strcpy(y[j]->name,nt->name);			//該城市的next 
		//	printf("%s,%s\n",x[j]->name,y[j]->name);
			nt = nt->next;	
			j++;
		}
	}
	
	for(a=0;a<j;a++){											
		min = a;
		for(b=a+1;b<j;b++){
			if(x[b]->name[0] < x[min]->name[0]){	//對開頭城市做ASCII排序 
				min=b;
			}
		}
		strcpy(tmp,x[min]->name);
		strcpy(x[min]->name,x[a]->name);
		strcpy(x[a]->name,tmp);
						
		strcpy(tmp,y[min]->name);
		strcpy(y[min]->name,y[a]->name);
		strcpy(y[a]->name,tmp);
	}
	for(a=0;a<j;a++){
		min = a;
		for(b=a+1;b<j;b++){
			if(x[b]->name[0] == x[min]->name[0]){	//若開頭城市相同，對他的next做排序 
				if(y[b]->name[0] < y[min]->name[0]){	
					min=b;
				}
			}
		}	
		strcpy(tmp,y[min]->name);
		strcpy(y[min]->name,y[a]->name);
		strcpy(y[a]->name,tmp);
	}

	for(a=0;a<j;a++){							
		printf("%s,%s\n",x[a]->name,y[a]->name);
	}
}

void reset(){
	int i;
	for(i=0;i<50;i++){
		city[i]->vertex = i;	//這裡
		city[i]->next = NULL; 	//這裡
		city[i]->weight='\0';	
		strcpy(city[i]->name,"");
		strcpy(x[i]->name,"");
		strcpy(y[i]->name,"");
	}
	while(front!=rear){
		deq();
	}
	front=-1;
	rear=-1;
}

int main(int argc, char *argv[]) {
	
	int i;
	for(i=0;i<50;i++){
		city[i]=(listPointer)malloc(sizeof(struct listNode));
		city[i]->vertex = i;	//這裡
		city[i]->next = NULL; 	//這裡	
		strcpy(city[i]->name,"");
		x[i]=(listPointer)malloc(sizeof(struct listNode));
		strcpy(x[i]->name,"");
		y[i]=(listPointer)malloc(sizeof(struct listNode));
		strcpy(y[i]->name,"");
	}
	
	char input[100000];
	char start[10];
	char end[10];
	char prev[10];
	

	char *cut=",->";
	char *str;
	int count=0,setTarget=0,goods=0,ending=0;
	int citynum=0,namecheck=0,setprev=0;
	
	FILE *fptr;
	fptr = fopen("input.txt","r");
	if(fptr==NULL){
		printf("File can't open.");
		return 0;
	}
	else{
		while(!feof(fptr) && ending==0){
			fscanf(fptr,"%s",&input);
			str=strtok(input,cut);
			while(str!=NULL){
				if(setTarget==0){
					if(count==0){
						strcpy(start,str);	//start city
					}
					else if(count==1){
						strcpy(end,str);	//traget city
					}
					else{
						goods=atoi(str);
						setTarget=1;
						count=0;
					//	printf("%s,%s,%d\n",city[0]->name,city[1]->name,goods);
					}			
				}
				else{
					if(strcmp(str,"0")==0){	
						printMap();
						bfs(start,end);
						reset();
						citynum=0;			
						setTarget=0;
						count=-1;
					}
					else if(strcmp(str,"1")==0){
						printMap();
						bfs(start,end);
						ending=1;
					}
					else{								 
					//	printf("%s,%d,",str,count);	
						if(count%2==1){				//城市名:count=奇數   
							for(i=0;i<50;i++){
								if(strcmp(city[i]->name,str)==0){
									namecheck=1;		//判斷城市名字有重複 
								}
							}
							if(namecheck==0){			//城市名字沒重複就新增 
								strcpy(city[citynum]->name,str);
							}
							
							if(setprev==0){
								strcpy(prev,str);		//每一行的第一個城市 
							}
							else if(setprev==1){
								//printf("%s->%s\n",prev,str);
								addedge(prev,str);
								strcpy(prev,str);
							}
							setprev=1;
						}
						else if(count%2==0){		//城市負載量:count=偶數 
							if(strcmp(city[citynum]->name," ")==1){
								city[citynum]->weight=atoi(str);
							//	printf("%d,%s,%d\n",citynum,city[citynum]->name,city[citynum]->weight);
								citynum++;	
							}
						}
						namecheck=0;	
					}
				}
				count++;
				str=strtok(NULL,cut);
			}
			setprev=0;
		}
		/*這裡*/
		/*for(i=0;i<50;i++){
			if(strcmp(city[i]->name,"")==0)
				break;
			printf("city:%d\n",i);
			printf("city name:%s\n",city[i]->name);
			printf("city attach:");
			listPointer nt = city[i]->next;
			while(nt!=NULL)
			{
				printf("%s,",nt->name);
				nt = nt->next;	
			}
			printf("\n\n");
		}*/
		
		fclose(fptr);
	}

	return 0;
}
