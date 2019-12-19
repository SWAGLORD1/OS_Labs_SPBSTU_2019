#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

#include <status.h>

struct ClientInfo
{
  int pid;
  bool attached;
  int deads_num;

  explicit ClientInfo(int pid) : pid(pid), attached(pid != 0), deads_num(0)
  {
  }
};

#endif // CLIENT_INFO_H