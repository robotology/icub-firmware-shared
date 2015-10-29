# icub-firmware-shared/README.txt
# this file contains instructions about how to use this repository
# marco.accame@iit.it wrote it on 29oct15
#

A.  WHAT THE REPOSITORY CONTAINS

this repository contains source code and include files which are commonly used by icub-main and icub-firmware
repositories. 


B.  HOW TO USE THIS REPOSITORY
  
the way this repository is used depends whether one wants to compile for icub-firmware or for icub-main. here are the two cases.

B.1 case of use with icub-firmware
    
    the repositories icub-firmware and icub-firmware-shared must be placed in the same folder as in following tree structure:

    <folder> ---
                |--<icub-firmware>
                |
                |--<icub-firmware-shared>
                
    nothing else is required because project inside icub-firmware will directly refer source files using a relative path
    which contains string "icub-firmware/../icub-firmware-shared/"

B.2 case of use with icub-main

    so far, the repository icub-firmware-shared is used to compile only on the pc104. to do so, one must:
    
    a.  create a subfolder <icub-firmware-shared>/build-pc104
    b.  go inside the build-pc104 folder
    c.  call "ccmake .."
    d.  inside ccmake digit "c" as many times are required and then "g" to generate the makefile.
    e.  run "make" to generate the correct dependencies required by icub-main (actually this step is not strictly required).
    
    at this point a run of cmake in icub-main will be able to find the suitable dependencies for icub-firmware-shared.
    
    
    
    
    