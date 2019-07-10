#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TAG "[starload-native-App] "
#define LEVEL_E  "Error: " 
#define LEVEL_D  "Debug: "  
#define LEVEL_I  "Infor: "
#define LOGE(a,arg...)\
	do{\
		printf(TAG  LEVEL_E "%s:%d: " a" \n", __FUNCTION__, __LINE__,##arg);\
	}while(0)
#define LOGD(a,arg...)\
	do{\
		printf(TAG  LEVEL_D "%s:%d: " a" \n", __FUNCTION__, __LINE__,##arg);\
	}while(0)
#define LOGI(a,arg...)\
	do{\
		printf(TAG  LEVEL_I "%s:%d: " a" \n", __FUNCTION__, __LINE__,##arg);\
	}while(0)

int main(void){
	LOGI("E");
	
	LOGI("X");
	return 0;
}
