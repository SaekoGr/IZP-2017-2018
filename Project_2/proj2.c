/*
 * Sabína Gregušová, xgregu02, xgregu02@stud.fit.vutbr.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define DEFAULTHEIGHT 1.5       // default height for measurement
#define EPSILON 0.0000000001    // accuracy for 10 decimal places

void invalidInput();
void printHelp();                                                   // prints instructions for input
void calculateTan(double a, unsigned int n, unsigned int m);        // enables calculation of tangens
void invalidTanInput();                                             // iteration error
void calculateHeight(double beta, double distance, double c);       // calculates height
void checkAngleValue(double angle);
void checkDoubleNumber(char *number);                               // checks if it is a number
double calculateDistance(double alpha, double c);                   // calculates distance
double taylor_tan(double x, unsigned int n);                         // calculates Taylor series
double cfrac_tan(double x, unsigned int n);                          // calculates cfrac Tangens
double absoluteValue(double difference);
unsigned int iterationEstimator(double x);                          // estimates how many iterations are required to be precise on 10 decimal places

int main(int argc, char* argv[]) {
    int n, m;                       // range variables
    double alpha;                   // angles in radians
    double beta;
    char callHelp[] = "--help";     // strings for argv comparison
    char callTan[] = "--tan";
    char height[] = "-c";
    char measure[] = "-m";
    char *isnumber = {'\0'};

    if(argc == 1){                  // checks for no arguments
        fprintf(stderr, "No arguments entered\n");
        fprintf(stderr, "If you need help, please enter: --help\n");
        exit(EXIT_FAILURE);
    }

    else if(strcmp(argv[1], callHelp) == 0)     // checks whether entered argument was "--help"
        printHelp();                            // prints instructions and exits

    else if(strcmp(argv[1], callTan) == 0){     // checks if tan entered
        if(!argv[2] || !argv[3] || !argv[4])    // checks whether all necessary arguments are present
            invalidInput();

        alpha = strtod(argv[2], &isnumber);     // converts arguments into double and ints
        checkDoubleNumber(isnumber);
        checkAngleValue(alpha);
        n = atoi(argv[3]);
        m = atoi(argv[4]);
        if(0 < n && n <= m && m < 14 ) {        // checks if iteration interval is valid
            calculateTan(alpha, n, m);          // sends arguments into calculate tan function
        }
        else                                    // if it is outside the range, exit program
            invalidTanInput();
    }

    else if(argv[1] && strcmp(argv[1], measure) == 0 && argc <= 4){     // checks whether -m is entered
        if(!argv[2])                                                    // second argument must be present
            invalidInput();

        alpha = strtod(argv[2], &isnumber);             // converts to alpha
        checkDoubleNumber(isnumber);
        checkAngleValue(alpha);                         // checks the acceptable range
        double c = DEFAULTHEIGHT;                       // default height is entered
        double distance = calculateDistance(alpha, c);  // calculates distance

        if(argv[3]) {                                   // if third argument is present, calculate height
            beta = strtod(argv[3], &isnumber);
            checkDoubleNumber(isnumber);
            checkAngleValue(beta);                      // checks the acceptable range
            calculateHeight(beta, distance, c);
        }
    }

    else if(argv[1] && strcmp(argv[1], height) == 0){   // checks if first argument sets height
        if(argv [3] && strcmp(argv[3], measure) == 0){  // checks if third argument is -m
            if(!argv[2] || !argv[4])                    // second and fourth argument must be present
                invalidInput();

            alpha = strtod(argv[4], &isnumber);         // converts argument to double
            checkDoubleNumber(isnumber);
            checkAngleValue(alpha);                     // checks the range
            double c = strtod(argv[2], &isnumber);
            checkDoubleNumber(isnumber);

            if(c <= 0 || c > 100){                      // checks whether set height is in valid range
                fprintf(stderr, "Entered height is outside of the acceptable range.\n");
                exit(EXIT_FAILURE);
            }

            double distance = calculateDistance(alpha, c);

            if(argv[5]) {                               // if fifth arguments is present, calculate height
                beta = strtod(argv[5], &isnumber);
                checkDoubleNumber(isnumber);
                checkAngleValue(beta);
                calculateHeight(beta, distance, c);
            }
        }
        else {  // error, if only height is set without any arguments
            printf("You set only height without arguments\nPlease try again\n");
            exit(EXIT_FAILURE);
        }
    }

    else        // if the input doesn't match anything, exit the program
        invalidInput();

    return 0;
}

void printHelp(){                               // prints out instructions for help
    printf("Instructions for input:\n");
    printf("--tan A N M\n");
    printf("A\tAngle alpha in radians\n");
    printf("N, M\tIteration variables (N <= M)\n\n");

    printf("[-c X] -m A [B]\n");
    printf("Arguments in square brackets are optional\n");
    printf("-c\tSets height of X; 0 < X <= 100\n");
    printf("-m\tCalculates and measures distance\n");
    printf("A\tAngle alpha in radians\n");
    printf("B\tAngle beta in radians\n");

    exit(EXIT_SUCCESS);
}

void calculateTan(double a, unsigned int n, unsigned int m){
    double tanResult = tan(a);      // calculates tan from math library

    while(n <= m) {                 // iterations
        double cfracResult = cfrac_tan(a, n);    // calculates tan by fractions
        double taylorResult = taylor_tan(a, n);  // calculates tan by Taylor polynomials
        double cfracError = absoluteValue(cfracResult - tanResult); // calculates absolute value
        double taylorError = absoluteValue(tanResult - taylorResult);
        printf("%d %e %e %e %e %e\n", n, tanResult, taylorResult, taylorError, cfracResult, cfracError);
        n++;
    }
}

void invalidTanInput(){         // error if iteration variables do not fulfill the condition n<=m
    fprintf(stderr, "Invalid input.\nPlease make sure the entered iteration values are from interval (0, 14)\n");
    exit(EXIT_FAILURE);
}

double absoluteValue(double difference){    // returns absolute value of variable
    if(difference < 0)
        return -(difference);
    return difference;
}

double cfrac_tan(double x, unsigned int n){
    double cf = 1;
    double a;
    double b = x*x;

    for(int k = n; k > 0; k--){
        a = 2*k - 1;
        cf = a - (b / cf);
    }
    cf = x / cf;
    return cf;
}

double taylor_tan(double x, unsigned int n){
    double sum = 0;
    double a = x;
    double num[13] = {1, 1, 2, 17, 62,
                      1382, 21844, 929569,
                      6404582, 443861162,
                      18888466084, 113927491862,
                      58870668456604};
    double denum[13] = {1, 3, 15, 315, 2835,
                        155925, 6081075, 638512875,
                        10854718875, 1856156927625,
                        194896477400625, 49308808782358125,
                        3698160658676859375};

    for(unsigned int i = 0; i < n; i++){
        sum += ((num[i]*a)/denum[i]);
        a = a * (x*x);
    }
    return sum;
}

double calculateDistance(double alpha, double c){
    double distance;
    unsigned int iterations = iterationEstimator(alpha);
    distance = c/(cfrac_tan(alpha, iterations));     // distance = height from the ground / alpha in radians
    printf("%.10e\n", distance);
    return distance;
}

void calculateHeight(double beta, double distance, double c){
    unsigned int iterations = iterationEstimator(beta);
    double opposite = distance * cfrac_tan(beta, iterations);    // opposite(height from c to top) = distance * beta in radians
    double height = opposite + c;
    printf("%.10e\n", height);
}

void invalidInput(){        // arguments are in wrong order or are missing
    fprintf(stderr, "Invalid input.\nPlease check the entered arguments and their order!\n");
    exit(EXIT_FAILURE);
}

void checkAngleValue(double angle){     // checks if argument is in valid range
    if(angle <= 0 || angle > 1.4){
       fprintf(stderr, "Entered angle is outside the acceptable range.\n");
       exit(EXIT_FAILURE);
    }
}

void checkDoubleNumber(char *number){     // checks whether entered value is numerical or not
    if (*number != '\0'){
        fprintf(stderr, "Entered value of argument is not numerical\n");
        exit(EXIT_FAILURE);
    }
}

unsigned int iterationEstimator(double x){  // estimates, how many iterations are required for accuracy of 10 decimal places
    unsigned int i = 1;
    double tanResult = tan(x);
    double temp = 1;

    while(temp >= EPSILON){
        temp = absoluteValue(tanResult - cfrac_tan(x, i));  // absolute difference between real tangens and cfrac result for i terations
        i++;                                                // counts how many iterations are needed for required accuracy
    }
    return i;
}

