
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

- [ ]  Adding other protection techniques
  - [ ]  Color Manipulation
    - [x]  Color Shifting
    - [x]  WhiteBalance Shifting
    - [ ]  Contrast & Brightness Shifting
  - [x]  Gaussian Blur
  - [x]  Median Blur
  - [x]  Noise
  - [ ]  Lens Distorsion
  - [ ]  Histogram Equalization
  - [ ]  Texture Scrambling
  - [ ]  Change Compression Settings
  - [ ]  Resize and Crop
- [ ]  Better, more intuitive CLI
- [ ]  Autocompletion tool
## Note

Try avoiding over/under expousure, as it will now destroy parts of the photo
## Authors

- [@stefanlft](https://www.github.com/stefanlft)

