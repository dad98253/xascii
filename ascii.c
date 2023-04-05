#include <stdio.h>

char *ascii_values1[132] =
{
"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
"BS ", "HT ", "LF ", "VT ", "FF ", "CR ", "SO ", "SI ",
"DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYC", "ETB",
"CAN", "EM ", "SUB", "ESC", "FS ", "GS ", "RS ", "US ",
"SPA", "!  ", "\"  ", "#  ", "$  ", "%  ", "&  ", "'  ",
"(  ", ")  ", "*  ", "+  ", ",  ", "-  ", ".  ", "/  ",
"0  ", "1  ", "2  ", "3  ", "4  ", "5  ", "6  ", "7  ",
"8  ", "9  ", ":  ", ";  ", "<  ", "=  ", ">  ", "?  ",
"@  ", "A  ", "B  ", "C  ", "D  ", "E  ", "F  ", "G  ",
"H  ", "I  ", "J  ", "K  ", "L  ", "M  ", "N  ", "O  ",
"P  ", "Q  ", "R  ", "S  ", "T  ", "U  ", "V  ", "W  ",
"X  ", "Y  ", "Z  ", "[  ", "\\  ", "]  ", "^  ", "_  ",
"`  ", "a  ", "b  ", "c  ", "d  ", "e  ", "f  ", "g  ",
"h  ", "i  ", "j  ", "k  ", "l  ", "m  ", "n  ", "o  ",
"p  ", "q  ", "r  ", "s  ", "t  ", "u  ", "v  ", "w  ",
"x  ", "y  ", "z  ", "{  ", "|  ", "}  ", "~  ", "DEL",
"   ", "   ", "   ", "   "						   
};	

char *ascii_values2[132] =
{
"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
" BS", " HT", " LF", " VT", " FF", " CR", " SO", " SI",
"DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYC", "ETB",
"CAN", " EM", "SUB", "ESC", " FS", " GS", " RS", " US",
"Spc", "   ", "   ", "   ", "   ", "   ", "   ", "   ",
"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ",
"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ",
"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ",
"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ",
"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ",
"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ",
"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ",
"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ",
"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ",
"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ",
"   ", "   ", "   ", "   ", "   ", "   ", "   ", "DEL",
"   ", "   ", "   ", "   "						   
};	



main()
{
    lay1();
}

/* horizontal 3 line ascii, decimal, hex */
lay1()
{
int i, i1;

    for ( i=0; i < 128; i+=16 )
    {

        /* ascii val */
        for (i1=0; i1<16; i1++ )
        {
        int j = i + i1;
        
            if ( j >= ' ' && i1 == 0 )
                printf(" ");
            if ( j < ' ' )
                printf(" ^%c ", j | 0x40 );
            else if ( j == 127 )
                printf("^?", j);
            else
            printf(" %c  ", j );
        } 
        printf("\n");

        /* decimal */
        for (i1=0; i1<16; i1++ )
        {
        int j = i + i1;

            printf("%3d ", j);
        }
        printf("\n");

        /* hex */
        for ( i1=0; i1<16; i1++ )
        {
        int j = i + i1;
        
            printf("%3x ", j);
        } 
        printf("\n");

        /* desc */
        for (i1=0; i1<16; i1++ )
        {
        int j = i + i1;

            printf("%s ", ascii_values2[j]);
        }
        printf("\n");
        printf("\n");
    }
}

        
