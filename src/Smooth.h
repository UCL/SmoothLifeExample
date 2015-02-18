#include <vector>
#include <string>

typedef double density;
typedef double distance;
typedef double filling;

class Smooth {
  public:
    Smooth(int sizex=100,
    int sizey=100,
    distance inner=21.0,
    int rank=0,
    int mpi_size=1,
    filling birth_1=0.278,
    filling birth_2=0.365,
    filling death_1=0.267,
    filling death_2=0.445,
    filling smoothing_disk=0.147,
    filling smoothing_ring=0.028);

    virtual ~Smooth();

    int Size();
    int Sizex();
    int LocalXSizeWithHalo();
    int LocalXSize();
    int Sizey();
    int Range();
    const std::vector<std::vector<density> > & Field() const;
    double Disk(distance radius) const;
    double Ring(distance radius) const;
    static double Sigmoid(double variable, double center, double width);
    density transition(filling disk, filling ring) const;
    static int TorusDifference(int x1,int x2, int size);
    double Radius(int x1, int y1, int x2, int y2) const;
    double NormalisationRing() const;
    double NormalisationDisk() const;
    filling FillingRing(int x,int y) const;
    filling FillingDisk(int x,int y) const;
    density NewState(int x, int y) const;
    void SeedRandom();
    void SeedRandomDisk();
    void SeedDisk(int at_x=0,int at_y=0);
    void SeedRing(int at_x=0,int at_y=0);
    void Update();
    void QuickUpdate();
    void Write(std::ostream &out);
    int Frame() const;
    void CommunicateLocal(Smooth &left_neighbour, Smooth &right_neighbour);
    void CommunicateMPI();
    void BufferLeftHaloForSend();
    void BufferRightHaloForSend();
    void UnpackRightHaloFromReceive();
    void UnpackLeftHaloFromReceive();
  private:
    int sizey;
    distance inner;
    int rank;
    int mpi_size;
    int left; // MPI rank of left neighbour
    int right; // MPI rank of right neighbour
    filling birth_1;
    filling birth_2;
    filling death_1;
    filling death_2;
    filling smoothing_disk;
    filling smoothing_ring;
    distance outer;
    distance smoothing;
    int frame;
    double normalisation_disk;
    double normalisation_ring;
    int range;
    int local_x_size;
    int local_x_size_with_halo;
    int total_x_size;
    int x_coordinate_offset;
    int local_x_min_calculate;
    int local_x_max_calculate;
    std::vector<std::vector<density> > field1;
    std::vector<std::vector<density> > field2;
    std::vector<std::vector<density> > * field;
    std::vector<std::vector<density> > * fieldNew;
    density *send_transport_buffer;
    density *receive_transport_buffer;
};

