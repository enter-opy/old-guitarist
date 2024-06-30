<h1>Old Guitarist</h2>

<p align="center"><img src="res/Screenshot.png" width="300"></p>

<br>

<p>
    Old guitarist is a physically modeled virtual guitar plugin.
</p>

## Features
- [x] Wave equation
- [ ] Stability condition
- [ ] Multiple Strings
- [x] IR Convolution
- [ ] Time-varying tension factor
- [ ] Hammer-ons
- [ ] Slides

## Physical Model

### Nomenclature

- $\gamma$: damping factor
- $\mu$: linear density
- $E$: Young's modulus
- $h$: Impulse response
- $I$: second moment of area ($\frac{\pi r^4}{4}$ for a cylinder)
- $l$: length
- $T$: tension
- $t$: time
- $x$: position

### Wave equation
Wave equation for a guitar string is given as:

$$
\begin{align*}
\frac{\partial^2 y}{\partial x^2} - \frac{\mu}{T(t)} \frac{\partial^2 y}{\partial t^2} - \gamma \frac{\partial y}{\partial t} - EI \frac{\partial^4 y}{\partial x^4} = 0
\end{align*}
$$

### Initial and Boundary Condition

$$
\begin{align*}
y(x, 0) =
\begin{cases} 
\frac{4x}{l} & \text{for } 0 \leq x < \frac{l}{4} \\
\frac{4 (l - x)}{3l} & \text{for } \frac{l}{4} \leq x \leq l
\end{cases}
\end{align*}
$$

$$
\begin{align*}
y(0, t) = y(l, t) = 0
\end{align*}
$$

### Finite Difference
Consider the expression given by

$$
\begin{align*}
f' = \lim_{{h \to 0}} \frac{{f(x + h) - f(x)}}{h}
\end{align*}
$$

&nbsp;&nbsp;&nbsp;&nbsp;In the context of the finite difference method, $h$ is a finite interval, and the difference quotient is used to approximate the derivative. Instead of taking the limit as $h$ approaches zero, a small but finite value of $h$ is chosen.

&nbsp;&nbsp;&nbsp;&nbsp;The wave equation is discretized to obtain the following form with `Spatial resolution`$= \frac{1}{\Delta x}$ and `Spatial resolution` $= \frac{1}{\Delta x}$:

$$
\begin{align*}
\frac{y_{x+1}^{t} - 2y_{x}^{t} + y_{x-1}^{t}}{\Delta x^2} - \frac{\mu}{T(t)}\frac{y_{x}^{t+1} - 2y_{x}^{t} + y_{x}^{t-1}}{\Delta t^2}- \gamma \frac{y_{x}^{t+1} - y_{x}^{t-1}}{2 \Delta t} - EI \frac{y_{x-2}^{t} -4y_{x-1}^{t} + 4y_{x}^{t} - 4y_{x+1}^{t} + y_{x+2}^{t}}{\Delta x^4} = 0 
\end{align*}
$$

### Stability condition
The stability analysis of finite difference schemes when applied to the numerical solution of partial differential equations is intricately tied to the Courant–Friedrichs–Lewy (CFL) condition, expressed as:

$$
\begin{align*}
C = \sqrt{\frac{\mu}{T}} \frac{\Delta x}{\Delta t} \leq C_{\text{max}}
\end{align*}
$$

$$
\text{Spatial Resolution} \leq \text{Temporal Resolution} \times \sqrt{\frac{\mu}{T}}
$$

### Convolution with Impulse Response

&nbsp;&nbsp;&nbsp;&nbsp; $y$ is sampled at each time step $t$ at $i=l/2$. An impulse response of a guitar body is convoluted with $y$ to introduce body resonance.

**Impulse response:** [Source](https://ccrma.stanford.edu/~jiffer8/420/project.html)

&nbsp;&nbsp;&nbsp;&nbsp;Instead of multiplication ($YH$) in frequency domain, same result can be achieved by convolution ($y*h$) in time domain. The formula for convolution is given as:

$$
(y * h)(t) = \int_{-\infty}^{\infty} x(\tau) h(t - \tau) \, d\tau
$$

## Installation

### Build from Source
To build Old guitarist from source:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/enter-opy/old-guitarist.git
   cd old-guitarist
2. **Install Dependencies:**
   - **Windows:** Make sure you have Visual Studio installed with the necessary components for C++ development.
   - **Mac:** Make sure you have Xcode installed with the command line tools.

3. **Build the Plugin:**

   **Windows:**
   - Open the project in Visual Studio.
   - Set the build configuration to `Release`.
   - Build the project by selecting `Build > Build Solution`.

   **Mac:**
   - Open the project in Xcode.
   - Set the scheme to `Release`.
   - Build the project by selecting `Product > Build`.
## Usage
- **Insert Plugin:** Load Old guitarist plugin into your preferred digital audio workstation (DAW).
- **Play the instrument:** Use a MIDI keyboard or your DAW's pianoroll to play the instrument.

## Contributing
Contributions to Old guitarist are welcome! If you'd like to contribute, follow these steps:
1. **Fork the Repository:** Start by forking the [Old guitarist repository](https://github.com/enter-opy/old-guitarist).
2. **Make Changes:** Create a new branch, make your changes, and commit them to your branch.
3. **Create a Pull Request:** Push your changes to your fork and submit a pull request to the original repository.
## License
This project is licensed under the GNU General Public License. See the [LICENSE](https://github.com/enter-opy/old-guitarist/blob/main/LICENSE) for details.

<h2><u>References</u></h2>

- Ka-Wing Ho, Yiu Ling & Chuck-jee Chau (2021). [Guitar Virtual Instrument using Physical Modelling with Collision Simulation](https://www.researchgate.net/publication/346562874_Guitar_Virtual_Instrument_using_Physical_Modelling_with_Collision_Simulation).</a><br>
- M. Shuppius (2017). [Physical modelling of guitar strings](https://www.youtube.com/watch?v=sxt5rxF_PdI).
</ul>