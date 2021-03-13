
#include <stdio.h>
#define TRUE 1
#define FALSE 0
//Acceleration
struct ListAxis{
    long int data[{{axis_size}}];
    int cursor;
    int length;
};

struct ListConstants{
    long int data[{{axis_size}}];
};

void insert(struct ListAxis* list, long int data){
    list->cursor--;
    if(list->cursor < 0){
        list->cursor += list->length;
    }
    list->data[list->cursor] = data;
}

long int filterProduct(struct ListAxis* a, struct ListConstants* alpha){
    long int total = 0;
    int cursor;
    long int data;
    for(int i = 0; i < a->length; i++){
        cursor = (i + a->cursor) % a->length;
        data = ((a->data[cursor])*(alpha->data[i]));
        data >>= {{lpfBitScalar}};
        total += data;
    }
    return total;
}

struct Acceleration{
    struct ListAxis* a;
    struct ListConstants* alpha;
    long int firA;
};

void fir(struct Acceleration* acceleration){
    acceleration->firA = filterProduct(acceleration->a,acceleration->alpha);
}

long int userA(struct Acceleration* acceleration){
    return (acceleration->a->data[acceleration->a->cursor] - acceleration->firA);
}

long int gravitationalA(struct Acceleration* acceleration){
    return (acceleration->firA);
}

//Accelerometer
struct ListAccelerometer{
    long int data[{{acceleration_size}}];
    int cursor;
    int length;
};

struct ListConstantsAccelerometer{
    long int data[{{acceleration_size}}];
};

struct Accelerometer{
    struct Acceleration* x;
    struct Acceleration* y;
    struct Acceleration* z;
    struct ListAccelerometer* a;
    struct ListConstantsAccelerometer* alpha;
};

void insertA(struct ListAccelerometer* list, long int data){
    list->cursor--;
    if(list->cursor < 0){
        list->cursor += list->length;
    }
    list->data[list->cursor] = data;
}

void insertAccelerometer(struct Accelerometer* accelerometer, long int x, long int y, long int z){
    insert(accelerometer->x->a,x);
    insert(accelerometer->y->a,y);
    insert(accelerometer->z->a,z);
}

long int userAccelerationGravitationalDirection(struct Accelerometer* accelerometer){
    fir(accelerometer->x);
    fir(accelerometer->y);
    fir(accelerometer->z);
    long int x_sum = userA(accelerometer->x) * gravitationalA(accelerometer->x);
    long int y_sum = userA(accelerometer->y) * gravitationalA(accelerometer->y);
    long int z_sum = userA(accelerometer->z) * gravitationalA(accelerometer->z);
    return (x_sum + y_sum + z_sum);
}

long int firAccelerometer(struct Accelerometer* accelerometer){
    long int total = 0;
    long int data;
    int cursor;
    for(int i = 0; i < accelerometer->a->length; i++){
        cursor = (i + accelerometer->a->cursor) % accelerometer->a->length;
        long int data = ((accelerometer->a->data[cursor])*(accelerometer->alpha->data[i]));
        data >>= {{bpfBitScalar}};
        total += data;
    }
    return total;
}

long int filter(struct Accelerometer* accelerometer){
    insertA(accelerometer->a,userAccelerationGravitationalDirection(accelerometer));
    return firAccelerometer(accelerometer);
}

//StepDetection
long int run_filter(struct Accelerometer* accelerometer, long int x, long int y, long int z){
    insertAccelerometer(accelerometer,x,y,z);
    return filter(accelerometer);
}

long int ewma(long data, long prev_value, unsigned weight, unsigned total_weight, unsigned iter) {
    // on first iteration no prev value, so return same
    if (iter != 0) {
        return data;
    }
    /* if a = 0.2 => weight = 2, total_weight = 10, indicates window size of ~9 */

    /* weight and total weight represent the fraction between 0-1 to improve the weight of the previous value*/
    return (weight*data + (total_weight-weight)*prev_value)/total_weight;
}
int main(){
    //first initialise the three acceleration axis;
    struct ListAxis x; x.cursor = {{axis_size}}; x.length = {{axis_size}};
    struct ListAxis y; y.cursor = {{axis_size}}; y.length = {{axis_size}};
    struct ListAxis z; z.cursor = {{axis_size}}; z.length = {{axis_size}};
    struct ListConstants alphaAxis = {.data = { {% for x in lpf[:-1] %} {{x}} , {% endfor %} {{lpf[axis_size-1]}}}};
    struct Acceleration accelerationX; accelerationX.a = &x; accelerationX.alpha = &alphaAxis; accelerationX.firA = 0;
    struct Acceleration accelerationY; accelerationY.a = &y; accelerationY.alpha = &alphaAxis; accelerationY.firA = 0;
    struct Acceleration accelerationZ; accelerationZ.a = &z; accelerationZ.alpha = &alphaAxis; accelerationZ.firA = 0;
    struct ListAccelerometer a; a.cursor = 0; a.length = {{acceleration_size}};
    struct ListConstantsAccelerometer alphaA = {.data = { {% for x in bpf[:-1] %} {{x}} , {% endfor %} {{bpf[acceleration_size-1]}}}};
    struct Accelerometer accelerometer;
    accelerometer.x = &accelerationX; accelerometer.y = &accelerationY; accelerometer.z = &accelerationZ;
    accelerometer.a = &a;
    accelerometer.alpha = &alphaA;

    long int xData[{{xdatalength}}] = { {% for x in xData[:-1]%} {{x}} , {%endfor%} {{xData[xdatalength-1]}}  };
    long int yData[{{ydatalength}}] = { {% for y in yData[:-1]%} {{y}} , {%endfor%} {{yData[xdatalength-1]}}  };
    long int zData[{{zdatalength}}] = { {% for z in zData[:-1]%} {{z}} , {%endfor%} {{zData[xdatalength-1]}}  };
    long int data;

    long int prev_value;

    int count = 0;
    int below = TRUE;
    long int peakMag = 10000000;
    long int lowMag = 0;
    for(unsigned int i = 0; i < {{xdatalength}}; i++){
        prev_value = data;
        data = run_filter(&accelerometer,xData[i],yData[i],zData[i]);
        data = ewma(data, prev_value, 2, 10, i);
        if(below){
            if(data > peakMag){
                count += 1;
                below = FALSE;
            }
        }else{
            if(data < lowMag){
                below = TRUE;
            }
        }
        printf("%ld ",data);
    }
    //printf("%d ",count);
}
