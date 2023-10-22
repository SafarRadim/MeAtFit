/**
 * IZG - Zaklady pocitacove grafiky - FIT VUT
 * Lab 01 - Redukce barevneho prostoru
 *
 * Copyright (c) 2022 by Michal Vlnas, ivlnas@fit.vutbr.cz
 *
 * Tato sablona slouzi vyhradne pro studijni ucely, sireni kodu je bez vedomi autora zakazano.
 */

#include "base.h"

void ImageTransform::grayscale()
{
    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++) 
        {
            auto p = getPixel(x, y);
            RGB newColor = std::round(0.114 * p.b + 0.587 * p.g + 0.299 * p.r);
            setPixel(x, y, newColor); 
        }
    }
}

void ImageTransform::threshold()
{
    grayscale();

    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            auto p = getPixel(x, y);
            if (p.r > cfg->threshold)
            {
                setPixel(x, y, COLOR_WHITE);
            }
            else
            {
                setPixel(x, y, COLOR_BLACK);
            }
        }
    }
}

void ImageTransform::randomDithering()
{
    grayscale();

    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            auto p = getPixel(x, y);
            uint8_t value = p.r > getRandom() ? 255 : 0;

            setPixel(x, y, RGB(value));
        }
    }
}

void ImageTransform::orderedDithering()
{
    int matrix[4][4] = {
        {0, 204, 51, 255},
        {68, 136, 187, 119},
        {34, 238, 17, 221},
        {170, 102, 153, 85}
    };

    grayscale();

    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            auto p = getPixel(x, y);
            
            if (p.r > matrix[y % 4][x % 4]) 
            {
                setPixel(x, y, COLOR_WHITE);
            }
            else 
            {
                setPixel(x, y, COLOR_BLACK);
            }
        }
    }

}

void ImageTransform::updatePixelWithError(uint32_t x, uint32_t y, float err)
{
    auto p = getPixel(x, y);
    int val = p.r + err;
    if (val < 0) {
        setPixel(x, y, 0);
    }
    else if (val > 255) {
        setPixel(x, y, 255);
    }
    else {
        setPixel(x, y, val);
    }
}


void UpdatePixels(uint32_t x, uint32_t y, float err, ImageTransform *transform)
{
    if (x + 1 < transform->cfg->width) {
        transform->updatePixelWithError(x + 1, y, err * 3 / 8);
    }
    if (y + 1 < transform->cfg->height) {
        transform->updatePixelWithError(x, y + 1, err * 3 / 8);
    }
    if (x + 1 < transform->cfg->width && y + 1 < transform->cfg->height) {
        transform->updatePixelWithError(x + 1, y + 1, err * 2 / 8);
    }
}

void ImageTransform::errorDistribution()
{
    grayscale();

    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            auto p = getPixel(x, y);
            if (p.r > 127) {
                float err = p.r - 255.0;
                UpdatePixels(x, y, err, this);
                setPixel(x, y, COLOR_WHITE);
            }
            else {
                float err = p.r * 1.0;
                UpdatePixels(x, y, err, this);
                setPixel(x, y, COLOR_BLACK);
            }
        }
    }
}

