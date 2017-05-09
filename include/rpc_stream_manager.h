//
// Created by tiancai on 2017/5/4.
//

#ifndef HEPTAPOD_RPC_STREAM_MANAGER_H
#define HEPTAPOD_RPC_STREAM_MANAGER_H

#include <unordered_set>

namespace hpt {

class RpcStream;

class RpcStreamManager
{
public:
    static RpcStreamManager& Instance()
    {
        static RpcStreamManager instance;
        return instance;
    }

    void Add(RpcStream* s) { _streams.insert(s); }
    void Del(RpcStream* s) { _streams.erase(s); }
    void Clear() { _streams.clear(); }
    void Size() { _streams.size(); }

    std::unordered_set<RpcStream*>& streams() { return _streams; }
    auto begin() { return _streams.begin(); }
    auto end() { return _streams.end(); }

private:
    std::unordered_set<RpcStream*> _streams;
};

}

#endif //HEPTAPOD_RPC_STREAM_MANAGER_H
