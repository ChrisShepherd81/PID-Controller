/*
 * FileWriter.h
 *
 *  Created on: 24.05.2017
 *      Author: christian@inf-schaefer.de
 */

#ifndef SRC_FILEWRITER_H_
#define SRC_FILEWRITER_H_

#include <fstream>
#include <sstream>

class FileWriter {
 public:
  FileWriter(std::string filename);

  void writeLine(double timestamp, double cte, double speed, double angle, double steering_angle,
                 double throttle, double total_cte, double avg_cte );

  void writeParameters(double kp, double ki, double kd );

  void writeLine(std::string line);

 private:
  std::string filename_;
  void writeHeader();

};

#endif /* SRC_FILEWRITER_H_ */
