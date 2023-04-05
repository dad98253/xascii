/*****************************************************************************
* XAscii                                                      by Ken Kirksey *
*                                                                            *
* XAscii displays a table of ascii characters and their respective values.   *
* The user choses which format the values are displayed in:  either hex,     *
* decimal or octal.                                                          *
*                                                                            *
* BASICALLY THE WAY IT WORKS                                                 *
*        In xascii.h four static arrays of strings are declared, one for      *
*       ascii characters, one for decimal values, one for hex values and one *
*        for octal values.  Each of these are broken up into five separate    *
*        strings, each of which make up a column (label widget) in the xascii *
*        window.  The ascii value columns remain static, they never change.   *
*        The label widgets that hold the values, however, change whenever the *
*        user changes the notational mode (clicks on the dec ,hex, or octal   *
*       button).  What happens when one of these buttons is clicked is that  *
*       an XtVaSetValues is done on the value label widgets to change their  *
*        label resource to display the values in the notation specified by the*
*        button press.                                                        *
*                                                                            *
* Send all kudos, complaints, suggestions, or bug reports to:                *
*        Ken Kirksey  (kkirksey@eng.auburn.edu)                               *
*                                                                            *
* BTW:  I use 4 column tabs for coding, so if your text editor isn't set for *
*         4 column tabs, this program probably looks really funny.             *
*                                                                               *
******************************************************************************
* Version 1.1                                                       5 Aug 91 *
*                                                                            *
*  Removed the #include <strings.h> as this was a portability no-no.         *
*  Added an Imakefile (courtesy of Dave Elliot) to the release archive.      *   *                                                                               *
******************************************************************************
* Version 1.2                                                       6 Aug 91 *
*                                                                            *
* Fixed the problem that was causing my use of the valueFont resource to     *
* kill the program.  The bug was pointed out by Dave Brooks.                 *
*                                                                               *
*****************************************************************************/                                                                           

#include <stdio.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xos.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Label.h>    
#include <X11/Xaw/Form.h>
#include "xascii.h"

#define ROWS 16
#define COLS  8  

#define MAJOR_VERT_DIST 15      /* between gps */
#define MINOR_VERT_DIST 10      /* within gps */

#define MAJOR_HORIZ_DIST 10     /* between grps */
#define MINOR_HORIZ_DIST 30     /* within or to align gps */

/*===========================================================================+
| Function Declarations                                                      |
+===========================================================================*/
static void Quit(),                    /* Callbacks                            */
            ToHex(),
            ToDec(),
            ToOct(),
            Done(),

            AboutXascii(),            /* Actions                                */

            Syntax();                /* Utilitie                                */

static char *ascii_val();

/*===========================================================================+
| Global Variable Declarations                                               |
+===========================================================================*/

static XtAppContext appl_context;

static XtActionsRec about_box[]= {
        {"about_xascii", AboutXascii},
        {"ToDec", ToDec},
        {"ToHex", ToHex},
        {"ToOct", ToOct},
        {"Quit",  Quit},
};

static Widget     toplevel,
                base_win,
                title_label,
                quit_button,
                toggles[3],
                ascii_list[COLS],
                value_list[COLS],
                nm_list[COLS],
                pshell,
                about_base_win,
                about_label,
                done_button;

char        **hex_columns,
            **decimal_columns,
            **octal_columns,
            **nm_columns,
            **ascii_columns;

char *keytranstbl="#override\n\
    <Key>q: Quit()\n\
    <Key>d: ToDec()\n\
    <Key>h: ToHex()\n\
    <Key>o: ToOct()\n\
";

