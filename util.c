#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct {
	char  name[20];
	uint32_t hosts;
	uint32_t ip;
	uint32_t mask;
	
} Subnet;

int get_byte(uint32_t n, int p){ return (n >> p*8) & 0xff;}

void ip2str(char* str,uint32_t ip,int _mask){
	if(_mask == -1)
		sprintf(str,"%i.%i.%i.%i",get_byte(ip,3),get_byte(ip,2),get_byte(ip,1),get_byte(ip,0));
	else
		sprintf(str,"%i.%i.%i.%i/%i",get_byte(ip,3),get_byte(ip,2),get_byte(ip,1),get_byte(ip,0),_mask);

}
int startWith(char* str,char* part){
	char c ;
	while((c = *part++)){
		if(c != *str++){
			return 0;
		}
	}
	if(*str == ' ' || *str == '\0') return 1;
	return 0;
}

char* getArgAt(char* input ,int pos){
	char c;
	int spaceCn = 0;
	while( (c = *input++) ){
		if( c == ' '){
			if(++spaceCn == pos) return input;
		}
	}
	return NULL;

}

void sortSubnets(Subnet* subnets,int size){
	for( int i=0; i < size;i++){
		for(int j = i+1; j < size;j++){
			if(subnets[i].hosts < subnets[j].hosts){
				Subnet tmp = subnets[i];
				subnets[i] = subnets[j];
				subnets[j] = tmp;
			}
		}

	}

}

void str2ip(char* str,uint32_t* _ip, uint32_t* _mask){
	char c;
	int curnum = 0;
	for(int i = 0; i < 5 ;i++){
		if(i == 4 && c != '/') break;
		int cn = 0;
		while( (c = *str++) >= '0' && c <= '9' ){
			curnum *= 10;
			curnum += c - '0';	
			cn++;
		}
		if(cn == 0){
			fprintf(stderr,"only [0-9\\.] allowed in ip '%i' ",c);
		}

		if(i == 4) *_mask = curnum;
		else {
			*_ip <<= 8;
			*_ip |= curnum;
			curnum = 0;
		}

	}
	if(*_mask > 30 || *_mask < 1) {
		puts("mask should be between 1-30");
		printf("%i will be ignored",*_mask);
		*_mask = 24;
	}

}

int arg2int(char* arg){
	char c;
	int res = 0;
	while((c= *arg++) && c != ' '){
		res *= 10;
		res += c - '0';
	}
	return res;

}

void cpArgAt(char* input,char* to,int pos){
	char c;
	int cn = 0;
	int cp = 0;
	while( (c = *input++) ){
		if(c == ' ' && cn == pos) break;

		if(cp) *to++ = c;
		else if( c == ' ' && ++cn == pos) cp = 1;
	}
	*to = '\0';

}
