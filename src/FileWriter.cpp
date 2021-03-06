/*
 * FileWriter.cpp
 *
 *  Created on: 24.05.2017
 *      Author: christian@inf-schaefer.de
 */

#include "FileWriter.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileWriter::FileWriter(std::string filename) {
  this->filename_ = filename;
  this->writeHeader();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileWriter::writeHeader()
{
  this->writeLine("timestamp,cte,speed,angle,steering_angle,throttle,total_cte,avg_cte");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileWriter::writePidParameters(std::string title, double kp, double ki, double kd )
{
  std::stringstream ss;
  ss << title << ",Kp," << kp << ",Ki," << ki << ",Kd," << kd;
  this->writeLine(ss.str());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileWriter::writeParameter(std::string name, double value)
{
  std::stringstream ss;
  ss << "," << name << "," << value;
  this->writeLine(ss.str());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileWriter::writeLine(double timestamp, double cte, double speed, double angle, double steering_angle,
                           double throttle, double total_cte, double avg_cte )
{
  std::stringstream ss;
  ss << timestamp << "," << cte << "," << speed << "," << angle << "," << steering_angle << "," << throttle << ","
      << total_cte << "," << avg_cte;
  this->writeLine(ss.str());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileWriter::writeLine(std::string line) {
  this->write(line + "\n");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileWriter::write(std::string data) {
  std::ofstream dataFile;
  dataFile.open(this->filename_, std::ios::app);
  dataFile << data;
  dataFile.close();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
