// this is the lock server
// the lock client has a similar interface

#ifndef lock_server_h
#define lock_server_h

#include <string>
#include "lock_protocol.h"

const int klocked = 1;
const int kfree = 0;
class lock_server {

 protected:
  int nacquire;
  std::map<lock_protocol::lockid_t,  lock_protocol::status> lock_table_;
  pthread_mutex_t table_mutex_;
  pthread_cond_t      cond;
 public:
  lock_server();
  ~lock_server() {};
  lock_protocol::status stat(int clt, lock_protocol::lockid_t lid, int &);
  lock_protocol::status acquire(int clt,lock_protocol::lockid_t lid, int &);
  lock_protocol::status release(int clt,lock_protocol::lockid_t lid, int &);
};

#endif







