/* Fill in your Name and GNumber in the following two comment fields
 * Name: Nooreldean Koteb
 * GNumber: G01085380
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fp.h"

int round_frac(float frac);
int compute_fp(float val);
float get_fp(int val);
int mult_vals(int source1, int source2);
int add_vals(int source1, int source2);
float power(int val, int raised);

/*
*This method will implement the Pow functionarlity
*/
float power(int val, int raised){
  int i = 1;
  float result = val;

  //If raised to power of 0 return 1
  if (raised == 0){
    return 1;
  }else{
    
    //If raised is negative
    if (raised < 0){
      while(i < (raised*-1)){
        result = result * val;
        i++;
      }

      result = 1 / result; 
    }else{
      //If raised is positive
      while(i < raised){
        result = result * val;
        i++;
      }
    }

  }

  return result;
}
/*
*This Method will round any given value
*/
int round_frac(float frac) {
  //takes in float, figure out if to not round, round up, down, or nearest even (0.5)
  int rounded = (frac * 64);
  float dec = (frac * 64) - rounded;

  //If there is a decimal value
  if(dec != 0){

    //Round Up
    if(dec > 0.50){
      return rounded + 1;
    }
    
    //Round Down
    if(dec < 0.50){
      return rounded;
    }
    
    //Nearest Even
    if((dec * 2) == 1){
      if(rounded % 2 == 0){
        return rounded;
      }else{
        return rounded +1;
      }
    } 
  }
  
  return rounded;
}

/* input: float value to be represented
 * output: integer representation of the input float
 *
 * Perform this the same way we did in class -
 *   either dividing or multiplying the value by 2
 *   until it is in the correct range (between 1 and 2).
 *
 * Follow the Project Documentation for Instructions
 */
int compute_fp(float val) {
  /* Implement this function */
  //Check input
  if (val == 0.0){
    return 0;
  }
  //Variable Initialization
  int s = 0;
  int bias = 7;
  int e = 0;
  int exp = 0;
  float m = val;
  int frac = 0;
  int bit = 0;

  //Checks if it is a negative number
  if(m < 0){
    s = 1;
    m *= -1;
  }

  //Puts number in range and records exponent
  while(m >= 2){
    m = m / 2;
    e++;
  }
  while(m < 1){
    m = m * 2;
    e--;
  }

  //Calculates exp & rounds the Frac
  exp = e + bias;
  frac = round_frac(m - 1);
  
  //determine if exp is overflow or special
  //Underflow
  if(exp <= 0){
    if(s == 1){
      return 1024;
    }else{
      return 0;
    }
  }

  //Overflow
  //Infinity
  if(exp >= 15){
    if(s == 1){
      return 1984;
    }else{
      return 960;
    }
  }


  //Binary Manipulation to string all values together
  if(s == 1){
    bit = bit | 1024;
  }

  bit = bit | (exp << 6);
  bit = bit | frac;

  return bit;
}

/* input: integer representation of our floats
 * output: float value represented by our integer encoding
 *
 * Follow the Project Documentation for Instructions
 */
float get_fp(int val) {
  /* Implement this function */
  //pulling values from int
  int s = val >> 10;
  int exp = (val & 960) >> 6;
  float m = (1+((val & 63)/64.0));
  int bias = 7;
  int e = exp - bias;

  //determine if exp is overflow or special
  //Underflow
  if(exp <= 0){
    return 0;
  }

  //Overflow
  //Infinity
  if(exp >= 15){
    if(s == 1){
      return -INFINITY;
    }else{
      return INFINITY;
    }
  }

  //Calculate the float
  float result = power(-1,s) * (m) * power(2,e);

  return result;
}

/* input: Two integer representations of our floats
 * output: The multiplication result in our integer representation
 *
 * You must implement this using the algorithm described in class:
 *   Xor the signs: S = S1 ^ S2
 *   Add the exponents: E = E1 + E2
 *   Multiply the Frac Values: M = M1 * M2
 *   If M is not in a valid range for normalized, adjust M and E.
 *
 * Follow the Project Documentation for Instructions
 */
int mult_vals(int source1, int source2) {
  /* Implement this function */
  //Checks for edge case
  if(source1 == 0 && source2 == 0){
    return 0;
  }
  //Initialize variables and pull information from int
  int result = 0;
  int bias = 7;
  int s = (source1 >> 10) ^ (source2 >> 10);
  int e = (((source1 & 960) >> 6)-bias) + (((source2 & 960) >> 6)-bias);
  float m = (1+((source1 & 63)/64.0)) * (1+((source2 & 63)/64.0));
  int frac = 0;
  int exp = e + bias;

  //Readjusts M if it is out of the range
  while(m >= 2){
    m = m/2;
    exp++;
  }
  while(m < 1){
    m = m * 2;
    exp--;
  }

  //Round and find Frac
  frac = round_frac(m - 1);


  //determine if exp is overflow or special
  //Underflow
  if(exp <= 0){
    if(s == 1){
      return 1024;
    }else{
      return 0;
    }
  }

  //Overflow
  //Infinity
  if(exp >= 15){
    if(s == 1){
      return 1984;
    }else{
      return 960;
    }
  }

  //String the Int together
  if(s == 1){
    result = result | 1024;
  }

  result = result | (exp << 6);
  result = result | frac;

  return result;
}

/* input: Two integer representations of our floats
 * output: The addition result in our integer representation
 *
 * You must implement this using the algorithm described in class:
 *   If needed, adjust the numbers to get the same exponent E
 *   Add the two adjusted Mantissas: M = M1 + M2
 *   Adjust M and E so that it is in the correct range for Normalized
 *
 * Follow the Project Documentation for Instructions
 */
int add_vals(int source1, int source2) {
  /* Implement this function */
  //Checks for edge case
  if(source1 == 0 && source2 == 0){
    return 0;
  }
  //Final use variables
  int result = 0;
  float m = 0;
  int frac = 0;
  int s = 0;
  //Extraction and calculation variables
  int s1 = (source1 >> 10);
  int s2 = (source2 >> 10);
  int exp1 = ((source1 & 960) >> 6);
  int exp2 = ((source2 & 960) >> 6);
  float m1 = (1+((source1 & 63)/64.0));
  float m2 = (1+((source2 & 63)/64.0));
 
 //Adjusts to the larger exponent
  while(exp1 > exp2){
    m2 = m2 / 2;
    exp2++;
  }
  while(exp2 > exp1){
    m1 = m1 / 2;
    exp1++;
  }

  //Final Mantisa
  m = (power(-1,s1) * m1) + (power(-1, s2) * m2);
  
  //Checks if negative after addition
  if(m < 0){
    s = 1;
    m *= -1;
  }

  //Checks if after addition it is larger than 6 bits
  while (m >= 2){
    m = m / 2;
    exp1++;
  }
  while(m < 1){
    m = m * 2;
    exp1--;
  }

  frac = round_frac(m - 1);

  //determine if exp is overflow or special
  //Underflow
  if(exp1 <= 0){
    if(s == 1){
      return 1024;
    }else{
      return 0;
    }
  }

  //Overflow
  //Infinity
  if(exp1 >= 15){
    if(s == 1){
      return 1984;
    }else{
      return 960;
    }
  }

  //String Int together
  if(s == 1){
    result = result | 1024;
  }

  result = result | (exp1 << 6);
  result = result | frac;

  return result;
}
