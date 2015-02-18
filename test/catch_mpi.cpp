// Next line tells CATCH we will use our own main function
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "Smooth.h"
#include <cmath>
#include <mpi.h>


int main(int argc, char * argv[]) {
    MPI_Init (&argc, &argv);
    int result = Catch::Session().run(argc, argv);
    MPI_Finalize();
    return result;
}


TEST_CASE ("MPI Tests"){
    int rank, size;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    REQUIRE(size==2); // test designed for a two-process situation
    SECTION("Basic ring communication works"){
       Smooth smooth(200,100,5,rank,size);
       smooth.SeedDisk(); // Half the Seeded Disk falls in smooth2's domain, so total filling will be half a disk.
       if (rank==0) {
         REQUIRE(abs(smooth.FillingDisk(15,0)-0.5)<0.1);
       }
       if (rank==1) {
         REQUIRE(smooth.FillingDisk(84,0)==0.0);
       }
       smooth.CommunicateMPI();
       if (rank==0) {
         REQUIRE(abs(smooth.FillingDisk(0,0)-0.5)<0.1);
       }
       if (rank==1) {
         REQUIRE(abs(smooth.FillingDisk(84,0)-0.5)<0.1);
       }
    }
}
