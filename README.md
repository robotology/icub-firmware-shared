# icub-firmware-shared
This file contains instructions about how to use this repository.
Created by "Marco Accame <marco.accame@iit.it>" on 29 October 2015.

## A. What the repository contains

This repository contains source code and include files which are commonly used by `icub-main` and `icub-firmware` repositories. 


## B. How to use the repository
  
The way this repository is used depends whether one wants to compile for `icub-firmware` or for `icub-main`. Here are the two cases.

### B.1 Case of use with `icub-firmware`
    
The repositories `icub-firmware` and `icub-firmware-shared` must be placed in the same folder as in the following tree structure:

    <folder> ---
                |--<icub-firmware>
                |
                |--<icub-firmware-shared>
                
Nothing else is required because projects inside `icub-firmware` will directly refer to source files using a relative path (which contains the string `icub-firmware/../icub-firmware-shared/`).

### B.2 Case of use with `icub-main`

So far, the repository `icub-firmware-shared` is used to compile only on the pc104. to do so, one must:
    
 * Create a build subfolder into `icub-firmware-shared`:
   ```mkdir build-pc104```
 * `cd` inside the `build-pc104` folder:
   ```cd build-pc104```
 * Call `ccmake ..`
 * Digit `c` as many times are required and then `g` to generate the makefile.
 * Run `make` to generate the correct dependencies required by `icub-main` (actually this step is not strictly required).
    
At this point a run of `ccmake ../` in `icub-main` build folder will be able to find the suitable dependencies for `icub-firmware-shared`.

## Wireshark dissector for embObj

The repository also contains a [Wireshark Dissector](https://www.wireshark.org/docs/wsdg_html_chunked/ChapterDissection.html) for simplifying the reading of dumps of embObj data in [Wireshark](https://www.wireshark.org/), contained in the source code at [`eth/embobj_wireshark.lua`](./eth/embobj_wireshark.lua) and that is installed at `share/icub_firwmware_shared/embobj_wireshark.lua` .

To install the Wireshark dissector, locate the "Personal Lua Plugins" used by your WireShark installation by opening Wireshark and navigating to Help -> About Wireshark -> Folders, and find the "Personal Lua Plugins"  location, then copy the `embobj_wireshark.lua` file to it.

For example on linux, you can just copy the `embobj_wireshark.lua` file to `~/.local/lib/wireshark/plugins`:

~~~bash
mkdir -p ~/.local/lib/wireshark/plugins
cp ./eth/embobj_wireshark.lua ~/.local/lib/wireshark/plugins
~~~

    
## Maintainers
This repository is maintained by:

| | |
|:---:|:---:|
| [<img src="https://github.com/marcoaccame.png" width="40">](https://github.com/marcoaccame) | [@marcoaccame](https://github.com/marcoaccame) |
