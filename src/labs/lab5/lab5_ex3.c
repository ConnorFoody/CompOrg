#include <stdio.h>

//#define CIRCLE_MACRO 
//#define CIRCLE_FUNCTION 

#define PI 3.14159
#define CIRCLE_AREA(r) (PI * (r) *( r))
double circleArea(double);

int main(void)
{
    double rad = 2;  
   
#ifdef CIRCLE_FUNCTION
    printf("Value of circleArea: %lf \n",circleArea(rad+2));
#endif

#ifdef CIRCLE_MACRO 
    printf("Value of CIRCLE_AREA: %lf \n",CIRCLE_AREA(rad+2));
#endif

    return 0;

}

double circleArea( double radius )
{
   return PI * radius * radius;
}
