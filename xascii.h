/*****************************************************************************
* Application Resource Definitions                                           *
*****************************************************************************/
#define XtNhighlightColor	"highlightColor"
#define XtCHighlightColor	"HighlightColor"
#define XtNvalueFont		"valueFont"
#define XtCValueFont		"ValueFont"

typedef struct 
{
	Pixel			highlight_color;
	XFontStruct		*value_font;
}  AppData, *AppDataPtr;

AppData		app_data;

static XtResource	resources[] = {
	{
		XtNhighlightColor,
		XtCHighlightColor,
		XtRPixel,
		sizeof (Pixel),
		XtOffset (AppDataPtr, highlight_color),
		XtRString,
		"red"
	},
	{
		XtNvalueFont,
		XtCValueFont,
		XtRFontStruct,
		sizeof (XFontStruct *),
		XtOffset (AppDataPtr, value_font),
		XtRString,
		XtDefaultFont
	},
};


/****************************************************************************
* Define Command line options.                                              *
****************************************************************************/
static XrmOptionDescRec	 options[] =
 {
	{ "-hc",			"*highlightColor",	XrmoptionSepArg, NULL},
	{ "-highlight",		"*highlightColor",	XrmoptionSepArg, NULL},
	{ "-vf",			"*valueFont",		XrmoptionSepArg, NULL},
};


/****************************************************************************
* String Constants.                                                         *
****************************************************************************/
static char white_bg [] = "white";
static char grey_bg  [] = "#cccccc";

static char title_font[] = "-*-helvetica-bold-r-*-*-14-140-*-*-*-*-*-*";

static char about_text [] =
"Modified X Ascii Chart                       by Ken Kirksey\n\
                                         Auburn University\n\n\
        Layout modified 19 Aug 93 by Mike Hopkirk";

static char defaultTranslations [] = "#override\n\
	<Btn1Down>:				about_xascii()";


String fallbackres[] =
{
    "*highlightColor:  	CornflowerBlue",
    "*valueFont:               9x15Bold",
    "*title_label*horizDistance:   80",
    "*ascii_list*vertDistance:     10",
    "*ascii_list*horizDistance:    10",
    "*ascii_list*borderWidth:      0",
    "*value_list*horizDistance:    10",
    "*value_list*vertDistance:     0",
    "*value_list*borderWidth:      0",
    "*name_list*horizDistance:     10",
    "*name_list*vertDistance:      0",
    "*name_list*borderWidth:       0",
    "*toggle0*horizDistance:       130",
    "*toggle0*background:          white",
    "*toggle1*background:          white",
    "*toggle2*background:          white",
    "*quit_button*fromVert:        toggle1",
    "*quit_button*fromHoriz:       toggle0",
    "*quit_button*vertDistance:    15",
    "*quit_button*background:      white",
    "*done_button*background:      white",
    "*done_button*vertDistance:     10",
    "*done_button*horizDistance:   165",
    "*done_button*label:           Close",
    NULL
};	



/*****************************************************************************
* Translation Data                                                           *
*****************************************************************************/
char *ascii_values[132] =
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
},	

*name_values[132] =
{
"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
" BS", " HT", " LF", " VT", " FF", " CR", " SO", " SI",
"DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYC", "ETB",
"CAN", " EM", "SUB", "ESC", " FS", " GS", " RS", " US",
"Spc",    "",    "",    "",    "",    "",    "",    "",
   "",    "",    "",    "",    "",    "",    "",    "",
   "",    "",    "",    "",    "",    "",    "",    "",
   "",    "",    "",    "",    "",    "",    "",    "",
   "",    "",    "",    "",    "",    "",    "",    "",
   "",    "",    "",    "",    "",    "",    "",    "",
   "",    "",    "",    "",    "",    "",    "",    "",
   "",    "",    "",    "",    "",    "",    "",    "",
   "",    "",    "",    "",    "",    "",    "",    "",
   "",    "",    "",    "",    "",    "",    "",    "",
   "",    "",    "",    "",    "",    "",    "",    "",
   "",    "",    "",    "",    "",    "",    "", "DEL",
   "",    "",    "",    ""						   
},



*octal_values[132] =
{	
"000", "001", "002", "003", "004", "005", "006", "007",	
"010", "011", "012", "013", "014", "015", "016", "017",	
"020", "021", "022", "023", "024", "025", "026", "027",
"030", "031", "032", "033", "034", "035", "036", "037",
"040", "041", "042", "043", "044", "045", "046", "047",
"050", "051", "052", "053", "054", "055", "056", "057",
"060", "061", "062", "063", "064", "065", "066", "067",
"070", "071", "072", "073", "074", "075", "076", "077",
"100", "101", "102", "103", "104", "105", "106", "107",
"110", "111", "112", "113", "114", "115", "116", "117",
"120", "121", "122", "123", "124", "125", "126", "127",
"130", "131", "132", "133", "134", "135", "136", "137",
"140", "141", "142", "143", "144", "145", "146", "147",
"150", "151", "152", "153", "154", "155", "156", "157",
"160", "161", "162", "163", "164", "165", "166", "167",
"170", "171", "172", "173", "174", "175", "176", "177",
		"   ", "   ", "   ", "   "
},

*hex_values [132] = 
{
"00","01","02","03","04","05","06","07","08","09","0A","0B","0C","0D","0E","0F",
"10","11","12","13","14","15","16","17","18","19","1A","1B","1C","1D","1E","1F",
"20","21","22","23","24","25","26","27","28","29","2A","2B","2C","2D","2E","2F",
"30","31","32","33","34","35","36","37","38","39","3A","3B","3C","3D","3E","3F",
"40","41","42","43","44","45","46","47","48","49","4A","4B","4C","4D","4E","4F",
"50","51","52","53","54","55","56","57","58","59","5A","5B","5C","5D","5E","5F",
"60","61","62","63","64","65","66","67","68","69","6A","6B","6C","6D","6E","6F",
"70","71","72","73","74","75","76","77","78","79","7A","7B","7C","7D","7E","7F",
"  ", "  ", "  ", "  "
},

*decimal_values [132] =
{
"000", "001", "002", "003", "004", "005", "006", "007", "008", "009",
"010", "011", "012", "013", "014", "015", "016", "017", "018", "019",
"020", "021", "022", "023", "024", "025", "026", "027", "028", "029",
"030", "031", "032", "033", "034", "035", "036", "037", "038", "039",
"040", "041", "042", "043", "044", "045", "046", "047", "048", "049",
"050", "051", "052", "053", "054", "055", "056", "057", "058", "059",
"060", "061", "062", "063", "064", "065", "066", "067", "068", "069",
"070", "071", "072", "073", "074", "075", "076", "077", "078", "079",
"080", "081", "082", "083", "084", "085", "086", "087", "088", "089",
"090", "091", "092", "093", "094", "095", "096", "097", "098", "099",
"100", "101", "102", "103", "104", "105", "106", "107", "108", "109",
"110", "111", "112", "113", "114", "115", "116", "117", "118", "119",
"120", "121", "122", "123", "124", "125", "126", "127", "   ", "   ",
"   ", "   "
};

