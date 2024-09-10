#ifndef APTLY_GRPC_CPP_APTLY_GRPC_H
#define APTLY_GRPC_CPP_APTLY_GRPC_H

#include <memory>
#include <string>
#include <string_view>
#include <iosfwd>

#include "service.pb.h"
#include "service.grpc.pb.h"
#include <grpcpp/grpcpp.h>

namespace aptly::client {
    using grpc::Channel;
    using grpc::ClientContext;
    using grpc::Status;
    //services
    using proto::RepositoryService;
    //requests
    using proto::RepositoryRequest;
    //responses
    using proto::Repository;
    class RepositoryClient {
    public:
        RepositoryClient(std::string_view conn);

        bool createRepo(std::string_view repo_name);
        bool getRepo(std::string_view repo_name);
        bool editRepo(std::string_view repo_name);
        bool dropRepo(std::string_view repo_name);
        int32_t addPackages(std::list<std::string> packages);

    private:
        std::unique_ptr<RepositoryService::Stub> stub_;
        std::ostream &logger;
    };
}
#endif //APTLY_GRPC_CPP_APTLY_GRPC_H
