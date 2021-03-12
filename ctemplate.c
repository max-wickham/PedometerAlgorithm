
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
    long int total;
    for(int i = 0; i < a->length; i++){
        int cursor = (i + a->cursor) % a->cursor;
        total += ((a->data[cursor])*(alpha->data[i]));
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
    for(int i = 0; i < accelerometer->a->length; i++){
        int cursor = (i + accelerometer->a->cursor) % accelerometer->a->cursor;
        total += ((accelerometer->a->data[cursor])*(accelerometer->alpha->data[i]));
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

int main(){
    //first initialise the three acceleration axis;
    struct ListAxis x; x.cursor = 0; x.length = {{axis_size}};
    struct ListAxis y; y.cursor = 0; y.length = {{axis_size}};
    struct ListAxis z; z.cursor = 0; z.length = {{axis_size}};
    struct ListConstants alphaAxis = {.data = { {% for x in lpf[:-1] %} {{x}} , {% endfor %} {{lpf[axis_size]}}}};
    struct Acceleration accelerationX; accelerationX.a = &x; accelerationX.alpha = &alphaAxis; accelerationX.firA = 0;
    struct Acceleration accelerationY; accelerationY.a = &y; accelerationY.alpha = &alphaAxis; accelerationY.firA = 0;
    struct Acceleration accelerationZ; accelerationZ.a = &z; accelerationZ.alpha = &alphaAxis; accelerationZ.firA = 0;
    struct ListAccelerometer a; a.cursor = 0; a.length = {{acceleration_size}};
    struct ListConstantsAccelerometer alphaA = {.data { {% for x in bpf[:-1] %} {{x}} , {% endfor %} {{bpf[acceleration_size]}}}};
    struct Accelerometer accelerometer;
    accelerometer.x = &accelerationX; accelerometer.y = &accelerationY; accelerometer.z = &accelerationZ;
    accelerometer.a = &a;
    accelerometer.alpha = &alphaA;

    long int xData;
    long int yData;
    long int zData;
    long int data;
    //while(1){
        //update the xData yData and zData values from the accelerometer
        data = run_filter(&accelerometer,xData,yData,zData);
    //}
}