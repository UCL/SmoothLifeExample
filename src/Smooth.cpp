#include <cmath>
#include <cstdlib>
#include <iostream>

#include "smooth.h"


Smooth::Smooth(int size,
        distance inner,
        filling birth_1,
        filling birth_2 ,
        filling death_1,
        filling death_2,
        filling smoothing_disk,
        filling smoothing_ring)
    : size(100),
    inner(inner),
    birth_1(birth_1),
    birth_2(birth_2),
    death_1(death_1),
    death_2(death_2),
    smoothing_disk(smoothing_disk),
    smoothing_ring(smoothing_ring),
    outer(inner*3),
    smoothing(1.0),
    field1(size,std::vector<density>(size)),
    field2(size,std::vector<density>(size)),
    field(&field1),
    fieldNew(&field2),
    frame(0)
{
  normalisation_disk=NormalisationDisk();
  normalisation_ring=NormalisationRing();
}

int Smooth::Size(){
  return size;
}

double Smooth::Disk(distance radius) const {
  if (radius>inner+smoothing/2) {
    return 0.0;
  }
  if (radius<inner-smoothing/2) {
    return 1.0;
  }
  return (inner+smoothing/2-radius)/smoothing;
}

double Smooth::Ring(distance radius) const {
  if (radius<inner-smoothing/2) {
    return 0.0;
  }
  if (radius<inner+smoothing/2) {
    return (radius+smoothing/2-inner)/smoothing;
  }
  if (radius<outer-smoothing/2) {
    return 1.0;
  }
  if (radius<outer+smoothing/2) {
    return (outer+smoothing/2-radius)/smoothing;
  }
  return 0.0;
 }

double Smooth::Sigmoid(double variable, double center, double width){
  return 1.0/(1.0+std::exp(4.0*(center-variable)/width));
}

density Smooth::transition(filling disk, filling ring) const {
  double t1=birth_1*(1.0-Sigmoid(disk,0.5,smoothing_disk))+death_1*Sigmoid(disk,0.5,smoothing_disk);
  double t2=birth_2*(1.0-Sigmoid(disk,0.5,smoothing_disk))+death_2*Sigmoid(disk,0.5,smoothing_disk);
  return Sigmoid(ring,t1,smoothing_ring)*(1.0-Sigmoid(ring,t2,smoothing_ring));
}

const std::vector<std::vector<density> > & Smooth::Field() const {
  return *field;
};

int Smooth::TorusDifference(int x1, int x2) const {
    int straight=std::abs(x2-x1);
    int wrapleft=std::abs(x2-x1+size);
    int wrapright=std::abs(x2-x1-size);
    if ((straight<wrapleft) && (straight<wrapright)) {
      return straight;
    } else {
      return (wrapleft < wrapright) ? wrapleft : wrapright;
    }
}

double Smooth::Radius(int x1,int y1,int x2,int y2) const {
  return std::sqrt(std::pow(TorusDifference(x1,x2),2)+std::pow(TorusDifference(y1,y2),2));
}

double Smooth::NormalisationDisk() const {
  double total=0.0;
  for (int x=0;x<size;x++) {
     for (int y=0;y<size;y++) {
       total+=Disk(Radius(0,0,x,y));
    }
  };
  return total;
}

double Smooth::NormalisationRing() const {
  double total=0.0;
  for (int x=0;x<size;x++) {
     for (int y=0;y<size;y++) {
       total+=Ring(Radius(0,0,x,y));
    }
  };
  return total;
}

filling Smooth::FillingDisk(int x, int y) const {
  double total=0.0;
  for (int x1=0;x1<size;x1++) {
     for (int y1=0;y1<size;y1++) {
       if (TorusDifference(x,x1) > inner+smoothing) continue;
       if (TorusDifference(y,y1) > inner+smoothing) continue;
       total+=(*field)[x1][y1]*Disk(Radius(x,y,x1,y1));
    }
  };
  return total/normalisation_disk;
}

filling Smooth::FillingRing(int x, int y) const {
  double total=0.0;
  for (int x1=0;x1<size;x1++) {
     for (int y1=0;y1<size;y1++) {
       if (TorusDifference(x,x1) > outer+smoothing) continue;
       if (TorusDifference(y,y1) > outer+smoothing) continue;
       total+=(*field)[x1][y1]*Ring(Radius(x,y,x1,y1));
    }
  };
  return total/normalisation_ring;
}

density Smooth::NewState(int x, int y) const {
  return transition(FillingDisk(x,y),FillingRing(x,y));
}

void Smooth::Update() {
  for (int x=0;x<size;x++) {
    for (int y=0;y<size;y++) {
      (*fieldNew)[x][y]=NewState(x,y);
   }
  }
  std::vector<std::vector<density> > * fieldTemp;
  fieldTemp=field;
  field=fieldNew;
  fieldNew=fieldTemp;
  frame++;
}

void Smooth::SeedRandom() {
   for (int x=0;x<size;x++) {
     for (int y=0;y<size;y++) {
      (*field)[x][y]=(static_cast<double>(rand()) / static_cast<double>(RAND_MAX));
     }
   }
}

void Smooth::SeedDisk() {
   for (int x=0;x<size;x++) {
     for (int y=0;y<size;y++) {
      (*field)[x][y]=Disk(Radius(0,0,x,y));
     }
   }
}

void Smooth::SeedRing() {
   for (int x=0;x<size;x++) {
     for (int y=0;y<size;y++) {
      (*field)[x][y]=Ring(Radius(0,0,x,y));
     }
   }
}

void Smooth::Write(std::ostream &out) {
   for (int x=0;x<size;x++) {
     for (int y=0;y<size;y++) {
        out << (*field)[x][y] << " , ";
     }
     out << std::endl;
   }
   out << std::endl;
}

int Smooth::Frame() const {
  return frame;
}
