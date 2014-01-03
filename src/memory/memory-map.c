/*
  memory_map.c
  initial: 3.7.2013
*/

char * irq;
char * dbr;
char * abrl;
char * abrh;
char * rw;

access_bus(char* adr1, char* adr2, int rw,char*data){
  if(startaddr>=cia_register && startaddr<=cia_register+15){
    cp_register(cia_read(start_addr%15),dbr);
  }
