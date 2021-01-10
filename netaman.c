#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


int32_t str_to_ip(char* str,int32_t* subnet);

int get_byte(int32_t n, int p){ return (n >> p*8) & 0xff;}

void ip_to_str(char* str,int32_t ip,int subnet){
	sprintf(str,"%i.%i.%i.%i/%i",get_byte(ip,3),get_byte(ip,2),get_byte(ip,1),get_byte(ip,0),subnet);
}


int main(int argc,char** argv){
	
	if(argc > 2){
		fprintf(stderr,"only one argument allowed");
		exit(1);
	}

	int32_t subnet = 0;
	int ip = str_to_ip(argv[1],&subnet);

	int32_t mask = 0xffffffff << (32 - subnet);
	
	char address[20];

	// Network Address
	int32_t netadd = ip & mask;
	ip_to_str(address,netadd,subnet);
	printf("Network Address is: %s\n", address );
	
	// number of hosts + 1 
	int hosts = 0xffffffff >> subnet;	

	// Network broadcast address
	int32_t broadcastadd = netadd | hosts ; 
	ip_to_str(address,broadcastadd,subnet);
	printf("Network broadcast address is: %s\n",address);

	printf("Total number of hosts is: %i \n",hosts - 1);
	printf("Total number of host bits is: %i",32 - subnet);
	

	return 0;
}


int32_t str_to_ip(char* str,int32_t* subnet){
	char c;
	int curnum = 0;
	int32_t res = 0;

	while((c = *str++) ){
		if(c == '.' || c == '/'){
			res |= curnum;
			if(c != '/')res <<= 8;
			curnum = 0;
			continue;
		}
		curnum *= 10;
		curnum += c - '0';
	}
	if(curnum > 30 ){
		fprintf(stderr,"hey hey hey subnet mask should be between 2-30 ");
		exit(1);
	}
	*subnet = curnum;
	return res;

}
