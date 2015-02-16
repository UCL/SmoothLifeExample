#include <cmath>

#include "smooth.h"


Smooth::Smooth(int size,
        distance inner,
        filling birth_1,
        filling birth_2 ,
        filling death_1,
        filling death_2,
        filling smoothing_inner,
        filling smoothing_outer)
    : size(100),
    inner(inner),
    birth_1(birth_1),
    birth_2(birth_2),
    death_1(death_1),
    death_2(death_2),
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

density Smooth::transition(filling inner, filling outer){
  double t1=birth_1*(1.0-Sigmoid(inner,0.5,smoothing_inner))+death_1*Sigmoid(inner,0.5,smoothing_inner);
  double t2=birth_2*(1.0-Sigmoid(inner,0.5,smoothing_inner))+death_2*Sigmoid(inner,0.5,smoothing_inner);
  return Sigmoid(outer,t1,smoothing_outer)*(1.0-Sigmoid(outer,t2,smoothing_outer));
}

const std::vector<std::vector<density> > & Smooth::Field() const {
  return field;
};