/*===========================================================================+
| MAIN                                                                       | +===========================================================================*/
main (argc,argv)
    int     argc; 
    char     ** argv;
{
    int        i, j;
    char buf[100], buf1[10];
    XtTranslations keytrans;
    
/*---------------------------------------------------------------------------+
| Break up the Ascii table and values into separate columns
| (ROWS rows, COLS cols) for display.
+---------------------------------------------------------------------------*/
    ascii_columns   = (char **)XtCalloc (COLS, sizeof (char *));
    hex_columns     = (char **) XtCalloc (COLS, sizeof (char *));
    decimal_columns = (char **) XtCalloc (COLS, sizeof (char *));
    octal_columns   = (char **) XtCalloc (COLS, sizeof (char *));
    nm_columns      = (char **) XtCalloc (COLS, sizeof (char *));

    for (i=0; i<COLS; i++)
    {
        ascii_columns[i]    = (char *)  XtCalloc (90, sizeof (char));
        hex_columns[i]      = (char *)  XtCalloc (90, sizeof (char));
        decimal_columns[i]  = (char *)  XtCalloc (90, sizeof (char));
        octal_columns[i]    = (char *)  XtCalloc (90, sizeof (char));
        nm_columns[i]       = (char *)  XtCalloc (90, sizeof (char));
    
        for (j= (i*ROWS); j < ((i+1) * ROWS); j++)
        {
            sprintf( buf, "%3.3s\n", ascii_val(j, buf1) );
            strcat (ascii_columns[i], buf);

            sprintf( buf, "%s\n", name_values[j]);
            strcat (nm_columns[i], buf );

            sprintf(buf, "%3x\n", j ) ;
            strcat (hex_columns[i], buf);

            sprintf(buf, "%3d\n", j ) ;
            strcat (decimal_columns[i], buf);

            sprintf(buf, "%03o\n", j ) ;
            strcat (octal_columns[i], buf);
        }
    }

    /*-----------------------------------------------------------------------+
    | Begin Widget Initialization.                                           |
    +-----------------------------------------------------------------------*/     
    toplevel = XtAppInitialize(
                    &appl_context,
                    "XAscii",
                    options,
                    XtNumber(options),
                    &argc,argv,NULL, NULL,0);

    /*-----------------------------------------------------------------------+
    | Check for Invalid command line options                                 |
    +-----------------------------------------------------------------------*/
    if (argc > 1)
        Syntax (argc, argv);

    
    keytrans = XtParseTranslationTable( keytranstbl );
    
    XtVaGetApplicationResources (toplevel,
                    &app_data,
                    resources,
                    XtNumber (resources),
                    NULL);

    base_win = XtVaCreateManagedWidget(
                    "base_win", 
                    formWidgetClass,
                    toplevel,
                    XtNtranslations, keytrans,
                    NULL, 0);

    /*----------------------------------------------------------------------+
    |  Note the translation that invokes that about box pop up on a button  |
    |  click in this label widget.                                          |
    +----------------------------------------------------------------------*/
    title_label    = XtVaCreateManagedWidget (
                    "title_label",
                    labelWidgetClass,
                    base_win,
                        XtNforeground, app_data.highlight_color,
                        XtNlabel, "X ASCII Chart",
                        XtNwidth, 408,
                        XtNtranslations,
                            XtParseTranslationTable(defaultTranslations),
                         XtNhorizDistance, 200,
                        XtVaTypedArg,
                            XtNbackground,
                            XtRString,
                            white_bg,
                            sizeof (white_bg),
                        XtVaTypedArg,
                            XtNfont,
                            XtRString,
                            title_font,
                            sizeof (title_font),
                    NULL, 0);

    /*-----------------------------------------------------------------------+
    |Create the label widgets for chart display. Default value mode is dec.  |
    +-----------------------------------------------------------------------*/
    for (i=0; i<COLS;  i++)
    {
        value_list[i] = XtVaCreateManagedWidget(
                        "value_list",
                        labelWidgetClass,
                        base_win,
                            XtNforeground, app_data.highlight_color,
                            XtNfont, app_data.value_font,  
                            XtNfromVert, title_label, 
                            XtNlabel, decimal_columns[i],
                            XtNhorizDistance, MAJOR_HORIZ_DIST,
                            XtNborderWidth, 0,
                            XtNvertDistance, MAJOR_VERT_DIST,
                            XtVaTypedArg,
                                XtNbackground,
                                XtRString,
                                white_bg,
                                sizeof (white_bg),

                        NULL);

        ascii_list[i] = XtVaCreateManagedWidget(
                        "ascii_list",
                        labelWidgetClass,
                        base_win,
                            XtNfont, app_data.value_font,  
                            XtNfromVert, title_label, 
                            XtNlabel, ascii_columns[i],
                            XtNfromHoriz, value_list[i],
                            XtNborderWidth, 0,
                            XtNvertDistance, MAJOR_VERT_DIST,
                            XtNhorizDistance, 0,
                            XtVaTypedArg,
                                XtNbackground,
                                XtRString,
                                white_bg,
                                sizeof (white_bg),

                        NULL);

        nm_list[i] = XtVaCreateManagedWidget(
                        "name_list",
                        labelWidgetClass,
                        base_win,
                            XtNfont, app_data.value_font,  
                            XtNfromVert, title_label,
                            XtNfromHoriz, ascii_list[i],
                            XtNlabel, nm_columns[i],
                            XtNborderWidth, 0,
                            XtNvertDistance, MAJOR_VERT_DIST,
                            XtNhorizDistance, 0,
                            XtVaTypedArg,
                                XtNbackground,
                                XtRString,
                                white_bg,
                                sizeof (white_bg),

                        NULL);


    }
        
    for (i=1; i<COLS; i++)
        XtVaSetValues (value_list[i], XtNfromHoriz, nm_list[i-1], NULL);
    /*-----------------------------------------------------------------------+
    | Create the buttons for value mode selection.  Make them a radio group, |
    | so that only one will be selected (highlighted) at a time.             |
    +-----------------------------------------------------------------------*/
    for (i=0; i<3; i++)
    {
    char name[40];

        sprintf(name,"toggle%d", i);
        toggles[i] = XtVaCreateManagedWidget (
                    name,
                    toggleWidgetClass, 
                    base_win,
                        XtNforeground, app_data.highlight_color,
                        XtNborderColor, app_data.highlight_color,
                        XtNfromVert, ascii_list[0],
                        XtNvertDistance, MAJOR_VERT_DIST,
                        XtNwidth, 95,
                        XtNresize, FALSE,
                        XtVaTypedArg,
                            XtNbackground,
                            XtRString,
                            white_bg,
                            sizeof (white_bg),

                    NULL, 0);

        if (i != 0)
            XtVaSetValues(toggles[i], XtNfromHoriz, toggles[i-1], NULL);

        XtVaSetValues (toggles[i], XtNradioGroup, toggles[0], NULL);
    }

    XtVaSetValues (toggles[0],
                    XtNlabel,           "Hex",
                    XtNhorizDistance,   MAJOR_HORIZ_DIST,
                    XtNfromHoriz,       nm_list[1],
                    NULL);

    XtVaSetValues (toggles[1],
                    XtNstate, TRUE,
                    XtNlabel, "Decimal", 
                    XtNhorizDistance, MINOR_HORIZ_DIST,
                    NULL);

    XtVaSetValues (toggles[2],
                    XtNlabel,    "Octal",
                    XtNhorizDistance, MINOR_HORIZ_DIST,
                    NULL);

    XtAddCallback (toggles[0], XtNcallback, ToHex, NULL); 
    XtAddCallback (toggles[1], XtNcallback, ToDec, NULL);
    XtAddCallback (toggles[2], XtNcallback, ToOct, NULL); 

    
    quit_button = XtVaCreateManagedWidget (
                    "quit_button", 
                    commandWidgetClass, 
                    base_win,
                        XtNfromVert, toggles[0],
                        XtNfromHoriz, toggles[0],
                        XtNlabel, "Quit",
                        XtNwidth, 100,
                        XtNvertDistance, MAJOR_VERT_DIST,
                        XtNhorizDistance, MINOR_HORIZ_DIST,
                        XtVaTypedArg,
                            XtNbackground,
                            XtRString,
                            white_bg,
                            sizeof (white_bg),

                    NULL, 0);

    XtAddCallback (quit_button, XtNcallback, Quit, NULL);

    /*-----------------------------------------------------------------------+
    | Create the About Box.  Invoked on click in title label.                |
    +-----------------------------------------------------------------------*/
    pshell = XtVaCreatePopupShell (
                    "pshell",
                    transientShellWidgetClass,
                    toplevel,
                    NULL);

    about_base_win = XtVaCreateManagedWidget(
                    "about_base_win",
                    formWidgetClass,
                    pshell,

                    NULL);

    about_label = XtVaCreateManagedWidget(
                    "about_label",
                    labelWidgetClass,
                    about_base_win,
                        XtVaTypedArg,
                            XtNbackground,
                            XtRString,
                            white_bg,
                            sizeof (white_bg),
                        XtNlabel, about_text,
                        XtNforeground, app_data.highlight_color,
                        XtNborderColor, app_data.highlight_color,
                        XtNborderWidth, 4,
                    NULL);

    done_button = XtVaCreateManagedWidget(
                    "done_button",
                    commandWidgetClass,
                    about_base_win,
                        XtVaTypedArg,
                            XtNbackground,
                            XtRString,
                            white_bg,
                            sizeof (white_bg),
                        XtNfromVert, about_label,
                        XtNvertDistance, 10,
                        XtNhorizDistance, 165,
                        XtNlabel, "Slainte!",
                    NULL);

    XtAddCallback (done_button, XtNcallback, Done, NULL);

    XtAppAddActions (appl_context, about_box, XtNumber(about_box) );

     XtRealizeWidget(toplevel);
     XtAppMainLoop(appl_context);
}

