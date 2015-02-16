#include <vector>
#include <string>

typedef double density;
typedef double distance;
typedef double filling;

class Smooth {
  public:
    Smooth(int size=100,
    distance inner=21.0,
    filling birth_1=0.278,
    filling birth_2=0.365,
    filling death_1=0.267,
    filling death_2=0.445,
    filling smoothing_inner=0.028,
    filling smoothing_outer=0.147);
    int Size();
    const std::vector<std::vector<density> > & Field() const;
    double Disk(distance radius) const;
    double Ring(distance radius) const;
    static double Sigmoid(double centre, double variable, double width);
    density transition(filling inner, filling outer) const;
    int TorusDifference(int x1,int x2) const ;
    double Radius(int x1, int y1, int x2, int y2) const;
    double NormalisationRing() const;
    double NormalisationDisk() const;
    filling FillingRing(int x,int y) const;
    filling FillingDisk(int x,int y) const;
    density NewState(int x, int y) const;
    void SeedRandom();
    void SeedDisk();
    void Update();
    void Write(std::ostream &out);
    int Frame() const;
  private:
    int size;
    std::vector<std::vector<density> > field1;
    std::vector<std::vector<density> > field2;
    std::vector<std::vector<density> > * field;
    std::vector<std::vector<density> > * fieldNew;
    distance inner;
    filling birth_1;
    filling birth_2;
    filling death_1;
    filling death_2;
    filling smoothing_inner;
    filling smoothing_outer;
    distance outer;
    distance smoothing;
    int frame;
    double normalisation_disk;
    double normalisation_ring;
};

