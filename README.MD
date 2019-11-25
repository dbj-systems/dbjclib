# dbjclib
#### &copy; by [dusanjovanovic.org](mailto:dbj@dbj.org)

### Motivation

C LIB to be used for R&D and/or for testing the DBJ C++ code.

DBJ are an Imagined Company. They use C++, but very often with no exceptions, no `<iostream>` and no std lib.


**Disclaimer: Various working and usable Proofs Of Concepts (POC). Unless stated otherwise. Thus, I am affraid, no warranties of any kind, whatsoever.**

### Testing

There is/will be, separate `tests` folder. Files under that folder are not part of the library.
To test this lib, link it in and also include files from under `tests` into your project.

Test unit footprint is:

```cpp
typedef int (*) (FILE *) test_unit ;
```

Thus test output will go wherever you want it to go.

### Building

This is LLVM C LIB made and used by Visual Studio 2019 solutions
Basically MSFT are denying any C involvment when talking about CL. LLVM + Visual Studio = the real C compiler.


-------------------------------------
## Contact

In case you hotly dislike or hotly like my work, or anything else on the long scale in between, you wish to discuss, please do write me.

#### License

#### CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/ 

[![dbj();](http://dbj.org/wp-content/uploads/2015/12/cropped-dbj-icon-e1486129719897.jpg)](http://www.dbj.org "dbj")  

