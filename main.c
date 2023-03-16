#include "header.h" // must link with -lm

// Buffer 1, shared resource between input thread separator thread
struct dataBuffer buffer_1;
int count_1 = 0;
int prod_idx_1 = 0;
int con_idx_1 = 0;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;

// Buffer 2, shared resource between separator thread and the plus thread
double buffer_2[MAX_LINE_LENGTH];
int count_2 = 0;
int prod_idx_2 = 0;
int con_idx_2 = 0;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;

// Buffer 3, shared resource between plus thread and output thread
double buffer_3[MAX_LINE_LENGTH];
int count_3 = 0;
int prod_idx_3 = 0;
int con_idx_3 = 0;
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;


void put_buff_1(char *data, int size){
    pthread_mutex_lock(&mutex_1);
    strcpy(buffer_1.data, data);
    buffer_1.size = size;
    pthread_cond_signal(&full_1);
    pthread_mutex_unlock(&mutex_1);
}

// void put_buff_2(char *data){
//   // Lock the mutex before putting the item in the buffer
//   pthread_mutex_lock(&mutex_2);
//   // Put the item in the buffer
//   buffer_2[prod_idx_2] = item;
//   // Increment the index where the next item will be put.
//   prod_idx_2 = prod_idx_2 + 1;
//   count_2++;
//   // Signal to the consumer that the buffer is no longer empty
//   pthread_cond_signal(&full_2);
//   // Unlock the mutex
//   pthread_mutex_unlock(&mutex_2);
// }

// void put_buff_3(char *data){
//   // Lock the mutex before putting the item in the buffer
//   pthread_mutex_lock(&mutex_3);
//   // Put the item in the buffer
//   buffer_3[prod_idx_3] = item;
//   // Increment the index where the next item will be put.
//   prod_idx_3 = prod_idx_3 + 1;
//   count_3++;
//   // Signal to the consumer that the buffer is no longer empty
//   pthread_cond_signal(&full_3);
//   // Unlock the mutex
//   pthread_mutex_unlock(&mutex_3);
// }

// void get_buff_1(char *buffer){
//     // Lock the mutex before checking if the buffer has data
//     pthread_mutex_lock(&mutex_1);
//     while (count_1 == 0)
//         // Buffer is empty. Wait for the producer to signal that the buffer has data
//         pthread_cond_wait(&full_1, &mutex_1);
//     int item = buffer_1[con_idx_1];
//     // Increment the index from which the item will be picked up
//     con_idx_1 = con_idx_1 + 1;
//     count_1--;
//     // Unlock the mutex
//     pthread_mutex_unlock(&mutex_1);
//     // Return the item
//     return item;
// }

// void get_buff_2(char *buffer){
//     // Lock the mutex before checking if the buffer has data
//     pthread_mutex_lock(&mutex_2);
//     while (count_2 == 0)
//         // Buffer is empty. Wait for the producer to signal that the buffer has data
//         pthread_cond_wait(&full_2, &mutex_2);
//     double item = buffer_2[con_idx_2];
//     // Increment the index from which the item will be picked up
//     con_idx_2 = con_idx_2 + 1;
//     count_2--;
//     // Unlock the mutex
//     pthread_mutex_unlock(&mutex_2);
//     // Return the item
//     return item;
// }

// void get_buff_3(char *buffer){
//     // Lock the mutex before checking if the buffer has data
//     pthread_mutex_lock(&mutex_3);
//     while (count_3 == 0)
//         // Buffer is empty. Wait for the producer to signal that the buffer has data
//         pthread_cond_wait(&full_3, &mutex_3);
//     double item = buffer_2[con_idx_3];
//     // Increment the index from which the item will be picked up
//     con_idx_3 = con_idx_3 + 1;
//     count_3--;
//     // Unlock the mutex
//     pthread_mutex_unlock(&mutex_3);
//     // Return the item
//     return item;
// }

void get_user_input(struct dataBuffer *buff){
        for(int i = 0; i < 1000; i++){
            buff->data[i] = fgetc(stdin);
            if(buff->data[i] == EOF || (buff->data[i] == '\n' && buff->data[i-1] == 'P' && buff->data[i-2] == 'O' && buff->data[i-3] == 'T' && buff->data[i-4] == 'S')){
                for(int j = i-4; j < i; j++){
                    buff->data[j] = '\0';
                }
                buff->size = i - 4;
                break;
            }
    }
}


void *input_thread(void *arg) {
    struct dataBuffer buffer;
    buffer.size = 0;
    get_user_input(&buffer);
    put_buff_1(buffer.data, buffer.size);

    return NULL;
}

// void separate_input(){}

// void *separator_thread(void *arg) {
//     while (!done) {
//         pthread_mutex_lock(&mutex);
//         while (input_index == 0 && !done)
//             pthread_cond_wait(&cond_input, &mutex);
//         if (!done) {
//             for (int i = 0; i < MAX_LINE_LENGTH && input_buffer[i] != '\n' && input_buffer[i] != '\0'; i++) {
//                 if (input_buffer[i] == '\r')
//                     separator_buffer[i] = ' ';
//                 else
//                     separator_buffer[i] = input_buffer[i];
//             }
//             separator_index = 0;
//             pthread_cond_signal(&cond_separator);
//             pthread_mutex_unlock(&mutex);
//         } else {
//             pthread_mutex_unlock(&mutex);
//             break;
//         }
//     }
//     return NULL;
// }

// void plus_check_input(){}

// void *plus_thread(void *arg) {
//     while (!done) {
//         pthread_mutex_lock(&mutex);
//         while (separator_index == 0 && !done)
//             pthread_cond_wait(&cond_separator, &mutex);
//         if (!done) {
//             for (int i = 0; i < MAX_LINE_LENGTH && separator_buffer[i] != '\0'; i++) {
//                 if (separator_buffer[i] == '+' && separator_buffer[i + 1] == '+') {
//                     plus_buffer[i] = '^';
//                     i++;
//                 } else {
//                     plus_buffer[i] = separator_buffer[i];
//                 }
//             }
//             plus_index = 0;
//             pthread_cond_signal(&cond_plus);
//             pthread_mutex_unlock(&mutex);
//         } else {
//             pthread_mutex_unlock(&mutex);
//             break;
//         }
//     }
//     return NULL;
// }

// void write_output(){}

// void *output_thread(void *arg) {
//     while (!done) {
//         pthread_mutex_lock(&mutex);
//         while (plus_index == 0 && !done)
//             pthread_cond_wait(&cond_plus, &mutex);
//         if (!done) {
//             for (int i = 0; i < MAX_LINE_LENGTH && plus_buffer[i] != '\0'; i++) {
//                 output_buffer[output_index] = plus_buffer[i];
//                 output_index++;
//                 if (output_index == 80) {
//                     output_buffer[output_index] = '\n';
//                     output_index = 0;
//                 }
//             }
//             pthread_cond_signal(&cond_output);
//             pthread_mutex_unlock(&mutex);
//         } else {
//             pthread_mutex_unlock(&mutex);
//             break;
//         }
//     }
//     return NULL;
// }


int main(int argc, char *argv[]){
    // pthread_t threads[4];
    // pthread_create(&threads[0], NULL, input_thread, NULL);
    // pthread_create(&threads[1], NULL, separator_thread, NULL);
    // pthread_create(&threads[2], NULL, plus_thread, NULL);
    // pthread_create(&threads[3], NULL, output_thread, NULL);
    
    // for (int i = 0; i < 4; i++) {
    //     pthread_join(threads[i], NULL);
    // }


    return 0;
}
