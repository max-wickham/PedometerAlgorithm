
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/alt_stdio.h>

#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_timer_regs.h"
#include "altera_up_avalon_accelerometer_spi.h"
#include "sys/alt_irq.h"
#include "system.h"

// Acceleration
struct ListAxis
{
    long int data[{{axis_size}}];
    int cursor;
    int length;
};

struct ListConstants
{
    long int data[{{axis_size}}];
};

void insert(struct ListAxis *list, long int data)
{
    list->cursor--;
    if (list->cursor < 0)
    {
        list->cursor += list->length;
    }
    list->data[list->cursor] = data;
}

long int filterProduct(struct ListAxis *a, struct ListConstants *alpha)
{
    long int total = 0;
    int cursor;
    long int data;
    for (int i = 0; i < a->length; i++)
    {
        cursor = (i + a->cursor) % a->length;
        data = ((a->data[cursor]) * (alpha->data[i]));
        data >>= {{lpfBitScalar}};
        total += data;
    }
    return total;
}

struct Acceleration
{
    struct ListAxis *a;
    struct ListConstants *alpha;
    long int firA;
};

void fir(struct Acceleration *acceleration)
{
    acceleration->firA = filterProduct(acceleration->a, acceleration->alpha);
}

long int userA(struct Acceleration *acceleration)
{
    return (acceleration->a->data[acceleration->a->cursor] - acceleration->firA);
}

long int gravitationalA(struct Acceleration *acceleration)
{
    return (acceleration->firA);
}

// Accelerometer
struct ListAccelerometer
{
    long int data[{{acceleration_size}}];
    int cursor;
    int length;
};

struct ListConstantsAccelerometer
{
    long int data[{{acceleration_size}}];
};

struct Accelerometer
{
    struct Acceleration *x;
    struct Acceleration *y;
    struct Acceleration *z;
    struct ListAccelerometer *a;
    struct ListConstantsAccelerometer *alpha;
};

void insertA(struct ListAccelerometer *list, long int data)
{
    list->cursor--;
    if (list->cursor < 0)
    {
        list->cursor += list->length;
    }
    list->data[list->cursor] = data;
}

void insertAccelerometer(struct Accelerometer *accelerometer, long int x, long int y, long int z)
{
    insert(accelerometer->x->a, x);
    insert(accelerometer->y->a, y);
    insert(accelerometer->z->a, z);
}

long int userAccelerationGravitationalDirection(struct Accelerometer *accelerometer)
{
    fir(accelerometer->x);
    fir(accelerometer->y);
    fir(accelerometer->z);
    long int x_sum = userA(accelerometer->x) * gravitationalA(accelerometer->x);
    long int y_sum = userA(accelerometer->y) * gravitationalA(accelerometer->y);
    long int z_sum = userA(accelerometer->z) * gravitationalA(accelerometer->z);
    return (x_sum + y_sum + z_sum);
}

long int firAccelerometer(struct Accelerometer *accelerometer)
{
    long int total = 0;
    long int data;
    int cursor;
    for (int i = 0; i < accelerometer->a->length; i++)
    {
        cursor = (i + accelerometer->a->cursor) % accelerometer->a->length;
        long int data = ((accelerometer->a->data[cursor]) * (accelerometer->alpha->data[i]));
        data >>= {{bpfBitScalar}};
        total += data;
    }
    return total;
}

long int filter(struct Accelerometer *accelerometer)
{
    insertA(accelerometer->a, userAccelerationGravitationalDirection(accelerometer));
    return firAccelerometer(accelerometer);
}

// StepDetection
long int run_filter(struct Accelerometer *accelerometer, long int x, long int y, long int z)
{
    insertAccelerometer(accelerometer, x, y, z);
    return filter(accelerometer);
}

long int ewma(long data, long prev_value, unsigned weight, unsigned total_weight)
{
    /* if a = 0.2 => weight = 2, total_weight = 10, indicates window size of ~9 */

    /* weight and total weight represent the fraction between 0-1 to improve the
     * weight of the previous value*/
    return (weight * data + (total_weight - weight) * prev_value) / total_weight;
}

