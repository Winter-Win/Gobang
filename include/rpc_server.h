#ifndef REST_RPC_RPC_SERVER_H_
#define REST_RPC_RPC_SERVER_H_

#include <thread>
#include <mutex>
#include "connection.h"
#include "io_service_pool.h"
#include "router.h"

using boost::asio::ip::tcp;

namespace rest_rpc {
namespace rpc_service {
class rpc_server : private boost::noncopyable {
 public:
  rpc_server(short port, size_t size, size_t timeout_seconds = 15, size_t check_seconds = 10)
      : io_service_pool_(size),
        acceptor_(io_service_pool_.get_io_service(), tcp::endpoint(tcp::v4(), port)),
        timeout_seconds_(timeout_seconds),
        check_seconds_(check_seconds) {
    router::get().set_callback(std::bind(&rpc_server::callback, this, std::placeholders::_1,
                                         std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    do_accept();
    check_thread_ = std::make_shared<std::thread>([this] { clean(); });
  }

  ~rpc_server() {
	stop_check_ = true;
	check_thread_->join();
    io_service_pool_.stop();
    thd_->join();
  }

  void run() {
    thd_ = std::make_shared<std::thread>([this] { io_service_pool_.run(); });
  }

  template<ExecMode model = ExecMode::sync, typename Function>
  void register_handler(std::string const& name, const Function& f) {
    router::get().register_handler<model>(name, f);
  }

  template<ExecMode model = ExecMode::sync, typename Function, typename Self>
  void register_handler(std::string const& name, const Function& f, Self* self) {
    router::get().register_handler<model>(name, f, self);
  }

  void response(int64_t conn_id, std::string&& result) {
    std::unique_lock<std::mutex> lock(mtx_);
    auto it = connections_.find(conn_id);
    if (it != connections_.end()) { it->second->response(std::move(result)); }
  }

 private:
  void do_accept() {
    conn_.reset(new connection(io_service_pool_.get_io_service(), timeout_seconds_));
    acceptor_.async_accept(conn_->socket(), [this](boost::system::error_code ec) {
      if (ec) {
        //LOG(INFO) << "acceptor error: " << ec.message();
      } else {
        conn_->start();
        std::unique_lock<std::mutex> lock(mtx_);
        conn_->set_conn_id(conn_id_);
        connections_.emplace(conn_id_++, conn_);
      }

      do_accept();
    });
  }

  void clean() {
    while (!stop_check_) {
      std::this_thread::sleep_for(std::chrono::seconds(check_seconds_));

      std::unique_lock<std::mutex> lock(mtx_);
      for (auto it = connections_.cbegin(); it != connections_.cend();) {
        if (it->second->has_closed()) {
          it = connections_.erase(it);
        } else {
          ++it;
        }
      }
    }
  }

  void callback(const std::string& topic, std::string&& result, connection* conn,
                bool has_error = false) {
    response(conn->conn_id(), std::move(result));
  }

  io_service_pool io_service_pool_;
  tcp::acceptor acceptor_;
  std::shared_ptr<connection> conn_;
  std::shared_ptr<std::thread> thd_;
  std::size_t timeout_seconds_;

  std::unordered_map<int64_t, std::shared_ptr<connection>> connections_;
  int64_t conn_id_ = 0;
  std::mutex mtx_;
  std::shared_ptr<std::thread> check_thread_;
  size_t check_seconds_;
  bool stop_check_ = false;
};
}  // namespace rpc_service
}  // namespace rest_rpc

#endif  // REST_RPC_RPC_SERVER_H_