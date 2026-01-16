#TO-DO:
    # Change lines 19-24 for readability. Generally I prefer verbosity for the sake of readability rather than complexity for the sake of conciseness.
ARG USERNAME=idf
ARG HOST_UID=1000
ARG HOST_GID=1000

FROM espressif/idf:v5.5.2

# Pass arguments into the build stage
ARG USERNAME
ARG HOST_UID
ARG HOST_GID

# Install dependencies and create the user that matches the host
RUN apt-get update && \
    # Install clangd version 20 specifically
    apt-get install -y wget gnupg software-properties-common && \
    wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc && \
    add-apt-repository -y "deb http://apt.llvm.org/noble/ llvm-toolchain-noble-20 main" && \
    apt-get update && \
    apt-get install -y clangd-20 && \
    # Create symlink to make clangd-20 the default clangd
    update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-20 100 && \
    # Check if target GID or UID is already taken and change the name if it is
    (getent group ${USERNAME} ||\ 
        (getent group ${HOST_GID} && groupmod -n ${USERNAME} $(getent group ${HOST_GID} | cut -d: -f1)) || \
        groupadd --gid ${HOST_GID} ${USERNAME}) && \
    (getent passwd ${USERNAME} || \
        (getent passwd ${HOST_UID} && usermod -l ${USERNAME} -d /home/${USERNAME} -m $(getent passwd ${HOST_UID} | cut -d: -f1)) || \
        useradd --uid ${HOST_UID} --gid ${HOST_GID} --create-home --shell /bin/bash ${USERNAME}) && \
    #Clean up apt cache to keep image small
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Switch to newly created non-root user
USER ${USERNAME}