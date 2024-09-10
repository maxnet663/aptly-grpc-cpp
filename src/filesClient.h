#ifndef APTLY_GRPC_LIB_FILESCLIENT_H
#define APTLY_GRPC_LIB_FILESCLIENT_H

#include <string_view>
#include "service.grpc.pb.h"
namespace aptly::client {
    class FilesClient {
    public:
        FilesClient(std::string_view conn, std::ostream &logger = std::cerr);
        bool UploadFile(std::string_view filename, std::string_view destination);
        std::list<std::string> ListDirectories();
        std::list<std::string> ListFiles(std::string_view dirname);
        bool DeleteDirectory(std::string_view dirname);
        bool DeleteFile(std::string_view filename, std::string_view dirname);


    private:
        std::unique_ptr<proto::FilesService::Stub> stub_;
        static const uint16_t buf_size{1024};
        std::ostream &logger;

        bool
        sendData(proto::FileChunk &request, std::ifstream &file, std::array<char, buf_size> &buffer,
                 std::unique_ptr<grpc::ClientReaderWriter<proto::FileChunk, proto::Response>> &writer);

    };
}

#endif //APTLY_GRPC_LIB_FILESCLIENT_H
