#include<stdio.h>
unsigned short cksum(unsigned short *addr, int count)
{
	 /*	 Compute Internet checksum for “count” bytes
	 	 * beginning at location “addr”.
	 */
register long sum = 0;
while ( count > 1 ) {
	 /* This is the inner loop*/
	 		 	 sum += *addr++;
	 		 	 count -=2;
	 }
/* Add left-over byte, if any */
if ( count > 0 )
sum += *addr;
	 /* Fold 32-bit sum to 16 bits */
	 printf("sum = %x \n", sum);
while (sum >>16)
	 sum = (sum & 0xffff) + (sum >> 16);
return ~sum;
}

int main ()
{
    unsigned short  a, ch;
    int c;


    printf("enter data\n");
    scanf("%x%d", &a,&c);
    ch=cksum(&a,c);
    printf("%x",ch);

}
