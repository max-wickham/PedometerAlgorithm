
#include <stdio.h>
#include "system.h"
#include "altera_up_avalon_accelerometer_spi.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include <sys/alt_stdio.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "altera_avalon_pio_regs.h"


// #define TRUE 1
// #define FALSE 0
// //Acceleration
// struct ListAxis{
//     long int data[243];
//     int cursor;
//     int length;
// };

// struct ListConstants{
//     long int data[243];
// };

// void insert(struct ListAxis* list, long int data){
//     list->cursor--;
//     if(list->cursor < 0){
//         list->cursor += list->length;
//     }
//     list->data[list->cursor] = data;
// }

// long int filterProduct(struct ListAxis* a, struct ListConstants* alpha){
//     long int total = 0;
//     int cursor;
//     long int data;
//     for(int i = 0; i < a->length; i++){
//         cursor = (i + a->cursor) % a->length;
//         data = ((a->data[cursor])*(alpha->data[i]));
//         data >>= 20;
//         total += data;
//     }
//     return total;
// }

// struct Acceleration{
//     struct ListAxis* a;
//     struct ListConstants* alpha;
//     long int firA;
// };

// void fir(struct Acceleration* acceleration){
//     acceleration->firA = filterProduct(acceleration->a,acceleration->alpha);
// }

// long int userA(struct Acceleration* acceleration){
//     return (acceleration->a->data[acceleration->a->cursor] - acceleration->firA);
// }

// long int gravitationalA(struct Acceleration* acceleration){
//     return (acceleration->firA);
// }

// //Accelerometer
// struct ListAccelerometer{
//     long int data[396];
//     int cursor;
//     int length;
// };

// struct ListConstantsAccelerometer{
//     long int data[396];
// };

// struct Accelerometer{
//     struct Acceleration* x;
//     struct Acceleration* y;
//     struct Acceleration* z;
//     struct ListAccelerometer* a;
//     struct ListConstantsAccelerometer* alpha;
// };

// void insertA(struct ListAccelerometer* list, long int data){
//     list->cursor--;
//     if(list->cursor < 0){
//         list->cursor += list->length;
//     }
//     list->data[list->cursor] = data;
// }

// void insertAccelerometer(struct Accelerometer* accelerometer, long int x, long int y, long int z){
//     insert(accelerometer->x->a,x);
//     insert(accelerometer->y->a,y);
//     insert(accelerometer->z->a,z);
// }

// long int userAccelerationGravitationalDirection(struct Accelerometer* accelerometer){
//     fir(accelerometer->x);
//     fir(accelerometer->y);
//     fir(accelerometer->z);
//     long int x_sum = userA(accelerometer->x) * gravitationalA(accelerometer->x);
//     long int y_sum = userA(accelerometer->y) * gravitationalA(accelerometer->y);
//     long int z_sum = userA(accelerometer->z) * gravitationalA(accelerometer->z);
//     return (x_sum + y_sum + z_sum);
// }

// long int firAccelerometer(struct Accelerometer* accelerometer){
//     long int total = 0;
//     long int data;
//     int cursor;
//     for(int i = 0; i < accelerometer->a->length; i++){
//         cursor = (i + accelerometer->a->cursor) % accelerometer->a->length;
//         long int data = ((accelerometer->a->data[cursor])*(accelerometer->alpha->data[i]));
//         data >>= 19;
//         total += data;
//     }
//     return total;
// }

// long int filter(struct Accelerometer* accelerometer){
//     insertA(accelerometer->a,userAccelerationGravitationalDirection(accelerometer));
//     return firAccelerometer(accelerometer);
// }

// //StepDetection
// long int run_filter(struct Accelerometer* accelerometer, long int x, long int y, long int z){
//     insertAccelerometer(accelerometer,x,y,z);
//     return filter(accelerometer);
// }

