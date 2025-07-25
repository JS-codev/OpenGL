
# Phong Lighting Model

The Phong Lighting Model is a widely-used shading technique in computer graphics. It consists of three main components:

**Build Preview (Tap on screen to view demo):**
[![Phong Lighting Model](https://img.youtube.com/vi/5p5zjIDg9Vw/maxresdefault.jpg)](https://youtu.be/5p5zjIDg9Vw)

- **Ambient Lighting**: Provides a constant level of light across the scene, simulating indirect light.
- **Diffuse Lighting**: Simulates the light scattered on rough surfaces, creating the base color.
- **Specular Lighting**: Adds highlights and reflections, simulating shiny surfaces.

#### Controls

- WASD Movement
- Hold Shift - sprint
- C key - move downward
- Space - move upward
- Mouse Movement — Look around
- ESC - Exit the program

---

## How to Run This Project:

This project was built using OpenGL with the following libraries:
- **GLEW**
- **GLFW**
- **GLM**

To get started with the project, follow these steps:

1. **Clone this repository** to your local machine:
   ```bash
   git clone https://github.com/JS-codev/OpenGL.git
   ```

2. **Navigate to the root folder:**
   ```bash
    cd "Phong lighting model"
   ```
3. **Compile and Run the Code:**
- Press `F1` (in VS Code) to run the code using the [code runner extension](https://marketplace.visualstudio.com/items?itemName=formulahendry.code-runner).
- Alternatively, run the following command in the `Git Bash` terminal:

   ```bash
    g++ -g -std=c++17 -Iinclude -Linclude/lib src/glad.c src/window.cpp src/main.cpp -lglfw3dll -lopengl32 -o build/run.exe && build/run.exe
   ```
   