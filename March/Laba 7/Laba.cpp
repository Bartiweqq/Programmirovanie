#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

using namespace std;

// Функция копирования файла
void copyFile(const string& src, const string& dst) {
    int srcFd = open(src.c_str(), O_RDONLY);
    if (srcFd == -1) {
        cerr << "Failed to open source file: " << src << " Error: " << strerror(errno) << endl;
        return;
    }

    int dstFd = open(dst.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (dstFd == -1) {
        cerr << "Failed to open destination file: " << dst << " Error: " << strerror(errno) << endl;
        close(srcFd);
        return;
    }

    char buffer[4096];
    ssize_t bytesRead, bytesWritten;
    while ((bytesRead = read(srcFd, buffer, sizeof(buffer))) > 0) {
        char* outPtr = buffer;
        do {
            bytesWritten = write(dstFd, outPtr, bytesRead);
            if (bytesWritten >= 0) {
                bytesRead -= bytesWritten;
                outPtr += bytesWritten;
            } else if (errno != EINTR) {
                cerr << "Write error: " << strerror(errno) << endl;
                close(srcFd);
                close(dstFd);
                return;
            }
        } while (bytesRead > 0);
    }

    if (bytesRead == -1) {
        cerr << "Read error: " << strerror(errno) << endl;
    }

    close(srcFd);
    close(dstFd);
}

// Функция для обработки подкаталогов
void copyDirectory(const string& srcDir, const string& dstDir);

void processEntry(const string& srcDir, const string& dstDir, const dirent* entry) {
    string srcPath = srcDir + "/" + entry->d_name;
    string dstPath = dstDir + "/" + entry->d_name;

    struct stat statbuf;
    if (stat(srcPath.c_str(), &statbuf) == -1) {
        cerr << "Failed to stat " << srcPath << " Error: " << strerror(errno) << endl;
        return;
    }

    if (S_ISREG(statbuf.st_mode)) {
        thread(copyFile, srcPath, dstPath).detach();
    } else if (S_ISDIR(statbuf.st_mode)) {
        if (mkdir(dstPath.c_str(), statbuf.st_mode) == -1 && errno != EEXIST) {
            cerr << "Failed to create directory " << dstPath << " Error: " << strerror(errno) << endl;
            return;
        }
        thread(copyDirectory, srcPath, dstPath).detach();
    }
}

// Функция копирования подкаталогов
void copyDirectory(const string& srcDir, const string& dstDir) {
    DIR* dirp = opendir(srcDir.c_str());
    if (!dirp) {
        cerr << "Failed to open directory: " << srcDir << " Error: " << strerror(errno) << endl;
        return;
    }

    struct dirent* entry;
    struct dirent* buf;
    size_t bufSize = sizeof(struct dirent) + pathconf(srcDir.c_str(), _PC_NAME_MAX) + 1;
    buf = (struct dirent*) malloc(bufSize);

    while (true) {
        int result = readdir_r(dirp, buf, &entry);
        if (result != 0) {
            cerr << "Error reading directory: " << srcDir << " Error: " << strerror(errno) << endl;
            break;
        }

        if (entry == nullptr) {
            break; // End of directory
        }

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip special entries
        }

        processEntry(srcDir, dstDir, entry);
    }

    free(buf);
    closedir(dirp);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <source_directory> <destination_directory>" << endl;
        return 1;
    }

    string srcDir = argv[1];
    string dstDir = argv[2];

    struct stat statbuf;
    if (stat(srcDir.c_str(), &statbuf) == -1) {
        cerr << "Source directory does not exist: " << srcDir << " Error: " << strerror(errno) << endl;
        return 1;
    }

    if (!S_ISDIR(statbuf.st_mode)) {
        cerr << "Source is not a directory: " << srcDir << endl;
        return 1;
    }

    if (mkdir(dstDir.c_str(), statbuf.st_mode) == -1 && errno != EEXIST) {
        cerr << "Failed to create destination directory: " << dstDir << " Error: " << strerror(errno) << endl;
        return 1;
    }

    copyDirectory(srcDir, dstDir);

    return 0;
}
