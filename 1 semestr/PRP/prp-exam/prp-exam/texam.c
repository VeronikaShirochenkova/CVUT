/*
 * File name: texam.c
 * Date:      2017/01/16 08:43
 * Author:    Jan Faigl
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "xwin_sdl.h"
#include "save_jpeg.h"
#include "exam_utils.h"

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

bool process_file(FILE *f, unsigned char *image, int w, int h, bool anim);

bool copy(unsigned char *img, int image_w, int image_h, int x1, int y1, int w, int h, int x2, int y2);
bool swap(unsigned char *img, int image_w, int image_h, int x1, int y1, int w, int h, int x2, int y2);

int main(int argc, char *argv[])
{   
    int x = 10;

    if (argc == 1) {
        printf("use '--help' ! %d\n", x);
        return 0;
    }
    else if (strcmp(argv[1], "--help") == 0) {
        printf("arg ...\n");
        return 0;
    }

    // read arguments
    const char *fimage = (argc > 1) ? argv[1] : NULL;
    const char *fcommands = (argc > 2) ? argv[2] : NULL;
    const char *foutput = (argc > 3) ? argv[3] : NULL;
    const bool animation = argc > 4 && strcmp(argv[4], "--anim") == 0;

    my_assert(fimage != NULL && fcommands != NULL && foutput != NULL, __func__, __LINE__, __FILE__);

    // read image
    int image_w, image_h;
    unsigned char *image = xwin_load_image(fimage, &image_w, &image_h);
    my_assert(image != NULL, __func__, __LINE__, __FILE__);

    if (animation)
    {
        xwin_init(image_w, image_h);
    }

    // process commands
    FILE *f = fopen(fcommands, "r");
    if (f == NULL)
    {
        fprintf(stderr, "ERROR: cannot open file '%s'\n", foutput);
        free(image);
        if (animation)
        {
            xwin_close();
        }
        exit(-1);
    }
    
    if (!process_file(f, image, image_w, image_h, animation)) {
        fprintf(stderr, "ERROR: processing file '%s'\n", foutput);
        free(image);
        fclose(f);
        if (animation)
        {
            xwin_close();
        }
        exit(-1);
    }

    fclose(f);

    // save output
    if (foutput && (strstr(foutput, ".jpeg") || strstr(foutput, ".jpg")))
    {
        save_image_jpeg(image_w, image_h, image, foutput);
    }
    else
    {
        save_image_rgb(image_w, image_h, image, foutput);
    }

    if (animation)
    {
        xwin_close();
    }
    free(image);

    return EXIT_SUCCESS;
}

bool process_file(FILE *f, unsigned char *image, int image_w, int image_h, bool anim)
{

    bool result = true;
    int x1, y1, x2, y2, w, h;
    char cmd[3];
    int counter = 0;
    while (result && !feof(f))
    {
        int r = fscanf(f, "%2s %d %d %d %d %d %d\n", cmd, &x1, &y1, &w, &h, &x2, &y2);
        if (r == 7 && strcmp(cmd, "cp") == 0)
        {
            result = copy(image, image_w, image_h, x1, y1, w, h, x2, y2);
        }
        else if (r == 7 && strcmp(cmd, "sw") == 0)
        {
            result = swap(image, image_w, image_h, x1, y1, w, h, x2, y2);
        }
        else
        {
            result = false;
            break;
        }
        counter+= 1;
        if (anim )
        {
            xwin_redraw(image_w, image_h, image);
            delay(1000);
        }
    }
    return result;
}

bool copy(unsigned char *img, int image_w, int image_h, int x1, int y1, int w, int h, int x2, int y2)
{
    bool result = true;

    for (int xi = 0; xi < w; xi++)
    {
        for (int yi = 0; yi < h; yi++)
        {
            const int sx = x1 + xi;
            const int sy = y1 + yi;
            const int dx = x2 + xi;
            const int dy = y2 + yi;
            if (sx >= 0 && sx < image_w && sy >= 0 && sy < image_h &&
                dx >= 0 && dx < image_w && dy >= 0 && dy < image_h)
            {
                for (int i = 0; i < 3; i++)
                {
                    img[(dy * image_w + dx ) * 3 + i] = img[(sy * image_w + sx) * 3 + i];
                }
            }
            else
            {
                printf( "ERROR: copy cmd out of range!\n");
                result = false;
                return result;
            }
        }
    }

    return result;
}

bool swap(unsigned char *img, int image_w, int image_h, int x1, int y1, int w, int h, int x2, int y2)
{
    bool result = true;

    for (int xi = 0; xi < w; xi++)
    {
        for (int yi = 0; yi < h; yi++)
        {
            const int sx = x1 + xi;
            const int sy = y1 + yi;
            const int dx = x2 + xi;
            const int dy = y2 + yi;
            if (sx >= 0 && sx < image_w && sy >= 0 && sy < image_h &&
                dx >= 0 && dx < image_w && dy >= 0 && dy < image_h)
            {
                for (int i = 0; i < 3; i++)
                {
                    unsigned char t = img[(sy * image_w + sx) * 3 + i];
                    img[(sy * image_w + sx) * 3 + i] = img[(dy* image_w + dx) * 3 + i];
                    img[(dy * image_w + dx ) * 3 + i] = t;
                }
            }
            else
            {
                printf( "ERROR: swap cmd out of range!\n");
                result = false;
                return result;
            }
        }
    }
    return result;
}

/* end of texam.c */
