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
 *              Don't forget to enable the plugin.
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

#include <pspkernel.h>
#include <pspdebug.h>
#include <psprtc.h>
#include <pspctrl.h>
#include <pspdisplay.h>

#include <string.h>

#include "PPi.h"


/*
 * My VSH-only debug functions
 *

void mypspDebug(char *texte)
{
	SceCtrlData pad;
	int i = 0;

	while (1)
	{
        sceCtrlPeekBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_LTRIGGER)
        {
            while (pad.Buttons & PSP_CTRL_LTRIGGER)
            {
                sceCtrlPeekBufferPositive(&pad, 1);

                pspDebugScreenSetXY(25, 27);
                pspDebugScreenPrintf("L Trigger still pressed");

                sceKernelDelayThread(1000);
            }

            break;
        }

        pspDebugScreenSetXY(25, 26);

        for (i = 0; i < strlen(texte); ++i)
            pspDebugScreenPrintf("%c", texte[i]);

        sceKernelDelayThread(1000);
	}
}

void my2pspDebug(int var, int i)
{
	SceCtrlData pad;

	while (1)
	{
        sceCtrlPeekBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_LTRIGGER)
        {
            while (pad.Buttons & PSP_CTRL_LTRIGGER)
            {
                sceCtrlPeekBufferPositive(&pad, 1);

                pspDebugScreenSetXY(25, 27);
                pspDebugScreenPrintf("L Trigger still pressed");

                sceKernelDelayThread(1000);
            }

            break;
        }

        pspDebugScreenSetXY(25, 26);
        pspDebugScreenPrintf("%d , %d", var, i);

        sceKernelDelayThread(1000);
	}
}
*/

void Is_PRX_Enabled(void)
{
	SceCtrlData pad;

    sceCtrlPeekBufferPositive(&pad, 1);

    if (pad.Buttons & PSP_CTRL_LTRIGGER && pad.Buttons & PSP_CTRL_RTRIGGER)
    {
        while (pad.Buttons & PSP_CTRL_LTRIGGER && pad.Buttons & PSP_CTRL_RTRIGGER)
        {
            sceCtrlPeekBufferPositive(&pad, 1);

            blit_prx(25, 28, "PSP Post it plugin is enabled", BLACK, YELLOW);

            sceKernelDelayThread(1000);
        }
    }
}

int Get_Int(char *pbf, int *ppos)
{
    int value = 0;
    value = (pbf[*ppos] - '0') * 10;
    ++(*ppos);
    value += (pbf[*ppos] - '0');
    ++(*ppos);

    return value;
}

PPi Init_SPPi(void)
{
    int i = 0;
    PPi t;

    t.ho = 0;
    t.mi = 0;
    t.da = 0;
    t.mo = 0;
    t.ye = 0;
    t.error = 0;

    for (i = 0; i < NB_C_TXT; ++i)
        t.text[i] = 0;

    return t;
}

PPi Read_TDT(char *pbf, int i)
{
    PPi t = Init_SPPi();

    t.ho = Get_Int(pbf, &i);

    if (pbf[i] == ':')
        ++i;
    else
    {
        t.error = 1;
        return t;
    }

    t.mi = Get_Int(pbf, &i);

    if (pbf[i] == ' ')
        ++i;
    else
    {
        t.error = 1;
        return t;
    }

    t.da = Get_Int(pbf, &i);

    if (pbf[i] == '.')
        ++i;
    else
    {
        t.error = 1;
        return t;
    }

    t.mo = Get_Int(pbf, &i);

    if (pbf[i] == '.')
        ++i;
    else
    {
        t.error = 1;
        return t;
    }

    t.ye = Get_Int(pbf, &i) * 100;
    t.ye += Get_Int(pbf, &i);

    if (pbf[i] == ' ')
        ++i;
    else
    {
        t.error = 1;
        return t;
    }

    int j = 0;

    for(j = 0; (j < (NB_C_EL + NB_C_TXT)) && (pbf[i + j] != '\n') && (pbf[i + j] != '\0'); ++j)
        t.text[j] = pbf[i + j];

    t.text[j] = '\0';

    return t;
}

