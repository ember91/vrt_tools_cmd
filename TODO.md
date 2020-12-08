* Use exhale/breathe/sphinx instead
* Check that input and output aren't the same files
* Prefer to use std::filesystem::path over std::string
* Remove static fields used with signals in favour of bind
* Cleanup in lib
* Add namespace for lib
* Should lib be SYSTEM or not in CMakeLists.txt? Make lib/internal and lib/external?
* Make input_stream more efficient. Perhaps do not copy all byte swap bytes and do not parse all fields
* Break out output_stream common code
