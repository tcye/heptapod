//
// Created by tiancai on 2017/3/8.
//

#ifndef HEPTAPOD_SERVER_H
#define HEPTAPOD_SERVER_H

#include "common.h"

namespace hpt {

class Server
    : private noncopyable,
      public std::enable_shared_from_this<Server> {
 public:
  Server(const Endpoint& endpoint);
  ~Server();
};

} //namespace hpt


#endif //HEPTAPOD_SERVER_H
