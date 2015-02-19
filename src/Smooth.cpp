#include <cmath>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cstring>

#include <mpi.h>

#include "Smooth.h"


Smooth::Smooth(int sizex,
        int sizey,
        distance inner,
        int rank,
        int mpi_size,
        filling birth_1,
        filling birth_2 ,
        filling death_1,
        filling death_2,
        filling smoothing_disk,
        filling smoothing_ring)
    :
    sizey(sizey),
    inner(inner),
    rank(rank),
    mpi_size(mpi_size),
    birth_1(birth_1),
    birth_2(birth_2),
    death_1(death_1),
    death_2(death_2),
    smoothing_disk(smoothing_disk),
    smoothing_ring(smoothing_ring),
    outer(inner*3),
    smoothing(1.0),
    frame(0),
    range(outer+smoothing/2),
    local_x_size(sizex/mpi_size),
    local_x_size_with_halo(local_x_size+2*range),
    total_x_size(sizex),
    x_coordinate_offset(rank*local_x_size-range),
    local_x_min_calculate(range),
    local_x_max_calculate(range+local_x_size),
    field1(local_x_size_with_halo,std::vector<density>(sizey)),
    field2(local_x_size_with_halo,std::vector<density>(sizey)),
    field(&field1),
    fieldNew(&field2)
{
  normalisation_disk=NormalisationDisk();
  normalisation_ring=NormalisationRing();
  assert(sizex%mpi_size==0); // WE WILL NOT TRY TO DEAL WITH REMAINDERS
  send_transport_buffer=new density[range*sizey];
  receive_transport_buffer=new density[range*sizey];
  left=rank-1;
  right=rank+1;
  if (rank==0){
    left=mpi_size-1;
  }
  if (rank==mpi_size-1){
    right=0;
  }
}

Smooth::~Smooth(){
  delete[] send_transport_buffer;
  delete[] receive_transport_buffer;
}

int Smooth::Range(){
  return range;
}

int Smooth::LocalXSize(){
  return local_x_size;
}

int Smooth::LocalXSizeWithHalo(){
  return local_x_size_with_halo;
}