/*===========================================================================+
| CALLBACK FUNCTIONS                                                          |
 +===========================================================================*/

/*---------------------------------------------------------------------------+
| Quit                                                                       |
|         Quit the application.  Invoked by click on quit button.            |
+---------------------------------------------------------------------------*/
static void Quit (w,ignore1,ignore2)
Widget w; XtPointer ignore1, ignore2;
{   XtDestroyApplicationContext(appl_context);
    exit(0);
}

/*---------------------------------------------------------------------------+
| ToHex                                                                      |
|         Changes the ascii character values to Hex notation.                  |
+---------------------------------------------------------------------------*/  
static void ToHex (w,ignore1,ignore2)
Widget w; XtPointer ignore1, ignore2;
{ 
  int i;

    for (i=0; i<COLS; i++)
        XtVaSetValues (value_list[i], XtNlabel, hex_columns[i], NULL);

}


/*---------------------------------------------------------------------------+
| ToDec                                                                      |
|         Changes the ascii character values to Decimal notation.              |
+---------------------------------------------------------------------------*/  
static void ToDec (w,ignore1,ignore2)
Widget w; XtPointer ignore1, ignore2;
{ 
  int i;

    for (i=0; i<COLS; i++)
        XtVaSetValues (value_list[i], XtNlabel, decimal_columns[i], NULL);

}


