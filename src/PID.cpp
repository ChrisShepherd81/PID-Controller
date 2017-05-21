#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;
  timestamp_ = system_clock::now();
}

void PID::UpdateError(double cte) {

  auto now = system_clock::now();
  double dt = std::chrono::duration_cast<std::chrono::milliseconds>(now-timestamp_).count();

  calculate(cte, dt);

  this->old_cte_ = cte;
  this->timestamp_ = now;
}

double PID::calculate(double cte, double dt)
{
  double p, i, d;
  this->cte_sum += cte;
  p = -this->Kp * cte;
  i = this->Ki * this->cte_sum;
  d = this->Kd* (cte - this->old_cte_)*(dt*1e-3);
  correction = p-i-d;

#if PRINT
  //std::cout << "P: " << p << " I: " << i << " D: " << d << " correction: " << correction <<  " dt: " << dt << std::endl;
#endif

  return correction;
}

double PID::TotalError() {
}

