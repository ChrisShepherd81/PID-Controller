#include <uWS/uWS.h>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <ctime>

#include "json.hpp"
#include "PID.h"
#include "Twiddle.h"
#include "FileWriter.h"

// for convenience
using json = nlohmann::json;

void reset_simulator(uWS::WebSocket<uWS::SERVER>& ws);
double normalize(double value);
std::string hasData(std::string s);

int main(int argc, char* argv[])
{
  uWS::Hub h;

  PID pid_cte;
  PID pid_speed;

  //manual determined values
  double kp_cte = 0.17;
  double ki_cte = 0.00042;
  double kd_cte = 400;
  double kp_speed = 0.01;
  double ki_speed = 0.0;
  double kd_speed = 0.0;

  //Set values if parameters given via console parameters
  if(argc >= 7)
  {
    kp_cte = std::stod(argv[1]);
    ki_cte = std::stod(argv[2]);
    kd_cte = std::stod(argv[3]);
    kp_speed = std::stod(argv[4]);
    ki_speed = std::stod(argv[5]);
    kd_speed = std::stod(argv[6]);
  }

  //Initialize the PID controllers.
  pid_cte.Init(kp_cte, ki_cte, kd_cte);
  pid_speed.Init(kp_speed, ki_speed, kd_speed);

  //Construct filename with current date time
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::stringstream ss;
  ss << "test_" << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S") << ".csv";

  FileWriter fileWriter(ss.str());

  fileWriter.writeParameters(kp_cte, ki_cte,kd_cte );

  //values under control
  double steer_value = 0.0;
  double throttle = 1.0;


  h.onMessage([&](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          if(std::fabs(cte) > 2.5) //car is off road
          {
            reset_simulator(ws);
            std::cout << "BANG\n" << std::endl;
            exit(-1);
          }

          pid_cte.Update(cte);
          steer_value = pid_cte.GetCorrection();
          steer_value = normalize(steer_value);

          pid_speed.Update(std::fabs(steer_value));
          throttle = 1 + pid_speed.GetCorrection();
          throttle = normalize(throttle);

#if PRINT
          std::cout << "CTE: " << cte <<  " Angle: " << angle << " Steering Value: " << steer_value <<  " Diff: " <<  (angle/25.0)-steer_value<<" Speed: " << speed << " Throttle: " << throttle << std::endl;
#endif
          fileWriter.writeLine(pid_cte.GetTimeStamp(), cte, speed, angle, steer_value, throttle, pid_cte.GetTotalError(), pid_cte.GetAveragedError() );

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          //std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

void reset_simulator(uWS::WebSocket<uWS::SERVER>& ws)
{
    // reset
    std::string msg("42[\"reset\", {}]");
    ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
}

double normalize(double value)
{
  if(value > 1.0)
    value = 1.0;
  if(value < -1.0)
    value = -1.0;
  return value;
}
