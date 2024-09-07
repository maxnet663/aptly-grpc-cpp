#ifndef APTLY_GRPC_CPP_APTLY_GRPC_H
#define APTLY_GRPC_CPP_APTLY_GRPC_H

#include <memory>
#include <string>
#include <iostream>

#include "service.pb.h"
#include "service.grpc.pb.h"
#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
//services
using aptly_grpc::RepositoryService;
//requests
using aptly_grpc::RepositoryRequest;
//responses
using aptly_grpc::Repository;

class RepositoryClient {
public:
    RepositoryClient(std::shared_ptr<Channel> channel)
        : stub_(RepositoryService::NewStub(channel)) {}

    void GetRepository(const std::string& repo_name);

private:
    std::unique_ptr<RepositoryService::Stub> stub_;
};

#endif //APTLY_GRPC_CPP_APTLY_GRPC_H
