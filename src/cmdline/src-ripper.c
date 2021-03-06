#include "stdio.h"
#include <stdlib.h>

#include <string.h>

#include "memory.h"

#define LINE_LENGTH 80

char src[MEMORY_SIZE][LINE_LENGTH];
void init_src(){
  int i,j;
  for(i=0;i<MEMORY_SIZE;i++){
	for(j=0;j<LINE_LENGTH;j++){
	  src[i][j]='\0';
	}
  }
}
void src_print(int adr){
  if(adr<MEMORY_SIZE){
	printf("%s\n",src[adr]);
  }
}
void src_table(int adr){
  int i;
  for(i=0;i<4;i++)
	if(adr+i<MEMORY_SIZE){
	  printf("src: %s\n",src[adr+1]);
	}
}

char *src_line(int adr){
  if(adr<MEMORY_SIZE){
	return src[adr];
  }
  return NULL;
}

char * src_str(int adr){
  if(adr<MEMORY_SIZE){
	return src[adr]+4;
  } else
	return NULL;
}


void src_read(char *fname){
  char line[LINE_LENGTH];
  int adr;
  FILE *stream;
  int i=0;

  if(!strcmp("-", fname)){
    stream = stdin;
  } else if ((stream = fopen( fname,"r")) == NULL){
    printf("Can't open %s\n", fname);
    return;
  }
  while(!feof(stream)){
    
	fgets(line, LINE_LENGTH, stream);

	sscanf(line, " %04x ", &adr);
	//	printf("Address: %04x line: %s\n", adr,line);
	if(adr>MEMORY_SIZE){
	  printf("Illegal Address %i\nSkipping line ..\n", adr);
	} else {
	  
	  for(i=0;i<LINE_LENGTH; i++){
		src[adr][i] = line[i];
	  }
	}
  }
  /*  for(i=0;i<4;i++){
	printf("\n src: %s", src[0x0600+i]);
	}
  */
}

