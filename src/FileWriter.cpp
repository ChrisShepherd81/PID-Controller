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
  this->writeLine("timestamp,cte,speed,steering_angle,throttle,total_cte,avg_cte");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileWriter::writeLine(double timestamp, double cte, double speed, double steering_angle,
                           double throttle, double total_cte, double avg_cte )
{
  std::stringstream ss;
  ss << timestamp << "," << cte << "," << speed << "," << steering_angle << "," << throttle << ","
      << total_cte << "," << avg_cte;
  this->writeLine(ss.str());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileWriter::writeLine(std::string line) {
  std::ofstream dataFile;
  dataFile.open(this->filename_, std::ios::app);
  dataFile << line << "\n";
  dataFile.close();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
