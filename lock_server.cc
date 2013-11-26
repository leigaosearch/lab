// the lock server implementation

#include "lock_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

lock_server::lock_server():
  nacquire (0)
{
  VERIFY(pthread_mutex_init(&table_mutex_, NULL) == 0);
  VERIFY(pthread_cond_init(&cond, NULL) == 0);
}

  lock_protocol::status
lock_server::stat(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  printf("stat request from clt %d\n", clt);
  r = nacquire;
  return ret;
}

  lock_protocol::status
lock_server::acquire(int clt,lock_protocol::lockid_t lid, int &r)
{

  auto it = lock_table_.find(lid);
  if (it == lock_table_.end()) {
    ScopedLock ml(&table_mutex_);
    if (it == lock_table_.end()) {
      //no this kind of lock, insert.
      auto lockstatus = klocked;
      auto t = std::make_pair(lid,lockstatus);
      lock_table_.insert(t);
    }
  }
  else {
    if (lock_table_[lid] == klocked) {
      ScopedLock ml(&table_mutex_);
      pthread_cond_wait(&cond, &table_mutex_);
      lock_table_[lid] = klocked;


    }

    else {
      ScopedLock ml(&table_mutex_);
      lock_table_[lid] = klocked;

    }





  }

  return 0;

}


  lock_protocol::status
lock_server::release(int clt,lock_protocol::lockid_t lid, int &r)
{

  auto it = lock_table_.find(lid);
  if (it != lock_table_.end()) {
    ScopedLock ml(&table_mutex_);
    lock_table_[lid] = kfree;
    pthread_cond_signal(&cond);

  }

  return 0;


}


