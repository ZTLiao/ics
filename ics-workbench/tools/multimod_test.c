#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main(int argv, char* argc[]) {
	char buf[1024] = {};
	char multimod[1024] = {};
	strcat(multimod, "python3 -c 'print(");
	strcat(multimod, argc[1]);
	strcat(multimod, ")'");
	printf("%s\n", multimod);
	FILE *fp = popen(multimod, "r");
	assert(fp);
	fscanf(fp, "%s", buf);
	printf("%s\n", buf);
	pclose(fp);
	return 0;
}
