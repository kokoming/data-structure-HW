#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node *treePointer;
typedef struct stack *stackPointer;

struct node{
	int value;			//node's weight
	char name[5];		//node's name
	treePointer leftchild,rightchild,parent; 
};
struct stack{
	char* data;
	stackPointer link;
};

stackPointer top[100];  //max stacks
int stacktop=-1;		//stack empty

void push(char* item){
	stackPointer temp;
	temp = (stackPointer)malloc(sizeof(*temp));
	temp->data = item;
	top[++stacktop] = temp;
}

char* pop(){
	char* none = NULL;
	if(stacktop<=-1){
		printf("no existing data in the stack!\n");
		return none;
	}
	stackPointer temp = top[stacktop];
	char* item;
		
	item = temp->data;
	stacktop--;
	free(temp);
	return item;
}

int total=0;	//counting path's weight
int minweight=0;	//find the minimum path

void traversal(treePointer ptr){		
	if(ptr){
		total = total + ptr->value;
		traversal(ptr->parent);			//finding path according to its parent node
		if(ptr->parent==NULL){			//if finding root
			if(minweight==0){
				minweight = total;
			}
			else if(minweight > total){	
				minweight = total;		//store minimum weight
			}
			total = 0;
		}
	}
}
void minpath(treePointer ptr){
	if(ptr){
		total = total + ptr->value;
		push(ptr->name);
		minpath(ptr->parent);
		if(ptr->parent==NULL && total==minweight){
			while(stacktop>-1){
				printf("%s",pop());		//print the minimum weight path
			}
			printf("\n");
			total = 0;
		}
		else if(ptr->parent==NULL && total!=minweight){
			while(stacktop>-1){
				pop();
			}
			total = 0;
		}
	}
}

int main(int argc, char *argv[]) {
	
	int i;
	
	treePointer leaf[100];
	for(i=0;i<100;i++){
		leaf[i]=(treePointer)malloc(sizeof(struct node));
		strcpy(leaf[i]->name,"");
		leaf[i]->leftchild=NULL;
		leaf[i]->rightchild=NULL;
		leaf[i]->parent=NULL;
	}
	
	char input[100000];
	char *str;
	char shit[5] = "(),";
	int count=0, num=1,weight=0;
	
	FILE *fptr;
	fptr = fopen("input2.txt","r");	//read input file
	
	if(fptr==NULL){
		printf("File can't open!");
		return 0;
	}
	else{		
		while(!feof(fptr)){
			fscanf(fptr,"%s",&input);	//read input
			
			str = strtok(input,shit);	//string token			
			while(str!=NULL){			
				if(count%2==0){			//set node's name and build the tree
					strcpy(leaf[num]->name,str);
				//	printf("%s,",leaf[num]->name);
					if(num>1){
						if(num%2==0){
							leaf[num/2]->leftchild = leaf[num];
							leaf[num]->parent = leaf[num/2];
						}
						else{
							leaf[(num-1)/2]->rightchild = leaf[num];
							leaf[num]->parent = leaf[(num-1)/2];
						}
					}
 
				}
				else if(count%2==1){		//set node's weight
					leaf[num]->value = atoi(str);
				//	printf("%d\n",leaf[num]->value);
					num++;
				}		
				count++;
				str = strtok(NULL,shit);
			}
		//	printf("%s",leaf[2]->parent->name);
			for(i=1;i<num;i++){		//find leaf node and starting traverse to get minimum weight
				if(leaf[i]->leftchild==NULL && leaf[i]->rightchild==NULL){	
					traversal(leaf[i]);
				}
			}
			
			printf("Path:");
			for(i=1;i<num;i++){		//according to minimum weight, finding its path
				if(leaf[i]->leftchild==NULL && leaf[i]->rightchild==NULL){
					minpath(leaf[i]);
				}
			}

			printf("Weight:%d\n",minweight);
			printf("\n");
			
			for(i=1;i<=num;i++){				//clear all information
				strcpy(leaf[i]->name,"");
				leaf[i]->leftchild = NULL;
				leaf[i]->rightchild = NULL;
				leaf[i]->parent = NULL;
			}
			minweight=0;
			count=0;
			num=1;
		}
		fclose(fptr);
	}
	
	return 0;
}
