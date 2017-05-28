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
std::string hasData(std::string s);

int main(int argc, char* argv[])
{
  uWS::Hub h;

  PID pid;

  //manual determined values
  double kp = 0.17;
  double ki = 0.0001;
  double kd = 160;
  double throttle = 0.3;

  //Set values if parameters given via console parameters
  if(argc >= 4)
  {
    kp = std::stod(argv[1]);
    ki = std::stod(argv[2]);
    kd = std::stod(argv[3]);
  }
  if(argc == 5)
  {
    throttle = std::stod(argv[4]);
  }

  //Initialize the pid variable.
  pid.Init(kp, ki, kd);

  //Construct filename with current date time
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::stringstream ss;
  ss << "test_" << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S") << ".csv";

  FileWriter fileWriter(ss.str());

  fileWriter.writeParameters(kp, ki,kd );
  Twiddle twiddle(pid);


  h.onMessage([&](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        //std::cout << s << std::endl;
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */

          pid.Update(cte, speed);
          steer_value = pid.GetCorrection();

#if PRINT
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << "Speed: " << speed << std::endl;
#endif
          fileWriter.writeLine(pid.GetTimeStamp(), cte, speed, steer_value, throttle, pid.GetTotalError(), pid.GetAveragedError() );

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
