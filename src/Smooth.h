#include <vector>

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
    double Disk(distance radius);
    double Ring(distance radius);
    static double Sigmoid(double centre, double variable, double width);
    density transition(filling inner, filling outer);
  private:
    int size;
    std::vector<std::vector<density> > field;
    distance inner;
    filling birth_1;
    filling birth_2;
    filling death_1;
    filling death_2;
    filling smoothing_inner;
    filling smoothing_outer;
    distance outer;
    distance smoothing;
};

