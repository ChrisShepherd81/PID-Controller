#ifndef PID_H
#define PID_H

#include <iostream>
#include <chrono>

using std::chrono::system_clock;

#define PRINT 1

class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();

  double correction = 0;
private:
  double calculate(double cte, double dt);
  double old_cte_ = 0;
  double cte_sum = 0;
  system_clock::time_point timestamp_;
};

#endif /* PID_H */
