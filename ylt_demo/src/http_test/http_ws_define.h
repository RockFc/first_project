#include <iostream>

#include "ylt/struct_json/json_reader.h"
#include "ylt/struct_json/json_writer.h"


struct GetInfo
{
  int id;
  std::string name;
};
REFLECTION(GetInfo, id, name);

struct StatusInfo
{
  int status;
  std::string msg;
};
REFLECTION(StatusInfo, status, msg);
