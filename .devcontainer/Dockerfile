FROM mcr.microsoft.com/vscode/devcontainers/universal:2-linux

# Install needed packages
RUN apt update -y && apt install -y libgtest-dev cmake

# Install GTest

RUN cd /usr/src/googletest && \
    cmake . && \
    cmake --build . --target install