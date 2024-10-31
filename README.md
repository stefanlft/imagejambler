
# Image Jambler

An app that makes photos untraceable from the camera that took them without losing a lot of quality.



## Installation

Install my-project with npm

```bash
  git clone https://github.com/stefanlft/imagejambler.git
  cd imagejambler
  mkdir -p build
  cd build
  cmake ..
  make
```
    
## Usage/Examples

```bash
./ImageJambler photo.jpeg
# or
./ImageJambler photo.jpeg 70
# where 70 is the protection factor. keep in mind the greater the factor
# the worst the quality of the output file.
# if not provided, the protection factor will default to 30%
```


## Roadmap

- [x]  Adding other protection techniques
  - [x]  Color Manipulation
    - [x]  Color Shifting
    - [x]  WhiteBalance Shifting
    - [x]  Contrast & Brightness Shifting // sucks
  - [x]  Gaussian Blur
  - [x]  Median Blur
  - [x]  Noise
  - [x]  Histogram Equalization
  - [x]  Texture Scrambling
  - [x]  Change Compression Settings
  - [x]  Resize and Crop
- [ ]  Better, more intuitive CLI
- [ ]  Autocompletion tool
## Note

Try avoiding over/under expousure, as it will now destroy parts of the photo
## Authors

- [@stefanlft](https://www.github.com/stefanlft)

