#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//prototype for the assembly function
extern void imgCvtGrayDoubleToInt(double *input, unsigned char *output, int height, int width);

// input pixel values manually
void inputPixelValues(double *image, int height, int width) {
    int i;
    printf("Enter double-precision pixel values (0.0 to 1.0):\n");
    for (i = 0; i < height * width; i++) {
        printf("Pixel [%d]: ", i + 1);
        if (scanf("%lf", &image[i]) != 1 || image[i] < 0.0 || image[i] > 1.0) {
            printf("Invalid value. Please enter a value between 0.0 and 1.0.\n");
            i--; // Retry current index
        }
    }
}

// generate random pixel values
void generateRandomPixelValues(double *image, int height, int width) {
    int i;
    srand(time(NULL)); // Seed the random number generator
    for (i = 0; i < height * width; i++) {
        image[i] = (double)rand() / RAND_MAX; // Generate random value between 0.0 and 1.0
    }
}

// print a double-precision grayscale image
void printDoubleImage(double *image, int height, int width) {
    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            printf("%.2f ", image[i * width + j]);
        }
        printf("\n");
    }
}

// print an 8-bit integer grayscale image
void printIntImage(unsigned char *image, int height, int width) {
    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            printf("%3d ", image[i * width + j]);
        }
        printf("\n");
    }
}

// compute the average of the execution times
double computeAverage(double A[], int ctr) {
    double sum = 0.0;
    int i;
    for (i = 0; i < ctr; i++) {
        sum += A[i];
    }
    return sum / ctr;
}

// measure the execution time of the conversion
double measureExecutionTime(int height, int width, double *inputImage, unsigned char *outputImage) {
    struct timespec start, end;
    double totalTime = 0.0;
    int runs = 30;
    int i;

    for (i = 0; i < runs; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        imgCvtGrayDoubleToInt(inputImage, outputImage, height, width);
        clock_gettime(CLOCK_MONOTONIC, &end);

        double elapsedTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        totalTime += elapsedTime;
    }

    return totalTime / runs;
}

int main() {
    int height, width, choice, looping, ctr;
    double timeAve = 0;
    double finalAverage[30]; // Array to store the execution times

    printf("Enter the height and width of the image: ");
    if (scanf("%d %d", &height, &width) != 2 || height <= 0 || width <= 0) {
        printf("Invalid input. Please enter positive integers for height and width.\n");
        return 1;
    }

    // Allocate memory for input and output images
    double *inputImage = (double *)malloc(height * width * sizeof(double));
    unsigned char *outputImage = (unsigned char *)malloc(height * width * sizeof(unsigned char));

    if (!inputImage || !outputImage) {
        printf("Memory allocation failed.\n");
        free(inputImage);
        free(outputImage);
        return 1;
    }

    // Ask how many times to run
    printf("How many times would you want to run the program\n");
    printf("1. Run one time\n");
    printf("2. Run thirty times\n");
    printf("Enter your choice:  \n");
    scanf("%d", &looping);

    // Choose input method
    printf("Choose input method:\n");
    printf("1. Manually input pixel values\n");
    printf("2. Generate random pixel values\n");
    printf("Enter your choice (1 or 2): ");
    if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("Invalid choice. Please enter 1 or 2.\n");
        free(inputImage);
        free(outputImage);
        return 1;
    }

    // Input pixel values based on the user's choice
    if (looping == 1) {
        if (choice == 1) {
            inputPixelValues(inputImage, height, width);
        } else {
            generateRandomPixelValues(inputImage, height, width);
        }
        printf("\nInput Image (Double-Precision Grayscale): \n");
        printDoubleImage(inputImage, height, width);
        
        imgCvtGrayDoubleToInt(inputImage, outputImage, height, width);
        
        printf("\nOutput Image (8-bit Integer Grayscale): \n");
        printIntImage(outputImage, height, width);
        
        double averageTime = measureExecutionTime(height, width, inputImage, outputImage);
        printf("\nAverage execution time for %dx%d image: %.9f seconds\n", height, width, averageTime);

    } else if (looping == 2) {
        for (ctr = 0; ctr < 30; ctr++) {
            if (choice == 1) {
                inputPixelValues(inputImage, height, width);
            } else {
                generateRandomPixelValues(inputImage, height, width);
            }
            printf("\nInput Image (Double-Precision Grayscale):\n");
            printDoubleImage(inputImage, height, width);

            imgCvtGrayDoubleToInt(inputImage, outputImage, height, width);

            printf("\nOutput Image (8-bit Integer Grayscale):\n");
            printIntImage(outputImage, height, width);

            double averageTime = measureExecutionTime(height, width, inputImage, outputImage); 
            printf("\nAverage execution time for %dx%d image: %.9f seconds\n", height, width, averageTime); 
            finalAverage[ctr] = averageTime;
        }

        double totalAverageTime = computeAverage(finalAverage, 30);
        printf("\nTotal Average Execution Time after 30 runs: %.9f seconds \n", totalAverageTime);
    }

    // Clean up
    free(inputImage);
    free(outputImage);

    return 0;
}
