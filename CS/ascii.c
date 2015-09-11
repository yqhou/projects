#include <stdio.h>

#define COLUMN 4
const char cpaAscii[][6] = 
{
    "NUL",
    "SOH",
    "STX",
    "ETX",
    "EOT",
    "ENQ",
    "ACK",
    "BEL",
    "BS",
    "TAB",
    "LF",
    "VT",
    "FF",
    "CR",
    "SO",
    "SI",
    "DLE",
    "DC1",
    "DC2",
    "DC3",
    "DC4",
    "NAK",
    "SYN",
    "ETB",
    "CAN",
    "EM",
    "SUB",
    "ESC",
    "FS",
    "GS",
    "RS",
    "US",
    "' '"
};

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
void ShowOneAscii( int c, char *endstring )
{
    if( c >= 128 ) return;
    printf( "%3d  %.2X  %3o  ", c, c, c);
    CharToBit( c );
    if( c < 33 )
        printf( "  %-3s", cpaAscii[c] );   
    else if( isprint( c ) )
        printf( "  %-3c", c );
    printf( "%s", endstring );
}

void ShowAscii()
{
    int i = 0, j = 0;
    int row = 128 / COLUMN;
    while( row * COLUMN < 128 ) row ++;
    for( i = 0; i<row; i++ )
    {
        for( j = 0; j<COLUMN; j++ )
        {
            ShowOneAscii( i+row*j, j==COLUMN-1 ? "\n" : "\t" );
        }
    }
}

void NewLine()
{
    printf( "\n" );
}

int main( int argc, char **argv )
{
    if( argc > 1 )
        ShowOneAscii( argv[1][0], "\n" );
    else
        ShowAscii();
}