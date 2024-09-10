#ifndef APTLY_GRPC_LIB_APTLYCLIENT_H
#define APTLY_GRPC_LIB_APTLYCLIENT_H

#include <string>
#include <list>
#include <memory>
namespace aptly::client {
    class RepositoryClient;
    class FilesClient;

    class AptlyClient {
    public:
        AptlyClient(std::string &conn);
        //RepoService
        //ListRepositories
        bool createRepo(std::string_view repo_name);
        bool getRepo(std::string_view repo_name);
        bool editRepo(std::string_view repo_name);
        bool dropRepo(std::string_view repo_name);

        int32_t addPackages(std::list<std::string> packages);
        //DeletePackages
        //ImportPackages
        //IncludePackage

        //FilesService
        std::list<std::string> ListDirectories();
        bool UploadFile(std::string_view filename, std::string_view destination);
        std::list<std::string> ListFiles(std::string_view dirname);
        bool DeleteDirectory(std::string_view dirname);
        bool DeleteFile(std::string_view filename, std::string_view dirname);


    private:
        std::unique_ptr<RepositoryClient> repositoryClient;
        std::unique_ptr<FilesClient> filesClient;
    };
}

#endif //APTLY_GRPC_LIB_APTLYCLIENT_H
