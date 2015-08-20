coding style
------------
- picked to conform to std library
- all types and variables lowercase with underscores
- macros and enum values all uppercase with underscores
- template parameters are mixed case
- open brackets start on same line

naming conventions
------------------
- prefer begin/end verbs to start/stop or others
- prefer try_xxx to attempt_xxx (returns bool)
- use nullptr instead of NULL
- prefer references to pointers at all times. minimize need for clients to check for nullptr.

data
----
- all data filenames must be all lowercase with only underscores. no exceptions.

global systems
--------------
utility/assert.h
	void set_assert_failed_handler(assert_failed_handler handler);
utility/trace.h
    void set_trace_handler(trace_handler handler);

string formatting
-----------------
- use cppformat internally
- python format syntax
    https://docs.python.org/2/library/string.html#formatstrings

tips and tricks
------------
- containers/container_helpers.h has lots of helpful one line functions for doing common container ops.
- convert a container (ex. std::vector<int>) into a comma seperated std::string using build_container_string() found in strings/string_build.h

preprocessor macros
--------------------
SOLAR__JSON_ARCHIVE_READER_NO_ALERT_UNUSED_VALUES
- disables alerts that json values read from the file are being unused by the code. this is potentially expensive due to all the string tracking.

