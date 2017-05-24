//
// Created by tiancai on 2017/5/24.
//

#ifndef HEPTAPOD_RPC_CONNECTION_H
#define HEPTAPOD_RPC_CONNECTION_H

#include "common.h"
#include "msgpack.hpp"
#include "detail/call_helper.h"

#include <functional>
#include <unordered_map>
#include <memory>

namespace hpt {
template <typename ServiceType>
class RpcConnection : public std::enable_shared_from_this<RpcConnection<ServiceType>>
{
    HPT_CLASS(RpcConnection)
    using PackageType = std::tuple<std::string, msgpack::object>;
    using AdaptorType = std::function<void(ServiceType*, const msgpack::object&)>;
    static std::unordered_map<std::string, AdaptorType> _adaptors;
    static const size_t MAX_READ_SIZE_PER_TIME = 65536;

public:
    static void InitServiceMap() { }
    enum
    {
        STATUS_INIT = 0,
        STATUS_CONNECTING = 1,
        STATUS_CONNECTED = 2,
        STATUS_CLOSED = 3,
    };

    RpcConnection() : _status(STATUS_INIT), _bytes_written(0) { }

    void Open(asio::io_service& io_service)
    {
        _socket = std::make_shared<asio::ip::tcp::socket>(io_service);
        _write_strand = std::make_shared<asio::strand>(io_service);
    }

    virtual ~RpcConnection()
    {
        Close();
    }

    void Close()
    {
        if (_status.exchange(STATUS_CLOSED) == STATUS_CONNECTED)
        {
            _socket->shutdown(asio::ip::tcp::socket::shutdown_both);
            _socket->close();
            _socket.reset();
            _write_strand.reset();
            logger()->info("Close: closed");
        }
        else
        {
            logger()->info("Close: duplicated close operation");
        }
    }

    template <typename Ret, typename... Args>
    static void Bind(const std::string& name, Ret (ServiceType::*func)(Args ...))
    {
        AdaptorType adaptor = [func](ServiceType* s, const msgpack::object& obj) -> void
        {
            std::tuple<std::decay_t<Args>...> args;
            obj.convert(args);
            detail::Call(func, s, args);
        };
        _adaptors.insert({name, adaptor});
    }

    void Dispatch(const msgpack::object& obj)
    {
        PackageType pkg;
        obj.convert(pkg);

        auto&& name = std::get<0>(pkg);
        auto&& args = std::get<1>(pkg);

        auto result = _adaptors.find(name);
        if (result != _adaptors.end())
        {
            result->second(static_cast<ServiceType*>(this), args);
        }
    }

    template <typename... Args>
    void CallRemote(const std::string& name, Args ... args)
    {
        auto pkg = std::make_tuple(name, std::make_tuple(args...));
        auto buf = std::make_shared<msgpack::sbuffer>();
        msgpack::pack(*buf, pkg);
        _write_strand->dispatch(MEM_FN(DoCallRemote, buf));
    }

    virtual void OnConnected()
    {
        
    }

    void SetSocketConnected()
    {
        int expected_status = STATUS_CONNECTING;
        if (_status.compare_exchange_strong(expected_status, STATUS_CONNECTED))
        {
            logger()->info("SetSocketConnected: succeeded");
            OnConnected();
            TriggerReceive();
            TriggerSend();
        }
        else
        {
            logger()->error("SetSocketConnected: last status is not expected! {}", expected_status);
        }
    }

    void TriggerReceive()
    {
        logger()->info("TriggerReceive");
        TryReadSome();
    }

    void TriggerSend()
    {
        logger()->info("TriggerSend");
    }

    void TryReadSome()
    {
        _unpacker.reserve_buffer(MAX_READ_SIZE_PER_TIME);
        _socket->async_read_some(asio::buffer(_unpacker.buffer(), MAX_READ_SIZE_PER_TIME), MEM_FN(OnReadSome, _1, _2));
    }

    void OnReadSome(const asio::error_code& ec, std::size_t transferred_size)
    {
        if (ec)
        {
            logger()->error("OnReadSome: Failed! reason: {}", ec.message());
            Close();
            return;
        }

        _unpacker.buffer_consumed(transferred_size);
        msgpack::unpacked result;
        while (_unpacker.next(result))
        {
            Dispatch(result.get());
        }

        if (_status == STATUS_CONNECTED)
        {
            TryReadSome();
        }
    }

    void DoCallRemote(std::shared_ptr<msgpack::sbuffer> buf)
    {
        _socket->async_write_some(asio::buffer(buf->data(), buf->size()), _write_strand->wrap(MEM_FN(OnWriteSome, _1, _2)));
        _write_queue.push_back(std::move(*buf));
    }

    void OnWriteSome(const asio::error_code& ec, std::size_t transferred_size)
    {
        if (ec)
        {
            return;
        }
        _bytes_written += transferred_size;

        if (!_write_queue.empty())
        {
            auto front_buf_size = _write_queue.front().size();
            if (_bytes_written >= front_buf_size)
            {
                _write_queue.pop_front();
                _bytes_written -= front_buf_size;
            }
        }
    }

    int status() const { return _status; }
    void set_status(int status) { _status = status; }

    asio::ip::tcp::socket& socket() const { return *_socket; }

private:
    std::shared_ptr<asio::ip::tcp::socket> _socket;
    std::shared_ptr<asio::strand> _write_strand;
    std::atomic_int _status;
    msgpack::unpacker _unpacker;
    std::deque<msgpack::sbuffer> _write_queue;
    size_t _bytes_written;
};

template <typename ServiceType>
std::unordered_map<std::string, typename RpcConnection<ServiceType>::AdaptorType>
RpcConnection<ServiceType>::_adaptors;
} // namespace hpt
#endif //HEPTAPOD_RPC_CONNECTION_H
