#include <stdio.h>
#include <stdlib.h>
#include "util.h"

#ifdef _WIN32
// support for useless peace of crap OS
#include <string.h>

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

#else
#include <editline/readline.h>
#include <histedit.h>
#endif
#define LENGTH(X)               (sizeof(X) / sizeof (X[0]))





// TYPES
typedef void (* CLI_func)(char* input);

typedef struct {
	char* name;
	CLI_func func;
} CLI_t;

// Variables
uint32_t ip;
uint32_t mask= 24; // default
Subnet* subnets;
int subnets_cn = 0;
int subnets_sz = 0;
// Functions
void parseString(char* input);



void help(char* input);
void setIP(char* input);
void nm_print(char* input);
void info(char* input);
void addSubnet(char* input);
void subneting(char* input);

const CLI_t commands[]={
	{"help",help},
	{"ip",setIP},
	{"print",nm_print},
	{"info",info},
	{"addsubnet",addSubnet},
	{"subnets",subneting}
};

const int CMD_SIZE = sizeof(commands)/ sizeof(CLI_t);


int main(){
	puts("Netaman Version 0.1");
	puts("Press Ctrl+c to Exit\n");

	while (1) {
		char* input = readline("@netaman> ");
		
		if(input[0] == '\0') continue;
    	
		add_history(input);

		parseString(input);
    	free(input);
  	}		

	return 0;
}

void parseString(char* input){
	
	for(int i =0; i < CMD_SIZE;i++){
		CLI_t t = commands[i];
		if(startWith(input,t.name)){
			t.func(input);
			break;
		}
	}

	return;

	if(startWith(input,"ip")){
		str2ip(getArgAt(input,1) , &ip , &mask);
		printf("%x/%i\n",ip,mask);
			
	}else if(startWith(input,"?")){
		puts("koraa no help");
	}else{
		puts("Unkown command, enter ? for help"); 

	}


}






// Functions

void help(char* input){
	puts("Help yoooo");

}
void setIP(char* input){
	char* val = getArgAt(input,1);
	str2ip(val,&ip,&mask);
}
void nm_print(char* input){
	printf("ip: %x",ip);
	printf("mask: %i",mask);
		
}

void info(char* input){	
	uint32_t _mask = 0xffffffff << (32 - mask);
	
	char address[20];

	// Network Address
	uint32_t netadd = ip & _mask;
	ip2str(address,netadd,mask);
	printf("\tNetwork Address is: %s\n", address );
	
	// number of hosts + 1 
	int hosts = 0xffffffff >> mask;	

	// Network broadcast address
	uint32_t broadcastadd = netadd | hosts ; 
	ip2str(address,broadcastadd,-1);
	printf("\tNetwork broadcast address is: %s\n",address);

	printf("\tTotal number of hosts is: %i \n",hosts - 1);
	printf("\tTotal number of host bits is: %i\n",32 - mask);
}


void addSubnet(char* input){
	if(subnets_sz == 0) {
		subnets = malloc(sizeof(Subnet));
		subnets_sz++;
	}
	else if( subnets_sz <= subnets_cn){
		subnets_sz += 10;
		subnets = (Subnet*) realloc(subnets, subnets_sz * sizeof(Subnet) );	
	}
	// add subnets max
	Subnet* sn = subnets+subnets_cn;
	cpArgAt(input,sn->name,1);
	
	char* maxHostArg  = getArgAt(input,2);
	sn->hosts = arg2int(maxHostArg);	

	subnets_cn++;

}

void subneting(char* input){

	sortSubnets(subnets,subnets_cn);

	int iip = ip;
	for(int i =0;i < subnets_cn ;i++){
		Subnet* sn = subnets+i;
		uint32_t max = 2;
		sn->mask = 31;
		while(sn->hosts > (max -2) ){
			max <<= 1;
			sn->mask--;
		}

		printf("-------SUBNET %d %i-------\n",i,max);
		char ipstr[20];
		ip2str(ipstr,iip,sn->mask);
		iip += max;
		printf("subnet address: %s\n",ipstr);	
		printf("max number of hosts: %i\n",max - 2);	
		printf("requested hosts: %i\n\n",sn->hosts);	
	
	}


}
