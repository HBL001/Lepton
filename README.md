# Lepton HDMI C++ App for Verdin i.MX8M Plus

This project provides a cross-compiled C++ application for visualizing thermal data using SDL2 and OpenGL ES on the Verdin i.MX8M Plus (TorizonCore 6.8.3). It is designed for developers working on host Linux/WSL2 environments with Docker and Visual Studio Code.

---

## 🚀 Project Overview

- Language: C++
- Graphics: SDL2 + OpenGL ES (no Qt)
- Target: Toradex Verdin i.MX8M Plus running TorizonCore
- Build Host: Docker-based Dev Container using `clotspot/verdin-dev:6.8.3-sdl2`

---

## 🧱 Prerequisites

Make sure you have the following tools installed:

- [Docker](https://docs.docker.com/get-docker/)
- [Visual Studio Code](https://code.visualstudio.com/)
- [Remote - Containers Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)
- A WSL2 distro (Ubuntu recommended, not root-based)
- Torizon Swiss-style Linux ARM64-AI in a Docker Container (see imx8x Plus repo) already made in:
    "clotspot/verdin-dev:6.8.3-sdl2"

---

## 🧭 Quick Start Instructions

### ✅ 1. On your host system:

```bash
mkdir ~duoSight
cd ~duoSight
```

### ✅ 2. Create Dev Container Configuration

Then create this file: .devcontainer/devcontainer.json

```json
{
  "name": "Lepton HDMI C++ App (i.MX8)",
  "image": "clotspot/verdin-dev:6.8.3-sdl2",
  "remoteUser": "torizon",
  "workspaceFolder": "/home/torizon",
  "customizations": {
    "vscode": {
      "settings": {
        "terminal.integrated.defaultProfile.linux": "bash",
        "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
      },
      "extensions": [
        "ms-vscode.cpptools",
        "ms-vscode.cmake-tools"
      ]
    }
  }
}
```

This ensures the container launches in /home/torizon and lets you clone your repo manually inside the container.

####  Custom cross-compilation image hosted on Docker Hub:

```
clotspot/verdin-dev:6.8.3-sdl2
```

It includes these, ready to work:

✅ aarch64 GCC cross-compiler

✅ SDL2 + OpenGL ES + EGL (ARM64 headers/libs)

✅ CMake, g++, rsync, git

✅ Preconfigured for TorizonCore 6.8.3 targets

---

### ✅  3. Open in VS Code and Rebuild Container

From your host terminal:

```
code .
```

When prompted:
"Folder contains a Dev Container..." → Click Reopen in Container

This will download and launch the clotspot/verdin-dev:6.8.3-sdl2 image.

### ✅  4. Clone the Repo Inside the Container

```
cd ~
git clone https://github.com/HBL001/Lepton.git lepton
cd lepton
```
Now you're working in /home/torizon/lepton with correct user ownership and no permission issues.

### ✅  5. Build the Project

```
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../imx8.cmake
make -j$(nproc)
```
The output binary will be built for the Verdin iMX8M Plus (ARM64 Linux).


### ✅  6. Push up the Project to the Docker Repo

Ready to put into the Docker Repo, and deployed on the target board