/*---------------------------------------------------------------------------+
| ToOct                                                                      |
|         Changest the ascii character values to Octal notation.               |
+---------------------------------------------------------------------------*/  
static void ToOct (w,ignore1,ignore2)
Widget w; XtPointer ignore1, ignore2;
{ 
  int i;

    for (i=0; i<COLS; i++)
        XtVaSetValues (value_list[i], XtNlabel, octal_columns[i], NULL);

}


/*---------------------------------------------------------------------------+
| Done                                                                       |
|         Pops down the about box popup shell.                                 |
+---------------------------------------------------------------------------*/  
static void Done (w,ignore1,ignore2)
Widget w; XtPointer ignore1, ignore2;
{ 
  int i;

    XtPopdown (pshell);
}


/*===========================================================================+
| ACTION FUNCTIONS                                                               | +===========================================================================*/

/*---------------------------------------------------------------------------+
| AboutXascii                                                                |
|             Positions and pops up the about box popup shell.                 |
+---------------------------------------------------------------------------*/  
static void AboutXascii (w, event, nuffin1, nuffin2)
Widget            w;
XButtonEvent    event;
String            *nuffin1;
Cardinal        *nuffin2;
{    
    Position    x, 
                y;

    Dimension    width, 
                height;

    int            i;

    XtVaGetValues (toplevel,
            XtNwidth, &width,
            XtNheight, &height,
            NULL);

    XtTranslateCoords (toplevel,
            (Position) 0,
            (Position) height/2,
            &x, &y);

    XtVaSetValues (pshell,
            XtNx, x,
            XtNy, y,
            NULL);

    XtPopup (pshell, XtGrabNonexclusive);
}

/*===========================================================================+
| UTILITY FUNCTIONS                                                              | +===========================================================================*/

/*---------------------------------------------------------------------------+
| Syntax                                                                     |
|        Parses off bad command line options (i.e. the stuff left over when   |
|         XtVaAppInitialize is through).                                       |
+---------------------------------------------------------------------------*/       static void Syntax (argc, argv)
int     argc;
char    **argv;
{
    int     i,
            err = 0;

    for (i=1; i < argc; i++)
    {
        if (!err++)
            fprintf (stderr, "\nxascii: unknown command line option\n");

        fprintf (stderr, "option:  %s\n", argv[i]);
    }

}

        

        
static char *ascii_val(j, s )
int j;
char *s;
{
    if ( j < ' ' )
        sprintf(s, "^%c", j | 0x40 );
    else if ( j == 127 )
        sprintf(s, "^?");
    else
        sprintf(s, "%c", j );

    return s;        
}












