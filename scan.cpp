#include "scan.h"
#include "common.h"
int getNextChar(fstream &origin){
	if(origin==NULL){
		return -1;//indicate reading file fail;
	}
	char buffer[256];
	memset(buffer,0,256);
	origin->read(buffer,5);
	origin->close();
	cout<<buffer;
	return 0;
}