int main(){
    // //first initialise the three acceleration axis;
    // struct ListAxis x; x.cursor = 243; x.length = 243;
    // struct ListAxis y; y.cursor = 243; y.length = 243;
    // struct ListAxis z; z.cursor = 243; z.length = 243;
    // struct ListConstants alphaAxis = {.data = {  650 ,  155 ,  174 ,  193 ,  214 ,  237 ,  260 ,  286 ,  312 ,  340 ,  370 ,  401 ,  435 ,  469 ,  506 ,  544 ,  585 ,  627 ,  671 ,  716 ,  764 ,  814 ,  866 ,  920 ,  976 ,  1034 ,  1094 ,  1156 ,  1221 ,  1288 ,  1356 ,  1428 ,  1501 ,  1577 ,  1655 ,  1734 ,  1817 ,  1901 ,  1988 ,  2076 ,  2167 ,  2260 ,  2355 ,  2452 ,  2551 ,  2652 ,  2756 ,  2861 ,  2968 ,  3076 ,  3187 ,  3299 ,  3413 ,  3529 ,  3646 ,  3765 ,  3885 ,  4007 ,  4129 ,  4253 ,  4378 ,  4504 ,  4631 ,  4759 ,  4888 ,  5017 ,  5147 ,  5277 ,  5408 ,  5538 ,  5670 ,  5801 ,  5932 ,  6062 ,  6193 ,  6323 ,  6453 ,  6582 ,  6710 ,  6837 ,  6964 ,  7089 ,  7213 ,  7336 ,  7457 ,  7577 ,  7695 ,  7811 ,  7926 ,  8038 ,  8148 ,  8256 ,  8362 ,  8465 ,  8565 ,  8663 ,  8758 ,  8851 ,  8940 ,  9026 ,  9109 ,  9189 ,  9265 ,  9339 ,  9408 ,  9474 ,  9536 ,  9595 ,  9650 ,  9701 ,  9748 ,  9792 ,  9831 ,  9866 ,  9897 ,  9925 ,  9948 ,  9966 ,  9981 ,  9992 ,  9998 ,  10000 ,  9998 ,  9992 ,  9981 ,  9966 ,  9948 ,  9925 ,  9897 ,  9866 ,  9831 ,  9792 ,  9748 ,  9701 ,  9650 ,  9595 ,  9536 ,  9474 ,  9408 ,  9339 ,  9265 ,  9189 ,  9109 ,  9026 ,  8940 ,  8851 ,  8758 ,  8663 ,  8565 ,  8465 ,  8362 ,  8256 ,  8148 ,  8038 ,  7926 ,  7811 ,  7695 ,  7577 ,  7457 ,  7336 ,  7213 ,  7089 ,  6964 ,  6837 ,  6710 ,  6582 ,  6453 ,  6323 ,  6193 ,  6062 ,  5932 ,  5801 ,  5670 ,  5538 ,  5408 ,  5277 ,  5147 ,  5017 ,  4888 ,  4759 ,  4631 ,  4504 ,  4378 ,  4253 ,  4129 ,  4007 ,  3885 ,  3765 ,  3646 ,  3529 ,  3413 ,  3299 ,  3187 ,  3076 ,  2968 ,  2861 ,  2756 ,  2652 ,  2551 ,  2452 ,  2355 ,  2260 ,  2167 ,  2076 ,  1988 ,  1901 ,  1817 ,  1734 ,  1655 ,  1577 ,  1501 ,  1428 ,  1356 ,  1288 ,  1221 ,  1156 ,  1094 ,  1034 ,  976 ,  920 ,  866 ,  814 ,  764 ,  716 ,  671 ,  627 ,  585 ,  544 ,  506 ,  469 ,  435 ,  401 ,  370 ,  340 ,  312 ,  286 ,  260 ,  237 ,  214 ,  193 ,  174 ,  155 ,  650}};
    // struct Acceleration accelerationX; accelerationX.a = &x; accelerationX.alpha = &alphaAxis; accelerationX.firA = 0;
    // struct Acceleration accelerationY; accelerationY.a = &y; accelerationY.alpha = &alphaAxis; accelerationY.firA = 0;
    // struct Acceleration accelerationZ; accelerationZ.a = &z; accelerationZ.alpha = &alphaAxis; accelerationZ.firA = 0;
    // struct ListAccelerometer a; a.cursor = 0; a.length = 396;
    // struct ListConstantsAccelerometer alphaA = {.data = {  348 ,  29 ,  29 ,  27 ,  25 ,  22 ,  19 ,  15 ,  10 ,  5 ,  0 ,  -4 ,  -8 ,  -12 ,  -15 ,  -16 ,  -17 ,  -16 ,  -13 ,  -10 ,  -5 ,  1 ,  8 ,  15 ,  23 ,  31 ,  38 ,  44 ,  48 ,  50 ,  49 ,  45 ,  37 ,  24 ,  7 ,  -13 ,  -40 ,  -71 ,  -106 ,  -145 ,  -187 ,  -232 ,  -278 ,  -325 ,  -371 ,  -415 ,  -455 ,  -491 ,  -520 ,  -541 ,  -553 ,  -555 ,  -544 ,  -522 ,  -486 ,  -437 ,  -374 ,  -298 ,  -209 ,  -108 ,  3 ,  123 ,  251 ,  384 ,  520 ,  655 ,  787 ,  913 ,  1030 ,  1136 ,  1227 ,  1300 ,  1355 ,  1387 ,  1397 ,  1382 ,  1342 ,  1276 ,  1186 ,  1071 ,  935 ,  777 ,  602 ,  412 ,  211 ,  1 ,  -211 ,  -423 ,  -631 ,  -830 ,  -1015 ,  -1184 ,  -1333 ,  -1457 ,  -1556 ,  -1627 ,  -1668 ,  -1679 ,  -1660 ,  -1612 ,  -1536 ,  -1436 ,  -1312 ,  -1169 ,  -1012 ,  -843 ,  -668 ,  -492 ,  -319 ,  -154 ,  -2 ,  134 ,  250 ,  344 ,  413 ,  455 ,  471 ,  459 ,  423 ,  363 ,  283 ,  187 ,  78 ,  -36 ,  -153 ,  -266 ,  -368 ,  -454 ,  -518 ,  -555 ,  -561 ,  -530 ,  -460 ,  -350 ,  -198 ,  -4 ,  228 ,  498 ,  801 ,  1130 ,  1480 ,  1842 ,  2208 ,  2569 ,  2914 ,  3233 ,  3517 ,  3755 ,  3937 ,  4055 ,  4101 ,  4069 ,  3953 ,  3750 ,  3459 ,  3079 ,  2613 ,  2066 ,  1444 ,  755 ,  10 ,  -779 ,  -1599 ,  -2436 ,  -3273 ,  -4095 ,  -4884 ,  -5624 ,  -6298 ,  -6892 ,  -7390 ,  -7779 ,  -8050 ,  -8191 ,  -8197 ,  -8063 ,  -7787 ,  -7371 ,  -6819 ,  -6136 ,  -5333 ,  -4422 ,  -3416 ,  -2332 ,  -1188 ,  -4 ,  1198 ,  2400 ,  3579 ,  4713 ,  5781 ,  6765 ,  7645 ,  8405 ,  9031 ,  9511 ,  9836 ,  10000 ,  10000 ,  9836 ,  9511 ,  9031 ,  8405 ,  7645 ,  6765 ,  5781 ,  4713 ,  3579 ,  2400 ,  1198 ,  -4 ,  -1188 ,  -2332 ,  -3416 ,  -4422 ,  -5333 ,  -6136 ,  -6819 ,  -7371 ,  -7787 ,  -8063 ,  -8197 ,  -8191 ,  -8050 ,  -7779 ,  -7390 ,  -6892 ,  -6298 ,  -5624 ,  -4884 ,  -4095 ,  -3273 ,  -2436 ,  -1599 ,  -779 ,  10 ,  755 ,  1444 ,  2066 ,  2613 ,  3079 ,  3459 ,  3750 ,  3953 ,  4069 ,  4101 ,  4055 ,  3937 ,  3755 ,  3517 ,  3233 ,  2914 ,  2569 ,  2208 ,  1842 ,  1480 ,  1130 ,  801 ,  498 ,  228 ,  -4 ,  -198 ,  -350 ,  -460 ,  -530 ,  -561 ,  -555 ,  -518 ,  -454 ,  -368 ,  -266 ,  -153 ,  -36 ,  78 ,  187 ,  283 ,  363 ,  423 ,  459 ,  471 ,  455 ,  413 ,  344 ,  250 ,  134 ,  -2 ,  -154 ,  -319 ,  -492 ,  -668 ,  -843 ,  -1012 ,  -1169 ,  -1312 ,  -1436 ,  -1536 ,  -1612 ,  -1660 ,  -1679 ,  -1668 ,  -1627 ,  -1556 ,  -1457 ,  -1333 ,  -1184 ,  -1015 ,  -830 ,  -631 ,  -423 ,  -211 ,  1 ,  211 ,  412 ,  602 ,  777 ,  935 ,  1071 ,  1186 ,  1276 ,  1342 ,  1382 ,  1397 ,  1387 ,  1355 ,  1300 ,  1227 ,  1136 ,  1030 ,  913 ,  787 ,  655 ,  520 ,  384 ,  251 ,  123 ,  3 ,  -108 ,  -209 ,  -298 ,  -374 ,  -437 ,  -486 ,  -522 ,  -544 ,  -555 ,  -553 ,  -541 ,  -520 ,  -491 ,  -455 ,  -415 ,  -371 ,  -325 ,  -278 ,  -232 ,  -187 ,  -145 ,  -106 ,  -71 ,  -40 ,  -13 ,  7 ,  24 ,  37 ,  45 ,  49 ,  50 ,  48 ,  44 ,  38 ,  31 ,  23 ,  15 ,  8 ,  1 ,  -5 ,  -10 ,  -13 ,  -16 ,  -17 ,  -16 ,  -15 ,  -12 ,  -8 ,  -4 ,  0 ,  5 ,  10 ,  15 ,  19 ,  22 ,  25 ,  27 ,  29 ,  29 ,  348}};
    // struct Accelerometer accelerometer;
    // accelerometer.x = &accelerationX; accelerometer.y = &accelerationY; accelerometer.z = &accelerationZ;
    // accelerometer.a = &a;
    // accelerometer.alpha = &alphaA;
    // long int data;

    // int count = 0;
    // int below = TRUE;
    // long int peakMag = 10000000;
    // long int lowMag = 0;

    // alt_32 x_read;
    // alt_32 y_read;
    // alt_32 z_read;
    // alt_up_accelerometer_spi_dev * acc_dev;
    // acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");
    // if (acc_dev == NULL) { // if return 1, check if the spi ip name is "accelerometer_spi"
    //     return 1;
    // }

    // int test_count = 0;
    // while (1) {

    //     alt_up_accelerometer_spi_read_x_axis(acc_dev, & x_read);
    //     alt_up_accelerometer_spi_read_y_axis(acc_dev, & y_read);
    //     alt_up_accelerometer_spi_read_z_axis(acc_dev, & z_read);
    //     //fprintf(fp," %ld ",x_read);

    //     data = run_filter(&accelerometer,x_read,y_read,z_read);
    //     if(below){
    //         if(data > peakMag){
    //             count += 1;
    //             below = FALSE;
    //         }
    //     }else{
    //         if(data < lowMag){
    //             below = TRUE;
    //         }
    //     }
    //     if((test_count % 100) == 0){
    //         printf("%ld ",data);
    //     }
    //     test_count += 1;



    // }
    printf("%d ",0);
}
