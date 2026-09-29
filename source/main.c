#include <nds.h>
#include <stdio.h>

#include "bmg.h"
#include "cheat.h"
#include "dialogue_bin.h"

#define LEWIS_SIZE   16
#define LEWIS_SPEED  1
#define TURBO_SPEED  3

#define DIALOGUE_ROW 12

static const uint16_t CHEAT_TURBO[] = {
    KEY_UP, KEY_UP, KEY_DOWN, KEY_DOWN,
    KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT,
    KEY_B, KEY_A,
};

#define CHEAT_KEYS (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT | KEY_A | KEY_B | \
                    KEY_X | KEY_Y | KEY_L | KEY_R | KEY_START | KEY_SELECT)

static void draw_dialogue(const char *text)
{
    char ascii[29];
    bmg_to_ascii(text, ascii, sizeof(ascii));
    printf("\x1b[%d;1H> %-27s", DIALOGUE_ROW, ascii);
}

static void clear_dialogue(void)
{
    printf("\x1b[%d;1H%-29s", DIALOGUE_ROW, "");
}

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
    printf("\x1b[1;1HHOME NOT FOUND");
    printf("\x1b[2;1HLuiz Miguel");
    printf("\x1b[4;1HMilestone 0.1");
    printf("\x1b[6;1HD-pad: mover Lewis");
    printf("\x1b[7;1HToque: abrir/fechar dialogo");

    Bmg dialogue;
    bool dialogue_ok = bmg_load(&dialogue, dialogue_bin, dialogue_bin_size);
    if (!dialogue_ok)
        printf("\x1b[20;1HErro: BMG invalido");

    bool dialogue_visible = false;

    Cheat turbo_cheat;
    cheat_init(&turbo_cheat, CHEAT_TURBO,
               sizeof(CHEAT_TURBO) / sizeof(CHEAT_TURBO[0]));
    int speed = LEWIS_SPEED;

    int x = (SCREEN_WIDTH - LEWIS_SIZE) / 2;
    int y = (SCREEN_HEIGHT - LEWIS_SIZE) / 2;

    while (1)
    {
        scanKeys();
        u16 held = keysHeld();
        u16 down = keysDown();

        if (cheat_feed(&turbo_cheat, down & CHEAT_KEYS))
        {
            speed = (speed == LEWIS_SPEED) ? TURBO_SPEED : LEWIS_SPEED;
            printf("\x1b[14;1H%-29s",
                   speed == TURBO_SPEED ? "CHEAT: turbo ligado" : "CHEAT: turbo desligado");
        }

        if (held & KEY_LEFT)  x -= speed;
        if (held & KEY_RIGHT) x += speed;
        if (held & KEY_UP)    y -= speed;
        if (held & KEY_DOWN)  y += speed;

        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > SCREEN_WIDTH  - LEWIS_SIZE) x = SCREEN_WIDTH  - LEWIS_SIZE;
        if (y > SCREEN_HEIGHT - LEWIS_SIZE) y = SCREEN_HEIGHT - LEWIS_SIZE;

        if (held & KEY_TOUCH)
        {
            touchPosition touch;
            touchRead(&touch);
            printf("\x1b[9;1HToque: %3d, %3d   ", touch.px, touch.py);
        }

        if ((down & KEY_TOUCH) && dialogue_ok)
        {
            dialogue_visible = !dialogue_visible;
            if (dialogue_visible)
            {
                const char *msg = bmg_get(&dialogue, 0);
                draw_dialogue(msg != NULL ? msg : "(mensagem vazia)");
            }
            else
            {
                clear_dialogue();
            }
        }

        oamSet(&oamMain, 0, x, y, 0, 0,
               SpriteSize_16x16, SpriteColorFormat_256Color,
               lewisGfx, -1, false, false, false, false, false);

        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }

    return 0;
}
