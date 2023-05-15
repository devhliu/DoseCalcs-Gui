
Welcome to DoseCalcs's documentation!
=====================================

DoseCalcs tool (`DoseCalcs application <https://github.com/TarikEl/DoseCalcs-Gui>`_) is built for Ubuntu and CentOS. It comprises a package named gui based on Qt-C++ and a package named core based on Geant4 toolkit. This combination enables users to leverage the strength of the core package while enjoying the convenience and ease of use provided by the gui package.

This core package is based on Geant4 toolkit, which is a set of physics libraries coded in C++ that serve to solve physics problems concerning the transport of radiation in matter using the Monte Carlo method. The core implements these functionalities to simulate the internal dosimetry problem. It allows the study of interaction of radiation emitted from a region source with the matter filling all other body regions in order to score the related internal dosimetry quantities. The core package can be installed by following the prerequisites installation (:ref:'GRunModeCommand'), then you can execute it separately using text commands by following the :ref:'GeneralConcept' and :ref:'Commands' documentation.

The gui package is a graphical user interface for core package. It is designed to simplify the installation, interaction, and execution of the core graphically through a visual interface. The gui package offers intuitive controls, graphical representations, and interactive elements that enhance the user experience. Users can easily navigate through various gui tabs to: install core prerequisites, build core package; save configuration; add simulation inputs; monitor the progress of simulation; visualize results, and access additional functionalities through the graphical interface. It simplifies the usage of core for users who are more comfortable with graphical interfaces or prefer a visual representation of their work.

DoseCalcs tool can be used for advanced research and as an educational tool in internal radiation protection field. `Download package. <https://codeload.github.com/TarikEl/DoseCalcs-Gui/zip/refs/heads/main>`_. The beginning of documentation will be with gui package, and then the core package can be manipulated graphically.

Use DoseCalcs-Core with GUI
------------------------------

When user clones or downloads DoseCalcs zip file and unzip it, he will find core package directory along with the gui package directory. To install the DoseCalcs-GUI package, which is a Qt C++ package, the user should install the prerequisites of Qt5 and xterm as mentioned in :ref:'GuiInstallations', also download the (`Download ICRPDATA and PreDefinedGeometry Zip file. <https://drive.google.com/file/d/1v9cmMbN5pNsTiqtlNA18uF_qvtlPtYpD/view?usp=sharing>`_) and unzip it under PackagesAndFiles directory in the DoseCalcs-Gui source directory, create an empty build directory and enter into, then build the DoseCalcs-GUI using qmake using DoseCalcs.pro file "qmake /home/user/../DoseCalcs.pro", which results in the generation of the [DoseCalcs] executable that user will use to open DoseCalcs-GUI application by calling it "./DoseCalcs". This is if the user will use directly the GUI version of DoseCalcs. 

Use DoseCalcs-Core without GUI
-------------------------------

Also, the user can use the core package directly, by installing the prerequisites as mentioned in :ref:'CoreInstallations' ; creating an empty directory and entering into this directory; then building the core using cmake command, which results in the generation of the [simulate], [merge], and [analysis] executables that user will use to launch the Monte Carlo simulation and merge and analyze the results.


.. toctree::
   :maxdepth: 2
   :caption: Contents:

   GUI
   Installation
   GeneralConcept
   Commands
   Applications

   
Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
