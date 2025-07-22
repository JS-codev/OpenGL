
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
---

### Explanation:

#### To calcualte each planets' speed:

Angular velocity (angular speed) is the rate at which an object moves around a circle (or orbital path).

We can compute it using the formula:

   **Angular velocity = speed / radius.**
 
Where:

- 𝜔 is the angular velocity in radians per second.

- 𝑣 is the orbital speed of the planet in km/s.

- 𝑟 is the radius (distance from the Sun) in AU (astronomical units).

**Results:**
| Planet      | Distance from Sun (AU) | Orbital Speed (km/s) | Angular Velocity (rad/s)                          |
| ----------- | ---------------------- | -------------------- | ------------------------------------------------- |
| **Mercury** | 0.39                   | 47.87                | $\omega = \frac{47.87}{0.39} \approx 122.8$ rad/s |
| **Venus**   | 0.72                   | 35.02                | $\omega = \frac{35.02}{0.72} \approx 48.6$ rad/s  |
| **Earth**   | 1.00                   | 29.78                | $\omega = \frac{29.78}{1.00} \approx 29.78$ rad/s |
| **Mars**    | 1.52                   | 24.07                | $\omega = \frac{24.07}{1.52} \approx 15.8$ rad/s  |
| **Jupiter** | 5.20                   | 13.07                | $\omega = \frac{13.07}{5.20} \approx 2.51$ rad/s  |
| **Saturn**  | 9.58                   | 9.69                 | $\omega = \frac{9.69}{9.58} \approx 1.01$ rad/s   |
| **Uranus**  | 19.19                  | 6.81                 | $\omega = \frac{6.81}{19.19} \approx 0.355$ rad/s |
| **Neptune** | 30.07                  | 5.43                 | $\omega = \frac{5.43}{30.07} \approx 0.181$ rad/s |
