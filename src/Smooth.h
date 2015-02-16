#include <vector>

typedef double density;
typedef double distance;
typedef double filling;

class Smooth {
  public:
    Smooth(int size=100,
    distance inner=21.0,
    filling birth_inner=0.278,
    filling birth_outer=0.365,
    filling death_inner=0.267,
    filling death_outer=0.445,
    filling smoothing_inner=0.028,
    filling smoothing_outer=0.147);
    int Size();
    const std::vector<std::vector<density> > & Field() const;
    double Disk(distance radius);
    double Ring(distance radius);
    static double Sigmoid(double centre, double variable, double width);
  private:
    int size;
    std::vector<std::vector<density> > field;
    distance inner;
    filling birth_inner;
    filling birth_outer;
    filling death_inner;
    filling death_outer;
    filling smoothing_inner;
    filling smoothing_outer;
    distance outer;
    distance smoothing;
};

