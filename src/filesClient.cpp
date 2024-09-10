#include <grpcpp/security/credentials.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/client_context.h>
#include <fstream>
#include <grpcpp/support/status.h>
#include "filesClient.h"
namespace aptly::client {
    template <typename RequestType, typename ResponseType, typename Func>
    grpc::Status makeRequest(std::unique_ptr<proto::FilesService::Stub>& stub, Func func, const RequestType& request, ResponseType& response) {
        grpc::ClientContext context;
        return (stub.get()->*func)(&context, request, &response);
    }
    FilesClient::FilesClient(std::string_view conn, std::ostream &logger) : logger(logger) {
        auto channel = grpc::CreateChannel(conn.data(), grpc::InsecureChannelCredentials());
        stub_ = proto::FilesService::NewStub(channel);
    }

    bool FilesClient::UploadFile(std::string_view filename, std::string_view destination) {
        proto::FileChunk request;
        request.set_name(std::string(filename));
        request.set_directory(std::string(destination));
        std::ifstream file{filename.data(), std::ifstream::binary};
        std::array<char, buf_size> buffer{0};
        grpc::ClientContext context;

        auto writer{stub_->UploadFiles(&context)};
        if(!sendData(request, file, buffer, writer))
            return false;
        request.clear_name();
        request.clear_directory();
        while(!file.eof()) {
            if(!sendData(request, file, buffer, writer))
                return false;
        }
        writer->WritesDone();
        auto status = writer->Finish();
        if (status.ok()) {
            logger << "Finished transfer file '" << filename << "'" << std::endl;
            return true;
        }
        logger << "Failed transfer file '" << filename << "'" << std::endl;
        return false;
    }

    bool FilesClient::sendData(proto::FileChunk &request, std::ifstream &file, std::array<char, buf_size> &buffer,
                                std::unique_ptr<grpc::ClientReaderWriter<proto::FileChunk, proto::Response>> &writer) {
        file.read(buffer.data(), buffer.size());
        request.set_chunk(buffer.data(), file.gcount());
        return writer->Write(request);
    }

    std::list<std::string> FilesClient::ListDirectories() {
        proto::DirectoriesList response;
        auto status = makeRequest<proto::Empty>(stub_, &proto::FilesService::Stub::ListDirectories, {}, response);
    }

    std::list<std::string> FilesClient::ListFiles(std::string_view dirname) {
        proto::DirectoryName request;
        request.set_name(dirname.data());
        proto::FilesList response;
        auto status = makeRequest<>(stub_, &proto::FilesService::Stub::ListFiles, request, response);
    }

    bool FilesClient::DeleteDirectory(std::string_view dirname) {
        proto::DirectoryName request;
        request.set_name(dirname.data());
        proto::Response response;
        auto status = makeRequest<>(stub_, &proto::FilesService::Stub::DeleteDirectory, request, response);
    }

    bool FilesClient::DeleteFile(std::string_view filename, std::string_view dirname) {
        proto::DeleteFileRequest request;
        request.set_filename(filename.data());
        request.set_directory(dirname.data());
        proto::Response response;
        auto status = makeRequest<>(stub_, &proto::FilesService::Stub::DeleteFile, request, response);
    }
}