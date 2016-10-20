# fork

**fork** was born from the need for simple forks. It is frustrating how difficult it is to fork a process in contrast to the simplicity of calling the *POSIX* function [fork()](http://man7.org/linux/man-pages/man2/fork.2.html) in a c program.

Have you ever had the terminal running and needed to open a GUI application? It's a mess.

First, you execute the application not on a subshell and it would be great to continue using the same terminal. Second, you forget to treat the process in the command line, then the debug output of the application and minor GUI errors fill up your terminal with trash. It's even worse if you execute the GUI application in a subshell with the &: a never-ending battle between you and the application's output begins. You finally win when you are able write to `killall firefox` in the terminal.

When you treat well a process in the command line, by using:

```
$ firefox > /dev/null &
```

Everything works fine, until the application starts writing on the `stderr` file descriptor, and the battle begins once again.

Oh, wait, why not redirecting both (stdout and stderr)?

```
$ firefox &> /dev/null &
[1] 25534
```

Ok, now the firefox is hooked up in my terminal. If I close the terminal, firefox goes along.

**fork** solves all of these problems.

## Usage

```
$ fork firefox
```

If you want to redirect the `stdout`:

```
$ fork -o output.txt firefox
```

If you want to redirect all `std*`:

```
$ fork --stdout stdout.txt --stdin stdin.txt --stderr stderr.txt
```

## Installation

```
$ make
$ sudo make install
```
