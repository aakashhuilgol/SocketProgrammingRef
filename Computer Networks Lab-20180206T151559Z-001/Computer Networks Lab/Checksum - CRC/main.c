/* Program to implement CRC error-detection using CRC-CCITT.
 (Direct CRC)  http://www.zorc.breitbandkatze.de/crc.html
*/

#include <stdio.h>
#include <stdlib.h>

typedef unsigned short CRC;

#define WIDTH    (8 * sizeof(CRC))
#define TOPBIT   (1 << (WIDTH - 1))
#define POLYNOMIAL 0x1021

CRC CalculateCRC(unsigned char const message[], int nBytes);

int main( )
{
    char message[64], resp, dummy[16];
    CRC crcCode, remainder;
    int l;

    printf("Enter a message: ");
    scanf("%s", message );

    crcCode = CalculateCRC( message, strlen(message));
    printf("Calculated CRC: %x\n", crcCode );

    printf("The code word is: %s%x\n", message, crcCode );

        // Form the code word
    l = strlen(message);
    message[l] = (char)(crcCode>>8);
    message[l+1] = (char)(crcCode&0x00ff);
    message[l+2] = '\0';

    gets(dummy);

    printf("Do you want to introduce an error?(Y/N) ");
    resp = getch( );

    if( resp == 'y' || resp == 'Y' )
        message[0] ^= 12;

    remainder = CalculateCRC( message, strlen(message));
    printf("\n\nRemainder is %x.", remainder );
    if( remainder != 0 )
        printf("Error present in code word.\n");
    else
        printf("No error in found.\n");

    system("PAUSE");
    return 0;
}

CRC CalculateCRC(unsigned char const message[], int nBytes)
{
    CRC  remainder = 0;
    int byte;
    unsigned char bit;

    /*
     * Perform modulo-2 division, a byte at a time.
     */
    for ( byte = 0; byte < nBytes; ++byte)
    {
        /*
         * Bring the next byte into the remainder.
         */
        remainder ^= (message[byte] << (WIDTH - 8));

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for ( bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    /*
     * The final remainder is the CRC result.
     */
    return (remainder);

}
