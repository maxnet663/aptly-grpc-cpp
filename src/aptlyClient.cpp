#include "aptlyClient.h"

#include <memory>
#include <utility>
#include "repositoryClient.h"
#include "filesClient.h"

namespace aptly::client {
    AptlyClient::AptlyClient(std::string &conn) {
        repositoryClient = std::make_unique<RepositoryClient>(conn);
        filesClient = std::make_unique<FilesClient>(conn);
    }

    bool AptlyClient::createRepo(std::string_view repo_name) {
        return repositoryClient->createRepo(repo_name);
    }

    bool AptlyClient::getRepo(std::string_view repo_name) {
        return repositoryClient->getRepo(repo_name);
    }

    bool AptlyClient::editRepo(std::string_view repo_name) {
        return repositoryClient->editRepo(repo_name);
    }

    bool AptlyClient::dropRepo(std::string_view repo_name) {
        return repositoryClient->dropRepo(repo_name);
    }

    int32_t AptlyClient::addPackages(std::list<std::string> packages) {
        return repositoryClient->addPackages(std::move(packages));
    }

    std::list<std::string> AptlyClient::ListDirectories() {
        return filesClient->ListDirectories();
    }

    bool AptlyClient::UploadFile(std::string_view filename, std::string_view destination) {
        return filesClient->UploadFile(filename, destination);
    }

    std::list<std::string> AptlyClient::ListFiles(std::string_view dirname) {
        return filesClient->ListFiles(dirname);
    }

    bool AptlyClient::DeleteDirectory(std::string_view dirname) {
        return filesClient->DeleteDirectory(dirname);
    }

    bool AptlyClient::DeleteFile(std::string_view filename, std::string_view dirname) {
        return filesClient->DeleteFile(filename, dirname);
    }
}

