
# Solar system
This project demonstrates Earth orbiting around the Sun using OpenGL. It also features a background image and utilizes Phong shading with texture mapping for realistic lighting and material effects.

- Click on this image for video preview:
[![Solar System](https://img.youtube.com/vi/cR0vXMFc648/maxresdefault.jpg)](https://youtu.be/cR0vXMFc648)

- Features:
   - Planet orbit simulation with correct orbital mechanics for Earth around the Sun.
   - Background image to simulate the space environment, giving the scene depth and immersion.
   - Diffuse maps for base color
   - Specular maps for shininess control
   - Material struct to define light interaction
   - Camera and light movement for dynamic scenes

#### Controls

- WASD Movement
- Hold Shift - sprint
- Left Alt - move downward
- Space - move upward
- Mouse Movement — Look around
- ESC - Exit the program

---

## How to Run This Project:

This project was built using OpenGL with the following libraries:
- **GLEW** — OpenGL Extension Wrangler
- **GLFW** — Window and input handling
- **GLM**  — OpenGL Mathematics

To get started with the project, follow these steps:

1. **Clone this repository** to your local machine:
   ```bash
   git clone <repository_u_rl>
   ```

2. **Navigate to the root folder:**
   ```bash
    cd path/to/root/folder
   ```
3. **Compile and Run the Code:**
- Press `F1` (in VS Code) to run the code using the [code runner extension](https://marketplace.visualstudio.com/items?itemName=formulahendry.code-runner).
- Alternatively, run the following command:

   ```bash
    g++ -g -std=c++17 -Iinclude -Linclude/lib src/glad.c src/window.cpp src/main.cpp -lglfw3dll -lopengl32 -o build/run.exe && build/run.exe
   ```
   