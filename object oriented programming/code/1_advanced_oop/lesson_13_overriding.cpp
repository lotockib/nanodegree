#include <assert.h>
#include <cmath>

#define PI 3.14159

// TODO: Declare abstract class VehicleModel
  
class VehicleModel {
    public:
    // TODO: Declare virtual function Move()
        virtual void Move(double, double) = 0;
};

// TODO: Derive class ParticleModel from VehicleModel
class ParticleModel : public VehicleModel {
    public:
    // TODO: Override the Move() function
    void Move(double, double) override;
    float theta;
    float x;
    float y;

};

void ParticleModel::Move(double v, double t) {
    theta += t;
    x += v * cos(theta);
    y += v * cos(theta);
};

// TODO: Derive class BicycleModel from ParticleModel
class BicycleModel : public ParticleModel {
    public:
        void Move(double, double) override;
        float L{10};
};

  // TODO: Override the Move() function
  // TODO: Define L
void BicycleModel::Move(double v, double phi) {
    theta += v / L * tan(phi);
    x += v * cos(theta);
    y += v * cos(theta);
}


// TODO: Pass the tests
int main() {
  // Test function overriding
  ParticleModel particle;
  BicycleModel bicycle;
  particle.Move(10, PI / 9);
  bicycle.Move(10, PI / 9);
  assert(particle.x != bicycle.x);
  assert(particle.y != bicycle.y);
  assert(particle.theta != bicycle.theta);
}