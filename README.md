
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
```


## Roadmap

- Adding other protection techniques
- Better, more intuitive CLI
- Autocompletion tool
## Authors

- [@stefanlft](https://www.github.com/stefanlft)

