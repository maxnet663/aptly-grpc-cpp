#include <iostream>
#include "repositoryClient.h"

namespace aptly::client {
    template <typename RequestType, typename ResponseType, typename Func>
    grpc::Status makeRequest(std::unique_ptr<proto::RepositoryService::Stub>& stub, Func func, const RequestType& request, ResponseType& response) {
        grpc::ClientContext context;
        return (stub.get()->*func)(&context, request, &response);
    }

    RepositoryClient::RepositoryClient(std::string_view conn) : logger(std::cerr) {
        auto channel = grpc::CreateChannel(conn.data(), grpc::InsecureChannelCredentials());
        stub_ = RepositoryService::NewStub(channel);
    }

    bool RepositoryClient::createRepo(std::string_view repo_name) {
        Repository request;
        request.set_name(repo_name.data());
        proto::Response response;
        auto status = makeRequest<>(stub_, &proto::RepositoryService::Stub::CreateRepository, request, response);
        if (status.ok()) {
            logger << std::to_string(response.code()) << response.message() << std::endl
                   << "Comment: " << std::endl;
            return true;
        }
        logger << "gRPC failed. Status code: " << status.error_code()
               << ", message: " << status.error_message() << std::endl;
        return false;
    }

    int32_t RepositoryClient::addPackages(std::list<std::string> packages) {
        proto::PackagesAddDeleteRequest request;
        request.set_repositoryname("test1");
        for(auto &i : packages) {
            request.add_packagerefs(i);
        }
        proto::PackagesAddDeleteResponse response;
        auto status = makeRequest<>(stub_, &proto::RepositoryService::Stub::AddPackages, request, response);
        if (status.ok()) {
            logger << std::string("Repository Name: ") << response.repository().name() << std::endl
                   << "Comment: " << response.repository().comment() << std::endl;
            logger << response.response().message() << std::endl;
            return response.backgrounddetails().task().id();
        }
        logger << "gRPC failed. Status code: " << status.error_code()
               << ", message: " << status.error_message() << std::endl;
        return response.backgrounddetails().task().id();
    }

    bool RepositoryClient::getRepo(std::string_view repo_name) {
        RepositoryRequest request;
        request.set_name(repo_name.data());
        proto::RepositoryResponse response;
        auto status = makeRequest<>(stub_, &proto::RepositoryService::Stub::GetRepository, request, response);
        if (status.ok()) {
            logger << response.repository().name() << std::endl
                   << "Comment: " << std::endl;
            return true;
        }
        logger << "gRPC failed. Status code: " << status.error_code()
               << ", message: " << status.error_message() << std::endl;
        return false;
    }

    bool RepositoryClient::editRepo(std::string_view repo_name) {
        Repository request;
        request.set_name(repo_name.data());
        proto::Response response;
        auto status = makeRequest<>(stub_, &proto::RepositoryService::Stub::CreateRepository, request, response);
        if (status.ok()) {
            logger << std::to_string(response.code()) << response.message() << std::endl;
            return true;
        }
        logger << "gRPC failed. Status code: " << status.error_code()
               << ", message: " << status.error_message() << std::endl;
        return false;
    }

    bool RepositoryClient::dropRepo(std::string_view repo_name) {
        proto::DropRepositoryRequest request;
        request.set_name(repo_name.data());
        proto::DropRepositoryResponse response;
        auto status = makeRequest<>(stub_, &proto::RepositoryService::Stub::DropRepository, request, response);
        if (status.ok()) {
            logger << std::to_string(response.response().code()) << response.response().message() << std::endl;
            return true;
        }
        logger << "gRPC failed. Status code: " << status.error_code()
               << ", message: " << status.error_message() << std::endl;
        return false;
    }
}