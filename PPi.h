/*
 *  "PSP Post it" prx/plugin for PSP
 *
 *      Author: Nicolas Lopez F. / pyroesp (pspgen, devsgen, lan.st, ...)
 *
 *      Release Date: 06.04.2010
 *
 *      Version: 0.1.2
 *
 *      Works in: VSH and GAME
 *
 *      What is this ?:
 *              "PSP Post it" allows you to show a note at a specific time and date.
 *              You just need to write your notes with the time and date, when it will show on the screen,
 *              in a txt file next to the plugin. Each note must not have more than 50 characters.
 *
 *      How To Use:
 *              Create a .txt file named PPi.
 *              Write in that txt file your notes, max 15 notes.
 *              HH:MM DD.MM.YYYY note goes here (50 characters max)
 *              ex. 14:30 07.07.2010 Birthday brother
 *              Don't forget to enable the plugin (in VSH and GAME).
 *              An example .txt file is present with this plugin.
 *
 *      Note: The .txt file must not end with an end of line char ('\n') !
 *
 *      Controls:
 *              Press R & L to test if the plugin is enabled.
 *
 *      Possible errors:
 *              - Time/Date structure error.
 *                        Check the time and date you wrote in the txt file,
 *                        there should be an error somewhere.
 *                        ex. of error: 14:5 05.06.2010 note goes here
 *                        it needs to be: 14:05 05.06.2010 note goes here
 *
 *              - Same time and date in line X and Y
 *                        Check the time and date at line X and Y. Change one of the 2 lines.
 *                        You can't run 2 or more notes at the same time and date.
 *
 *              - You have written X notes   //X > 15
 *                        The program supports only 15 notes, please delete (X - 15) notes from
 *                        your PPi.txt file.
 *
 *      Special Thanks to:
 *              - kevinr / K91220
 *              - Mark Martinec <mark.martinec@ijs.si> for snprintf.c
 *              - Everyone else I forgot to mention (sorry ^^)
 *
 *      Note:
 *              This is an open source plugin. You can use this code for your own, but I'll appreciate
 *              if you write my name or pseudo in your program/readme/whatever (and the name of the prx).
 *
 *      Thank you, I hope you'll appreciate this program.
 *
 */

#ifndef PPI_H_INCLUDED
#define PPI_H_INCLUDED

#define SW 67

#define NB_L 15
#define NB_C_TXT 50
#define NB_C_EL 1
#define NB_C_TD 17
#define NB_C (NB_C_TXT + NB_C_TD + NB_C_EL) //50+17+1 = 68

#define OFFSET 1000

#define BLACK 0xFF000000
#define YELLOW 0xFF00F3F3

typedef struct
{
    int ho, mi, da, mo, ye;
    int error;
    char text[NB_C_TXT+NB_C_EL];
}PPi;

PPi Init_SPPi(void);

//void mypspDebug(char *texte);
//void my2pspDebug(int var, int i);

void Is_PRX_Enabled(void);

int Get_Int(char *pbf, int *ppos);
int Number_Lines_File(char *pbf);
int Search_Endline(char *pbf, int position);
PPi Read_TDT(char *pbf, int i);

void Error_TMN(int el);
void Error_Time(void);
void Is_Equal_TD(PPi* pt, int nb_pi);

void Post_It_Screen(PPi tt, pspTime t);

/*
 * Special Thanks to Mickael2054 from devsgen.com for the following functions
 */
static u32 Adjust_Alpha(u32 col);
int blit_prx(int sx, int sy, char *txt, int fg_col,int bg_col);

#endif // PPI_H_INCLUDED
