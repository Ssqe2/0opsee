#include "p7.h"  // Include the header file "p7.h"
#define TEST_SIZE 10000  // Define a constant for the test size

double duration;  // Declare a variable to store the duration
clock_t start, stop;  // Declare variables to store the start and stop times
int arr_test[100000000];  // Declare an array to store test data

// Function to test insertion operation
double testInsertion(MySkipList *list, int size,int MAX_LEVEL) {
    double result=0;
    for (int i = 0; i < size; i++) {
        int item = rand() % (3*size);  // Generate a random item
        arr_test[i] = item;  // Store the item in the array
  //      printf("insert %d\n",item);
        insertNewElement(list, item,MAX_LEVEL);  // Insert the item into the skip list
  //      printNewList(list);
    }
    for(int i=0;i<size;i++){
        int item= rand() % size;
        deleteNewElement(list,arr_test[item],MAX_LEVEL);
        start=clock();
        insertNewElement(list,arr_test[item],MAX_LEVEL);
        stop=clock();
        result+=(double)(stop-start)/CLOCKS_PER_SEC;
    }
    return result;
}

// Function to test deletion operation
double testDeletion(MySkipList *list, int size,int MAX_LEVEL) {
    double result=0;
    for(int i=0;i<size;i++){
        int item= rand() % size;
        start=clock();
        deleteNewElement(list,arr_test[item],MAX_LEVEL);
        stop=clock();
        result+=(double)(stop-start)/CLOCKS_PER_SEC;
        insertNewElement(list,arr_test[item],MAX_LEVEL);
    }
    return result;
}

void testInsertion_correct(MySkipList *list, int size,int MAX_LEVEL) {
    for (int i = 0; i < size; i++) {
        int item = rand() % (3*size);  // Generate a random item
        arr_test[i] = item;  // Store the item in the array
        printf("insert %d\n",item);
        insertNewElement(list, item,MAX_LEVEL);  // Insert the item into the skip list
        printNewList(list);
    }
    MySkipNode *p;
    p=list->header;
    while(p->forward[0]!=NULL){
        if(p->forward[0]->value>=p->value){
            printf("wrong!\n");
            return;
        }
        p=p->forward[0];
    }
}

// Function to test search operation
void testSearch(MySkipList *list, int size) {
    for (int i = 0; i < size; i++) {
        searchNewElement(list, arr_test[i]);  // Search for the item in the skip list
    }
    return;
}
void testSearch_2(MySkipList *list, int size) {
    MySkipNode* p;
    for (int i = 0; i < size; i++) {
        p=searchNewElement(list, arr_test[i]);  // Search for the item in the skip list
        if(p==NULL){
            printf("wrong!\n");
            return;
        }
    }
}
void testDeletion_2(MySkipList *list, int size,int MAX_LEVEL) {
    for(int i=0;i<size;i++){
        printf("delete %d \n",arr_test[i]);
        deleteNewElement(list,arr_test[i],MAX_LEVEL);
        printNewList(list);
    }
    return;
}
int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));  // Seed the random number generator
    int which_test=0;
    int loop_time=1;
    int testSize = TEST_SIZE;  // Initialize test size to TEST_SIZE
    int MAX_LEVEL = (int)log2((double)testSize);
    double duration_insert=0;
    double duration_search=0;
    double duration_delete=0;
    if (argc > 1) {
        testSize = atoi(argv[1]);  // If there are command line arguments, set test size to the first argument
    }
    if(argc > 2){
        loop_time=atoi(argv[2]);
    }
    if(argc>3){
        which_test=atoi(argv[3]);
    }
    if(which_test==1){
        MySkipList *list = createNewList(MAX_LEVEL);  // Create a new skip list
        testInsertion_correct(list,testSize,MAX_LEVEL);
        testSearch_2(list,  testSize);
        testDeletion_2(list,testSize,MAX_LEVEL);
        printf("success\n");
    }
    while(loop_time&&which_test==0){
    MySkipList *list = createNewList(MAX_LEVEL);  // Create a new skip list

    printf("Testing insertion...\n");  // Print message indicating start of insertion test
  //  start = clock();  // Record start time
    duration=testInsertion(list, testSize,MAX_LEVEL);  // Run insertion test
 //   stop = clock();  // Record stop time
 //   duration = ((double)(stop - start)) / CLOCKS_PER_SEC;  // Calculate duration
    printf("total time:%lf\n", duration);  // Print duration of insertion test
    printf("avg time based %d:%.20f\n", testSize,duration/testSize);  // Print duration of insertion test
    duration_insert+=duration;
//    printNewList(list);

    printf("Testing search...\n");  // Print message indicating start of search test
    start = clock();  // Record start time
    testSearch(list, testSize);  // Run search test
    stop = clock();  // Record stop time
    duration = ((double)(stop - start)) / CLOCKS_PER_SEC;  // Calculate duration
    printf("total time:%lf\n", duration);  // Print duration of search test
    printf("avg time based %d:%.20f\n", testSize,duration/testSize);  // Print duration of insertion test
    duration_search+=duration;

    printf("Testing deletion...\n");  // Print message indicating start of deletion test
//    start = clock();  // Record start time
    duration=testDeletion(list, testSize,MAX_LEVEL);  // Run deletion test
//    stop = clock();  // Record stop time
//    duration = ((double)(stop - start)) / CLOCKS_PER_SEC;  // Calculate duration
    printf("total time:%lf\n", duration);  // Print duration of deletion test
    printf("avg time based %d:%.20f\n", testSize,duration/testSize);  // Print duration of insertion test
    duration_delete+=duration;
  //  printNewList(list);

    freeNewList(list);  // Free the skip list
    loop_time--;
    }
    if(argc>2&&which_test==0){
        printf("-------------------------------------------\n");
        printf("this test includes %d times of insertion,search,deletion\nand repeated %d times\n",atoi(argv[1]),atoi(argv[2]));
        printf("total time of insertion is %lf,avg time of one insertion based %d is %.20f\n",duration_insert,testSize,duration_insert/(testSize*atoi(argv[2])));
        printf("total time of searching is %lf,avg time of one searching based %d is %.20f\n",duration_search,testSize,duration_search/(testSize*atoi(argv[2])));
        printf("total time of delete is %lf,avg time of one delete based %d is %.20f\n",duration_delete,testSize,duration_delete/(testSize*atoi(argv[2])));
        printf("-------------------------------------------\n");
    }
    return 0;  // Return 0 to indicate successful execution
}
