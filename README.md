# L'Arve - A Receiver Toolkit
Exploring ways of receiving UDP data from hybrid pixel detectors

### Ideas
- Loosley copupled receiver toolkit 
- Used to build a receiver, not a receiver
- Assume no packet loss for fast path
- Get the fundamentals right...

### Open questions
- Driven from UDP side or assembly side?
- How deep do we go with templates?
- C++ 20/23?
- Python bindings using cppyy or pybind11?
- Move image data/data span to it's own library??


### Dependencies (at the moment)
- A C++17 compiler
- Catch2 v3 - Unit tests
- libfmt - Sensible formatting and printing


## Documentation/discussion

- [Packets](docs/packets.md)