#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define PTE_SIZE 8


int get_pte_bit(unsigned long vadr){
	// getting off set for fseek
	uint64_t offset = vadr / getpagesize() * PTE_SIZE;
	
	FILE * f_pm = fopen("/proc/self/pagemap", "rb");	// file ptr to /proc/self/pagmap
	if(fseek(f_pm, offset, SEEK_SET))
		return -1;
	
	int i;
	unsigned char c;
	// getting byte which has soft dirty(55th) bit
	//printf("%lu\n", vadr);
	for(i=0; i<PTE_SIZE; ++i){
		c = getc(f_pm);
		if(c == -1)		//EOF
			return 0;

		if(i == 6)	// 55th bit is last bit of 6th byte ( counting from 0)
			printf("%d : 0x%02X\n", i, c);
	}

	return 0;
}

int main(){
	char * path = "/proc/self/maps";
	char * maps = NULL;
	size_t len = 0;
	unsigned long adr;

	FILE * f = fopen(path, "r");
	//fseek(f, -2, SEEK_END);
	while(getline(&maps, &len, f) != -1){
		char va_start[12], va_end[12];	//start and end of given va range
		//<12-chars>-<12-chars>
		memcpy(va_start, maps, 12);
		va_start[12] = '\0';
		
		memcpy(va_end, maps+12, 25);
		va_end[12] = '\0';
		
		printf("%lu\n", strtol(va_end, NULL, 16) - strtol(va_start, NULL, 16));
		//for(adr = strtol(va_start, NULL, 16); adr < strtol(va_end, NULL, 16); ++adr)
		//	get_pte_bit(adr);
	}
	fclose(f);
	return 0;
}
