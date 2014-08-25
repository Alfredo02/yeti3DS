#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <3ds/types.h>
#include <3ds/srv.h>
#include <3ds/APT.h>
#include <3ds/GSP.h>
#include <3ds/HID.h>
#include <3ds/svc.h>
#include <3ds/gfx.h>

#include "yeti.h"

u8 leftOrRight;

void yetiUpdateKeyboard(yeti_t* y)
{
	u32 keys=((u32*)0x10000000)[7];

	y->keyboard.a       = keys&KEY_A;
	y->keyboard.b       = keys&KEY_B;
	y->keyboard.select  = keys&KEY_SELECT;
	y->keyboard.left    = keys&KEY_LEFT;
	y->keyboard.right   = keys&KEY_RIGHT;
	y->keyboard.up      = keys&KEY_UP;
	y->keyboard.down    = keys&KEY_DOWN;
	y->keyboard.r       = keys&KEY_R;
	y->keyboard.l       = keys&KEY_L;
}

yeti_t yeti;

int main()
{
	srvInit();
	
	aptInit(APPID_APPLICATION);

	gfxInit();

	hidInit(NULL);

	// aptSetupEventHandler();

	yeti_init(
		&yeti,
		(framebuffer_t*)gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL),
		(framebuffer_t*)gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL),
		textures, palette, lua
	);

	game_init(&yeti);

	APP_STATUS status;
	while((status=aptGetStatus())!=APP_EXITING)
	{
		// if(status == APP_RUNNING)
		{
			yeti.viewport.front = yeti.viewport.back;
			yeti.viewport.back = (framebuffer_t*)gfxGetFramebuffer(GFX_TOP, leftOrRight?GFX_LEFT:GFX_RIGHT, NULL, NULL);
			
			game_draw(&yeti);
			game_tick(&yeti);

			yetiUpdateKeyboard(&yeti);

			gfxFlushBuffers();
			gfxSwapBuffers();
			leftOrRight^=1;
		}
		// else if(status == APP_SUSPENDING)
		// {
		// 	aptReturnToMenu();
		// }
		// else if(status == APP_SLEEPMODE)
		// {
		// 	aptWaitStatusEvent();
		// }
		svcSleepThread(16666666/2);
	}

	gfxExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}
