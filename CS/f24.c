#include <stdio.h>

typedef unsigned char* byte_pointer;

void show_bytes( byte_pointer start, int len )
{
	int i ;
	for( i = 0; i<len; i++ )
		printf( " %.2x", start[i] );
    printf( "\n" );
}

void show_int( int x )
{
    show_bytes( (byte_pointer)&x, sizeof(int) );
}

void show_float( float x )
{
    show_bytes( (byte_pointer)&x, sizeof(float) );
}

void show_pointer( void *x )
{
    show_bytes( (byte_pointer)&x, sizeof(void*) );
}

void test_show_bytes( int val )
{
    int ival = val;
    float fval = (float) ival;
    int *pval = &ival;
    show_int( fval );
    show_float( fval );
    show_pointer( pval );
}

void NewLine()
{
    printf( "\n" );
}
void CharToBit( int c )
{
    char cpDest[9+1];
    cpDest[0] = (( c & 0x80 )>>7) + '0'; /** ¸ß8Î» **/
    cpDest[1] = (( c & 0x40 )>>6) + '0';
    cpDest[2] = (( c & 0x20 )>>5) + '0';
    cpDest[3] = (( c & 0x10 )>>4) + '0';
    cpDest[4] = ' ';
    cpDest[5] = (( c & 0x08 )>>3) + '0';
    cpDest[6] = (( c & 0x04 )>>2) + '0';
    cpDest[7] = (( c & 0x02 )>>1) + '0';
    cpDest[8] = (( c & 0x01 )) + '0';
    cpDest[9] = 0;
    printf( "%9s", cpDest );
}
void Test1()
{
    
    char a = 'i'; /* 0110 1001 */
    char b = 'U'; /* 0101 0101 */
    CharToBit( ~a ); /* 1001 0110 */
    NewLine();
    CharToBit( ~b ); /* 1010 1010 */
    NewLine();
    CharToBit( a & b ); /* 0100 0001 */
    NewLine();
    CharToBit( a | b ); /* 0111 1101 */
    NewLine();
    CharToBit( a ^ b ); /*0011 1100 */
    NewLine();
    
}

void Swap( int *x, int *y )
{
    *y = *x ^ *y;
    *x = *x ^ *y;
    *y = *x ^ *y;
}
void ReverseArray( int a[], int cnt )
{
    int first, last;
    for( first=0, last=cnt-1; first < last; first++,last-- )
    {
        Swap( &a[first], &a[last] );
    }
}
void Test2()
{
    int arr[6] = {1,2,3,4,5,6};
    ReverseArray( arr, 6 );
    int i = 0;
    for( i=0; i<6; i++ )
        printf( "%d ", arr[i] );
}
void Test3()
{
    int a = 0x87654321;
    int b = a & 0xFF;
    printf( "%x\n", b );
    printf( "%x, %x, %x\n", a ^ ~0xff, a ^ ~0, ~a);
    printf( "%x\n",  a | 0xff );
}

int main()
{
    Test3();
}
