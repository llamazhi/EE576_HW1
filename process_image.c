#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

// use clamp strategy for padding
int clamp(int value, int min, int max)
{
    if (value < min)
    {
        return min;
    }
    else if (value > max)
    {
        return max;
    }
    else
    {
        return value;
    }
}

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    int h = im.h;
    int w = im.w;
    int size = h * w;

    x = clamp(x, 0, w - 1);
    y = clamp(y, 0, h - 1);
    // note that data in CHW format
    float value = im.data[c * size + y * w + x];
    return value;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    int h = im.h;
    int w = im.w;
    int ch = im.c;
    int size = h * w;
    if ((x >= 0 && x <= w - 1) && (y >= 0 && y <= h - 1) && (c >= 0 && c <= ch))
    {
        im.data[c * size + y * w + x] = v;
    }
    else
    {
        printf("test if invalid coord found\n");
        return;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    for (int i = 0; i < im.w * im.h * im.c; i++)
    {
        copy.data[i] = im.data[i];
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    int size = im.w * im.h;
    for (int i = 0; i < size; i++)
    {
        gray.data[i] = 0.299 * im.data[i] + 0.587 * im.data[i + 1 * size] + 0.114 * im.data[i + 2 * size];
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    int size = im.w * im.h;
    for (int i = 0; i < size; i++)
    {
        im.data[i + c * size] += v;
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    int size = im.c * im.h * im.w;
    for (int i = 0; i < size; i++)
    {
        if (im.data[i] < 0)
        {
            im.data[i] = 0;
        }
        else if (im.data[i] > 1)
        {
            im.data[i] = 1;
        }
        else
        {
            continue;
        }
    }
}

// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    int size = im.h * im.w;

    for (int i = 0; i < size; i++)
    {
        float R = im.data[i];
        float G = im.data[i + 1 * size];
        float B = im.data[i + 2 * size];
        float V = three_way_max(R, G, B);
        float m = three_way_min(R, G, B);
        float C = V - m;
        float S = 0;
        float H;
        float H_prime;
        if (V == 0)
        {
            S = 0;
        }
        else
        {
            S = C / V;
        }
        if (C == 0)
        {
            H = 0;
        }
        else
        {
            if (V == R)
            {
                H_prime = (G - B) / C;
            }
            else if (V == G)
            {
                H_prime = (B - R) / C + 2;
            }
            else
            {
                H_prime = (R - G) / C + 4;
            }

            if (H_prime < 0)
            {
                H = H_prime / 6 + 1;
            }
            else
            {
                H = H_prime / 6;
            }
        }
        im.data[i] = H;
        im.data[i + 1 * size] = S;
        im.data[i + 2 * size] = V;
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    int size = im.h * im.w;
    for (int i = 0; i < size; i++)
    {
        float H = im.data[i];
        float S = im.data[i + 1 * size];
        float V = im.data[i + 2 * size];
        float Hi;
        H = H * 6;
        Hi = floor(H);
        float F = H - Hi;
        float P = V * (1 - S);
        float Q = V * (1 - F * S);
        float T = V * (1 - (1 - F) * S);
        float R;
        float G;
        float B;
        if (Hi == 0)
        {
            R = V;
            G = T;
            B = P;
        }
        else if (Hi == 1)
        {
            R = Q;
            G = V;
            B = P;
        }
        else if (Hi == 2)
        {
            R = P;
            G = V;
            B = T;
        }
        else if (Hi == 3)
        {
            R = P;
            G = Q;
            B = V;
        }
        else if (Hi == 4)
        {
            R = T;
            G = P;
            B = V;
        }
        else if (Hi == 5)
        {
            R = V;
            G = P;
            B = Q;
        }
        im.data[i] = R;
        im.data[i + 1 * size] = G;
        im.data[i + 2 * size] = B;
    }
}
