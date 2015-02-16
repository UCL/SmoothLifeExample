#include <cmath>

#include "smooth.h"


Smooth::Smooth(int size,
        distance inner,
        filling birth_inner,
        filling birth_outer,
        filling death_inner,
        filling death_outer,
        filling smoothing_inner,
        filling smoothing_outer)
    : size(100),
    inner(inner),
    birth_inner(birth_inner),
    birth_outer(birth_outer),
    death_inner(death_inner),
    death_outer(death_outer),
    smoothing_inner(smoothing_inner),
    smoothing_outer(smoothing_outer),
    outer(inner*3),
    smoothing(1.0),
    field(size,std::vector<density>(size))
{
}

int Smooth::Size(){
  return size;
}

double Smooth::Disk(distance radius){
  if (radius>inner+smoothing/2) {
    return 0.0;
  }
  if (radius<inner-smoothing/2) {
    return 1.0;
  }
  return (inner+smoothing/2-radius)/smoothing;
}

double Smooth::Ring(distance radius){
  if (radius<inner-smoothing/2) {
    return 0.0;
  }
  if (radius<inner+smoothing/2) {
    return (inner+smoothing/2-radius)/smoothing;
  }
  if (radius<outer-smoothing/2) {
    return 1.0;
  }
  if (radius<outer+smoothing/2) {
    return (outer+smoothing/2-radius)/smoothing;
  }
  return 0.0;
 }

double Smooth::Sigmoid(double center, double variable, double width){
  return 1.0/(1.0+std::exp(4.0*(center-variable)/width));
}

const std::vector<std::vector<density> > & Smooth::Field() const {
  return field;
};

