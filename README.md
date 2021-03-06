# three-dim-util

[![Build Status](https://travis-ci.com/yuki-koyama/three-dim-util.svg?branch=master)](https://travis-ci.com/yuki-koyama/three-dim-util)

A utility library for prototyping 3D applications based on Qt, Eigen, and legacy OpenGL.

## Target Users

The target of this library is users who ...

- Use Qt for visualization and interaction
  - This library provides a template of Qt-based OpenGL widget with simple mouse interaction.
- Use Eigen for vector and matrix representation
  - This library has APIs for Eigen matrix and vector representation.
- Do not much care about compute efficiency
  - This library is NOT optimized for efficiency and sometimes favors simplicity.
- Do not care about using legacy OpenGL APIs
  - This library relies on legacy OpenGL APIs that were declared deprecated.

## Requirements

- Eigen 3 <http://eigen.tuxfamily.org/>
- Qt 5 <https://www.qt.io/>
- OpenGL 2.1 (or compatible with 2.1)

Ubuntu 18.04 LTS:
```
sudo apt install libeigen3-dev qt5-default
```

maxOS:
```
brew install eigen qt
```

## Projects using three-dim-util

- bvh11 <https://github.com/yuki-koyama/bvh11>
- OptiMo <https://github.com/yuki-koyama/optimo>

## License

MIT License.

## Future Work

- Support OpenGL 3+.
