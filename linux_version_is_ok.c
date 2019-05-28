#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv){
	if (argc != 4){
		printf("Param Error argc(%d) \n",argc);
		return -1;
	}

	int arg1 = atoi(argv[1]);
	int arg2 = atoi(argv[2]);
	int arg3 = atoi(argv[3]);

	int this_module_version =   (((arg1) << 16) + ((arg2) << 8) + (arg3)); 
	
	printf("compare_version = %d , this_module = 327683\n",this_module_version);

	if (this_module_version > 327683)
		printf("this littler \n");
	else 
		printf("this more larger \n");

	return 0;
}
