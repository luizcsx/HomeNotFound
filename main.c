#include <nds.h>
#include <stdio.h>

#define LEWIS_SIZE   16
#define LEWIS_SPEED  1

int main(void)
{
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_SPRITE);

    oamInit(&oamMain, SpriteMapping_1D_32, false);

    u16 *lewisGfx = oamAllocateGfx(&oamMain, SpriteSize_16x16,
                                   SpriteColorFormat_256Color);

    SPRITE_PALETTE[1] = RGB15(31, 20, 0);

    for (int i = 0; i < (LEWIS_SIZE * LEWIS_SIZE) / 2; i++)
        lewisGfx[i] = 1 | (1 << 8);

    consoleDemoInit();
    printf("\x1b[1;1HHome Not Found");
    printf("\x1b[3;1HMilestone 0");
    printf("\x1b[5;1HD-pad: mover Lewis");
    printf("\x1b[6;1HToque: ver coordenadas");

    int x = (SCREEN_WIDTH - LEWIS_SIZE) / 2;
    int y = (SCREEN_HEIGHT - LEWIS_SIZE) / 2;

    while (1)
    {
        scanKeys();
        u16 held = keysHeld();

        if (held & KEY_LEFT)  x -= LEWIS_SPEED;
        if (held & KEY_RIGHT) x += LEWIS_SPEED;
        if (held & KEY_UP)    y -= LEWIS_SPEED;
        if (held & KEY_DOWN)  y += LEWIS_SPEED;

        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > SCREEN_WIDTH  - LEWIS_SIZE) x = SCREEN_WIDTH  - LEWIS_SIZE;
        if (y > SCREEN_HEIGHT - LEWIS_SIZE) y = SCREEN_HEIGHT - LEWIS_SIZE;

        if (held & KEY_TOUCH)
        {
            touchPosition touch;
            touchRead(&touch);
            printf("\x1b[8;1HToque: %3d, %3d   ", touch.px, touch.py);
        }

        oamSet(&oamMain, 0, x, y, 0, 0,
               SpriteSize_16x16, SpriteColorFormat_256Color,
               lewisGfx, -1, false, false, false, false, false);

        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }

    return 0;
}
