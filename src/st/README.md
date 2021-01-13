# st.h - a simple single-file library that simplifies test writing, logging, and debugging
## using
just include the st.h file with `include`
for example so:
```c
...

#include "st.h"

...
```

## tests
the framework of any test looks like this:
```c
st_name("test_name");
st_start();
st_descr("test description");

...

if (...) { // if an error was detected:
	st_err("error message");
}

...

st_step(); // if you want to describe the next step of the program

...

st_end();
```

## debug
in `st.h`, it is possible to include / exclude the execution of parts of the code by means of 2 macros:
```c
st_debug_start(1); // 1 - to execute all of the code being debugged, 0 - skip them

int main() {
	...
}

void dbg_f() {
	st_debug {
		// debugged part of the code
	}
}

void yet_another_f() {
	st_debug {
		// yet another code
	}
}
```

as you can see, the parts of the code to be debugged are enclosed in the `st_debug` section

## logging
logging takes place in a file whose name is set to `st_name`
example:
```c
#include <stdio.h>

st_name("test_name");

int main() {
	st_log("%s: Hello!\n", __func__); // st_log accepts a message for logging in the form of printf arguments
	...
}

void f() {
	st_logf("in function f"); // st_logf accepts just a message, and writes a string of the form to the log: %func name% <%line%>: %your message%
}
```
