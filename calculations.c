
struct ListEntry{
    struct ListEntry* nextEntry;
    struct ListEntry* previousEntry;
    long int value;
};

struct ListBegin{
    struct ListEntry* listEnd;
    struct ListEntry* firstEntry;
};

void insert(struct ListBegin* list, long int data){
    //add a new entry
    struct ListEntry* newEntry;
    newEntry->value = data;
    newEntry->nextEntry = list->firstEntry;
    newEntry->nextEntry->previousEntry = newEntry;
    list->firstEntry = newEntry;
    //remove the last entry
    struct ListEntry* end = list->listEnd->previousEntry;
    free(list->listEnd);
    list->listEnd = end;
}

long int dotProduct(struct ListBegin* x, struct ListBegin* y){
    long int result = 0;
    struct ListEntry* xEntry = x->firstEntry;
    struct ListEntry* yEntry = y->firstEntry;
    result += xEntry->value + yEntry->value;
    while(xEntry->nextEntry != 0 || yEntry->nextEntry != 0){
        result += xEntry->value + yEntry->value;
        xEntry = xEntry->nextEntry;
        yEntry = yEntry->nextEntry;
    }
    return result;
}

struct Acceleration{
    struct ListBegin* a;
    struct ListBegin* alpha;
    long int firA;
};

void fir(struct Acceleration* acceleration){
    acceleration->firA = dotProduct(acceleration->a,acceleration->alpha);
}

long int userA(struct Acceleration* acceleration){
    return (acceleration->a->firstEntry->value - acceleration->firA);
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
    long int x_sum = 
}

// def userAccelerationGravitationalDirection(self):
//         self.x.fir()
//         self.y.fir()
//         self.z.fir()
//         x_sum = self.x.userA() * self.x.gravitaionalA()
//         y_sum = self.y.userA() * self.y.gravitaionalA()
//         z_sum = self.z.userA() * self.z.gravitaionalA()
//         return x_sum + y_sum + z_sum

int main(){
    //step one filter the previous data to get the current direction of gravity

}