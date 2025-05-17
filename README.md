# MyPacman
Folder Explanation
The files you find directly in the main folder of the repository contain the last working version of the project, it is made of:</br>
* pacman.cpp
* textures.hpp
* functions.hpp
* gameStructrues_fwd.cpp
* gameStructrues.hpp
* functions.cpp
* handlers.cpp
* RisorseProgetto (folder containing the resources necessary to run the project)

Inside of the "VersioniIntermedie" folder you can find snapshots of the previous versions, 
which are cited in the documentation (which will be soon commited as a PDF in the main folder).</br>
Again, inside of that folder you can find "LePrimeFasi" which instead of containing a whole snaphsot,
it contains the four stand-alone .cpp files containing the backups of the very first phases of development.</br>
</br>
In order to run the snapshots you still need to include RisorseProgetto, I included it only once not to fight with the space limitations of GitHub (and because it would be duplicate, which is bad). </br>
Being this project made in SFML you need to have it (and his dependencies) installed in order to actually run and compile the files.</br>
In the main folder you can find a CMakeLists.txt necessary to compile the files, it is set to also compile all of the intermediary versions giving them names going from 01 to 19, you can change them accordingly to your needs.</br>
Also, you'll either  need to change the name of your folder cointaing the project to "progetto" or edit the add_executable lines writing your folders' name for them to compile correctly.</br>
</br>
As you will be able to read from the documentation, all the inclusions are already managed so it is enough to compile the pacman.cpp file (for each executable).


