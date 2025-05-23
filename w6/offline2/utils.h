#ifndef UTILS_H
#define UTILS_H
double generate_normal (double mu, double sigma);
void populate_array_gaussian(double *array, int size, double mu, double sigma, unsigned int seed);
void populate_array_uniform(double *array, int size, double min, double max, unsigned int seed);
#endif