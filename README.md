# three-dim-util

[![Build Status](https://travis-ci.com/yuki-koyama/three-dim-util.svg?branch=master)](https://travis-ci.com/yuki-koyama/three-dim-util)

A library of utility functions for prototyping OpenGL applications.

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

## Dependencies

- OpenGL
- Eigen <http://eigen.tuxfamily.org/>
- Qt <https://www.qt.io/>
- glm <https://glm.g-truc.net/>

## Projects using three-dim-util

- bvh11 <https://github.com/yuki-koyama/bvh11>
- OptiMo <https://github.com/yuki-koyama/optimo>

## License

MIT License.
