
# Solar system
This project demonstrates Earth orbiting around the Sun using OpenGL. It also features a background image and utilizes Phong shading with texture mapping for realistic lighting and material effects.

**Build Preview (Tap on screen to view demo):**

[![Solar-system build](https://img.youtube.com/vi/oHW6O9A2qII/maxresdefault.jpg)](https://youtu.be/oHW6O9A2qII)

- **Preview of the initial stage build:** [Earth & Sun Orbitation](https://youtu.be/cR0vXMFc648)

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
- C key - move downward
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
   git clone https://github.com/JS-codev/OpenGL.git
   ```

2. **Navigate to the root folder:**
   ```bash
    cd "Solar system"
   ```
3. **Compile and Run the Code:**
- Press `F1` (in VS Code) to run the code using the [code runner extension](https://marketplace.visualstudio.com/items?itemName=formulahendry.code-runner).
- Alternatively, execute the following command in the `Git Bash` terminal:

   ```bash
    g++ -g -std=c++17 -Iinclude -Linclude/lib src/glad.c src/window.cpp src/main.cpp -lglfw3dll -lopengl32 -o build/run.exe && build/run.exe
   ```
---

## Explanation:
- **Counter-clockwise rotation:** Mercury, Earth, Mars, Jupiter, Saturn and Neptune.
- **Clockwise rotation:** Venus and Uranus.

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

--

#### To calculate planet's scale:
The idea is to scale each planet according to its diameter ratio relative to Earth, and then multiply that by the scale you've used for Earth (i.e., 0.75f). The formula for each planet will be:

**Planet Scale = Earth Scale × Planet Diameter Ratio**

Given our Earth is `0.75f` size:

| Planet      | Diameter Ratio (to Earth) | Calculated Scale         |
| ----------- | ------------------------- | ------------------------ |
| **Mercury** | 0.382                     | `0.75 * 0.382 ≈ 0.287f`  |
| **Venus**   | 0.949                     | `0.75 * 0.949 ≈ 0.712f`  |
| **Earth**   | 1.000                     | `0.75f`   |
| **Mars**    | 0.531                     | `0.75 * 0.531 ≈ 0.398f`  |
| **Jupiter** | 10.947                    | `0.75 * 10.947 ≈ 8.210f` |
| **Saturn**  | 9.126                     | `0.75 * 9.126 ≈ 6.844f`  |
| **Uranus**  | 3.981                     | `0.75 * 3.981 ≈ 2.986f`  |
| **Neptune** | 3.868                     | `0.75 * 3.868 ≈ 2.901f`  |

--

#### The chronological order, distance and axial tilt of each planets:

| **Planet**  | **Distance from Sun (AU)** | **Axial Tilt**              |
| ----------- | -------------------------- | --------------------------- |
| **Sun**     | 0.00 AU                    | 7.25°          |
| **Mercury** | 0.39 AU                    | 0.034°                      |
| **Venus**   | 0.72 AU                    | 177.4° (retrograde)         |
| **Earth**   | 1.00 AU                    | 23.5°                       |
| **Mars**    | 1.52 AU                    | 25.2°                       |
| **Jupiter** | 5.20 AU                    | 3.13°                       |
| **Saturn**  | 9.58 AU                    | 26.7°                       |
| **Uranus**  | 19.18 AU                   | 97.77° (retrograde)         |
| **Neptune** | 30.07 AU                   | 28.3°                       |

