#!/usr/bin/awk -f
# Generates a Plain PPM image of random noise
# http://netpbm.sourceforge.net/doc/ppm.html
# Usage: awk -f noisegen.awk | convert - -scale 200% random-$(date "+%s%N").png"

function rng(n) { return 1 + int(rand() * n) }

BEGIN {
    srand()
    width=320
    height=240
    colordepth=1
    printf("P3 %d %d %d\n", width, height, colordepth)
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            px=rng(colordepth+1) - 1
            printf(" %d %d %d ", px, px, px)
            if (x == (width - 1))
                printf("\n")
        }
    }
}
