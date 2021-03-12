
#define NULL 0

struct ListAxis{
    long int data[1000];
    int cursor;
    int length;
};

struct ListConstants{
    long int data[1000];
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

struct Accelerometer{
    struct Acceleration* x;
    struct Acceleration* y;
    struct Acceleration* z;
    struct ListBegin* alpha;
};

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

long int firAccelerometer(accelerometer, )

    // def fir(self,a,alpha):
    //     total = 0
    //     for x in [a[i]*alpha[i] for i in range(_length)]:
    //         total += x
    //     return total


int main(){
    //step one filter the previous data to get the current direction of gravity

}