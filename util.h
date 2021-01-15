#include <stdint.h>

typedef struct {
	char  name[20];
	uint32_t hosts;
	uint32_t ip;
	uint32_t mask;
	
} Subnet;
int get_byte(uint32_t n, int p);
void ip2str(char* str,uint32_t ip,int _mask);
void str2ip(char* str,uint32_t* _ip, uint32_t* _mask);
int startWith(char* str,char* part);
char* getArgAt(char* input ,int pos);
void sortSubnets(Subnet* subnets,int size);
int arg2int(char* arg);
void cpArgAt(char* input,char* to,int pos);
