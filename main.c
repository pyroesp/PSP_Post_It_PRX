/*

	PSP Post It - PRX
		by pyroesp

*/

#include <pspkernel.h>
#include <pspdisplay.h>
#include <psptypes.h>
#include <pspctrl.h>
#include <psprtc.h>


#include "post_it.h"
#include "blit.h"


#define COLOR_BLACK 0xFF000000
#define COLOR_YELLOW 0xFF00F3F3


#define POST_IT_PRX_PATH "ms0:/seplugins/"
#define POST_IT_PRX_FILE "post it.json"


// int postit_thread(SceSize args, void *argp);


PSP_MODULE_INFO("PSP Post It - PRX", 0x1000, 1, 1);

PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);


// int module_start(SceSize args, void *argp){
	// int	thid;
	// thid = sceKernelCreateThread("PostIt_thread", postit_thread, 25, 0x1000, 0, NULL);

	// if (thid >= 0)
		// sceKernelStartThread(thid, 0, 0);
	
	// return 0;
// }


int module_stop(SceSize args, void *argp){
	return 0;
}

int main(int argc, char **argv){
// int postit_thread(SceSize args, void *argp){
	// wait for psp boot
	// sceKernelDelayThread(3700000);
	sceKernelDelayThread(5000);


	int file = 0;
	file = sceIoOpen("ms0:/debug.txt", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);

	if (!file)
        return 0;
	
	char buffer[] = "test prx loaded";
	sceIoWrite(file, buffer, sizeof(buffer)); 

    //sceIoRead(file, bf, sizeof(bf));
	sceIoClose(file);
	

	pspTime t;
	SceCtrlData pad;

	while (1){
        //sceRtcGetCurrentClockLocalTime(&t);
		//sceCtrlPeekBufferPositive(&pad, 1);
		
		//blit setup 
		blit_setup();
		//blit_set_color(COLOR_BLACK, COLOR_YELLOW);
		blit_string(50, 175, "PSP Post It - PRX : enabled");
		
		// if (pad.Buttons & (PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER)){
			// blit_string(50, 200, "PSP Post It - PRX : enabled");
		// }

		sceDisplayWaitVblankStart();
		sceKernelDelayThread(50000);
	}

	return 0;
}

