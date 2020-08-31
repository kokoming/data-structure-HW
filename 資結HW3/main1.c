#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <io.h>
typedef struct node *treePointer;
struct node{
	int freq;			//node's frequency
	int asc;			//ascii number
	int visit;			//check whether leaf node being visited 
	char code[100];		//store node's huffman code
	treePointer leftchild,rightchild; 
};

void traversal(treePointer ptr, int len){		
	static int a[100];
	char b[100]={};
	int i,j;
	if(ptr){
		if(ptr->leftchild==NULL && ptr->rightchild==NULL){
		//	printf("%c:",ptr->asc);		//print the character
			for(i=0;i<len;i++){
			//	printf("%d\n", a[i]);		//print its huffman code
				if(a[i]==1){
					b[i]='1';
				}else if(a[i]==0){
					b[i]='0';
				}
			}			
			strcpy(ptr->code,b);		//store its huffman code
		}
		else{
			a[len]=0;
			traversal(ptr->leftchild, len+1);
			a[len]=1;
			traversal(ptr->rightchild, len+1);
		}
	}
}
void printTree(treePointer ptr){		//print the huffman tree
	if(ptr){
		printf("%d",ptr->freq);
		if(ptr->leftchild!=NULL || ptr->rightchild!=NULL){
			printf("(");
			printTree(ptr->leftchild);
			if(ptr->rightchild!=NULL){
				printf(",");
			}
			printTree(ptr->rightchild);
			printf(")");
		}
		
	}
}
int main(int argc, char *argv[]) {
	
	int i=0,j=0;
	int termfreq[127]={0};		//character frequency
	char input[500000]={};		//file's input
	
	FILE *fptr;
	fptr = fopen("input1.txt","r");
	
	int fno,fsize;			//calculate input file size
	fno = fileno(fptr);
	fsize = filelength(fno);
	printf("file size before compression:%d bytes\n",fsize);
	
	if(fptr==NULL){
		printf("File can't open!");
		return 0;
	}
	else{
		while(!feof(fptr)){
			fscanf(fptr,"%c",&input[i]);
			i++;
		}
		fclose(fptr);
	}
	
	while(input[j]!='\0'){		//check and store term frequency
		++termfreq[input[j]];	
		++j;
	}
	/*
	printf("¦r¼Æ²Î­p:\n");
	for(i=32;i<126;i++){
		printf("%c:%d\n",i,termfreq[i]);
	}
	*/
	
	treePointer leaf[200];  //store ascii and its parent node
	for(i=0;i<200;i++){		//initialize
		leaf[i]=(treePointer)malloc(sizeof(struct node));
		leaf[i]->asc=0;
		leaf[i]->freq=0;
		leaf[i]->visit=0;
		leaf[i]->leftchild=NULL;
		leaf[i]->rightchild=NULL;
		strcpy(leaf[i]->code,"");
	}
	
	int k=0,min1=500000,min2=500000,idmin1,idmin2;
	for(i=32;i<127;i++){				//ascii number 32~126
		if(termfreq[i]!=0){
			leaf[k]->asc=i;				//ascii number
			leaf[k]->freq=termfreq[i];	//its frequency
			k++;
		}
	}
	int leafnum = k;
	int p=k;	//parent node starting position

	for(i=0;i<leafnum;i++){		
		for(j=0;j<200;j++){		//find the least two frequency node's id
			if(leaf[j]->freq!=0 && leaf[j]->visit==0){	
				if(leaf[j]->freq < min1){
					min2 = min1;
					min1 = leaf[j]->freq;		//min frequency
					idmin2 = idmin1;			//its node id
					idmin1=j;	
				}else if(leaf[j]->freq < min2){
					min2 = leaf[j]->freq;		//second min frequency
					idmin2=j;					//its node id
				}
			}
		}
	
		if(min1!=500000 && min2!=500000){		//link two nodes with same parent node
		//	printf("%d,%d\n",leaf[idmin1]->freq,leaf[idmin2]->freq);
			leaf[p]->leftchild = leaf[idmin1];
			leaf[p]->rightchild = leaf[idmin2];
			leaf[p]->freq = leaf[idmin1]->freq + leaf[idmin2]->freq;
			leaf[idmin1]->visit=1;				//set node to visited 
			leaf[idmin2]->visit=1;
			p++;
		}
		else{
			break;
		}
		
		min1=500000;	
		min2=500000;
	}
	
	treePointer root;
	root=(treePointer)malloc(sizeof(struct node));
	root=leaf[p-1];		//setting root of the huffman tree
	traversal(root,0);	
//	printTree(root);
	printf("\n");
	/*
	for(i=0;i<leafnum;i++){
		printf("%c:",leaf[i]->asc);
		printf("%s\n",leaf[i]->code);
	}
	*/
	
	int w=0,bufnum=0;
	char buf[500000]={};
	FILE *output;
	output = fopen("output.txt","w");
	
	if(output==NULL){
		printf("open fail!");
		return 0;
	}
	while(input[w]!='\0'){
		for(i=0;i<leafnum;i++){
			if(input[w]==leaf[i]->asc){
				break;
			}
		}
		strcat(buf,leaf[i]->code);	//change input character to huffman code
	//	printf("%s",leaf[i]->code);
		w++;						//and store in buf[]
	}
	while(buf[bufnum]!='\0'){
		bufnum++;					//compute length of buf[]
	}
	
	int times=bufnum/8;
	char zip;
	for(i=0;i<times;i++){
		for(j=i*8;j<i*8+8;j++){
			zip+=(buf[j]-48)*(pow(2,7-(j%8)));	//make 8bits in one char
		}
		fprintf(output,"%c",zip);				//write in output.txt
		zip=0;
	}
	for(i=times*8;i<times*8+8;i++){			//remainder,if (buffnum %8)!=0
		if(buf[i]!='\0'){
			zip+=(buf[i]-48)*(pow(2,7-(i%8)));
		}else{
			zip+=(0)*(pow(2,7-(i%8)));		//fill 8bits with 0
		}
		zip=0;
	}
	fprintf(output,"%c",zip);
	fclose(output);
	
	output = fopen("output.txt","r");
	fno = fileno(output);
	fsize = filelength(fno);
	printf("\nfile size after compression:%d bytes\n",fsize);
	
	fclose(output);

	
	return 0;
}
