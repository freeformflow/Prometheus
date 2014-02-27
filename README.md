Prometheus
==========

The Free and Open Chemical Physics Suite (Currently Pre-Alpha)

This software is meant to allow the user to build, visualize, run, and then analyze chemistry simulations.  The focus is currently on classical mechanics, but quantum simulations will eventually be included.

<img src="https://raw.github.com/PandaPup/Prometheus/master/README/PrometheusPlan.png">

This diagram provides a rough structure to Prometheus.  (See the folder README).  

Starting on the left, the User interacts with two main program elements.  One is a viewer, implemented using OpenGL, that can perform live 3D rendering of models stored in memory.  The other element is where all the buttons are kept.  It allows the User to launch functions and manipulate the database.

In the middle are the guts and plumbing of Prometheus.  As a field, computational chemistry exhibits a format and platform fragmentation.  Pandora is meant to accommodate this and parse a variety of filetypes.  Atlas holds the whole system together, storing data and simulation results in a way that can be accessed.  It is currently an ugly multidimensional array.  It will be converted to a true database in the future.  There are also classes that group code that automatically builds and analyzes models.

On the far right are the classes that hold simulation code.  LAMMPS is a piece of software developed by the Sandia National Laboratories.  It stands for Large-scale Atomic/Molecular Massively Parallel Simulator, and contains a wide variety of classical potentials contributed by research groups.  Prometheus will wrap the LAMMPS library and provide a GUI interface for classical simulation.

For quantum mechanics, there is nothing quite as developed as LAMMPS.  So, Artemis will be a custom built quantum package for finite and periodic calculations.

  

Prometheus was built using the Qt SDK to take advantage of C++ efficiency and Qt's platform agnostic qualities.  There are two main interface windows.  One is a visualizer window, that performs 3D live renderings using OpenGL.  The other is a window with GUI button elements....   not all of which work at the moment, lol. 

When the program is active, all you have to do is drag a model's coordinate file over the visualizer window to get started. 

Example file included:  ba.bgf  (See the folder README)

