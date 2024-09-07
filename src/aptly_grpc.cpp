#include "aptly_grpc.h"

void RepositoryClient::GetRepository(const std::string &repo_name) {
    RepositoryRequest request;
    request.set_name(repo_name);

    aptly_grpc::RepositoryResponse response;
    ClientContext context;

    Status status = stub_->GetRepository(&context, request, &response);

    if (status.ok()) {
        std::cout << "Repository Name: " << response.repository().name() << std::endl;
        std::cout << "Comment: " << response.repository().comment() << std::endl;
    } else {
        std::cerr << "gRPC failed. Status code: " << status.error_code()
                  << ", message: " << status.error_message() << std::endl;
    }
}