int Smooth::Sizex(){
  return total_x_size;
}
int Smooth::Sizey(){
  return sizey;
}
int Smooth::Size(){
  return total_x_size*sizey;
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

density Smooth::Field(int x,int y) const {
  return (*field)[x][y];
};

void Smooth::SetNewField(int x, int y, density value){
  (*fieldNew)[x][y]=value;
}

void Smooth::SetField(int x, int y, density value){
  (*field)[x][y]+=value;
}

void Smooth::SeedField(int x, int y, density value){
  SetField(x,y,value);
  if (Field(x,y)>1.0){
     SetField(x,y,1.0);
  }
}

int Smooth::TorusDifference(int x1, int x2, int size) {
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
  int xdiff=TorusDifference(x1+x_coordinate_offset,x2+x_coordinate_offset,total_x_size);
  int ydiff=TorusDifference(y1,y2,sizey);
  return std::sqrt(xdiff*xdiff+ydiff*ydiff);
}

double Smooth::NormalisationDisk() const {
  double total=0.0;
  for (int x=0;x<total_x_size;x++) {
     for (int y=0;y<sizey;y++) {
       total+=Disk(Radius(0,0,x,y));
    }
  };
  return total;
}

double Smooth::NormalisationRing() const {
  double total=0.0;
  for (int x=0;x<total_x_size;x++) {
     for (int y=0;y<sizey;y++) {
       total+=Ring(Radius(0,0,x,y));
    }
  };
  return total;
}

filling Smooth::FillingDisk(int x, int y) const {
  double total=0.0;
  for (int x1=0;x1<local_x_size_with_halo;x1++) {
     for (int y1=0;y1<sizey;y1++) {
       total+=Field(x1,y1)*Disk(Radius(x,y,x1,y1));
    }
  };
  return total/normalisation_disk;
}

filling Smooth::FillingRing(int x, int y) const {
  double total=0.0;
  for (int x1=0;x1<local_x_size_with_halo;x1++) {
     for (int y1=0;y1<sizey;y1++) {
       total+=Field(x1,y1)*Ring(Radius(x,y,x1,y1));
    }
  };
  return total/normalisation_ring;
}

density Smooth::NewState(int x, int y) const {
  return transition(FillingDisk(x,y),FillingRing(x,y));
}

void Smooth::Update() {
   for (int x=local_x_min_calculate;x<local_x_max_calculate;x++) {
     for (int y=0;y<sizey;y++) {
      SetNewField(x,y,NewState(x,y));
     }
   }

  std::vector<std::vector<density> > * fieldTemp;
  fieldTemp=field;
  field=fieldNew;
  fieldNew=fieldTemp;
  frame++;

}

void Smooth::QuickUpdate() {
  for (int x=local_x_min_calculate;x<local_x_max_calculate;x++) {
    for (int y=0;y<sizey;y++) {
      double ring_total=0.0;
      double disk_total=0.0;

      for (int x1=0;x1<local_x_size_with_halo;x1++) {
          int deltax=TorusDifference(x+x_coordinate_offset,x1+x_coordinate_offset,total_x_size);
          if (deltax>outer+smoothing/2) continue;

          for (int y1=0;y1<sizey;y1++) {
            int deltay=TorusDifference(y,y1,sizey);
            if (deltay>outer+smoothing/2) continue;

            double radius=std::sqrt(deltax*deltax+deltay*deltay);
            double fieldv=Field(x1,y1);
            ring_total+=fieldv*Ring(radius);
            disk_total+=fieldv*Disk(radius);
          }
      }

      SetNewField(x,y,transition(disk_total/normalisation_disk,ring_total/normalisation_ring));
    }
  }

  t_field * fieldTemp;
  fieldTemp=field;
  field=fieldNew;
  fieldNew=fieldTemp;
  frame++;
}

void Smooth::SeedRandom() {
   for (int x=local_x_min_calculate;x<local_x_max_calculate;x++) {
     for (int y=0;y<sizey;y++) {
      SeedField(x,y,(static_cast<double>(rand()) / static_cast<double>(RAND_MAX)));
     }
   }
}

void Smooth::SeedDisk(int at_x,int at_y) {
   for (int x=local_x_min_calculate;x<local_x_max_calculate;x++) {
     for (int y=0;y<sizey;y++) {
      SeedField(x,y,Disk(Radius(at_x-x_coordinate_offset,at_y,x,y)));
     }
   }
}

void Smooth::SeedRing(int at_x,int at_y) {
   for (int x=local_x_min_calculate;x<local_x_max_calculate;x++) {
     for (int y=0;y<sizey;y++) {
      SeedField(x,y,Ring(Radius(at_x-x_coordinate_offset,at_y,x,y)));
     }
   }
}

void Smooth::SeedRandomDisk() {
  int x=rand()%total_x_size;
  int y=rand()%sizey;
  SeedDisk(x,y);
}

void Smooth::Write(std::ostream &out) {
   for (int x=local_x_min_calculate;x<local_x_max_calculate;x++) {
     for (int y=0;y<sizey;y++) {
        out << Field(x,y) << " , ";
     }
     out << std::endl;
   }
   out << std::endl;
}

int Smooth::Frame() const {
  return frame;
}

void Smooth::BufferLeftHaloForSend(){
  for (int x=local_x_min_calculate; x<local_x_min_calculate+range;x++){
    for (int y=0; y<sizey;y++){
      send_transport_buffer[y*range+x-local_x_min_calculate]=Field(x,y);
    }
  }
}

void Smooth::BufferRightHaloForSend(){
  for (int x=local_x_max_calculate-range; x<local_x_max_calculate;x++){
    for (int y=0; y<sizey;y++){
      send_transport_buffer[y*range+x-local_x_max_calculate+range]=Field(x,y);
    }
  }
}

void Smooth::UnpackLeftHaloFromReceive(){
  for (int x=0; x<range;x++){
    for (int y=0; y<sizey;y++){
      SetField(x,y,receive_transport_buffer[y*range+x]);
    }
  }
}

void Smooth::UnpackRightHaloFromReceive(){
  for (int x=local_x_max_calculate; x<local_x_size_with_halo;x++){
    for (int y=0; y<sizey;y++){
      SetField(x,y,receive_transport_buffer[y*range+x-local_x_max_calculate]);
    }
  }
}

void Smooth::CommunicateLocal(Smooth &left, Smooth &right){
  BufferLeftHaloForSend();
  std::memcpy(left.receive_transport_buffer,send_transport_buffer,sizeof(density)*range*sizey);
  left.UnpackRightHaloFromReceive();
  BufferRightHaloForSend();
  std::memcpy(right.receive_transport_buffer,send_transport_buffer,sizeof(density)*range*sizey);
  right.UnpackLeftHaloFromReceive();
}

void Smooth::CommunicateMPI(){
  BufferLeftHaloForSend();
  MPI_Sendrecv(send_transport_buffer,range*sizey,MPI_DOUBLE,left,rank,
      receive_transport_buffer, range*sizey,MPI_DOUBLE,right,right,
      MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  UnpackRightHaloFromReceive();
  BufferRightHaloForSend();
  MPI_Sendrecv(send_transport_buffer,range*sizey,MPI_DOUBLE,right,mpi_size+rank,
      receive_transport_buffer,range*sizey,MPI_DOUBLE,left,mpi_size+left,
      MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  UnpackLeftHaloFromReceive();
}
