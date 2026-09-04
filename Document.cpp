#include "Document.hpp"

#include <fstream>
#include <sstream>

Document::Document(std::string title, std::string contents)
    : title_(std::move(title)), sourcePath_(), contents_(std::move(contents)) {
}

bool Document::operator==(const Document& other) const {
    return title_ == other.title_ &&
           sourcePath_ == other.sourcePath_ &&
           contents_ == other.contents_;
}

bool Document::operator!=(const Document& other) const {
    return !(*this == other);
}

bool Document::load(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    if (!file.good() && !file.eof()) {
        return false;
    }

    // extract  final filename component from the path (handles '/' and '\\')
    std::size_t lastSlash = path.find_last_of("/\\");
    std::string filename =
        (lastSlash == std::string::npos) ? path : path.substr(lastSlash + 1);

    sourcePath_ = path;
    title_ = filename;
    contents_ = buffer.str();
    return true;
}

const std::string& Document::title() const noexcept {
    return title_;
}

const std::string& Document::sourcePath() const noexcept {
    return sourcePath_;
}

const std::string& Document::contents() const noexcept {
    return contents_;
}

void Document::setTitle(std::string title) {
    title_ = std::move(title);
}

std::size_t Document::characterCount() const noexcept {
    return contents_.size();
}

bool Document::empty() const noexcept {
    return contents_.empty();
}

