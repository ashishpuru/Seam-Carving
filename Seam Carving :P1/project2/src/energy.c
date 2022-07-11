#include "energy.h"

#include <stdlib.h>
#include <string.h>

#include "indexing.h"
#include "util.h"

uint32_t max(uint32_t a, uint32_t b) { return a > b ? a : b; }

uint32_t min(uint32_t a, uint32_t b) { return a < b ? a : b; }

/**
 * Calculate the difference of two color values @p a and @p b.
 * The result is the sum of the squares of the differences of the three (red,
 * green and blue) color components.
 * */
inline uint32_t diff_color(struct pixel const a, struct pixel const b) {
    int ra ,rb ,ga , gb , ba ,bb ,x ,y,z ;
    ra = a.r;
    rb = b.r;
    ga = a.g;
    gb = b.g;
    ba = a.b;
    bb = b.b;
    x= (ra-rb)* (ra-rb);
    y= (ga-gb)*(ga-gb);
    z= (ba-bb)*(ba-bb);
    return x+y+z;
}

/**
 * Calculate the total energy at every pixel of the image @p `img`,
 * but only considering columns with index less than @p `w`.
 * To this end, first calculate the local energy and use it to calculate the
 * total energy.
 * @p `energy` is expected to have allocated enough space
 * to represent the energy for every pixel of the whole image @p `img.
 * @p `w` is the width up to (excluding) which column in the image the energy
 * should be calculated. The energy is expected to be stored exactly analogous
 * to the image, i.e. you should be able to access the energy of a pixel with
 * the same array index.
 */
void calculate_energy(uint32_t* const energy, struct image* const img,
                      int const w) { 
    int i;int j;
    int wid,ht;
    wid = img->w;
    ht = img->h;
    int localEnergy[wid*ht];
    for(i=0;i<ht;i++)
    {
        for(j=0;j<w;j++)
        {
            if (i==0 && j==0){
                energy[i*wid +j] =  0;
            }
            else if(j==0 && i>0){
                energy[i*wid +j] = diff_color(img->pixels[yx_index(i,j,wid)], img->pixels[yx_index(i-1,j,wid)]);
            }
            else if(i==0 && j>0){
                energy[i*wid +j] = diff_color(img->pixels[yx_index(i,j,wid)], img->pixels[yx_index(i,j-1,wid)]);
            }
            else{
                energy[i*wid +j] = diff_color(img->pixels[yx_index(i,j,wid)], img->pixels[yx_index(i-1,j,wid)]);
                energy[i*wid +j] += diff_color(img->pixels[yx_index(i,j,wid)], img->pixels[yx_index(i,j-1,wid)]);
            }
        }
    }
    int mini = 0;
    for(i=1;i<ht;i++)
    {
        for(j=0;j<w;j++)
        {
            
            if (j == 0){
                energy[i*wid + j] += min(energy[(i-1)*wid + j],energy[(i-1)*wid + j+1]);
            }
            else if (j == w-1){
                energy[i*wid + j] += min(energy[(i-1)*wid + j],energy[(i-1)*wid + j-1]);
            }
            else{
                energy[i*wid + j] += min(min(energy[(i-1)*wid + j],energy[(i-1)*wid + j+1]),energy[(i-1)*wid + j-1]);
            }
        }
    }
    return;
}

/**
 * Calculate the index of the column with the least energy in bottom row.
 * Expects that @p `energy` holds the energy of every pixel of @p `img` up to
 * column (excluding) @p `w`. Columns with index `>= w` are not considered as
 * part of the image.
 * @p `w0` states the width of the energy matrix @p `energy`
 * @p `h` states the height of the energy matrix @p `energy`
 */
int calculate_min_energy_column(uint32_t const* const energy, int const w0,
                                int const w, int const h) {
    // TODO implement (assignment 3.2)
    int mini = energy[(h-1)*w0];
    int minIndex = 0;
    for(int i=1;i<w;i++){
        if (energy[(h-1)*w0 + i] < mini){
            mini = energy[(h-1)*w0 + i];
            minIndex = i;
        }
    }
    return minIndex;
}

/**
 * Calculate the optimal path (i.e. least energy), according to the energy
 * entries in @p `energy` up to (excluding) column @p `w`. The path is stored in
 * @p `seam`. Columns with index `>= w` are not considered as part of the image.
 * @p `x` is the index in the bottom row where the seam starts.
 * @p `w0` states the width of the energy matrix @p `energy`
 * @p `h` states the height of the energy matrix @p `energy`
 */
void calculate_optimal_path(uint32_t const* const energy, int const w0,
                            int const w, int const h, int x,
                            uint32_t* const seam) {
    seam[h-1]=x;
    int topLeft,top,topRight;
    int energyLeft,energyTop,energyRight;
    int colNo;
    int j = x;
    int seamId  = h-1;
    for(int i=h-2;i>=0;i--){
        colNo = seam[seamId];
        seamId--;
        // Need to go though three cases
        energyTop = energy[yx_index(i,colNo,w0)];
        if (colNo == 0){
            energyRight = energy[yx_index(i,colNo+1,w0)];
            if (min(energyRight,energyTop) == energyTop) {
                seam[seamId] = colNo;
            }
            else{
                seam[seamId] = colNo+1;
            }
        }
        else  if (colNo == w-1){
            energyLeft = energy[yx_index(i,colNo-1,w0)];
            if (min(energyLeft,energyTop) == energyTop) {
                seam[seamId] = colNo;
            }
            else{
                seam[seamId] = colNo-1;
            }
        }
        else{
            energyLeft = energy[yx_index(i,colNo-1,w0)];
            energyRight = energy[yx_index(i,colNo+1,w0)];
            if (min(min(energyLeft,energyTop),energyRight) == energyTop){
                seam[seamId] = colNo;
            }
            else if (min(energyRight,energyLeft) == energyLeft){
                seam[seamId] = colNo-1;
            }
            else{
                seam[seamId] = colNo+1;
            }
        }
    }
    return;
}
