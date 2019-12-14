#include<stdio.h>

main()
{
char username[50],password[50];

FILE *fp;
fp =fopen("database.txt","r");
while(!feof(fp)){
	fscanf(fp,"%s\t%s\n",username,password);
	printf("%s\t%s\n",username,password);
}


}