int main()
{
    // first initialise the three acceleration axis;
    struct ListAxis x;
    x.cursor = {{axis_size}};
    x.length = {{axis_size}};
    struct ListAxis y;
    y.cursor = {{axis_size}};
    y.length = {{axis_size}};
    struct ListAxis z;
    z.cursor = {{axis_size}};
    z.length = {{axis_size}};
    struct ListConstants alphaAxis = {.data = { {% for x in lpf[:-1] %} {{x}} , {% endfor %} {{lpf[axis_size-1]}}}};
    struct Acceleration accelerationX;
    accelerationX.a = &x;
    accelerationX.alpha = &alphaAxis;
    accelerationX.firA = 0;
    struct Acceleration accelerationY;
    accelerationY.a = &y;
    accelerationY.alpha = &alphaAxis;
    accelerationY.firA = 0;
    struct Acceleration accelerationZ;
    accelerationZ.a = &z;
    accelerationZ.alpha = &alphaAxis;
    accelerationZ.firA = 0;
    struct ListAccelerometer a;
    a.cursor = 0;
    a.length = {{acceleration_size}};
    struct ListConstantsAccelerometer alphaA = {.data = { {% for x in bpf[:-1] %} {{x}} , {% endfor %} {{bpf[acceleration_size-1]}}}};
    struct Accelerometer accelerometer;
    accelerometer.x = &accelerationX;
    accelerometer.y = &accelerationY;
    accelerometer.z = &accelerationZ;
    accelerometer.a = &a;
    accelerometer.alpha = &alphaA;
    long int data = 0;
    long int gravX;
    long int gravY;
    long int gravZ;
    int gravTotal;
    bool correctGrav = false;

    int count = 0;
    int below = true;
    long int peakMag = 5500;
    long int lowMag = 0;

    alt_32 x_read;
    alt_32 y_read;
    alt_32 z_read;
    alt_up_accelerometer_spi_dev *acc_dev;
    acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");
    if (acc_dev == NULL)
    { // if return 1, check if the spi ip name is "accelerometer_spi"
        return 1;
    }

    int test_count = 0;
    long int prev_value;
    bool is_first_cycle = true;
    while (1)
    {
        alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);
        alt_up_accelerometer_spi_read_y_axis(acc_dev, &y_read);
        alt_up_accelerometer_spi_read_z_axis(acc_dev, &z_read);
        // fprintf(fp," %ld ",x_read);

        prev_value = data;
        data = run_filter(&accelerometer, x_read, y_read, z_read);
        gravX = gravitationalA(accelerometer.x);
        gravY = gravitationalA(accelerometer.y);
        gravZ = gravitationalA(accelerometer.z);
        gravTotal = sqrt((gravX * gravX + gravY * gravY + gravZ * gravZ));
        /* Calibrate thresholds */

        if
            !(is_first_cycle)
            {
                data = ewma(data, prev_value, {{ewma_weight}}, {{ewma_total_weight}});
                prev_gravTotal = ewma(gravTotal, prev_gravTotal, {{ewma_weight}}, {{ewma_total_weight}});
            }
        else
        {
            is_first_cycle = false;
            prev_gravTotal = gravTotal;
            continue;
        }
        /*
          Todo: change correctGrav to use epsilon between prev_gravTotal and gravTotal
        **/
        correctGrav = (gravTotal < 450) & (gravTotal > 350);

        /*

          To calibrate thresholds we need to create a method to check if the Grav
        has reached a consistent value In order to do this, we could use the EWMA
        again and see if the deviations reach a value, epsilon.

        **/

        // printf("%ld\n",gravTotal);
        if (correctGrav)
        {
            if (below)
            {
                if (data > peakMag)
                {
                    count += 1;
                    printf("%ld\n", count);
                    below = false;
                }
            }
            else
            {
                if (data < lowMag)
                {
                    below = true;
                }
            }
            if ((test_count % 100) == 0)
            {
            }
            test_count += 1;
        }
    }
    // printf("%d ",count);
}
// 56 hz optimum
