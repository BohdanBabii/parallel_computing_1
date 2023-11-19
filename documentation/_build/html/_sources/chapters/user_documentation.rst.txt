.. _User_Documentation:

User Documentation
==================

.. _ch:introduction:

introduction
------------

Welcome to the Parallel Computing 1! This user documentation will guide you through the process of downloading the project's source code from GitHub 
and compiling it so that you can start working with the software.

.. _ch:Getting_Started:

Getting Started
---------------

Prerequisites
^^^^^^^^^^^^^
Before you can begin, ensure that you have the following prerequisites installed on your system:

* Python3
* C++ (Version 11 or later)
* CMake
* git

Downloading the Code
^^^^^^^^^^^^^^^^^^^^
#. Open your terminal or command prompt.

#. Change to the directory where you want to store the project.

#. Run the following command to clone the project repository from GitHub:

    .. code-block:: bash
        
        git clone https://github.com/BohdanBabii/parallel_computing_1.git

.. _ch:Compiling_the_Code:

Compiling the code
------------------

Compiling Documentation
^^^^^^^^^^^^^^^^^^^^^^^

#. Run the following command to generate the documentation:

    .. code-block::

        cd documentation
        make html

#. Now you can open the documentation in your browser of choice by opening the following URL:

    .. code-block::

        file:///path/to/parallel_computing_1/documentation/_build/html/index.html


.. _ch:Troubleshooting:

Troubleshooting
---------------

If you encounter any issues while working with the Parallel Computing 1, please refer to the following common issues and contact information for assistance.

Common issues
^^^^^^^^^^^^^

* Compilation Errors:
    If you encounter compilation errors, ensure that you have all the necessary prerequisites installed, as mentioned in the Prerequisites section. Verify that you have the correct versions of Python, C++, and CMake.

Contact Information
^^^^^^^^^^^^^^^^^^^

If you are unable to resolve your issue or face any other difficulties not mentioned here, please feel free to contact the Parallel Computing 1 maintainers for assistance. You can reach out to us at the following email addresses:

* Bohdan Babii: bohdan.babii@uni-jena.de
* Denis Novikau: denis.novikau@uni-jena.de
* Lucas Obitz: lucas.obitz@uni-jena.de
