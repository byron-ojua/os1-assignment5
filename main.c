#include "header.h" // must link with -lm

// Buffer 1, shared resource between input thread separator thread
struct dataBuffer buffer_1;
int count_1 = 0;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;

// Buffer 2, shared resource between separator thread and the plus thread
struct dataBuffer buffer_2;
int count_2 = 0;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;

// Buffer 3, shared resource between plus thread and output thread
struct dataBuffer buffer_3;
int count_3 = 0;
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;


// TODO Fix race condition

void put_buff_1(struct dataBuffer *buff){
    pthread_mutex_lock(&mutex_1);
    strcpy(buffer_1.data, buff->data);
    buffer_1.size = buff->size;
    pthread_cond_signal(&full_1);
    pthread_mutex_unlock(&mutex_1);
}

void put_buff_2(struct dataBuffer *buff){
    pthread_mutex_lock(&mutex_2);
    strcpy(buffer_2.data, buff->data);
    buffer_2.size = buff->size;
    pthread_cond_signal(&full_2);
    pthread_mutex_unlock(&mutex_2);
}

void put_buff_3(struct dataBuffer *buff){
    pthread_mutex_lock(&mutex_3);
    strcpy(buffer_3.data, buff->data);
    buffer_3.size = buff->size;
    pthread_cond_signal(&full_3);
    pthread_mutex_unlock(&mutex_3);
}

void get_buff_1(struct dataBuffer *buff){
    pthread_mutex_lock(&mutex_1);
    while (buffer_1.size == 0){
        pthread_cond_wait(&full_1, &mutex_1);
    }
    strcpy(buff->data, buffer_1.data);
    buff->size = buffer_1.size;
    pthread_mutex_unlock(&mutex_1);
}

void get_buff_2(struct dataBuffer *buff){
    pthread_mutex_lock(&mutex_2);

    while (buffer_2.size == 0){
        pthread_cond_wait(&full_2, &mutex_2);
    }

    strcpy(buff->data, buffer_2.data);
    buff->size = buffer_2.size;
    pthread_mutex_unlock(&mutex_2);
}

void get_buff_3(struct dataBuffer *buff){
    pthread_mutex_lock(&mutex_3);

    while (buffer_1.size == 0){
        pthread_cond_wait(&full_3, &mutex_3);
    }

    strcpy(buff->data, buffer_3.data);
    buff->size = buffer_3.size;
    pthread_mutex_unlock(&mutex_3);
}

void get_user_input(struct dataBuffer *buff){
        for(int i = 0; i < 1000; i++){
            buff->data[i] = fgetc(stdin);
            if(buff->data[i] == EOF || (buff->data[i] == LINE_SEPARATOR && buff->data[i-1] == 'P' && buff->data[i-2] == 'O' && buff->data[i-3] == 'T' && buff->data[i-4] == 'S' && buff->data[i-5] == LINE_SEPARATOR)){
                for(int j = i-5; j < i; j++){
                    buff->data[j] = '\0';
                }
                buff->size = i - 5;
                break;
            }
    }
}

void *input_thread(void *arg) {
    struct dataBuffer buffer;
    buffer.size = 0;
    get_user_input(&buffer);
    put_buff_1(&buffer);

    return NULL;
}

void *separator_thread(void *arg) {
    struct dataBuffer buffer;
    buffer.size = 0;
    get_buff_1(&buffer);

    for(int i = 0; i < buffer.size; i++){
        if(buffer.data[i] == '\n'){
            buffer.data[i] = ' ';
        }
    }

    put_buff_2(&buffer);
    return NULL;
}

void *plus_thread(void *arg) {
    struct dataBuffer buffer;
    buffer.size = 0;
    get_buff_2(&buffer);

    for(int i = 0; i < buffer.size; i++){
        if(buffer.data[i] == '+' && buffer.data[i+1] == '+'){
            buffer.data[i] = '^';
            buffer.data[i+1] = '\0';
            for(int j = i+1; j < buffer.size; j++){
                buffer.data[j] = buffer.data[j+1];
            }
        }
    }

    put_buff_3(&buffer);
    return NULL;
}

void *output_thread(void *arg) {
    struct dataBuffer buffer;
    buffer.size = 0;
    char *temp = calloc(80, sizeof(char));
    get_buff_3(&buffer);

    for(int i = 0; i < buffer.size; i++){
        if(i % 80 == 0 && i != 0){
            printf("%s", temp);
            printf("\n");
            fflush(stdout);
        }
        temp[i % 80] = buffer.data[i];
    }

    free(temp);
}

int main(int argc, char *argv[]){
    srand(time(0));
    pthread_t threads[4];
    pthread_create(&threads[0], NULL, input_thread, NULL);
    pthread_create(&threads[1], NULL, separator_thread, NULL);
    pthread_create(&threads[2], NULL, plus_thread, NULL);
    pthread_create(&threads[3], NULL, output_thread, NULL);
    
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
