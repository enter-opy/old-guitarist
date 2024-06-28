<h1>Old Guitarist</h2>

<p align="center"><img src="res/Screenshot.png" width="300"></p>

<br>

<p>
    Old guitarist is a physically modeled virtual guitar plugin.
</p>

## TODOs
- [x] Wave equation
- [ ] Stability condition
- [ ] Multiple Strings
- [x] IR Convolution

## Installation
### VST3 Installation (Windows)

1. **Download the Plugin:** Download old.guitarist.vst3 from [releases](https://github.com/enter-opy/old-guitarist/releases).

2. **Install the Plugin:**
   - **Copy the file into your DAW's plugin directory.**
   - **Rescan Plugins:** Follow your DAW's instructions to rescan plugins or manually add the plugin if necessary.

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