int Number_Lines_File(char *pbf)
{
    int i = 0,  n = 0;

    for (i = 0; i < strlen(pbf); i++)
    {
        if ((pbf[i] == '\n') || (pbf[i] == '\0'))
            ++n;
    }

    return n;
}

int Search_Endline(char *pbf, int pos)
{
    int i = 0;

    for (i = pos; i < strlen(pbf); ++i)
    {
        if (pbf[i] == '\n')
            break;
    }

    return i+1;
}

void Error_TMN(int el)
{
	while (1)
	{
        pspDebugScreenSetXY(20, 30);
        pspDebugScreenPrintf("You have written %d notes.", el);
        pspDebugScreenSetXY(20, 31);
        pspDebugScreenPrintf("Max number notes is set to %d", NB_L);
        pspDebugScreenSetXY(20, 32);
        pspDebugScreenPrintf("Check your notes and delete %d of them", el - NB_L);

        sceKernelDelayThread(1000);
	}
}

void Error_Time(void)
{
	while (1)
	{
        pspDebugScreenSetXY(20, 30);
        pspDebugScreenPrintf("Time/Date structure error");
        pspDebugScreenSetXY(20, 31);
        pspDebugScreenPrintf("Please check time and date for errors");

        sceKernelDelayThread(1000);
	}
}

void Is_Equal_TD(PPi* pt, int nb_pi)
{
    int i = 0, j = 0, error = 0;

    for (i = 0; i < nb_pi; ++i)
    {
        for (j = 0; j < nb_pi; ++j)
        {
            if (i != j)
            {
                if (pt[i].ye == pt[j].ye)
                {
                    if (pt[i].mo == pt[j].mo)
                    {
                        if (pt[i].da == pt[j].da)
                        {
                            if (pt[i].ho == pt[j].ho)
                            {
                                if (pt[i].mi == pt[j].mi)
                                {
                                    error = 1;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (error)
            break;
    }

    i++;
    j++;

    while (error)
    {
        pspDebugScreenSetXY(20, 30);
        pspDebugScreenPrintf("Same time and date in line %d and %d", i, j);
        pspDebugScreenSetXY(20, 31);
        pspDebugScreenPrintf("Please correct the error and restart the PSP");

        sceKernelDelayThread(1000);
    }
}

void Post_It_Screen(PPi tt, pspTime t)
{
    if (t.year == tt.ye)
    {
        if (t.month == tt.mo)
        {
            if (t.day == tt.da)
            {
                if (t.hour == tt.ho)
                {
                    if (t.minutes == tt.mi)
                        blit_prx((SW - strlen(tt.text))/2, 30, tt.text, BLACK, YELLOW);
                }
            }
        }
    }
}

/*
 * Special Thanks to Mickael2054 from devsgen.com for the following functions
 */
static u32 Adjust_Alpha(u32 col)
{
    u32 c1, c2;
    u32 alpha = col>>24;
    u8 mul;

    if(alpha == 0)
        return col;
    if(alpha == 0xff)
        return col;

   c1 = col & 0x00ff00ff;
   c2 = col & 0x0000ff00;

   mul = (u8)(255 - alpha);

   c1 = ((c1*mul) >> 8) & 0x00ff00ff;
   c2 = ((c2*mul) >> 8) & 0x0000ff00;

   return (alpha << 24) | c1 | c2;
}

int blit_prx(int sx, int sy, char *txt, int fg_col,int bg_col)
{
    int pwidth, pheight, bufferwidth, pixelformat, unk;
    unsigned int* vram32;

    sceDisplayGetMode(&unk, &pwidth, &pheight);
    sceDisplayGetFrameBuf((void*)&vram32, &bufferwidth, &pixelformat, (int)&unk);

    fg_col = Adjust_Alpha(fg_col);
    bg_col = Adjust_Alpha(bg_col);

    if(bufferwidth == 0)
        return -1;

    pspDebugScreenSetColorMode(pixelformat);
    pspDebugScreenSetOffset(0);
    pspDebugScreenSetBase((u32*)0x44000000);

    pspDebugScreenSetBackColor(bg_col);
    pspDebugScreenSetTextColor(fg_col);
    pspDebugScreenSetXY(sx,sy);

    return pspDebugScreenPuts(txt);
}
