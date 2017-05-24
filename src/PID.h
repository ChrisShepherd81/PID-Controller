#ifndef PID_H
#define PID_H

#include <iostream>
#include <chrono>

using std::chrono::system_clock;

#define PRINT 1

class PID {
 public:
  // Coefficients
  double Kp;
  double Ki;
  double Kd;

  PID();
  virtual ~PID();
  void Init(double Kp, double Ki, double Kd);
  void Update(double cte, double speed);

  // Getters
  double GetTotalError() const;
  double GetAveragedError() const;
  double GetTimeStamp() const;
  double GetCorrection() const;

private:
  double correction = 0;
  double realtive_timestamp = 0;
  system_clock::time_point last_timestamp_;

  double old_cte_ = 0;
  double cte_sum = 0;

  //Overflows not handled
  size_t counter = 0;
  double cte_sum_abs = 0;

  void calculate(double cte, double dt);
};

#endif /* PID_H */
